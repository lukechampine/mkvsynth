#ifndef startupAndTermination_c_
#define startupAndTermination_c_

void signalStartupCompletion(MkvsynthOutput *output) {
	sem_post(output->metaData->startupComplete);
}

// Something should probably be added to indicate which filter failed
void filterError(char *error) {
	printf("Filter Error: %s", error);
	exit(0);
}

void mkvsynthTerminate() {
	pthread_exit(0);
}

#endif
