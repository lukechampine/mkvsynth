/* stores a lot of variable with easy lookup by name */

#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>

/* the idea of this hash table is to basically be templated, *
 * in that it does not need to be reimplemented for any data *
 * type that needs to be stored. However, due to C's lack of *
 * support for this, it requires the use of void pointers,   *
 * which can require a bit less intuitive code.              */
typedef struct {
  void *table;

  int size, tableSize, dataSize;
} hashTable;

// basically member function equivalents
hashTable *makeHashTable(int dataSize, hashTable *makeThis);
void deleteHashTable(hashTable *deleteThis);
void resizeTable(hashTable *resizeThis);
void *insertData(void *insertThis, hashTable *inThis);
void *findData(char *findThis, hashTable *inThis, int insert);

int hash(char *hashThis);

#endif
