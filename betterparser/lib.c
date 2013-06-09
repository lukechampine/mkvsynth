#include "lib.h"

// This function serves to initialize some data
void *dataInit(void *data, char *args) {
  // requires true data is a datastruct
  dataStruct *makeThis = (dataStruct *)data;
  char *type = strtok(args,","),
    *val = strtok(NULL,"\"");

  // gets type
  makeVar(makeThis,type);
  
  // gets data
  switch ( makeThis->typeInt ) {
  case STR:
    strcpy((char *)makeThis->data,val);
    break;
  default: break;
  }
  
  return makeThis;
}

// This function appends data to a string
void *dataStrAppend(void *data,char *args) {
  // gets new data
  char * appendThis = strtok(args,"\"");
  
  // appends it
  strcat((char *)(((dataStruct *)data)->data),appendThis);
  return data;
}

// This function outputs a string
void * dataStrOutput(void *data,char *args) {
  printf("Variable: %s\n\tType: String\tValue: %s\n",
	 (char *)((dataStruct *)data)->varName,
	 (char *)(((dataStruct *)data)->data));
  return data;
}


// adds the functions in this library to the working set
void addFunctions(hashTable *addTo) {
  function init, append, output;

  // creates the function structures
  makeFun(&init,"init",&dataInit);
  makeFun(&append,"append",&dataStrAppend);
  makeFun(&output,"output",&dataStrOutput);

  // adds them to the hash table
  insertData(&init,addTo);
  insertData(&append,addTo);
  insertData(&output,addTo);
}
