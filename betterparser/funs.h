#ifndef FUNS_H
#define FUNS_H

/* This header describes how we do function lookup in the parser */

#include "vars.h"

/* Each function has a name, and a function pointer, *
 * which takes a dataStruct and its arguments.       */
typedef struct {
  char *functionName;
  void * (*function)(void*,char *);
} function;

function *makeFun(function *fillThis, char *name, 
		  void * (*fun)(void *,char *));
function *funCpy(function *left, function *right);

#endif
