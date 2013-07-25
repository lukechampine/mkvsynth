#ifndef _bufferAllocation_c_
#define _bufferAllocation_c_

MkvsynthOutput *createOutputBuffer(int outputBreadth) {
	MkvsynthMetaData *metaData = malloc(sizeof(MkvsynthMetaData));
	MkvsynthOutput *output = malloc(sizeof(MkvsynthOutput));
	
	sem_t *remainingBuffer = malloc(sizeof(sem_t) * outputBreadth);
	sem_t *consumedBuffer = malloc(sizeof(sem_t) * outputBreadth);
	
	int i;
	for(i = 0; i < outputBreadth; i++) {
		sem_init(&remainingBuffer[i], 0, outputBreadth);
		sem_init(&consumedBuffer[i], 0, 0);
	}
	
	output->outputBreadth = outputBreadth;
	output->remainingBuffer = remainingBuffer;
	output->consumedBuffer = consumedBuffer;
	output->recentFrame = malloc(sizeof(MkvsynthFrame)); // never gets deallocated
	output->metaData = metaData;
	
	return output;
}

MkvsynthInput *createInputBuffer(MkvsynthOutput *output, int semaphoreIndex) {
	MkvsynthInput *input = malloc(sizeof(MkvsynthInput));
	
	input->remainingBuffer = &output->remainingBuffer[semaphoreIndex];
	input->consumedBuffer = &output->consumedBuffer[semaphoreIndex];
	input->currentFrame = output->recentFrame;
	input->metaData = output->metaData;
	
	return input;
}
