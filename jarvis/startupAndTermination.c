#ifndef startupAndTermination_c_
#define startupAndTermination_c_

void spawn(ASTParams *filterParams, void *(*filter) (void *)) {
	// MkvsynthOutput **outputs = getOutputs(filterParams); // I don't know how to write this piece
	// int outputsLength = countOutputs(filterParams);
	// 
	// MkvsynthInput **inputs = getInputs(filterParams);
	// int inputsLength = countInputs(filterParams);
	
	int i;
	for(i = 0; i < outputsLength; i++)
		sem_init(outputs[i]->metaData->startupComplete, 0, 0);

	// Wait to spawn filter until the previous filters have filled out the metaData
	for(i = 0; i < inputsLength; i++)
		sem_wait(inputs[i]->metaData->startupComplete);
	
	pthread_t newThread;
	pthread_create(&newThread, NULL, filter, filterParams);
	pthread_join(newThread, NULL);
}

void signalStartupCompletion(MkvsynthOutput *output) {
	sem_post(output->metaData->startupComplete);
}

// Something should probably be added to indicate which filter failed
void filterError(char *error) {
	printf("A Filter Encountered An Error: %s", error);
	exit(0);
}

void mkvsynthTerminate() {
	pthread_exit(0);
}

#endif
