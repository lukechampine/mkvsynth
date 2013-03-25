#include <string.h>
#include <stdio.h>

#include "hash.h"
#include "vars.h"
#include "funs.h"

hashTable vars;

int parseline( char *lineBuff) {
  char *fun, *var, *args;

  var = strtok(lineBuff,".");
  while ( ( fun = strtok(NULL,").(") ) != NULL ) {
    
    if( strcmp(fun,"init") == 0 ) {
      dataStruct makeThis;
      memset(&makeThis,0,sizeof(dataStruct));

      args = strtok(NULL,",");

      // creates a string with name var
      makeVar(&makeThis,args,var);
    
      // need to get second arg
      args = strtok(NULL,"\"");
      sprintf((char *)makeThis.data,args); // sets the value to the argument

      insertData(&makeThis,&vars);
    }

    else {
      dataStruct *found = (dataStruct *)findData(var,&vars);
    
      /* have to apply the correct filter */
      if ( strcmp(fun,"append") == 0 ) {
	args = strtok(NULL,"\"");
	strcat((char *)(found->data),args);
      }
      else if ( strcmp(fun,"output") == 0 ) printVar(found);
    }
  }

  return 0;
}


int main() {
  char lineBuff[1024];
  makeHashTable(sizeof(dataStruct),&vars);

  while ( fgets(lineBuff,1024,stdin) != NULL ) 
    parseline(lineBuff);
  return 0;
}
