#ifndef datatypes_h_
#define datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

struct MkvsynthMetaData{
	int colorspace;
	int width;
	int height;
	int channels;
	int depth;
	int bytes;
};

struct MkvsynthSemaphoreList{
	sem_t remainingBuffer;
	sem_t consumedBuffer;
	struct MkvsynthSemaphoreList *next;
};

struct MkvsynthFilterQueue {
	pthread_t thread;
	void *(*filter)(void *);
	void *filterParams;
	struct MkvsynthFilterQueue *next;
};

struct MkvsynthFrame {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	struct MkvsynthFrame *nextFrame;
};

struct MkvsynthInput {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	struct MkvsynthFrame *currentFrame;
	struct MkvsynthMetaData *metaData;
};

struct MkvsynthOutput {
	int outputBreadth;
	struct MkvsynthSemaphoreList *semaphores;

	struct MkvsynthFrame *recentFrame;
	struct MkvsynthMetaData *metaData;
};

#endif
