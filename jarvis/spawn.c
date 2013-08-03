#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "spawn.h"
#include "../delbrot/delbrot.h"

static MkvsynthFilterQueue *head = 0;
static MkvsynthFilterQueue *tail = 0;
// Hopefully these will be visible between function calls

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

void mkvsynthSpawn() {
	MkvsynthFilterQueue *current = head;
	while(current != NULL) {
		pthread_create(&current->thread, NULL, current->filter, current->filterParams);
		current = current->next;
	}
}

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

ASTnode *go_AST(ASTnode *p, ASTnode *args) {
	checkArgs("go", args, 0);
	printf("Initiating Multithreaded Filters\n");
	mkvsynthSpawn();
	printf("All filters are running\n");
	mkvsynthJoin();
	printf("All filters have completed\n");

	RETURNVAL(0);
}
