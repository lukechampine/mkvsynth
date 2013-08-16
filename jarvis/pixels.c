#ifndef pixels_c_
#define pixels_c_

#include "pixels.h"
#include <stdio.h>

MkvsynthPixel getPixel(uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {

#ifdef DEBUG
	if(metaData->colorspace < 1 || metaData->colorspace > 4) {
		prints("getPixel() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	MkvsynthPixel pixel;
	uint16_t *deepChannel = (uint16_t *)pixel.channel;
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = heightOffset * metaData->width + widthOffset;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			deepChannel[0]           = deepPayload[offset*3];
			deepChannel[1]           = deepPayload[offset*3+1];
			deepChannel[2]           = deepPayload[offset*3+2];
			break;
		case MKVS_RGB24:
			pixel.channel[0]         = payload[offset*3];
			pixel.channel[1]         = payload[offset*3+1];
			pixel.channel[2]         = payload[offset*3+2];
			break;
		case MKVS_YUV444_48:
			deepChannel[0]           = deepPayload[offset*3];
			deepChannel[1]           = deepPayload[offset*3+1];
			deepChannel[2]           = deepPayload[offset*3+2];
			break;
		case MKVS_YUV444_24:
			pixel.channel[0]         = payload[offset*3];
			pixel.channel[1]         = payload[offset*3+1];
			pixel.channel[2]         = payload[offset*3+2];
			break;
	}

	return pixel;
}

void putPixel(MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {
	
#ifdef DEBUG
	if(metaData->colorspace < 1 || metaData->colorspace > 4) {
		prints("getPixel() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	uint16_t *deepChannel = (uint16_t *)pixel->channel;
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = heightOffset * metaData->width + widthOffset;
	offset *= 3;
	//printf("%i\n", offset);
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
		prints("overlay() debug error: colorspace is not valid.\n");
		exit(0);
	}
#endif

	uint16_t *destChars          = (uint16_t *)destination->channel;
	uint16_t *sourceChars        = (uint16_t *)source->channel;
	switch(colorspace) {
		case MKVS_RGB48:
			//printf("[%llx] + [%llx] * %g= ", *((unsigned long long *)&destination->channel[0]), *((unsigned long long *)&source->channel[0]), strength);
			destChars[0]            += sourceChars[0] * strength;
			destChars[1]            += sourceChars[1] * strength;
			destChars[2]            += sourceChars[2] * strength;
			//printf("[%llx]\n", *((unsigned long long *)&destination->channel[0]));
			break;
		case MKVS_RGB24:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
		case MKVS_YUV444_48:
			destChars[0]            += source->channel[0] * strength;
			destChars[1]            += source->channel[1] * strength;
			destChars[2]            += source->channel[2] * strength;
			break;
		case MKVS_YUV444_24:
			destination->channel[0] += source->channel[0] * strength;
			destination->channel[1] += source->channel[1] * strength;
			destination->channel[2] += source->channel[2] * strength;
			break;
	}
}

#endif
