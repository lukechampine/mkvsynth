#ifndef frameControl_c_
#define frameControl_c_

#include "frameControl.h"
#include <string.h>
#include <stdio.h>

/******************************************************************************
 * This function returns a frame from the buffer. First sem_wait() is called  *
 * to ensure that there is a frame in the buffer. Then a mutex lock is called *
 * to prevent other threads from making changes to the frame variables at the *
 * same time as we are using it.                                              *
 *                                                                            *
 * getFrame() returns a unique frame to the next filter, meaning the next     *
 * filter can be sure that no other frames will be looking at the data or     *
 * modifying it. If multiple other filters need to look at the frame still    *
 * (IE filtersRemaining > 1), then getFrame will memcpy the data and create   *
 * a completely fresh frame for the calling filter to use. If the calling     *
 * filter is the last filter (IE filtersRemaining < 2), it will just return   *
 * a pointer to the frame without copying it.                                 *
 *                                                                            *
 * All frames returned from getFrame() will have a filtersRemaining value of  *
 * 0.                                                                         *
 *                                                                            *
 * Once the copying is done, the mutex is unlocked and then sem_post() is     *
 * called to inform the output filter that another space has opened up in the *
 * buffer.                                                                    *
 *****************************************************************************/
MkvsynthFrame *getFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);
	pthread_mutex_lock(&params->currentFrame->lock);
	
	MkvsynthFrame *newFrame;

	if(params->currentFrame->filtersRemaining > 1) {
		newFrame = malloc(sizeof(MkvsynthFrame));
		newFrame->payload = malloc(getBytes(params->metaData));

		// The first frame is NULL, so we have to make sure we are not
		// 	looking at the first frame here.
		if(params->currentFrame->payload != NULL)
			memcpy(newFrame->payload, params->currentFrame->payload, getBytes(params->metaData));
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

/******************************************************************************
 * getReadOnlyFrame() is like getFrame() except that it assumes that the      *
 * filter will not be modifying the frame data. For that reason, the filter   *
 * does not need a unique copy of the frame. filtersRemaining also is not     *
 * decremented however, because the filter may still be looking at the data.  *
 *****************************************************************************/
MkvsynthFrame *getReadOnlyFrame(MkvsynthInput *params) {
	sem_wait(params->remainingBuffer);

	MkvsynthFrame *newFrame = params->currentFrame;
	params->currentFrame = params->currentFrame->nextFrame;

	sem_post(params->consumedBuffer);
	return newFrame;
}

/******************************************************************************
 * putFrame places a frame into the buffer. Because the buffer is shared by   *
 * many filters, a list of semaphores needs to be used. There is only space   *
 * in the buffer if each of the next filters reports that there is space in   *
 * the buffer.                                                                *
 *                                                                            *
 * recentFrame is a frame that has been allocated but not filled with data.   *
 * putFrame will fill out recentFrame with the payload from the output filter *
 * and initialize the mutex on the frame. filtersRemaining will be equal to   *
 * the outputBreadth of the output filter.                                    *
 *                                                                            *
 * Memory is allocated so the recentFrame can be restored to its previous     *
 * status of pointing to a shell of a frame.                                  *
 *                                                                            *
 * Finally sem_post() is called for all the input filters to let them know    *
 * that a new frame has been added to the buffer.                             *
 *****************************************************************************/
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

/******************************************************************************
 * clearFrame() will delete a frame but not the payload. The assumption here  *
 * is that the payload modified and then used in the next frame. If           *
 * filtersRemaining() is not 0 there was probably an error, because it either *
 * means that the filter calling clearFrame() used getReadOnlyFrame() to grab *
 * the frame, which is an error.                                              *
 *****************************************************************************/
void clearFrame(MkvsynthFrame *usedFrame) {

#ifdef DEBUG
	if(usedFrame->filtersRemaining != 0) {
		printf("clearFrame ERROR: filtersRemaining should equal 0!\n");
		exit(0);
	}
#endif

	pthread_mutex_destroy(&usedFrame->lock);
	free(usedFrame);
}

/******************************************************************************
 * clearReadOnlyFrame() checks that all filters (except the calling filter)   *
 * have finished looking at the frame. If not, filtersReamining is reduced.   *
 * This check must happen using a mutex to prevent race conditions between    *
 * filters calling clearReadOnlyFrame().                                      *
 *                                                                            *
 * If it is the last filter, then the payload is destroyed as well as the     *
 * frame. clearReadOnlyFrame() is different because it needs to check         *
 * filtersRemaining and then it free's the payload.                           *
 *****************************************************************************/
void clearReadOnlyFrame(MkvsynthFrame *usedFrame) {
	pthread_mutex_lock(&usedFrame->lock);
	if(usedFrame->filtersRemaining == 1) {
		// kill the frame
		pthread_mutex_unlock(&usedFrame->lock);
		free(usedFrame->payload);
		pthread_mutex_destroy(&usedFrame->lock);
		free(usedFrame);
	} else {
		pthread_mutex_unlock(&usedFrame->lock);
		usedFrame->filtersRemaining--;
	}
}

#endif
