#ifndef properties_h_
#define properties_h_

#include "datatypes.h"
#include <stdio.h>

int getDepth(MkvsynthMetaData *metaData);
int getBytes(MkvsynthMetaData *metaData);
int isMetaDataValid(MkvsynthMetaData *metaData);
int getLinesize(MkvsynthMetaData *metaData);

#endif
