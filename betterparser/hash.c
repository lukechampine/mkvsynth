/*******************************************************************************/
/*******************************************************************************/
/***** functions for dealing with the hash table class.                    *****/
/*******************************************************************************/
/*******************************************************************************/

#include <stdio.h>

#include "hash.h"

#include "vars.h"

// This function initializes a hashtable with data that has
// a size of dataSize.
hashTable * makeHashTable(int dataSize, hashTable *makeThis) {
  makeThis->dataSize = dataSize;
  makeThis->size = 0;
  makeThis->table = (void *)calloc(32,dataSize);
  makeThis->tableSize = 32;

  return makeThis;
}

void deleteHashTable(hashTable *deleteThis) {
  free(deleteThis->table);
}

/* This function expands a table once   *
 * it is no longer sufficiently sparse. */
void resizeTable(hashTable *resizeThis) {
  // allocates twice as much memory for our newtable
  int oldSize = resizeThis->tableSize, i;
  void *newTable = (void *)calloc(resizeThis->tableSize*2,resizeThis->dataSize),
    *oldTable = resizeThis->table;
  resizeThis->table = newTable;

  // adds all the values in the old table to the new table
  for ( i = 0; i < oldSize; ++i ) 
    insertData(oldTable+(i*resizeThis->dataSize),resizeThis);

  // frees the old memory
  free(oldTable);
}

// This is the lame hash function for the table
int hash(char *hashThis) {
  if ( hashThis == NULL ) return 0;
  int i = 0, hashVal = 0;
  
  while ( hashThis[i] != '\0' ) {
    hashVal += ((int)hashThis[i])*(i+1);
    ++i;
  }

  return hashVal;
}


void *insertData(void *insertThis, hashTable *inThis) {
  // returns null if the hashTable or the insertion is NULL
  if ( inThis == NULL || insertThis == NULL ) return NULL;

  // resizes if we have a good chance of collisions.
  if ( inThis->size*2 > inThis->tableSize ) resizeTable(inThis);

  // hashes the inserted value
  void *ret;
  int loc = hash(*(char **)insertThis) % inThis->tableSize;

  // finds first empty location in the hash table.
  while ( *((char **)(ret = inThis->table + loc*inThis->dataSize)) != NULL ) {
    // returns null if the variable is already inserted.
    if ( strcmp(*(char **)ret,*(char **)insertThis) == 0 ) return NULL;

    // increments location
    loc = ( loc + 1 ) % inThis->tableSize;
    ret = inThis->table+loc*inThis->dataSize;
  }

  // inserts the value and returns a pointer to what we inserted
  memcpy(ret,insertThis,inThis->dataSize);
  return ret;
}

void *findData(char *findThis, hashTable *inThis, int insert) {
  // hashes the string and points res to the correct location
  int loc = hash(findThis) % inThis->tableSize;
  void *res = inThis->table + loc*inThis->dataSize;

  /* requires that the first thing in our *
   * entries is a pointer to the name.    */
  while ( *(char **)res != NULL && 
	  strcmp(*(char **)res,findThis) != 0 )
    res = 
      inThis->table + (loc = ( loc + 1 ) % inThis->tableSize)*inThis->dataSize;
  
  // inserts an empty value with the proper name into the table.
  if ( *(char **)res == NULL ) {
    if ( insert ) {
      memset(res,0,inThis->dataSize);
      *(char **) res = (char *)calloc(strlen(findThis)+1,sizeof(char));
      strcpy(*(char **)res,findThis);
    }
    else return NULL;
  }
  
  // returns our result.
  return res;
}
