#include <string.h>
#include <stdio.h>

#include "vars.h"

struct varStorage vars;

int parseline( char *lineBuff) {
  char *fun, *var, *args;
  struct dataStruct *makeThis = NULL;
  
  fun = strtok(lineBuff," ");
  var = strtok(NULL," ");
  args = strtok(NULL," =(\"");

  if( strcmp(fun,"string") == 0 || strcmp(fun,"int") == 0 || 
      strcmp(fun,"double") == 0 ) {
    makeVar(&makeThis,fun,var);
    strcat(makeThis->typePtr.isStr,args);
    insertData(makeThis,&vars);
  }

  else {
    makeThis = findData(var,&vars);
    if ( strcmp(fun,"filter") == 0 ) {
      /* have to apply the correct filter */
      if ( strcmp(args,"append") == 0 ) {
	args = strtok(NULL,"\"");
	strcat(makeThis->typePtr.isStr,args);
      }
    }
    else if ( strcmp(fun,"output") == 0 ) {
      printVar(makeThis);
    }
  }
    
  return 0;
}


int main() {
  char lineBuff[1024];
  vars = makeVarStorage();

  while ( fgets(lineBuff,1024,stdin) != NULL ) 
    parseline(lineBuff);
  return 0;
}
