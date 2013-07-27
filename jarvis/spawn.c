#ifndef spawn_c_
#define spawn_c_

#include <stdlib.h>
#include "datatypes.h"

static struct MkvsynthFilterQueue *head = 0;
static struct MkvsynthFilterQueue *tail = 0;
// Hopefully these will be visible between function calls

void queueFunction(void *filterParams, void *(*filter) (void *)) {
	struct MkvsynthFilterQueue *new = malloc(sizeof(struct MkvsynthFilterQueue));
	new->filter = filter;
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
	struct MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		pthread_create(&current->thread, NULL, current->filter, current->filterParams);
		current = current->next;
	}
}

void joinFunctions() {
	struct MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		void *retval;
		pthread_join(current->thread, &retval);
		current = current->next;
	}
}

#endif
