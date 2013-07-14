#ifndef _pixelMath_h_
#define _pixelMath_h_

#include <stdbool.h>
#include "helperfunctions.c"

bool confirmColorSpace(int colorspaceCode) {
	if(colorspaceCode == GENERIC_RGB)
		return true;

	return false;
}

long long getPixel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left) {
	// Need to add support/checks for RGB24,RGB32,RGB36,RGB48,RGB64
	// For the time, other RGB values are not checked
	//
	// Still not quite sure how to do all of this
}

long long putPixel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left) {

}

long long getChannel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left, int channel) {

}

long long putChannel(MkvsynthMetaData *metaData, uint8_t *payload, int top, int left, int channel) {

}
#endif
