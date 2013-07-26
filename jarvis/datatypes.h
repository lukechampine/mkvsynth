#ifndef datatypes_h_
#define datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	int colorspace;
	void *colorspaceData;
} MkvsynthMetaData;

typedef struct {
	sem_t remainingBuffer;
	sem_t consumedBuffer;
	struct MkvsynthSemaphoreList *next;
} MkvsynthSemaphoreList;

typedef struct {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	struct MkvsynthFrame *nextFrame;
} MkvsynthFrame;

typedef struct {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthInput;

typedef struct {
	int outputBreadth;
	MkvsynthSemaphoreList *semaphores;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
} MkvsynthOutput;

#endif
