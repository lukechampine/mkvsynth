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
			pixel.rgb48.red               = deepPayload[offset];
			pixel.rgb48.green             = deepPayload[offset+1];
			pixel.rgb48.blue              = deepPayload[offset+2];
			break;
		case MKVS_RGB24:
			pixel.rgb24.red               = payload[offset];
			pixel.rgb24.green             = payload[offset+1];
			pixel.rgb24.blue              = payload[offset+2];
			break;
		case MKVS_YUV444_48:
			pixel.yuv444_48.luma          = deepPayload[offset];
			pixel.yuv444_48.cb            = deepPayload[offset+1];
			pixel.yuv444_48.cr            = deepPayload[offset+2];
			break;
		case MKVS_YUV444_24:
			pixel.yuv444_24.luma          = payload[offset];
			pixel.yuv444_24.cb            = payload[offset+1];
			pixel.yuv444_24.cr            = payload[offset+2];
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

	uint16_t *deepChannel = (uint16_t *)pixel->channel;
	uint16_t *deepPayload = (uint16_t *)payload;
	int offset = 3 * (heightOffset * metaData->width + widthOffset);

	switch(metaData->colorspace) {
		case MKVS_RGB48:
			deepPayload[offset]           = pixel.rgb48.red;
			deepPayload[offset+1]         = pixel.rgb48.green;
			deepPayload[offset+2]         = pixel.rgb48.blue;
			break;
		case MKVS_RGB24:
			payload[offset]               = pixel.rgb24.red;
			payload[offset+1]             = pixel.rgb24.green;
			payload[offset+2]             = pixel.rgb24.blue;
			break;
		case MKVS_YUV444_48:
			deepPayload[offset]           = pixel.yuv444_48.luma;
			deepPayload[offset+1]         = pixel.yuv444_48.cb;
			deepPayload[offset+2]         = pixel.yuv444_48.cr;
			break;
		case MKVS_YUV444_24:
			payload[offset]               = pixel.yuv444_24.luma;
			payload[offset+1]             = pixel.yuv444_24.cb;
			payload[offset+2]             = pixel.yuv444_24.cr;
			break;
	}
}

void addPixel (MkvsynthPixel *destination, MkvsynthPixel *source, uint16_t colorspace, double strength) {

#ifdef DEBUG
	checkColorspace(metaData->colorspace, "addPixel");
#endif

	switch(colorspace) {
		case MKVS_RGB48:
			destination.rgb48.red        += source.rgb48.red * strenght;
			destination.rgb48.green      += source.rgb48.green * strength;
			destination.rgb48.blue       += source.rgb48.blue * strength;
			break;
		case MKVS_RGB24:
			destination.rgb24.red        += source.rgb24.red * strenght;
			destination.rgb24.green      += source.rgb24.green * strength;
			destination.rgb24.blue       += source.rgb24.blue * strength;
			break;
		case MKVS_YUV444_48:
			destination.yuv444_48.red    += source.yuv444_48.red * strength;
			destination.yuv444_48.green  += source.yuv444_48.green * strength;
			destination.yuv444_48.blue   += source.yuv444_48.blue * strength;
			break;
		case MKVS_YUV444_24:
			destination.yuv444_24.red    += source.yuv444_24.red * strength;
			destination.yuv444_24.green  += source.yuv444_24.green * strength;
			destination.yuv444_24.blue   += source.yuv444_24.blue * strength;
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
			rgbRed = pixel.rgb48.red;
			break;

		case MKVS_RGB24:
			rgbRed = pixel.rgb24.red;
			rgbRed = rgbRed * 256;
			break;

		case MKVS_YUV444_48:
			result = (float)pixel.yuv444_48.luma + ((float)pixel.yuv444_48.cr / .877);
			result += .5; // for accurate rounding

			// Check for overflow
			if(result >= 65535) {
				rgbRed = 65535;
			} else {
				rgbRed = (int)result;
			}
			break;

		case MKVS_YUV444_24:
			result = (float)pixel.yuv444_24.luma + ((float)pixel.yuv444_24.cr / .877);
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
