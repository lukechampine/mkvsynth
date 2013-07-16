#ifndef _pixelMath_h_
#define _pixelMath_h_

#include <stdbool.h>
#include "helperfunctions.c"

bool confirmColorSpace(MkvsynthMetaData *metaData) {
	if(colorspaceCode == GENERIC_RGB)
		return true;

	return false;
}

long long getPixel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left) {
}

long long putPixel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left) {
}

long long getChannel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left, int channel) {
	if(metaData->colorspace == GENERIC_RGB) {
		 
		int bitOffset = width*depth*channels+height*depth*channels;
		int byteOffset = bitOffset / 8;
		int leftovers = bitOffset % 8;

		uint8_t leadingMask = (1 << leftovers) - 1;
		if(
	}
}

long long putChannel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left, int channel) {
}
#endif
