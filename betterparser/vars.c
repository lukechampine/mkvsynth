#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vars.h"

/********************************************************************************/
/********************************************************************************/
/***** Simpler functions for dealing with the variable storage class        *****/
/********************************************************************************/
/********************************************************************************/

struct dataStruct *makeVar(struct dataStruct **fillThis, 
			   char *type, char *name) {
  *fillThis = (struct dataStruct *)calloc(1,sizeof(struct dataStruct));
  enum typeVals ofType;
  if ( strcmp(type,"string") == 0 ) ofType = STR;
  else if ( strcmp(type,"int") == 0 ) ofType = INT;
  else ofType = DBL;
  
  (*fillThis)->typeInt = ofType;
  strcpy((*fillThis)->varName,name);

  switch ( ofType ) {
  case STR: (*fillThis)->typePtr.isStr = (char *)calloc(1024,sizeof(char));
    break;
  case INT: (*fillThis)->typePtr.isInt = (int *)calloc(32,sizeof(int));
    break;
  case DBL: (*fillThis)->typePtr.isDbl = 
      (double *)calloc(32,sizeof(double));
    break;
  }

  return *fillThis;
}

struct dataStruct *varCpy(struct dataStruct *left, struct dataStruct *right) {
  strcpy(left->varName,right->varName);
  left->typeInt = right->typeInt;
  left->typePtr = right->typePtr;
  return left;
}

void printVar(struct dataStruct *printThis) {
  if ( printThis == NULL || printThis->typePtr.isDbl == NULL  ) return;

  switch (printThis->typeInt) {
  case STR: 
    printf("Variable: %s\n\tType: String\tValue: %s\n",
	   printThis->varName,printThis->typePtr.isStr);
    break;
  case INT: 
    printf("Variable: %s\n\tType: Int\tValue: %d\n",
	   printThis->varName,*printThis->typePtr.isInt);
    break;
  case DBL: 
    printf("Variable: %s\n\tType: Double\tValue: %e\n",
	   printThis->varName,*printThis->typePtr.isDbl);
    break;
  }
}

/********************************************************************************/
/********************************************************************************/
/***** functions for dealing with the hash table class.                     *****/
/********************************************************************************/
/********************************************************************************/

struct varStorage makeVarStorage() {
  struct varStorage makeThis;
  
  makeThis.size = 0;
  makeThis.varTable = (struct dataStruct *)calloc(32,sizeof(struct dataStruct));
  makeThis.tableSize = 32;

  return makeThis;
}

void deleteVarStorage(struct varStorage *deleteThis) {
  free(deleteThis->varTable);
}

void resizeTable(struct varStorage *resizeThis) {
  int oldSize = resizeThis->tableSize, i;
  struct dataStruct *newTable = (struct dataStruct *) 
    calloc(resizeThis->tableSize*2,sizeof(struct dataStruct)),
    *oldTable = resizeThis->varTable;
  resizeThis->varTable = newTable;

  for ( i = 0; i < oldSize; ++i ) 
    insertData(&oldTable[i],resizeThis);

  free(oldTable);
}

int hash(char *hashThis) {
  if ( hashThis == NULL ) return 0;
  int i = 0, hashVal = 0;
  
  while ( hashThis[i] != '\0' ) {
    hashVal += ((int)hashThis[i])*i;
    ++i;
  }
  
  return hashVal;
}


struct dataStruct *insertData(struct dataStruct *insertThis,
			      struct varStorage *inThis) {
  if ( inThis == NULL || insertThis == NULL ) return NULL;
  if ( inThis->size*2 > inThis->tableSize ) resizeTable(inThis);
 
  struct dataStruct * ret;
  int loc = hash(insertThis->varName) % inThis->tableSize;
  
  if ( strcmp((ret = &inThis->varTable[loc])->varName,insertThis->varName) == 0 )
    return NULL;

  while ( ret->varName[0] != '\0' ) {
    loc = ( loc + 1 ) % inThis->tableSize;
    ret = &inThis->varTable[loc];
  }
  
  varCpy(&inThis->varTable[loc],insertThis);
  return &inThis->varTable[loc];
}

struct dataStruct *findData(char *findThis, struct varStorage *inThis) {
  struct dataStruct *res;

  int loc = hash(findThis) % inThis->tableSize;
  res = &inThis->varTable[loc];

  while ( strcmp(res->varName,findThis) != 0 &&
	  strlen(res->varName) > 0 ) {
    loc = ( loc + 1 ) % inThis->tableSize;
    res = &inThis->varTable[loc];
  }

  if ( strcmp(res->varName,findThis) == 0 ) return res;
  else return NULL;
}
