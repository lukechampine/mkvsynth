#ifndef _datatypes_h_
#define _datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
	int width;
	int height;
	int colorDepth;
	int colorSpace;
	int payloadBytes;

	pthread_mutux_t bufferLock;
	sem_t *remainingBuffer; // an array outputBreadth long
	sem_t *consumedBuffer;  // an array outputBreadth long

	int outputBreadth;
	MkvsynthFrame *recentFrame;

} MkvsynthControlNode;

typedef struct {
	uint8_t *payload;
	int filtersRemaining;

	MkvsynthFrame *nextFrame;

} MkvsynthFrame;

typedef struct {
	MkvsynthFrame *currentFrame;
	int payloadBytes;

	pthread_mutex_t *bufferLock;
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

} MkvsynthGetParams;

MkvsynthFrame * getFrame(MkvsynthGetParams *params) {
	MkvsynthFrame *newFrame = malloc(sizeof(MkvsynthFrame));

	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(params->bufferLock);

	if(params->currentFrame->filtersRemaining > 1) {
		params->currentFrame->filtersRemaining--;
		newFrame->payload = malloc(params->payloadBytes);
		*newFrame->payload = *params->currentFrame->payload;
	} else {
		newFrame = params->currentFrame;
	}

	newFrame->filtersRemaining = 0;

	pthread_mutex_unlock(params.bufferLock);
	sem_post(&consumedBuffer);

	return newFrame;
}

void putFrame(MkvsynthControlNode *params, MkvsynthFrame *newFrame) {
	int i;
	for(i = 0; i < params->outputBreadth; i++)
		sem_wait(params->consumedBuffer + sizeof(sem_t) * i);

	newFrame->nextFrame = NULL;
	params->recentFrame->nextFrame = newFrame;
	params->recentFrame = newFrame;

	for(i = 0; i < params->outputBreadth; i++)
		sem_post(params->remainingBuffer + sizeof(sem_t) * i);
}

void clearFrame(MkvsynthFrame *usedFrame) {
	if(usedFrame->filtersRemaining == 0) {
		free(usedFrame->payload);
		free(usedFrame);
	} else {
		// complicated process, still need to think about it
	}
}

#endif
