#include "funs.h"

function *makeFun(function *fillThis, char *name,
		  void * (*fun)(void *,char *)) {
  int i = strlen(name) + 1;
  fillThis->functionName = (char *)calloc(i,sizeof(char));
  sprintf(fillThis->functionName,name);
  fillThis->function = fun;
  return fillThis;
}

function *funCpy(function *left, function *right) {
  
  return NULL;
}
