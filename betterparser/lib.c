#include "lib.h"

void *dataInit(void *data, char *args) {
  dataStruct *makeThis = (dataStruct *)data;
  char *type = strtok(args,","),
    *val = strtok(NULL,"\"");

  makeVar(makeThis,type);
  switch ( makeThis->typeInt ) {
  case STR:
    strcpy((char *)makeThis->data,val);
    break;
  default: break;
  }
  return makeThis;
}

void *dataStrAppend(void *data,char *args) {
  char * appendThis = strtok(args,"\"");
  strcat((char *)(((dataStruct *)data)->data),appendThis);
  return data;
}

void * dataStrOutput(void *data,char *args) {
  printf("Variable: %s\n\tType: String\tValue: %s\n",
	 (char *)((dataStruct *)data)->varName,
	 (char *)(((dataStruct *)data)->data));
  return data;
}

void addFunctions(hashTable *addTo) {
  function init, append, output;
  makeFun(&init,"init",&dataInit);
  makeFun(&append,"append",&dataStrAppend);
  makeFun(&output,"output",&dataStrOutput);
  insertData(&init,addTo);
  insertData(&append,addTo);
  insertData(&output,addTo);
}
