#include "pixels.h"

#ifdef DEBUG
void checkColorspace (int colorspace, char *functionName) {
	if(colorspace < 1 || colorspace > 4) {
		printf("%s() debug error: colorspace is not valid.\n", functionName);
		exit(0);
	}
}
#endif

/******************************************************************************
 * Pulls a pixel out of payload given the widthOffset and heightOffset.       *
 * Payload starts at (0, 0) in the top left corner of the image.              *
 *****************************************************************************/

MkvsynthPixel getPixel (uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {

#ifdef DEBUG
	checkColorspace(metaData->colorspace, "getPixel");
#endif

	MkvsynthPixel pixel = {{{0}}};
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = 3 * (heightOffset * metaData->width + widthOffset);

	switch(metaData->colorspace) {
		case MKVS_RGB48:
			pixel.rgb48.r                 = deepPayload[offset];
			pixel.rgb48.g                 = deepPayload[offset+1];
			pixel.rgb48.b                 = deepPayload[offset+2];
			break;
		case MKVS_RGB24:
			pixel.rgb24.r                 = payload[offset];
			pixel.rgb24.g                 = payload[offset+1];
			pixel.rgb24.b                 = payload[offset+2];
			break;
		case MKVS_YUV444_48:
			pixel.yuv444_48.y             = deepPayload[offset];
			pixel.yuv444_48.u             = deepPayload[offset+1];
			pixel.yuv444_48.v             = deepPayload[offset+2];
			break;
		case MKVS_YUV444_24:
			pixel.yuv444_24.y             = payload[offset];
			pixel.yuv444_24.u             = payload[offset+1];
			pixel.yuv444_24.v             = payload[offset+2];
			break;
	}

	return pixel;
}

/******************************************************************************
 * Takes a pixel as input and puts it in the offset location in the payload,  *
 * where payload is a video frame in the frame stream.                        *
 *****************************************************************************/
void putPixel (MkvsynthPixel *pixel, uint8_t *payload, MkvsynthMetaData *metaData, int widthOffset, int heightOffset) {
	
#ifdef DEBUG
	checkColorspace(metaData->colorspace, "putPixel");
#endif

	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = 3 * (heightOffset * metaData->width + widthOffset);

	switch(metaData->colorspace) {
		case MKVS_RGB48:
			deepPayload[offset]           = pixel->rgb48.r;
			deepPayload[offset+1]         = pixel->rgb48.g;
			deepPayload[offset+2]         = pixel->rgb48.b;
			break;
		case MKVS_RGB24:
			payload[offset]               = pixel->rgb24.r;
			payload[offset+1]             = pixel->rgb24.g;
			payload[offset+2]             = pixel->rgb24.b;
			break;
		case MKVS_YUV444_48:
			deepPayload[offset]           = pixel->yuv444_48.y;
			deepPayload[offset+1]         = pixel->yuv444_48.u;
			deepPayload[offset+2]         = pixel->yuv444_48.v;
			break;
		case MKVS_YUV444_24:
			payload[offset]               = pixel->yuv444_24.y;
			payload[offset+1]             = pixel->yuv444_24.u;
			payload[offset+2]             = pixel->yuv444_24.v;
			break;
	}
}

void addPixel (MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength) {

#ifdef DEBUG
	checkColorspace(metaData->colorspace, "addPixel");
#endif

	switch(colorspace) {
		case MKVS_RGB48:
			destination->rgb48.r         += source->rgb48.r * strength;
			destination->rgb48.g         += source->rgb48.g * strength;
			destination->rgb48.b         += source->rgb48.b * strength;
			break;
		case MKVS_RGB24:
			destination->rgb24.r          += source->rgb24.r * strength;
			destination->rgb24.g          += source->rgb24.g * strength;
			destination->rgb24.b          += source->rgb24.b * strength;
			break;
		case MKVS_YUV444_48:
			destination->yuv444_48.y      += source->yuv444_48.y * strength;
			destination->yuv444_48.u      += source->yuv444_48.u * strength;
			destination->yuv444_48.v      += source->yuv444_48.v * strength;
			break;
		case MKVS_YUV444_24:
			destination->yuv444_24.y      += source->yuv444_24.y * strength;
			destination->yuv444_24.u      += source->yuv444_24.u * strength;
			destination->yuv444_24.v      += source->yuv444_24.v * strength;
			break;
	}
}

//pulls the rgb red value from the pixel
uint16_t getRed (MkvsynthPixel *pixel, MkvsynthMetaData *metaData) {
	uint16_t rgbRed = 0;
	float result = 0;
	
	uint16_t *deepChannel = (uint16_t *)pixel->channel;
	switch(metaData->colorspace){
		case MKVS_RGB48:
			rgbRed = pixel->rgb48.r;
			break;

		case MKVS_RGB24:
			rgbRed = pixel->rgb24.r;
			rgbRed = rgbRed * 256;
			break;

		case MKVS_YUV444_48:
			result = (float)pixel->yuv444_48.y + ((float)pixel->yuv444_48.v / .877);
			result += .5; // for accurate rounding

			// Check for overflow
			if(result >= 65535) {
				rgbRed = 65535;
			} else {
				rgbRed = (int)result;
			}
			break;

		case MKVS_YUV444_24:
			result = (float)pixel->yuv444_24.y + ((float)pixel->yuv444_24.v / .877);
			result += .5; // for accurate rounding
			result *= 256;

			// Check for overflow
			if(result >= 65535) {
				rgbRed = 65535;
			} else {
				rgbRed = (int)result;
			}
			break;
	}

	return rgbRed;
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
	uint16_t *deepChannel = (uint16_t *)pixel->channel;
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
	uint16_t *deepChannel = (uint16_t *)pixel->channel;
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
