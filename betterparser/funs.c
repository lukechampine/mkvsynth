#include "funs.h"

function *makeFun(function *fillThis, char *name,
		  void * (*fun)(void *,char *)) {
  int i = strlen(name) + 1;
  fillThis->functionName = (char *)calloc(i,sizeof(char));
  sprintf(fillThis->functionName,"%s",name);
  fillThis->function = fun;
  return fillThis;
}

void deleteFun(function *deleteThis) {
  if ( deleteThis->functionName != NULL ) free(deleteThis->functionName);
  memset(deleteThis,0,sizeof(function));
}

