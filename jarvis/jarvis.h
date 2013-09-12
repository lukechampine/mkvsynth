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

/*******************************************************************************
 * Also see MkvsynthFrame                                                      *
 *                                                                             *
 * Between each filter is a buffer of frames. The filter outputting frames     *
 * points to that buffer using an MkvsynthOutput. The filter using the buffer  *
 * as input points to the buffer using an MkvsynthInput. Both the              *
 * MkvsynthOutput and the MkvsynthInput point to the same MkvsynthMetaData     *
 * object. The output filter uses the MkvsynthMetaData to tell the input       *
 * filter(s) how to interpret the payload data.                                *
 *                                                                             *
 * int outputBreadth:                                                          *
 *   Multiple filters can use the same buffer of frames for input. Each filter *
 * using the output for input needs to see and process each frame. Therefore   *
 * the output needs to be aware of how many times it is being used for input.  *
 *                                                                             *
 * recentFrame:                                                                *
 *   The most recently output frame from the output filter. When the output    *
 * filter adds another frame it gets connected to recentFrame.                 *
 *                                                                             *
 * *** I am not sure that these are all the needed variables               *** *
 ******************************************************************************/
struct MkvsynthMetaData {
	short colorspace;
	int width;
	int height;
	int fpsNumerator;
	int fpsDenominator;
	void *extraData;
};

/*******************************************************************************
 * Each buffer is only allowed to contain a finite number of frames because    *
 * mkvsynth would otherwise potentially consume all of system memory and       *
 * eventually crash. Because everything operates in a different pthread,       *
 * semaphores are needed to control the size of the buffer.                    *
 *                                                                             *
 * Each buffer can be used by multiple filters for input, and each input may   *
 * be looking at a different frame in the buffer. Because of this, each input  *
 * needs to have a different semaphore for communicating with the output       *
 * filter. Each input however only needs to be aware of it's own place in the  *
 * buffer.                                                                     *
 ******************************************************************************/
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


/*******************************************************************************
 * Each frame needs to know how many filters have not finished looking at it,  *
 * so that the functions in frameControl.c do not deallocate memory            *
 * prematurely. The mutex is needed to prevent multiple threads (aka filters)  *
 * from decrementing the count at the same time.                               *
 *                                                                             *
 * The payload for a frame conatins all of the raw pixel data. You need an     *
 * MkvsynthMetaData to be able to interpret the raw data. The structure        *
 * that points to the frame should also point to an associated                 *
 * MkvsynthMetaData.                                                           *
 *                                                                             *
 * nextFrame is needed because a video is a linked list of MkvsynthFrames.     *
 ******************************************************************************/
struct MkvsynthFrame {
	uint8_t *payload;

	int filtersRemaining;
	pthread_mutex_t lock;

	MkvsynthFrame *nextFrame;
};

/*******************************************************************************
 * Also see MkvsynthMetaData and MkvsynthSemaphoreList                         *
 *                                                                             *
 * An MkvsynthOutput and an MkvsynthInput wrap around the buffer that exists   *
 * between filters. Ultimately, the each wrap the same data. MkvsynthOutput    *
 * faces the filter that is outputting the data.                               *
 *                                                                             *
 * int outputBreadth:                                                          *
 *   Multiple filters can use the same buffer of frames for input. Each filter *
 * using the output for input needs to see and process each frame. Therefore   *
 * the output needs to be aware of how many times it is being used for input.  *
 *                                                                             *
 * recentFrame:                                                                *
 *   The most recently output frame from the output filter. When the output    *
 * filter adds another frame it gets connected to recentFrame.                 *
 *                                                                             *
 * *** I am considering changing semaphores from a pointer to a data value *** *
 ******************************************************************************/
struct MkvsynthOutput {
	int outputBreadth;
	MkvsynthSemaphoreList *semaphores;

	MkvsynthFrame *recentFrame;
	MkvsynthMetaData *metaData;
};

/*******************************************************************************
 * Also see MkvsynthOutput, MkvsynthSemaphoreList, and MkvsynthMetaData        *
 *                                                                             *
 * currentFrame:                                                               *
 *   Current frame is the frame that is currently being accessed by the filter *
 * using the MkvsynthInput. It may (or may not) be different from recentFrame  *
 * in the associated MkvsynthOutput.                                           *
 ******************************************************************************/
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
