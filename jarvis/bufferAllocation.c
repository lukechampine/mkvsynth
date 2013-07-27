#ifndef _bufferAllocation_c_
#define _bufferAllocation_c_

#include "datatypes.h"

struct MkvsynthOutput *createOutputBuffer() {
	struct MkvsynthMetaData *metaData = malloc(sizeof(struct MkvsynthMetaData));
	struct MkvsynthOutput *output = malloc(sizeof(struct MkvsynthOutput));
	
	output->outputBreadth = 0;
	output->semaphores = malloc(sizeof(struct MkvsynthSemaphoreList));
	output->recentFrame = malloc(sizeof(struct MkvsynthFrame));
	output->recentFrame->filtersRemaining = 0;
	output->metaData = metaData;
	
	return output;
}

struct MkvsynthInput *createInputBuffer(struct MkvsynthOutput *output) {
	struct MkvsynthInput *input = malloc(sizeof(struct MkvsynthInput));
	
	int i;
	struct MkvsynthSemaphoreList *tmp = output->semaphores;
	for(i = 0; i < output->outputBreadth; i++)
		tmp = tmp->next;
		
	output->outputBreadth++;
	tmp->next = malloc(sizeof(struct MkvsynthSemaphoreList));
	
	input->remainingBuffer = &tmp->remainingBuffer;
	input->consumedBuffer = &tmp->consumedBuffer;
	
	sem_init(input->remainingBuffer, 0, 5);
	sem_init(input->consumedBuffer, 0, 0);
	
	input->currentFrame = output->recentFrame;
	input->metaData = output->metaData;
	
	return input;
}

#endif
