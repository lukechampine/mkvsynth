#ifndef _bufferAllocation_c_
#define _bufferAllocation_c_

MkvsynthOutput *createOutputBuffer() {
	MkvsynthMetaData *metaData = malloc(sizeof(MkvsynthMetaData));
	MkvsynthOutput *output = malloc(sizeof(MkvsynthOutput));
	
	output->outputBreadth = 0;
	output->semaphores = malloc(sizeof(MkvsynthSemaphoreList));
	output->recentFrame = malloc(sizeof(MkvsynthFrame));
	output->recentFrame->filtersRemaining = 0;
	output->metaData = metaData;
	
	return output;
}

MkvsynthInput *createInputBuffer(MkvsynthOutput *output) {
	MkvsynthInput *input = malloc(sizeof(MkvsynthInput));
	
	int i;
	MkvsynthSemaphoreList *tmp = output->remainingBuffer;
	for(i = 0; i < output->outputBreadth; i++)
		tmp = tmp->next;
		
	output->outputBreadth++;
	tmp->next = malloc(sizeof(MkvsynthSemaphoreList));
	
	input->remainingBuffer = &tmp->remainingBuffer;
	input->consumedBuffer = &tmp->consumedBuffer;
	
	sem_init(input->remainingBuffer, 0, 5);
	sem_init(input->consumedBuffer, 0, 0);
	
	input->currentFrame = output->recentFrame;
	input->metaData = output->metaData;
	
	return input;
}
