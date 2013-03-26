#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "hash.h"
#include "funs.h"


void *dataInit(void *data, char *args);
void *dataStrAppend(void *data,char *args);
void * dataStrOutput(void *data,char *args);

void addFunctions(hashTable *addTo);
