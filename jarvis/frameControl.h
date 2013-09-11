#include "jarvis.h"

struct MkvsynthFrame *getFrame(struct MkvsynthInput *params);
struct MkvsynthFrame *getReadOnlyFrame(struct MkvsynthInput *params);
void putFrame(struct MkvsynthOutput *params, uint8_t *payload);
void clearFrame(struct MkvsynthFrame *usedFrame);
void clearReadOnlyFrame(struct MkvsynthFrame *usedFrame);
