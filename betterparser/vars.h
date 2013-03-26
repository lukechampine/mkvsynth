/* This header describes how we do variable lookup in the parser */

#ifndef VARS_H
#define VARS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMTYPES 3
typedef enum { INT, STR, DBL } typeVals;

/* dataStruct and dataType define how variables *
 * are actually stored: we have storage that    *
 * tells what the name of the variable is, what *
 * type of variable it is, and holds a pointer  *
 * to the actual memory that it uses.           */

/* holds info about the variable */
typedef struct {
  char *varName;
  typeVals typeInt;
  void * data;
} dataStruct;

dataStruct * makeVar(dataStruct *fillThis, char *type);
dataStruct * varCpy(dataStruct *left, dataStruct *right);
void deleteVar(dataStruct *deleteThis);
void printVar(dataStruct *printThis);

#endif
