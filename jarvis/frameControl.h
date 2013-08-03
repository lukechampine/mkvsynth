#ifndef frameControl_h_
#define frameControl_h_

#include <string.h>
#include "datatypes.h"

struct MkvsynthFrame *getFrame(struct MkvsynthInput *params);
void putFrame(struct MkvsynthOutput *params, uint8_t *payload);
void clearFrame(struct MkvsynthFrame *usedFrame, int freePayload);

#endif
