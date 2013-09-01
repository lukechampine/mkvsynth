#include "spawn.h"

/////////////////////////////////////////////////
// these variables are used to prevent a need to
// put an MkvsynthFilterQueue in a struct that 
// travels around with the Inputs and Outputs
/////////////////////////////////////////////////
static MkvsynthFilterQueue *head = 0;
static MkvsynthFilterQueue *tail = 0;

/////////////////////////////////////////////////
// first create the node associated with the incoming filter
// then chain it to the existing linked list
//
// eventually, more features may be added
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
// go through the linked list and call pthread_create
// more features may be added
/////////////////////////////////////////////////
void mkvsynthSpawn() {
	MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		pthread_create(&current->thread, NULL, current->filter, current->filterParams);
		current = current->next;
	}
}

/////////////////////////////////////////////////
// first go through the linked list and wait for all filters to finish
// then deallocate the linked list
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
// cheater function for delbrot.
/////////////////////////////////////////////////
ASTnode *go_AST(ASTnode *p, ASTnode *args) {
	checkArgs("go", args, 0);
	printf("Initiating Multithreaded Filters\n");
	mkvsynthSpawn();
	printf("All filters are running\n");
	mkvsynthJoin();
	printf("All filters have completed\n");

	RETURNVAL(0);
}
