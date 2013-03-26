#include <string.h>
#include <stdio.h>

#include "hash.h"
#include "vars.h"
#include "funs.h"
#include "lib.h"

hashTable vars;
hashTable funs;

int parseline( char *lineBuff) {
  char *var, *funName, *args, **strtokPtr = &lineBuff;
  function *fun;

  var = strtok_r(lineBuff,".",strtokPtr);
  dataStruct *found = (dataStruct *)findData(var,&vars,1);
  while ( ( funName = strtok_r(NULL," \t\n.(",strtokPtr) ) != NULL ) {
    fun = findData(funName,&funs,0);
    if ( fun != NULL ) {
      args = strtok_r(NULL,")",strtokPtr);
      fun->function(found,args);
    }
    else printf("No function %s found.",funName);
  }

  return 0;
}


int main() {
  char lineBuff[1024];
  makeHashTable(sizeof(dataStruct),&vars);
  makeHashTable(sizeof(function),&funs);
  addFunctions(&funs);

  while ( fgets(lineBuff,1024,stdin) != NULL ) 
    parseline(lineBuff);
  return 0;
}
