/* stores a lot of variable with easy lookup by name */

#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>

typedef struct {
  void *table;

  int size, tableSize, dataSize;
} hashTable;

hashTable * makeHashTable(int dataSize, hashTable *makeThis);
void deleteHashTable(hashTable *deleteThis);
void resizeTable(hashTable *resizeThis);
void *insertData(void *insertThis, hashTable *inThis);
void *findData(char *findThis, hashTable *inThis);

int hash(char *hashThis);

#endif
