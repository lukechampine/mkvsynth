#include "spawn.h"
#include <stdio.h>

/******************************************************************************
 * The head and tail of the list are currently implemented outside of the     *
 * scope of the fucntions. This was to minimize the number of variables that  *
 * the user has to pass around while making filters.                          *
 *****************************************************************************/
static MkvsynthFilterQueue *head = 0;
static MkvsynthFilterQueue *tail = 0;

/******************************************************************************
 * The incoming arguments are a function (to spawn in a pthread) and the      *
 * input struct for that function. Because no filter should start processing  *
 * until all filters have finished their startup phase, the filter is just    *
 * put into a queue.                                                          *
 *****************************************************************************/
void mkvsynthQueue(void *filterParams, void *(*filter) (void *)) {
	MkvsynthFilterQueue *new = malloc(sizeof(MkvsynthFilterQueue));
	new->filter = filter;
	new->filterParams = filterParams;
	new->next = NULL;

	if(head == NULL) {
		head = new;
		tail = new;
	} else {
		tail->next = new;
		tail = new;
	}
}

/******************************************************************************
 * All filters have finished their startup: go through the queue and create   *
 * a bunch of pthreads.                                                       *
 *****************************************************************************/
void mkvsynthSpawn() {
	MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		pthread_create(&current->thread, NULL, current->filter, current->filterParams);
		current = current->next;
	}
}

/******************************************************************************
 * This is just to make sure that all pthreads finish normally                *
 *****************************************************************************/
void mkvsynthJoin() {
	MkvsynthFilterQueue *current = head;
	MkvsynthFilterQueue *prev;
	void *retval;

	while(current != NULL) {
		pthread_join(current->thread, &retval);
		current = current->next;
	}

	while(current != NULL) {
		prev = current;
		current = current->next;
		free(prev->filterParams);
		free(prev);
	}

	head = NULL;
}

/******************************************************************************
* This is what the interpreter calls once all the filters have been queued. *
*****************************************************************************/
ASTnode *go_AST(ASTnode *p, ASTnode *args) {
	checkArgs("go", args, 0);
	if (head != 0) {
		printf("Initiating Multithreaded Filters\n");
		mkvsynthSpawn();
		printf("All filters are running\n");
		mkvsynthJoin();
		printf("All filters have completed\n");
	}

	RETURNNULL();
}