#ifndef COLORSPACING_H_
#define COLORSPACING_H_

// Ideally the colorspacing libraries would be independent of the jarvis
//   libraries. 
#include "../jarvis/jarvis.h"

/*******************************************************************************
 * A pixel is a 64 bit field for data. This can be divided any way desired.    *
 *                                                                             *
 * By convention, data is always stored as the type that is closest to the     *
 * 	data size. This is for endian reason. If you are working with four fields  *
 * 	that are 2 bytes long, the type 'uint16_t' should be used. If you are      *
 *  using a single two byte field followed by a four byte field, you should    *
 *  use one uint16_t followed by a uint32_t.                                   *
 ******************************************************************************/

typedef struct MkvsynthPixel MkvsynthPixel;

struct MkvsynthPixel {
	uint8_t channel[8];
};

#include "pixels.h"
#include "properties.h"

#endif
