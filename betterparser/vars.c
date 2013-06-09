#include "vars.h"
#include "lib.h"

/*******************************************************************************/
/*******************************************************************************/
/***** Simpler functions for dealing with the variable storage class       *****/
/*******************************************************************************/
/*******************************************************************************/


// this function allocates space and reads in the values for a variable
dataStruct *makeVar(dataStruct *fillThis, char *type) {
  // gets type of data
  typeVals ofType;
  if ( strcmp(type,"string") == 0 ) ofType = STR;
  else if ( strcmp(type,"int") == 0 ) ofType = INT;
  else ofType = DBL;
  fillThis->typeInt = ofType;

  // allocates space for the data
  switch ( ofType ) {
  case STR: fillThis->data = calloc(1024,sizeof(char));
    break;
  case INT: fillThis->data = calloc(32,sizeof(int));
    break;
  case DBL: fillThis->data = calloc(32,sizeof(double));
    break;
  }

  return fillThis;
}

/* This function copies another datastruct. Note that   *
 * name and type are truly copied, but the data is only *
 * a shallow copy.                                      */
dataStruct *varCpy(dataStruct *left, dataStruct *right) {
  strcpy(left->varName,right->varName);
  left->typeInt = right->typeInt;
  left->data = right->data;
  return left;
}

// This function frees space for a datastruct
void deleteVar(dataStruct *deleteThis) {
  if ( deleteThis->varName != NULL ) free(deleteThis->varName);
  if ( deleteThis->data != NULL ) free(deleteThis->data);
  memset(&deleteThis,0,sizeof(dataStruct));
}

// This function prints out the data in a dataStruct
void printVar(dataStruct *printThis) {
  // no data
  if ( printThis == NULL || printThis->data == NULL  ) return;

  // prints proper info based on type
  switch (printThis->typeInt) {
  case STR:       dataStrOutput(printThis,NULL);        break;
  case INT: 
    printf("Variable: %s\n\tType: Int\tValue: %d\n",
	   printThis->varName,*((int *)printThis->data));
    break;
  case DBL: 
    printf("Variable: %s\n\tType: Double\tValue: %e\n",
	   printThis->varName,*((double *)printThis->data));
    break;
  }
}

