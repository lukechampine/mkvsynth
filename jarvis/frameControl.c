// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#ifndef frameControl_c_
#define frameControl_c_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	int colorSpace;
	int width;
	int height;
	int channels;
	int depth;
	int bytes;
	
	sem_t startupComplete;
} MkvsynthMetaData;

typedef struct {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	MkvsynthFrame *nextFrame;
} MkvsynthFrame;

typedef struct {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthInput;

typedef struct {
	int outputBreadth;
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthOutput;

MkvsynthFrame *getFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	A:
	MkvsynthFrame *newFrame;
	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(params->payloadBytes);
		memcpy(newFrame->payload, params->currentFrame->payload, params->payloadBytes);
		newFrame->filtersRemaining = 0;
		pthread_mutex_init(&newFrame->lock, NULL);
		newFrame->nextFrame = params->currentFrame->nextFrame;
		params->currentFrame->filtersRemaining--;
	} else if(params->currentFrame->filtersRemaining == 1) {
		params->currentFrame->filtersRemaining = 0;
		newFrame = params->currentFrame;
	} else {
		MkvsynthFrame *tmp = params->currentFrame;
		params->currentFrame = params->currentFrame->nextFrame;
		clearFrame(tmp);
		goto A;
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

void clearFrame(MkvsynthFrame *usedFrame) {
	if(usedFrame->filtersRemaining == 0) {
		pthread_mutex_destroy(usedFrame->lock);
		free(usedFrame->payload);
		free(usedFrame);
	} else {
		// case not implemented yet
	}
}

// There should probably be error checking statements around all of the mallocs(), frees(), and inits()

#endif
