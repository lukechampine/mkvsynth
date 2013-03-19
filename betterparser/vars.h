/* This header describes how we do variable lookup in the parser */

enum typeVals { INT, STR, DBL };

/* dataStruct and dataType define how variables *
 * are actually stored: we have storage that    *
 * tells what the name of the variable is, what *
 * type of variable it is, and holds a pointer  *
 * to the actual memory that it uses.           */

/* pointer to the correct data type */
union dataType {
  int *isInt;
  char *isStr;
  double *isDbl;
  /* other possibilities?!!? */
};

/* holds info about the variable */
struct dataStruct {
  char varName[32];
  enum typeVals typeInt;
  union dataType typePtr;
};
struct dataStruct *makeVar(struct dataStruct **fillThis, 
			   char *type, char *name);
struct dataStruct *varCpy(struct dataStruct *left, struct dataStruct *right);
void printVar(struct dataStruct *printThis);


/* stores a lot of variable with easy lookup by name */
struct varStorage {
  struct dataStruct *varTable;

  int size, tableSize;
};
struct varStorage makeVarStorage();
void deleteVarStorage(struct varStorage *deleteThis);
void resizeTable(struct varStorage *resizeThis);
struct dataStruct *insertData(struct dataStruct *insertThis,
			      struct varStorage *inThis);
struct dataStruct *findData(char *findThis, struct varStorage *inThis);

int hash(char *hashThis);

