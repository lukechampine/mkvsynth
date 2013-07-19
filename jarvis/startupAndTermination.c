#ifndef startupAndTermination_c_
#define startupAndTermination_c_

void spawn(ASTParams *filterParams, void *(*filter) (void *)) {
	pthread_t newThread;
	pthread_create(&newThread, NULL, filter, filterParams);
}

void signalStartupCompletion() {
	// Empty function, just to indicate that something needs to go here	
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
