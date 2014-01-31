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
			uint16_t r;
			uint16_t g;
			uint16_t b;
		} rgb48;

		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
		} rgb24;

		struct {
			uint16_t y;
			uint16_t u;
			uint16_t v;
		} yuv444_48;

		struct {
			uint8_t y;
			uint8_t u;
			uint8_t v;
		} yuv444_24;
		
		struct {
			uint8_t h;
			uint8_t s;
			uint8_t v;
		} hsv24;
		
		struct {
			uint16_t h;
			uint16_t s;
			uint16_t v;
		} hsv48;
		
		struct {
			uint8_t h;
			uint8_t s;
			uint8_t l;
		} hsl24;
		
		struct {
			uint16_t h;
			uint16_t s;
			uint16_t l;
		} hsl48;

		struct {
			uint8_t channel[8];
		} generic;
	};
};

#include "pixels.h"
#include "properties.h"

#endif
