#ifndef datatypes_h_
#define datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct MkvsynthMetaData MkvsynthMetaData;
struct MkvsynthMetaData {
	int colorspace;
	int width;
	int height;
	int channels;
	int depth;
	int bytes;
};

typedef struct MkvsynthSemaphoreList MkvsynthSemaphoreList;
struct MkvsynthSemaphoreList {
	sem_t remainingBuffer;
	sem_t consumedBuffer;
	MkvsynthSemaphoreList *next;
};

typedef struct MkvsynthFilterQueue MkvsynthFilterQueue;
struct MkvsynthFilterQueue {
	pthread_t thread;
	void *(*filter)(void *);
	void *filterParams;
	MkvsynthFilterQueue *next;
};

typedef struct MkvsynthFrame MkvsynthFrame;
struct MkvsynthFrame {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	MkvsynthFrame *nextFrame;
};

typedef struct MkvsynthInput MkvsynthInput;
struct MkvsynthInput {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
};

typedef struct MkvsynthOutput MkvsynthOutput;
struct MkvsynthOutput {
	int outputBreadth;
	MkvsynthSemaphoreList *semaphores;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
};

#endif