#ifndef MPL_DATATYPES_H_
#define MPL_DATATYPES_H_

#include "../jarvis/datatypes.h"

// A pixel is just a 64bit field for data.
// if your depth is 16, you can have up to 4 channels
// if your depth is 8, you can have up to 8 channels
typedef struct MkvsynthPixel MkvsynthPixel;
struct MkvsynthPixel {
	uint8_t channel[8];
};

#endif
