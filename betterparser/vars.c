#include "vars.h"

/*******************************************************************************/
/*******************************************************************************/
/***** Simpler functions for dealing with the variable storage class       *****/
/*******************************************************************************/
/*******************************************************************************/

dataStruct *makeVar(dataStruct *fillThis, char *type, char *name) {
  typeVals ofType;
  int len = strlen(name) + 1;
  if ( strcmp(type,"string") == 0 ) ofType = STR;
  else if ( strcmp(type,"int") == 0 ) ofType = INT;
  else ofType = DBL;
  
  fillThis->typeInt = ofType;
  fillThis->varName = (char *)calloc(len,sizeof(char));
  strcpy(fillThis->varName,name);

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

dataStruct *varCpy(dataStruct *left, dataStruct *right) {
  strcpy(left->varName,right->varName);
  left->typeInt = right->typeInt;
  left->data = right->data;
  return left;
}

void printVar(dataStruct *printThis) {
  if ( printThis == NULL || printThis->data == NULL  ) return;

  switch (printThis->typeInt) {
  case STR: 
    printf("Variable: %s\n\tType: String\tValue: %s\n",
	   printThis->varName,(char *)printThis->data);
    break;
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

