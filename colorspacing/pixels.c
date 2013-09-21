#include "pixels.h"

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

	MkvsynthPixel pixel = {{0}};
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
void addPixel(MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength) {

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

//pulls the red value from the pixel
uint16_t getRed (MkvsynthPixel *pixel, MkvsynthMetaData *metaData) {
	short colorspace = metaData->colorspace;
	uint16_t value = 0;
	//these are only needed for the yuv colorspaces
	int y = 0;
	int v = 0;
	float yf = 0.0;
	float vf = 0.0;
	float result1 = 0;
	int check = 0;
	
	uint16_t *deepChannel = (uint16_t *)pixel->channel
	switch(colorspace){
		case MKVS_RGB48:
			value = deepChannel[0];
			break;
		case MKVS_RGB24:
			value = pixel->channel[0];
			value = value * 256;
			break;
		case MKVS_YUV444_48:
			y = deepChannel[0];
			v = deepChannel[2];
			yf = y;
			vf = v;
			result1 = yf + vf / .877 + .5;
			check = (int) result1;
			if(check > 65535){
				//this color is not in the rgb colorspace.
				value = 65535;
				break;
			}
			value = check;
			break;
		case MKVS_YUV444_24:
			y = pixel->channel[0];
			y = y * 256;
			v = pixel->channel[2];
			v = v * 256;
			yf = y;
			vf = v;
			result1 = yf + vf / .877 + .5;
			check = (int) result1;
			if(check > 65535){
				//this color is not in the rgb colorspace.
				value = 65535;
				break;
			}
			value = check;
			break;
	}
	return value;
}

//pulls the green value from the pixel
uint16_t getGreen (MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	short colorspace = metaData->colorspace;
	uint16_t value = 0;
	int y = 0;
	int u = 0;
	int v = 0;
	int check = 0;
	float yf = 0.0;
	float uf = 0.0;
	float vf = 0.0;
	float result1 = 0;
	switch(colorspace){
		case MKVS_RGB48:
			value = deepChannel[1];
			break;
		case MKVS_RGB24:
			value = pixel->channel[1];
			value = value * 256;
			break;
		case MKVS_YUV444_48:
			y = deepChannel[0];
			u = deepChannel[1];
			v = deepChannel[2];
			yf = y;
			uf = u;
			vf = v;
			result1 = yf - .395 * uf - .581 * vf + .5;
			check = (int) result1;
			if(check < 0){
				//this color is not in the rgb colorspace.
				value = 0;
				break;
			}
			value = check;
			break;
		case MKVS_YUV444_24:
			y = pixel->channel[0];
			y = y * 256;
			u = pixel->channel[1];
			u = u * 256;
			v = pixel->channel[2];
			v = v * 256;
			yf = y;
			uf = u;
			vf = v;
			result1 = yf - .395 * uf - .581 * vf + .5;
			check = (int) result1;
			if(check < 0){
				//this color is not in the rgb colorspace.
				value = 0;
				break;
			}
			value = check;
			break;
	}
	return value;
}

//pulls the red value from the pixel
uint16_t getBlue (MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	short colorspace = metaData->colorspace;
	uint16_t value = 0;
	int y = 0;
	int u = 0;
	int check = 0;
	float yf = 0.0;
	float uf = 0.0;
	float result1 = 0;
	switch(colorspace){
		case MKVS_RGB48:
			value = deepChannel[2];
			break;
		case MKVS_RGB24:
			value = pixel->channel[2];
			value = value * 256;
			break;
		case MKVS_YUV444_48:
			y = deepChannel[0];
			u = deepChannel[1];
			yf = y;
			uf = u;
			result1 = yf + uf / .492 + .5;
			check = (int) result1;
			if(check > 65535){
				//this color is not in the rgb colorspace.
				value = 0;
				break;
			}
			value = check;
			break;
		case MKVS_YUV444_24:
			y = pixel->channel[0];
			y = y * 256;
			u = pixel->channel[1];
			u = u * 256;
			yf = y;

			result1 = yf + uf / .492 + .5;
			check = (int) result1;
			if(check > 65535){
				//this color is not in the rgb colorspace.
				value = 65535;
				break;
			}
			value = check;
			result1 = y + u / .492 + .5;
			check = (int) result1;
			if(check > 65535){
				//this color is not in the rgb colorspace.
				value = 65535;
				break;
			}
			value = check;
			break;
	}
	return value;
}
