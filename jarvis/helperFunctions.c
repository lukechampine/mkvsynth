// There should probably be error checking statements around all of the mallocs(), frees(), and inits()
// And probably some decent way to handle errors needs to be added to the whole system.

#ifndef _datatypes_h_
#define _datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

// bytes can be calculated from the other information, but it's a datapoint
// anyway to minimize the number of times it needs to be computed. Maybe it's
// not actually needed.
typedef struct {
	int colorSpace;

	int width;
	int height;
	int channels;
	int depth;

	int bytes;
} MkvsynthMetaData;

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
	int outputBreadth;
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthPutParams;

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
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthGetParams;

///////////////////////////////////////
// getFrame(MkvsynthGetParams *params)
// 	
//	PARAMS
//		getFrame() uses the params struct to keep track of the current frame, and
//		and to know which semaphore in the control node array is the correct semaphore.
//		getFrame() also needs to know how many bytes the payload is so it can call
//		malloc()
//
//	MUTEXES AND SEMAPHORES
//		Due to potential race conditions, getFrame() must first wait until it's respective
//		putFrame() has signaled that the next frame is ready. That's why sem_wait() is called
//		first. Once it recieves a signal, it needs exlusive access to the value filtersRemaining,
//		so it locks the semaphore associated with the frame delivered by putFrame(). 
//
//		Because mkvsynth assumes that all cores share RAM, there is no benefit to having multiple
//		threads copy the payload at the same time, so the mutex is kept locked just to make life
//		easier.
//
//		When all modifications are complete, the frame is unlocked so that other threads may
//		have access. A sem_post is then placed to indicate to the respective putFrame() that
//		there is additional room in the buffer for a new frame.
//
//	COPYING FRAMES OVER
//		getFrame assumes that the calling filter will use the memory allocated for the payload
//		as a workspace, and thus gives each input filter its own copy of the data. The goal is
//		for each copied frame to be a working replacement of the original frame. 
//			1. Two things must be allocated: the frame itself and then space for the payload. 
//			2. The payload needs to be copied from the original frame to the new frame.
//			3. filtersRemaining is set to 0 because no other filters will see the new frame.
//			4. The mutex gets initialized because an initialized mutex is part of an mkvsynthFrame
//			5. nextFrame points to NULL, because this frame is in a 'dead' stream -> no output
//				filter will ever concatenate to it. Maybe one future feature will be tweaking
//				these functions such that as getFrame() pulls more frames, it also links them
//				together so that they form their own proper chain. But right now the idea is
//				making my head spin because in order for that chain to be proper, it will need
//				a control node and semaphores.
//		to be copied from the original frame to the new frame. Then filtersRemaining is set to
//		0 because the only filter that needs to read the new frame already recieved it. Then
//		the mutex needs to be initialized
//
//		If this filter is the last filter to request the frame, there is no reason to do all the copying.
//		instead, newFrame is just pointed to the same location as params->currentFrame. filtersRemaining
//		is set to 0 to acknowledge that every filter has seen and used the data.
//
//	FINISHING
//		getFrame advances params->currentFrame so that it's pointing in the right place the next
//		time that it gets called. Finally, it returns newFrame.
//
///////////////////////////////////////

MkvsynthFrame * getFrame(MkvsynthGetParams *params) {
	MkvsynthFrame *newFrame;

	sem_wait(params->remainingBuffer);
	
	// If the current frame is NULL after sem_wait, then we've reached the end of the video
	if(params->currentFrame == NULL)
		return NULL;

	pthread_mutex_lock(&params->currentFrame->lock);

	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(params->payloadBytes);
		memcpy(newFrame->payload, params->currentFrame->payload, params->payloadBytes);
		newFrame->filtersRemaining = 0;
		pthread_mutex_init(&newFrame->lock, NULL);
		newFrame->nextFrame = params->currentFrame->nextFrame;
		
		params->currentFrame->filtersRemaining--;
	} else {
		params->currentFrame->filtersRemaining = 0;
		newFrame = params->currentFrame;
	}

	pthread_mutex_unlock(&params->currentFrame->lock);
	sem_post(params->consumedBuffer);
	
	params->currentFrame = params->currentFrame->nextFrame;
	return newFrame;
}

///////////////////////////////////////
// putFrame(MkvsynthControlNode *params, uint8_t *payload)
// 	
//	PARAMS
//		putFrame() needs to access all of the semaphores related to its output buffer,
//		as well as access to the number of frames that use the output as input
//		(params->outputBreadth), as well as access to the frame that was most recently
//		output so that it can modify the pointers correctly.
//
//	SEMAPHORES
//		Because the buffer is shared between all filters that use the output,
//		putFrame() cannot add a frame to the buffer until every single filter
//		has said that there is room for it. Similarly, after it has created the
//		frame it must alert every single input frame that a new frame has been
//		added to the buffer, because each filter using the output has its own
//		pair of semaphores.
//
//	FRAME CREATION
//		putFrame() gets a payload, not a frame, because the job belongs to putFrame()
//		to actually allocate and manage the frame.
//			1. Allocate the frame in dynamic memory
//			2. Point payload to the already-dynamically-allocated payload (filter did that)
//			3. Correctly instantiate filtersRemaining to the outputBreadth
//			4. Initialize the mutex, each frame has its own mutex
//			5. The next frame has not been created, set it to null in the new frame.
//			6. Point the previous frame at the new frame
//			7. Update the most recent frame output to being the new frame.
//
///////////////////////////////////////

void putFrame(MkvsynthControlNode *params, uint8_t *payload) {	
	int i;
	for(i = 0; i < params->outputBreadth; i++)
		sem_wait(params->consumedBuffer + sizeof(sem_t) * i);

	MkvsynthFrame *newFrame = malloc(sizeof(MkvsynthFrame));
	newFrame->payload = payload;
	newFrame->filtersRemaining = params->outputBreadth;
	pthread_mutex_init(&newFrame->lock, NULL);
	newFrame->nextFrame = NULL;
	params->recentFrame->nextFrame = newFrame;
	params->recentFrame = newFrame;

	for(i = 0; i < params->outputBreadth; i++)
		sem_post(params->remainingBuffer + sizeof(sem_t) * i);
}

// Sometimes when clear is called, the whole frame needs to be deleted.
// Other times however, it was a readOnly frame and only needs to be
// 	deleted under certain conditions.
// getReadOnlyFrame() will also necessitate a sem_post()
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

// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#endif
