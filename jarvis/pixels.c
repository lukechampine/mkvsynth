#ifndef pixels_c_
#define pixels_c_

#include "pixels.h"
#include <stdio.h>

/////////////////////////////////////////////////
// One of the major assumptions of Mkvsynth is that
// 16 bit colorspaces will be stored as uint16_t and
// 8 bit colorspaces will be stored as uint8_t.
// This is only enforced by convention and is hard to
// error-check
/////////////////////////////////////////////////

MkvsynthPixel getPixel(uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {

#ifdef DEBUG
	if(metaData->colorspace < 1 || metaData->colorspace > 4) {
		printf("getPixel() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	MkvsynthPixel pixel = {0};
	uint16_t *deepChannel = (uint16_t *)pixel.channel;
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = 3 * (heightOffset * metaData->width + widthOffset);

	switch(metaData->colorspace) {
		case MKVS_RGB48:
			deepChannel[0]           = deepPayload[offset];
			deepChannel[1]           = deepPayload[offset+1];
			deepChannel[2]           = deepPayload[offset+2];
			break;
		case MKVS_RGB24:
			pixel.channel[0]         = payload[offset];
			pixel.channel[1]         = payload[offset+1];
			pixel.channel[2]         = payload[offset+2];
			break;
		case MKVS_YUV444_48:
			deepChannel[0]           = deepPayload[offset];
			deepChannel[1]           = deepPayload[offset+1];
			deepChannel[2]           = deepPayload[offset+2];
			break;
		case MKVS_YUV444_24:
			pixel.channel[0]         = payload[offset];
			pixel.channel[1]         = payload[offset+1];
			pixel.channel[2]         = payload[offset+2];
			break;
	}

	return pixel;
}

void putPixel(MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {
	
#ifdef DEBUG
	if(metaData->colorspace < 1 || metaData->colorspace > 4) {
		printf("putPixel() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	uint16_t *deepChannel = (uint16_t *)pixel->channel;
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = 3 * (heightOffset * metaData->width + widthOffset);

	switch(metaData->colorspace) {
		case MKVS_RGB48:
			deepPayload[offset]      = deepChannel[0];
			deepPayload[offset+1]    = deepChannel[1];
			deepPayload[offset+2]    = deepChannel[2];
			break;
		case MKVS_RGB24:
			payload[offset]          = pixel->channel[0];
			payload[offset+1]        = pixel->channel[1];
			payload[offset+2]        = pixel->channel[2];
			break;
		case MKVS_YUV444_48:
			deepPayload[offset]      = deepChannel[0];
			deepPayload[offset+1]    = deepChannel[1];
			deepPayload[offset+2]    = deepChannel[2];
			break;
		case MKVS_YUV444_24:
			payload[offset]          = pixel->channel[0];
			payload[offset+1]        = pixel->channel[1];
			payload[offset+2]        = pixel->channel[2];
			break;
	}
}

// Overlay takes one pixel and adds values based on the overlay.
// overflow checks will need to be added
void addPixel(MkvsynthPixel *destination, MkvsynthPixel *source, short colorspace, double strength) {

#ifdef DEBUG
	if(colorspace < 1 || colorspace > 4) {
		printf("overlay() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	uint16_t *destDeep          = (uint16_t *)destination->channel;
	uint16_t *sourceDeep        = (uint16_t *)source->channel;

	switch(colorspace) {
		case MKVS_RGB48:
			destDeep[0]             += sourceDeep[0] * strength;
			destDeep[1]             += sourceDeep[1] * strength;
			destDeep[2]             += sourceDeep[2] * strength;
			break;
		case MKVS_RGB24:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
		case MKVS_YUV444_48:
			destDeep[0]             += sourceDeep[0] * strength;
			destDeep[1]             += sourceDeep[1] * strength;
			destDeep[2]             += sourceDeep[2] * strength;
			break;
		case MKVS_YUV444_24:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
	}
}

#endif
