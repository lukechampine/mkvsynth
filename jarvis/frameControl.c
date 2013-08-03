// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#ifndef frameControl_c_
#define frameControl_c_

#include "frameControl.h"
#include <stdio.h>

MkvsynthFrame *getFrame(MkvsynthInput *params) {
	int value;
	sem_getvalue(params->remainingBuffer, &value);
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	MkvsynthFrame *newFrame;

	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(params->metaData->bytes);
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

void clearFrame(MkvsynthFrame *usedFrame, int freePayload) {
// the mutex needs to be used
	if(usedFrame->filtersRemaining == 0) {
		pthread_mutex_destroy(&usedFrame->lock);

		if(freePayload == 1)
			free(usedFrame->payload);

		free(usedFrame);
	} else {
		// case not implemented yet
		// will need to be implemented, because there's another
		// consumer that I forgot about: the producer. The frame
		// can't be cleared until the producer has pointed to the
		// next one.
	}
}

// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#endif
