#ifndef FUNS_H
#define FUNS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* This header describes how we do function lookup in the parser */

/* Each function has a name, and a function pointer, *
 * which takes a dataStruct and its arguments.       */
typedef struct {
  char *functionName;
  void * (*function)(void*,char *);
} function;

function *makeFun(function *fillThis, char *name, 
		  void * (*fun)(void *,char *));
void deleteFun(function *deleteThis);

#endif // FUNS_H
