// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#ifndef frameControl_c_
#define frameControl_c_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

MkvsynthFrame *getFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	MkvsynthFrame *newFrame;

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

void putFrame(MkvsynthOutput *params, uint8_t *payload) {	
	int i;
	for(i = 0; i < params->outputBreadth; i++)
		sem_wait(params->consumedBuffer + sizeof(sem_t) * i);

	params->recentFrame->payload = payload;
	params->recentFrame->filtersRemaining = params->outputBreadth;
	pthread_mutex_init(&params->recentFrame->lock, NULL);

	MkvsynthFrame *newFrame = malloc(sizeof(MkvsynthFrame));
	newFrame->nextFrame = NULL;
	params->recentFrame->nextFrame = newFrame;
	params->recentFrame = newFrame;

	for(i = 0; i < params->outputBreadth; i++)
		sem_post(params->remainingBuffer + sizeof(sem_t) * i);
}

void clearFrame(MkvsynthFrame *usedFrame) {
// the mutex needs to be used
	if(usedFrame->filtersRemaining == 0) {
		pthread_mutex_destroy(usedFrame->lock);
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
