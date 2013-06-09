#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "hash.h"
#include "funs.h"

/* This header defines how additional functions can be added *
 * to the scripting language. As it stands, this requires a  *
 * fuller implementation than is currently available, but    *
 * this will hold a library of basic functions while that is *
 * not yet implemented. Note that all the functions are      *
 * required to take a void pointer and a string as input.    */

// basic functions
void *dataInit(void *data, char *args);
void *dataStrAppend(void *data,char *args);
void *dataStrOutput(void *data,char *args);

// adds the functions in the library to the working set
void addFunctions(hashTable *addTo);
