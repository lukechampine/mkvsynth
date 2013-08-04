#ifndef frameControl_c_
#define frameControl_c_

#include "frameControl.h"
#include <stdio.h>

// producer-consumer problem: first step is to wait for the frame
// second step is to lock the frame
//
// second to last step is to unlock the frame
// last step is to post on the frame
//
// if there are multiple filters remaining, make a complete copy
// of the frame and return it to the requesting filter.
//
// if this is the last filter, just return a pointer to the frame
MkvsynthFrame *getFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	MkvsynthFrame *newFrame;

	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(params->metaData->bytes);

		// ugly situation: without this if-else block, a
		// memcpy will happen on a pointer that's not initialized: seg fault
		if(params->currentFrame->payload != NULL)
			memcpy(newFrame->payload, params->currentFrame->payload, params->metaData->bytes);
		else
			newFrame->payload = NULL;

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

// producer-consumer problem: first make sure there's room
// in the buffer by insuring that all input frames have finished
// remember that each input has a unique semaphore
// (no mutex needed)
//
// finish by doing sem_post for every input filter
//
// recentFrame points to an allocated-but-empty frame,
// we fill it out with the payload data and set the filtersRemaining value
//
// then we allocate a new frame and make it recentFrame.
//
// It's done this way so that at the very beginnning, the 
// inputs and outputs can be pointed to an existing first frame
// even though no data has been filled out
void putFrame(MkvsynthOutput *params, uint8_t *payload) {	
	int i;
	MkvsynthSemaphoreList *tmp = params->semaphores;

	for(i = 0; i < params->outputBreadth; i++) {
		sem_wait(&tmp->consumedBuffer);
		tmp = tmp->next;
	}

	params->recentFrame->payload = payload;
	params->recentFrame->filtersRemaining = params->outputBreadth;
	pthread_mutex_init(&params->recentFrame->lock, NULL);

	MkvsynthFrame *newFrame = malloc(sizeof(MkvsynthFrame));
	newFrame->nextFrame = NULL;
	params->recentFrame->nextFrame = newFrame;
	params->recentFrame = newFrame;

	tmp = params->semaphores;
	for(i = 0; i < params->outputBreadth; i++) {
		sem_post(&tmp->remainingBuffer);
		tmp = tmp->next;
	}
}

// right now it's pretty simple because getFrame does all the work.
// if freePayload = 1, then deallocate payload.
// otherwise assume that the payload was passed to the next filter
void clearFrame(MkvsynthFrame *usedFrame, int freePayload) {
	if(usedFrame->filtersRemaining == 0) {
		pthread_mutex_destroy(&usedFrame->lock);

		if(freePayload == 1)
			free(usedFrame->payload);

		free(usedFrame);
	} else {
		// will be used if getReadOnlyFrame() is added
	}
}

#endif
