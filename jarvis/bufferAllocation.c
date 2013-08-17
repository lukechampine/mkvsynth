#include "bufferAllocation.h"

// Allocate memory for all fields
// OutputBreadth starts at 0, will be increased as inputs add themsevles
// Semaphores will be added to SempahoreList as inputs add themselves
// First frame starts empty, but will be filled out by first call to putFrame
MkvsynthOutput *createOutputBuffer() {
	MkvsynthOutput *output = malloc(sizeof(MkvsynthOutput));
	output->outputBreadth = 0;
	output->semaphores = malloc(sizeof(MkvsynthSemaphoreList));
	output->recentFrame = malloc(sizeof(MkvsynthFrame));
	output->recentFrame->filtersRemaining = 0;
	output->metaData = malloc(sizeof(MkvsynthMetaData));
	return output;
}

// add a semaphore and increase outputBreadth
// initialize the semaphores to a buffer of 5
// semaphore linked list has a blank at the end
MkvsynthInput *createInputBuffer(MkvsynthOutput *output) {
	MkvsynthInput *input = malloc(sizeof(MkvsynthInput));
	
	int i;
	MkvsynthSemaphoreList *tmp = output->semaphores;
	for(i = 0; i < output->outputBreadth; i++)
		tmp = tmp->next;
		
	output->outputBreadth++;
	tmp->next = malloc(sizeof(MkvsynthSemaphoreList));
	
	input->remainingBuffer = &tmp->remainingBuffer;
	input->consumedBuffer = &tmp->consumedBuffer;
	
	sem_init(input->remainingBuffer, 10, 0);
	sem_init(input->consumedBuffer, 0, 10);
	
	input->currentFrame = output->recentFrame;
	input->metaData = output->metaData;
	
	return input;
}
