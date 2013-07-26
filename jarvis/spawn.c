#ifndef spawn_c_
#define spawn_c_

MkvsynthFilterQueue *head = NULL;
MkvsynthFilterQueue *tail = NULL;
// Hopefully these will be visible between function calls

// Create a linked list of functions to create as pthreads
void queueFunction(void *filterParams, void *(*filter) (void *)) {
	MkvsynthFilterQueue *new = malloc(sizeof(MkvsynthFilterQueue));
	new->fuction = filter; // No idea if that will work
	new->filterParams = filterParams;
	new->next = NULL;

	if(head == NULL) {
		head = new;
		tail = new;
	} else {
		tail->next = new;
		tail = tail->next;
	}
}

void spawnFunctions() {
	MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		pthread_create(&current->thread, NULL, current->filter, current->filterParams);
		current = current->next;
	}
}

void joinFunctions() {
	MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		void *retval;
		pthread_join(current->thread, &retval);
		current = current->next;
	}
}
#endif
