#ifndef spawn_h_
#define spawn_h_

#include "datatypes.h"
#include <stdlib.h>

void mkvsynthQueue(void *filterParams, void *(*filter) (void *));
void mkvsynthSpawn();
void mkvsynthJoin();

#endif
