#ifndef datatypes_h_
#define datatypes_h_

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

// I've been very uncertain what metaData to add.
// In most systems, the 'colorspace' field is
// sufficient, but I wanted a more generic approach.
//
// I'm still not happy with these values
typedef struct MkvsynthMetaData MkvsynthMetaData;
struct MkvsynthMetaData {
	int colorspace;
	int width;
	int height;
	int channels;
	int depth;
	int bytes;
	int fpsNumerator;
	int fpsDenominator;
};

// Linked list structure, containing 2 semaphores to
// solve the producer-consumer problems when editing video
typedef struct MkvsynthSemaphoreList MkvsynthSemaphoreList;
struct MkvsynthSemaphoreList {
	sem_t remainingBuffer;
	sem_t consumedBuffer;
	MkvsynthSemaphoreList *next;
};

// Linked list of pthreads, parameters, and function pointers
// This list is used for mkvsynthSpawn() and mkvsynthJoin()
typedef struct MkvsynthFilterQueue MkvsynthFilterQueue;
struct MkvsynthFilterQueue {
	pthread_t thread;
	void *(*filter)(void *);
	void *filterParams;
	MkvsynthFilterQueue *next;
};

// videos are linked lists of frames, payload is raw data
// mutexes are used on a per-frame basis
// filtersRemaining is a counter so that the frame isn't free()'d before
//   every filter has had a chance to collect the needed data
typedef struct MkvsynthFrame MkvsynthFrame;
struct MkvsynthFrame {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	MkvsynthFrame *nextFrame;
};

// An output can be used by multiple filters as input. Because
// frames are output only once and each input can be at different
// frames, every input gets a unique set of semaphores. Output
// must call sem_wait and sem_post on the whole list every time.
//
// recentFrame points at the most recently output frame in the
// video.
//
// metaData is shared between the output and all the inputs
// using said output
typedef struct MkvsynthOutput MkvsynthOutput;
struct MkvsynthOutput {
	int outputBreadth;
	MkvsynthSemaphoreList *semaphores;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
};

// The semaphores are pointers, not arrays. Each input has a unique
// pair of semaphores that it shares with its output
//
// metaData is also shared with the output
//
// inputs can be at different places, and therefore are
// individually responsible for knowing what frame they are on
typedef struct MkvsynthInput MkvsynthInput;
struct MkvsynthInput {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
};

#endif
