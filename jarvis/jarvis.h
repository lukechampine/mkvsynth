#ifndef JARVIS_H_
#define JARVIS_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

#define MKVS_RGB48     1
#define MKVS_RGB24     2
#define MKVS_YUV444_48 3
#define MKVS_YUV444_24 4

typedef struct MkvsynthMetaData MkvsynthMetaData;
typedef struct MkvsynthSemaphoreList MkvsynthSemaphoreList;
typedef struct MkvsynthFilterQueue MkvsynthFilterQueue;
typedef struct MkvsynthFrame MkvsynthFrame;
typedef struct MkvsynthOutput MkvsynthOutput;
typedef struct MkvsynthInput MkvsynthInput;

// I've been very uncertain what metaData to add.
// In most systems, the 'colorspace' field is
// sufficient, but I wanted a more generic approach.
//
// I'm still not happy with these values
struct MkvsynthMetaData {
	short colorspace;
	int width;
	int height;
	int fpsNumerator;
	int fpsDenominator;
	void *extraData;
};

// Linked list structure, containing 2 semaphores to
// solve the producer-consumer problems when editing video
struct MkvsynthSemaphoreList {
	sem_t remainingBuffer;
	sem_t consumedBuffer;
	MkvsynthSemaphoreList *next;
};

// Linked list of pthreads, parameters, and function pointers
// This list is used for mkvsynthSpawn() and mkvsynthJoin()
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
struct MkvsynthInput {
	sem_t *remainingBuffer;
	sem_t *consumedBuffer;

	MkvsynthFrame *currentFrame;
	MkvsynthMetaData *metaData;
};

#include "../delbrot/delbrot.h"
#include "../colorspacing/colorspacing.h"
#include "bufferAllocation.h"
#include "frameControl.h"
#include "spawn.h"

#endif
