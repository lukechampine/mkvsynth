#ifndef spawn_c_
#define spawn_c_

typedef struct {
	ASTnode *p;
	ASTnode *args;
	void *filterArgs;
} FilterParams;

void spawn(FilterParams *filterParams, void *(*filter) (void *)) {
	// before doing a create, allocate a new thread and add it
	// to the global linked list of threads?
	pthread_t newThread;
	pthread_create(&newThread, NULL, filter, filterParams);
}

#endif
