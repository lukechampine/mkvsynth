#ifndef frameControl_c_
#define frameControl_c_

#include "frameControl.h"
#include "properties.h"
#include <stdio.h>

/////////////////////////////////////////////////
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
/////////////////////////////////////////////////
MkvsynthFrame *getFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	MkvsynthFrame *newFrame;

	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(getBytes(params->metaData));

		// ugly situation: without this if-else block, a
		// memcpy will happen on a pointer that's not initialized: seg fault
		if(params->currentFrame->payload != NULL)
			memcpy(newFrame->payload, params->currentFrame->payload, getBytes(params->metaData));
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

/////////////////////////////////////////////////
// frame is read-only, which means we just wait on the semaphores
// and return the frame. No values get modified (much faster)
/////////////////////////////////////////////////
MkvsynthFrame *getReadOnlyFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);

	MkvsynthFrame *newFrame = params->currentFrame;
	params->currentFrame = params->currentFrame->nextFrame;

	sem_post(params->consumedBuffer);
	return newFrame;
}

/////////////////////////////////////////////////
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
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
// clearFrame destroys the mutex and frame but
// leaves the payload intact
//
// clearFrame assumes that filtersRemaining == 0
/////////////////////////////////////////////////
void clearFrame(MkvsynthFrame *usedFrame) {

#ifdef DEBUG
	if(usedFrame->filtersRemaining != 0) {
		printf("clearFrame ERROR: filtersRemaining should equal 0!\n");
		exit(0);
	}
#endif

	pthread_mutex_destroy(&usedFrame->lock);
	free(usedFrame);
}

/////////////////////////////////////////////////
// clearReadOnlyFrame destroys the 
/////////////////////////////////////////////////
void clearReadOnlyFrame(MkvsynthFrame *usedFrame) {
	pthread_mutex_lock(&usedFrame->lock);
	if(usedFrame->filtersRemaining == 1) {
		// kill the frame
		pthread_mutex_unlock(&usedFrame->lock);
		free(usedFrame->payload);
		pthread_mutex_destroy(&usedFrame->lock);
		free(usedFrame);
	} else {
		pthread_mutex_unlock(&usedFrame->lock);
		usedFrame->filtersRemaining--;
	}
}

#endif
