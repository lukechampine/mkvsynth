#ifndef bufferAllocation_h_
#define bufferAllocation_h_

#include "datatypes.h"
#include <semaphore.h>

MkvsynthOutput *createOutputBuffer();
MkvsynthInput *createInputBuffer(MkvsynthOutput *output);

#endif
