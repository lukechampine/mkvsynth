#ifndef COLORSPACING_H_
#define COLORSPACING_H_

#include "../jarvis/jarvis.h"

/*******************************************************************************
 * A pixel is a 64 bit field for data. This can be divided any way desired.    *
 *                                                                             *
 * By convention, data is always stored as the type that is closest to the     *
 * data size. This is for endian reason. If you are working with four fields   *
 * that are 2 bytes long, the type 'uint16_t' should be used. If you are       *
 * using a single two byte field followed by a four byte field, you should     *
 * use one uint16_t followed by a uint32_t.                                    *
 ******************************************************************************/

typedef struct MkvsynthPixel MkvsynthPixel;

struct MkvsynthPixel {
	union {
		struct {
			uint16_t red;
			uint16_t green;
			uint16_t blue;
		} rgb48;

		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		} rgb24;

		struct {
			uint16_t luma;
			uint16_t cb;
			uint16_t cr;
		} yuv444_48;

		struct {
			uint8_t luma;
			uint8_t cb;
			uint8_t cr;
		} yuv444_24;

		struct {
			uint8_t channel[8];
		} generic;
	};
};

#include "pixels.h"
#include "properties.h"

#endif
