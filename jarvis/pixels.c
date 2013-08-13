#ifndef pixels_c_
#define pixels_c_

#include "pixels.h"

MkvsynthPixel getPixel(MkvsynthFrame *frame, int widthOffset, int heightOffset) {
	MkvsynthPixel returnVal = {0};
	return returnVal;
}

void putPixel(MkvsynthPixel *pixel, uint8_t *payload, int widthOffset, int heightOffset) {
	// do something
}

// Overlay takes one pixel and adds values based on the overlay.
// currently there is no check for overflow, but that will be added.
void overlay(MkvsynthPixel *destination, MkvsynthPixel *source, double strength) {

#ifdef DEBUG
	if(destination->colorspace != source->colorspace) {
		printf("overlay() debug error: source and destination colorspace must match\n");
		exit(0);
	}

	if(destination->colorspace < 1 || destination->colorspace > 4) {
		prints("overlay() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	uint8_t *destChars       = (uint8_t *)destination->channel;
	uint8_t *sourceChars     = (uint8_t *)source->channel;
	switch(destination->colorspace) {
		case MKVS_RGB48:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
		case MKVS_RGB24:
			destChars[0]            += source->channel[0] * strength;
			destChars[1]            += source->channel[1] * strength;
			destChars[2]            += source->channel[2] * strength;
			break;
		case MKVS_YUV444_48:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
		case MKVS_YUV444_24:
			destChars[0]            += source->channel[0] * strength;
			destChars[1]            += source->channel[1] * strength;
			destChars[2]            += source->channel[2] * strength;
			break;
	}
}

#endif
