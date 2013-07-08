#ifndef _datatypes_h_
#define _datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

///////////////////////////////////////
// MkvsynthControlNode 
// 	
//	METADATA	
// 		meant to contain information about the video stream for filters and
// 		memory management. Currently it's only colorSpace and payloadBytes.
// 		Technically, one should be able to determine most of the information
// 		about the video by the resolution fields and colorspace fields, but
// 		between sar values and rounded screens (occulus rift) it's a confusing
// 		situation and I'm not entirely sure how best to deal with the metadata.
// 		At some point, room for custom fields will be added.
//
//		height - a resolution field
//		width - a resolution field
//		colorDepth - number of bits used to define each color sample
// 		colorSpace - still working out a good way to handle this
// 		payloadBytes - needed for malloc and avoiding seg faults
//
//	SEMAPHORES
//		a buffer should only have 1 filter outputting to it but may have multiple
//		filters using it as input. Each input filter could be at a different
//		place in the buffer, and therefore there needs to be 1 set of semaphores
//		for each filter.
//	
//		outputBreadth - how many filters are using the buffer as input
// 		reamainingBuffer - an array of semaphores indicating on a per-filter
// 			basis how many frames are in the buffer but have
// 			not been read by the input filter
// 		consumedBuffer - an array of semaphores indicating on a per-filter basis
// 			how many empty spaces are in the buffer, so that the
// 			output filter knows if the buffer has room for more
// 			output. The output buffer must wait until every
// 			input filter has at least 1 empty space for a frame
//
//	MOST RECENT OUTPUT FRAME
//		each frame keeps its own pointer to the next frame, but the output filter
//		needs to know which frame doesn't have a next frame, so that it can keep
//		the list going.
//
///////////////////////////////////////

typedef struct {
	int width;
	int height;
	int colorDepth;
	int colorSpace;
	int payloadBytes;

	int outputBreadth;
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *recentFrame;

} MkvsynthControlNode;

///////////////////////////////////////
// MkvsynthFrame
// 	
//	PAYLOAD
//		just a string of bytes, which contain information based on the meta-data.
//		for the most part, pixels are read in left-to-right, top-to-bottom, and
//		each pixel is fully defined before the next pixel is defined. In
//		mkvsynth, raw frames are not compressed in any way.
//
//		At some point, a feature may be added that applies simple compression to
//		the payload, using some blazing fast compression algorithm that can
//		compress potentially repetitive data down by a fair amount. This could
//		cause a decent sized speedup, depending on where the actual bottlenecks
//		end up being.
//
//		For the time being, I'm going to establish an 'Mkvsynth Standard'
//		colorspace, with the idea that most filters should be able to process
//		frames within that colorspace. I'm picking RGB36, because 8 bit depth is
//		no longer the edge of what technology can produce, and 10bit depth may
//		not last very long. At 12bits however, we should be beyond what even
//		trained and unusually perceptive eyes can discern. More advanced
//		colorspaces will still availiable, but hopefully the majority of mkvsynth
//		filters will be able to support the 'Mkvsynth Standard Colorspace'
//
//	FILTERS REMAINING
//		multiple filters may be using the frame as input, before the memory can be
//		deallocated every single filter must signal (through clearFrame) that it
//		has finished using the frame and no longer needs to see it.
//
//		when input threads are reading and modifying this value, a mutex is needed
//		to be sure that only 1 thread has access to the value at a time. Multiple
//		filters can be accessing the payload at a time, however.
//
//	NEXT FRAME
//		videos are just a stream of frames, and mkvsynth keeps track of the frames
//		through a linked list. The list is maintained within the MkvsynthFrame
//		struct.
//
///////////////////////////////////////

typedef struct {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	MkvsynthFrame *nextFrame;

} MkvsynthFrame;

///////////////////////////////////////
// MkvsynthGetParams
// 	
//	FRAME DATA
//		Each filter will be at its own space when reading the output from the
//		previous filter, and so each filter needs some way to recognize where
//		the current frame being viewed is.
//
//		payloadBytes is also kept in the controlNode, but it's repeated in this
//		struct because an int is smaller than a pointer to the control node.
//
//	SEMAPHORES
//		The real reason that getFrame doesn't just get a control node is because
//		each filter using getFrame is looking at only 1 semaphore, that pointer
//		is to a single semaphore and not to an array. This eliminates the need for
//		the getFrame function to know which semaphore belongs to it instead of the
//		other filters sharing the same control node.
//
///////////////////////////////////////

typedef struct {
	MkvsynthFrame *currentFrame;
	int payloadBytes;

	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

} MkvsynthGetParams;

// getFrame should probably only be returning a payload, just like putFrame
// only takes a payload as input

//getFrame allocates a new
MkvsynthFrame * getFrame(MkvsynthGetParams *params) {
	MkvsynthFrame *newFrame = malloc(sizeof(MkvsynthFrame));

	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(params->currentFrame->lock);

	if(params->currentFrame->filtersRemaining > 1) {
		params->currentFrame->filtersRemaining--;
		newFrame->payload = malloc(params->payloadBytes);
		*newFrame->payload = *params->currentFrame->payload;
		newFrame->nextFrame = params->currentFrame->nextFrame;
		newFrame->filtersRemaining = 0;
	} else {
		params->currentFrame->filtersRemaining = 0;
		return params->currentFrame;
	}

	// Because this function is copying frames over, and all cores share
	// memory, the program will not be made faster by allowing multiple
	// threads/cores to access the data for copying at the same time.

	pthread_mutex_unlock(params->currentFrame->lock);
	sem_post(&consumedBuffer);

	return newFrame;
}

// input changed from a frame to just a payload; the filters should only
// be worrying about the payload. Jarvis handles all of the other things
//
// it's late and I'm too tired to see this pointer math clearly, so I'm
// going to update the function at a later date to reflect the fact that
// the input was changed from a frame to just a pointer to a payload.
void putFrame(MkvsynthControlNode *params, uint8_t *payload) {
	int i;
	for(i = 0; i < params->outputBreadth; i++)
		sem_wait(params->consumedBuffer + sizeof(sem_t) * i);

	newFrame->nextFrame = NULL;
	params->recentFrame->nextFrame = newFrame;
	params->recentFrame = newFrame;

	if(pthread_mutex_init(&newFrame->lock, NULL) != 0) {
		// Mutex creation fail error
	}

	for(i = 0; i < params->outputBreadth; i++)
		sem_post(params->remainingBuffer + sizeof(sem_t) * i);
}

// Sometimes when clear is called, the whole frame needs to be deleted.
// Other times however, it was a readOnly frame and only needs to be
// 	deleted under certain conditions.
void clearFrame(MkvsynthFrame *usedFrame) {
	if(usedFrame->filtersRemaining == 0) {
		pthread_mutex_destroy(usedFrame->lock);
		free(usedFrame->payload);
		free(usedFrame);
	} else {
		// Was created by getReadOnlyFrame
		// which doesn't exist yet so error
	}
}

#endif
