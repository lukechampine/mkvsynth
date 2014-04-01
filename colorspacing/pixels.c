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
		case MKVS_HSV48:
			pixel.hsv48.h                 = deepPayload[offset];
			pixel.hsv48.s                 = deepPayload[offset+1];
			pixel.hsv48.v                 = deepPayload[offset+2];
			break;
		case MKVS_HSV24:
			pixel.hsv24.h                 = payload[offset];
			pixel.hsv24.s                 = payload[offset+1];
			pixel.hsv24.v                 = payload[offset+2];
			break;
		case MKVS_HSL48:
			pixel.hsl48.h                 = deepPayload[offset];
			pixel.hsl48.s                 = deepPayload[offset+1];
			pixel.hsl48.l                 = deepPayload[offset+2];
			break;
		case MKVS_HSL24:
			pixel.hsl24.h                 = payload[offset];
			pixel.hsl24.s                 = payload[offset+1];
			pixel.hsl24.l                 = payload[offset+2];
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
		case MKVS_HSV48:
			deepPayload[offset]           = pixel->hsv48.h;
			deepPayload[offset+1]         = pixel->hsv48.s;
			deepPayload[offset+2]         = pixel->hsv48.v;
			break;
		case MKVS_HSV24:
			payload[offset]               = pixel->hsv24.h;
			payload[offset+1]             = pixel->hsv24.s;
			payload[offset+2]             = pixel->hsv24.v;
			break;
		case MKVS_HSL48:
			deepPayload[offset]           = pixel->hsl48.h;
			deepPayload[offset+1]         = pixel->hsl48.s;
			deepPayload[offset+2]         = pixel->hsl48.l;
			break;
		case MKVS_HSL24:
			payload[offset]               = pixel->hsl24.h;
			payload[offset+1]             = pixel->hsl24.s;
			payload[offset+2]             = pixel->hsl24.l;
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
		case MKVS_HSV48:
			destination->hsv48.h         += source->hsv48.h * strength;
			destination->hsv48.s         += source->hsv48.s * strength;
			destination->hsv48.v         += source->hsv48.v * strength;
			break;
		case MKVS_HSV24:
			destination->hsv24.h          += source->hsv24.h * strength;
			destination->hsv24.s          += source->hsv24.s * strength;
			destination->hsv24.v          += source->hsv24.v * strength;
			break;
		case MKVS_HSL48:
			destination->hsl48.h         += source->hsl48.h * strength;
			destination->hsl48.s         += source->hsl48.s * strength;
			destination->hsl48.l         += source->hsl48.l * strength;
			break;
		case MKVS_HSL24:
			destination->hsl24.h          += source->hsl24.h * strength;
			destination->hsl24.s          += source->hsl24.s * strength;
			destination->hsl24.l          += source->hsl24.l * strength;
			break;
	}
}

//pulls the rgb red value from the pixel
uint16_t getRed (MkvsynthPixel *pixel, MkvsynthMetaData *metaData) {
	uint16_t rgbRed = 0;
	float result = 0;
	float hdeg = 0; //value of hsv or hsl hue in degrees instead of out of 255
	float hc = 0; //the next three variables are used in hsv to rgb conversions
	float hx = 0;
	float hm = 0;
	float fsat = 0; //hsv saturation and hsv value represented as floats out of 1 instead of ints out of 255
	float fval = 0;
	float tempx = 0; //placeholder for calculating hx
	int y = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			rgbRed = pixel->rgb48.r;
			break;
			
		case MKVS_RGB24:
			rgbRed = pixel->rgb24.r;
			rgbRed = rgbRed * 256;
			break;
			
		case MKVS_YUV444_48:
			//result = (float)pixel->yuv444_48.y + ((float)pixel->yuv444_48.v / .877); //this is the Y'UV conversion
			result = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768); //this is the YCbCr conversion for full-range values
			result += .5; // for accurate rounding
			
			// Check for overflow
			if(result >= 65535) {
				rgbRed = 65535;
			} else {
				rgbRed = (int)result;
			}
			break;
			
		case MKVS_YUV444_24:
			//result = (float)pixel->yuv444_24.y + ((float)pixel->yuv444_24.v / .877); //this is the Y'UV conversion
			result = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128); //this is the YCbCr conversion for full-range values
			result *= 256;
			result += .5; // for accurate rounding
			
			// Check for overflow
			if(result >= 65535) {
				rgbRed = 65535;
			} else {
				rgbRed = (int)result;
			}
			break;
			
		case MKVS_HSV24:
			hdeg = (float)pixel->hsv24.h * 360.0 / 256.0;
			fsat = (float)pixel->hsv24.s / 256.0;
			fval = (float)pixel->hsv24.v / 256.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if((hdeg < 60) || (hdeg >= 300 && hdeg < 360)){
				result = hc + hm;
			}else if(hdeg >= 120 && hdeg < 240)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbRed = (int)result;
			break;
		
		case MKVS_HSV48:
			hdeg = (float)pixel->hsv48.h * 360.0 / 65536.0;
			fsat = (float)pixel->hsv48.s / 65536.0;
			fval = (float)pixel->hsv48.v / 65536.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if((hdeg < 60) || (hdeg >= 300 && hdeg < 360)){
				result = hc + hm;
			}else if(hdeg >= 120 && hdeg < 240)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbRed = (int)result;
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}

	return rgbRed;
}

//pulls the rgb green value from the pixel
uint16_t getGreen (MkvsynthPixel *pixel, MkvsynthMetaData *metaData) {
	uint16_t rgbGreen = 0;
	float result = 0;
	float hdeg = 0; //value of hsv or hsl hue in degrees instead of out of 255
	float hc = 0; //the next three variables are used in hsv to rgb conversions
	float hx = 0;
	float hm = 0;
	float fsat = 0; //hsv saturation and hsv value represented as floats out of 1 instead of ints out of 255
	float fval = 0;
	float tempx = 0; //placeholder for calculating hx
	int y = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			rgbGreen = pixel->rgb48.g;
			break;
			
		case MKVS_RGB24:
			rgbGreen = pixel->rgb24.g;
			rgbGreen *= 256;
			break;
			
		case MKVS_YUV444_48:
			//result = (float)pixel->yuv444_48.y - ((float)pixel->yuv444_48.v * .581) - ((float)pixel->yuv444_48.u * .395); //this is the Y'UV conversion
			result = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768); //this is the YCbCr conversion for full-range values
			result += .5; // for accurate rounding
			
			//check for underflow
			if(result <= 0) {
				rgbGreen = 0;
			}else {
				rgbGreen = (int)result;
			}
			break;
			
		case MKVS_YUV444_24:
			//result = (float)pixel->yuv444_24.y - ((float)pixel->yuv444_24.v * .581) - ((float)pixel->yuv444_24.u * .395); //this is the Y'UV conversion
			result = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128); //this is the YCbCr conversion for full-range values
			result *= 256;
			result += .5; // for accurate rounding
			
			//check for underflow
			if(result <= 0) {
				rgbGreen = 0;
			}else {
				rgbGreen = (int)result;
			}
			break;
			
		case MKVS_HSV24:
			hdeg = (float)pixel->hsv24.h * 360.0 / 256.0;
			fsat = (float)pixel->hsv24.s / 256.0;
			fval = (float)pixel->hsv24.v / 256.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if(hdeg >= 60 && hdeg < 180){
				result = hc + hm;
			}else if(hdeg >= 240)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbGreen = (int)result;
			break;
		
		case MKVS_HSV48:
			hdeg = (float)pixel->hsv48.h * 360.0 / 65536.0;
			fsat = (float)pixel->hsv48.s / 65536.0;
			fval = (float)pixel->hsv48.v / 65536.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if(hdeg >= 60 && hdeg < 180){
				result = hc + hm;
			}else if(hdeg >= 240)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbGreen = (int)result;
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
	return rgbGreen;
}

//pulls the rgb blue value from the pixel
uint16_t getBlue (MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t rgbBlue = 0;
	float result = 0;
	float hdeg = 0; //value of hsv or hsl hue in degrees instead of out of 255
	float hc = 0; //the next three variables are used in hsv to rgb conversions
	float hx = 0;
	float hm = 0;
	float fsat = 0; //hsv saturation and hsv value represented as floats out of 1 instead of ints out of 255
	float fval = 0;
	float tempx = 0; //placeholder for calculating hx
	int y = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			rgbBlue = pixel->rgb48.b;
			break;
			
		case MKVS_RGB24:
			rgbBlue = pixel->rgb24.b;
			rgbBlue *= 256;
			break;
			
		case MKVS_YUV444_48:
			//result = (float)pixel->yuv444_48.y + ((float)pixel->yuv444_48.u / .492); //this is the Y'UV conversion
			result = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768); //this is the YCbCr conversion for full-range values
			result += .5; // for accurate rounding
			
			//check for overflow
			if(result >= 65535) {
				rgbBlue = 65535;
			}else {
				rgbBlue = (int)result;
			}
			break;
			
		case MKVS_YUV444_24:
			//result = (float)pixel->yuv444_24.y + ((float)pixel->yuv444_24.u / .492); //this is the Y'UV conversion
			result = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128); //this is the YCbCr conversion for full-range values
			result *= 256;
			result += .5; // for accurate rounding
			
			//check for overflow
			if(result >= 65535) {
				rgbBlue = 65535;
			}else {
				rgbBlue = (int)result;
			}
			break;
			
		case MKVS_HSV24:
			hdeg = (float)pixel->hsv24.h * 360.0 / 256.0;
			fsat = (float)pixel->hsv24.s / 256.0;
			fval = (float)pixel->hsv24.v / 256.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if(hdeg >= 180 && hdeg < 300){
				result = hc + hm;
			}else if(hdeg < 120)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbBlue = (int)result;
			break;
		
		case MKVS_HSV48:
			hdeg = (float)pixel->hsv48.h * 360.0 / 65536.0;
			fsat = (float)pixel->hsv48.s / 65536.0;
			fval = (float)pixel->hsv48.v / 65536.0;
			hc = fsat * fval;
			hm = fval - hc;
			tempx = hdeg / 60.0;
			for(y = 0; tempx > 2.0; y++){
				tempx -= 2.0;
			}
			tempx -= 1;
			if(tempx < 0){
				tempx *= -1.0;
			}
			hx = hc * tempx;
			if(hdeg >= 180 && hdeg < 300){
				result = hc + hm;
			}else if(hdeg < 120)
			{
				result = hm;
			}else{
				result = hx + hm;
			}
			result *= 65535;
			rgbBlue = (int)result;
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
	return rgbBlue;
}


//sets the RGB red value to a particular value
void setRed(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			pixel->rgb48.r = (uint16_t) value;
			break;
			
		case MKVS_RGB24:
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			pixel->rgb24.r = (uint8_t) value;
			break;
		
		case MKVS_YUV444_48:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			redval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			redval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}


//sets the RGB green value to a particular value
void setGreen(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			pixel->rgb48.g = (uint16_t) value;
			break;
			
		case MKVS_RGB24:
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			pixel->rgb24.g = (uint16_t) value;
			break;
		
		case MKVS_YUV444_48:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			grnval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			grnval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}


//sets the RGB blue value to a particular value
void setBlue(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			pixel->rgb48.b = (uint16_t) value;
			break;
			
		case MKVS_RGB24:
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			pixel->rgb24.b = (uint16_t) value;
			break;
		
		case MKVS_YUV444_48:
		//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(value <= 0) {
				value = 0;
			}else if(value >= 65535) {
				value = 65535;
			}
			bluval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(value <= 0) {
				value = 0;
			}else if(value >= 255) {
				value = 255;
			}
			bluval = (float)value;
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}

//increments the appropriate rgb value by the given value
void adjustRed(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(pixel->rgb48.r + intensity <= 0) {
				pixel->rgb48.r = 0;
			}else if(pixel->rgb48.r + intensity >= 65535) {
				pixel->rgb48.r = 65535;
			}else{
				pixel->rgb48.r += (uint16_t)intensity;
			}
			break;
			
		case MKVS_RGB24:
			if(pixel->rgb24.r + intensity <= 0) {
				pixel->rgb24.r = 0;
			}else if(pixel->rgb24.r + intensity >= 255) {
				pixel->rgb24.r = 255;
			}else{
				pixel->rgb24.r += (uint8_t) intensity;
			}
			break;
		
		case MKVS_YUV444_48:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(redval + intensity <= 0) {
				redval = 0;
			}else if(redval + (uint16_t) intensity >= 65535) {
				redval = 65535;
			}else{
				redval += (uint16_t) intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(redval + (uint8_t) intensity <= 0) {
				redval = 0;
			}else if(redval + (uint8_t) intensity >= 255) {
				redval = 255;
			}else{
				redval += (uint8_t)intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}

//increments the appropriate rgb value by the given value
void adjustGreen(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(pixel->rgb48.g + intensity <= 0) {
				pixel->rgb48.g = 0;
			}else if(pixel->rgb48.g + intensity >= 65535) {
				pixel->rgb48.g = 65535;
			}else{
				pixel->rgb48.g += (uint16_t)intensity;
			}
			break;
			
		case MKVS_RGB24:
			if(pixel->rgb24.g + intensity <= 0) {
				pixel->rgb24.g = 0;
			}else if(pixel->rgb24.g + intensity >= 255) {
				pixel->rgb24.g = 255;
			}else{
				pixel->rgb24.g += (uint8_t) intensity;
			}
			break;
		
		case MKVS_YUV444_48:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(grnval + (uint16_t) intensity <= 0) {
				grnval = 0;
			}else if(grnval + (uint16_t) intensity >= 65535) {
				grnval = 65535;
			}else{
				grnval += (uint16_t) intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(grnval + (uint8_t) intensity <= 0) {
				grnval = 0;
			}else if(grnval + (uint8_t) intensity >= 255) {
				grnval = 255;
			}else{
				grnval += (uint8_t)intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}

//increments the appropriate rgb value by the given value
void adjustBlue(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData) {
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			if(pixel->rgb48.b + intensity <= 0) {
				pixel->rgb48.b = 0;
			}else if(pixel->rgb48.b + intensity >= 65535) {
				pixel->rgb48.b = 65535;
			}else{
				pixel->rgb48.b += (uint16_t) intensity;
			}
			break;
			
		case MKVS_RGB24:
			if(pixel->rgb24.b + intensity <= 0) {
				pixel->rgb24.b = 0;
			}else if(pixel->rgb24.b + intensity >= 255) {
				pixel->rgb24.b = 255;
			}else{
				pixel->rgb24.b += (uint8_t) intensity;
			}
			break;
		
		case MKVS_YUV444_48:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_48.y + 1.4 * ((float)pixel->yuv444_48.v - 32768);
			grnval = (float)pixel->yuv444_48.y - .343 * ((float)pixel->yuv444_48.u - 32768) - .711 * ((float)pixel->yuv444_48.v - 32768);
			bluval = (float)pixel->yuv444_48.y + 1.765 * ((float)pixel->yuv444_48.u - 32768);
			if(bluval + (uint16_t) intensity <= 0) {
				bluval = 0;
			}else if(bluval + (uint16_t) intensity >= 65535) {
				bluval = 65535;
			}else{
				bluval += (uint16_t) intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_48.y = (uint16_t)yval;
			uval = 32768 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_48.u = (uint16_t)uval;
			vval = 32768 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_48.v = (uint16_t)vval;
			break;
		
		case MKVS_YUV444_24:
			//pulls the equivalent rgb values from the pixel, changes the appropriate one, then converts back to yuv.
			redval = (float)pixel->yuv444_24.y + 1.4 * ((float)pixel->yuv444_24.v - 128);
			grnval = (float)pixel->yuv444_24.y - .343 * ((float)pixel->yuv444_24.u - 128) - .711 * ((float)pixel->yuv444_24.v - 128);
			bluval = (float)pixel->yuv444_24.y + 1.765 * ((float)pixel->yuv444_24.u - 128);
			if(bluval + (uint8_t) intensity <= 0) {
				bluval = 0;
			}else if(bluval + (uint8_t) intensity >= 255) {
				bluval = 255;
			}else{
				bluval += (uint8_t)intensity;
			}
			yval = redval * .299 + grnval * .587 + bluval * .114;
			pixel->yuv444_24.y = (uint8_t)yval;
			uval = 128 + .5 * bluval - .169 * redval - .331 * grnval;
			pixel->yuv444_24.u = (uint8_t)uval;
			vval = 128 + .5 * redval - .419 * grnval - .081 * bluval;
			pixel->yuv444_24.v = (uint8_t)vval;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}

//pulls the YUV Luma (Y) value from a pixel
uint16_t getLuma(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t yuvLuma = 0;
	float result = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			result = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			result += .5;
			yuvLuma = (int)result;
			break;
			
		case MKVS_RGB24:
			result = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			result *= 256;
			result += .5;
			yuvLuma = (int)result;
			break;
			
		case MKVS_YUV444_48:
			yuvLuma = pixel->yuv444_48.y;
			break;
			
		case MKVS_YUV444_24:
			yuvLuma = pixel->yuv444_24.y;
			yuvLuma *= 256;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
	return yuvLuma;
}


//pulls the YUV Cb (U) value from a pixel
uint16_t getCb(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t yuvCb = 0;
	float result = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			result = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			result += .5;
			yuvCb = (int)result;
			break;
			
		case MKVS_RGB24:
			result = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			result *= 256;
			result += .5;
			yuvCb = (int)result;
			break;
			
		case MKVS_YUV444_48:
			yuvCb = pixel->yuv444_48.u;
			break;
			
		case MKVS_YUV444_24:
			yuvCb = pixel->yuv444_24.u;
			yuvCb *= 256;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
	return yuvCb;
}


//pulls the YUV Cr (V) value from a pixel
uint16_t getCr(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t yuvCr = 0;
	float result = 0;
	
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			result = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			result += .5;
			yuvCr = (int)result;
			break;
			
		case MKVS_RGB24:
			result = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			result *= 256;
			result += .5;
			yuvCr = (int)result;
			break;
			
		case MKVS_YUV444_48:
			yuvCr = pixel->yuv444_48.v;
			break;
			
		case MKVS_YUV444_24:
			yuvCr = pixel->yuv444_24.v;
			yuvCr *= 256;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
	return yuvCr;
}


//Sets the Luma (Y) value for a pixel
void setLuma(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			yval = value;
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			yval = value;
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			pixel->yuv444_48.y = (uint16_t) value;
			break;
		
		case MKVS_YUV444_24:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			pixel->yuv444_24.y = (uint16_t) value;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}



//Sets the Cb (U) value for a pixel
void setCb(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			uval = value;
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			uval = value;
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			pixel->yuv444_48.u = (uint16_t) value;
			break;
		
		case MKVS_YUV444_24:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			pixel->yuv444_24.u = (uint16_t) value;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}




//Sets the Cr (V) value for a pixel
void setCr(MkvsynthPixel *pixel, double value, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			vval = value;
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			vval = value;
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 65535){
				value = 65535;
			}
			pixel->yuv444_48.v = (uint16_t) value;
			break;
		
		case MKVS_YUV444_24:
			if(value <= 0)
			{
				value = 0;
			}else if(value >= 255){
				value = 255;
			}
			pixel->yuv444_24.v = (uint16_t) value;
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		}
}



//increments the appropriate yuv value by the given value
void adjustLuma(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(yval + (uint16_t) intensity <= 0)
			{
				yval = 0;
			}else if(yval + (uint16_t) intensity >= 65535){
				yval = 65535;
			}else{
				yval =+ (uint16_t) intensity;
			}
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(yval + (uint8_t) intensity <= 0)
			{
				yval = 0;
			}else if(yval + (uint8_t) intensity >= 255){
				yval = 255;
			}else{
				yval += (uint8_t) intensity;
			}
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(pixel->yuv444_48.y + (uint16_t) intensity <= 0)
			{
				yval = 0;
			}else if(pixel->yuv444_48.y + (uint16_t) intensity >= 65535){
				yval = 65535;
			}else{
				pixel->yuv444_48.y += (uint16_t) intensity;
			}
			break;
		
		case MKVS_YUV444_24:
			if(pixel->yuv444_24.y + (uint8_t) intensity <= 0)
			{
				yval = 0;
			}else if(pixel->yuv444_24.y + (uint8_t) intensity >= 255){
				yval = 255;
			}else{
				pixel->yuv444_24.y += (uint8_t) intensity;
			}
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
}




//increments the appropriate yuv value by the given value
void adjustCb(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(uval + (uint16_t) intensity <= 0)
			{
				uval = 0;
			}else if(uval + (uint16_t) intensity >= 65535){
				uval = 65535;
			}else{
				uval =+ (uint16_t) intensity;
			}
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(uval + (uint8_t) intensity <= 0)
			{
				uval = 0;
			}else if(uval + (uint8_t) intensity >= 255){
				uval = 255;
			}else{
				uval += (uint8_t) intensity;
			}
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(pixel->yuv444_48.u + (uint16_t) intensity <= 0)
			{
				uval = 0;
			}else if(pixel->yuv444_48.u + (uint16_t) intensity >= 65535){
				uval = 65535;
			}else{
				pixel->yuv444_48.u += (uint16_t) intensity;
			}
			break;
		
		case MKVS_YUV444_24:
			if(pixel->yuv444_24.u + (uint8_t) intensity <= 0)
			{
				uval = 0;
			}else if(pixel->yuv444_24.u + (uint8_t) intensity >= 255){
				uval = 255;
			}else{
				pixel->yuv444_24.u += (uint8_t) intensity;
			}
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
}



//increments the appropriate yuv value by the given value
void adjustCr(MkvsynthPixel *pixel, double intensity, MkvsynthMetaData *metaData){
	float redval = 0;
	float bluval = 0;
	float grnval = 0;
	float yval = 0;
	float uval = 0;
	float vval = 0;
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			yval = (float)pixel->rgb48.r * .299 + (float)pixel->rgb48.g * .587 + (float)pixel->rgb48.b * .114;
			uval = 32768 + .5 * (float)pixel->rgb48.b - .169 * (float)pixel->rgb48.r - .331 * (float)pixel->rgb48.g;
			vval = 32768 + .5 * (float)pixel->rgb48.r - .419 * (float)pixel->rgb48.g - .081 * (float)pixel->rgb48.b;
			if(vval + (uint16_t) intensity <= 0)
			{
				vval = 0;
			}else if(vval + (uint16_t) intensity >= 65535){
				vval = 65535;
			}else{
				vval =+ (uint16_t) intensity;
			}
			redval = yval + 1.4 * (vval - 32768);
			grnval = yval - .343 * (uval - 32768) - .711 * (vval - 32768);
			bluval = yval + 1.765 * (uval - 32768);
			pixel->rgb48.r = redval;
			pixel->rgb48.g = grnval;
			pixel->rgb48.b = bluval;
			break;
			
		case MKVS_RGB24:
			yval = (float)pixel->rgb24.r * .299 + (float)pixel->rgb24.g * .587 + (float)pixel->rgb24.b * .114;
			uval = 128 + .5 * (float)pixel->rgb24.b - .169 * (float)pixel->rgb24.r - .331 * (float)pixel->rgb24.g;
			vval = 128 + .5 * (float)pixel->rgb24.r - .419 * (float)pixel->rgb24.g - .081 * (float)pixel->rgb24.b;
			if(vval + (uint8_t) intensity <= 0)
			{
				vval = 0;
			}else if(vval + (uint8_t) intensity >= 255){
				vval = 255;
			}else{
				vval += (uint8_t) intensity;
			}
			redval = yval + 1.4 * (vval - 128);
			grnval = yval - .343 * (uval - 128) - .711 * (vval - 128);
			bluval = yval + 1.765 * (uval - 128);
			pixel->rgb24.r = redval;
			pixel->rgb24.g = grnval;
			pixel->rgb24.b = bluval;
			break;
		
		case MKVS_YUV444_48:
			if(pixel->yuv444_48.v + (uint16_t) intensity <= 0)
			{
				vval = 0;
			}else if(pixel->yuv444_48.v + (uint16_t) intensity >= 65535){
				vval = 65535;
			}else{
				pixel->yuv444_48.v += (uint16_t) intensity;
			}
			break;
		
		case MKVS_YUV444_24:
			if(pixel->yuv444_24.v + (uint8_t) intensity <= 0)
			{
				vval = 0;
			}else if(pixel->yuv444_24.v + (uint8_t) intensity >= 255){
				vval = 255;
			}else{
				pixel->yuv444_24.v += (uint8_t) intensity;
			}
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
	}
}


uint16_t getHue(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t hue = 0;
	double rp = 0;
	double bp = 0;
	double gp = 0; //these variable represent the portion of the red, green, or blue value that is present in the color
	double cmin, cmax, delta, dhue, fhue; //dhue represents the hue out of one, fhue is the float form of hue
	int x;
	switch(metaData->colorspace){
		case MKVS_RGB24:
		rp = (double)pixel->rgb24.r / 256.0;
		gp = (double)pixel->rgb24.g / 256.0;
		bp = (double)pixel->rgb24.b / 256.0;
		x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
		//This is a maximum and minimum function
		if(rp < gp){
			if(bp > gp){
				cmin = rp;
				x = 3;
				cmax = bp;
			}else if(bp < rp){
				cmin = bp;
				x = 2;
				cmax = gp;
			}else{
				cmin = rp;
				x = 2;
				cmax = gp;
			}
		}else{
			if(bp > rp){
				cmin = gp;
				x = 3;
				cmax = bp;
			}else if(bp < gp){
				cmin = bp;
				x = 1;
				cmax = rp;
			}else{
				cmin = gp;
				x = 1;
				cmax = rp;
			}
		}
		delta = cmax - cmin;
		if(cmax == 0){
			hue = 0;
		}else if(x == 1){
			dhue = (gp - bp) / delta;
			if(dhue < 0)
			{
				dhue += 6;
			}
		}else if(x == 2){
			dhue = (bp - rp) / delta + 2;
		}else{
			dhue = (rp - gp) /delta + 4;
		}
		fhue = dhue / 6.0 * 65535.0;
		hue = (int)fhue;
		break;
		
		case MKVS_RGB48:
			rp = (double)pixel->rgb48.r / 65536.0;
			gp = (double)pixel->rgb48.g / 65536.0;
			bp = (double)pixel->rgb48.b / 65536.0;
			x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
			//This is a maximum and minimum function
			if(rp < gp){
				if(bp > gp){
					cmin = rp;
					x = 3;
					cmax = bp;
				}else if(bp < rp){
					cmin = bp;
					x = 2;
					cmax = gp;
				}else{
					cmin = rp;
					x = 2;
					cmax = gp;
				}
			}else{
				if(bp > rp){
					cmin = gp;
					x = 3;
					cmax = bp;
				}else if(bp < gp){
					cmin = bp;
					x = 1;
					cmax = rp;
				}else{
					cmin = gp;
					x = 1;
					cmax = rp;
				}
			}
			delta = cmax - cmin;
			if(cmax == 0){
				hue = 0;
			}else if(x == 1){
				dhue = (gp - bp) / delta;
				if(dhue < 0){
					dhue += 6;
				}
			}else if(x == 2){
				dhue = (bp - rp) / delta + 2;
			}else{
				dhue = (rp - gp) /delta + 4;
			}
			fhue = dhue / 6.0 * 65535.0;
			hue = (int)fhue;
			break;
		
		case MKVS_YUV444_24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV24:
			hue = pixel->hsv24.h;
			hue *= 256;
			break;
		
		case MKVS_HSV48:
			hue = pixel->hsv48.h;
			break;
		
		case MKVS_HSL24:
			hue = pixel->hsl24.h;
			hue *= 256;
			break;
		
		case MKVS_HSL48:
			hue = pixel->hsl48.h;
			break;
		
	}
	return hue;
}



uint16_t getHSVSaturation(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t hsvs = 0;
	double rp = 0;
	double bp = 0;
	double gp = 0; //these variable represent the portion of the red, green, or blue value that is present in the color
	double cmin, cmax, delta, ds, fs; //ds represents the saturation out of one, fs is the float form of saturation
	int x, y;
	switch(metaData->colorspace){
		case MKVS_RGB24:
			rp = (double)pixel->rgb24.r / 256.0;
			gp = (double)pixel->rgb24.g / 256.0;
			bp = (double)pixel->rgb24.b / 256.0;
			x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
			y = -1;
			//This is a maximum and minimum function
			if(rp < gp){
				if(bp > gp){
					y = 1;
					cmin = rp;
					x = 3;
					cmax = bp;
				}else if(bp < rp){
					y = 3;
					cmin = bp;
					x = 2;
				}else{
					y = 1;
					cmin = rp;
					x = 2;
					cmax = gp;
				}
			}else{
				if(bp > rp){
					y = 2;
					cmin = gp;
					x = 3;
					cmax = bp;
				}else if(bp < gp){
					y = 3;
					cmin = bp;
					x = 1;
					cmax = rp;
				}else{
					y = 2;
					cmin = gp;
					x = 1;
					cmax = rp;
				}
			}
			delta = cmax - cmin;
			if(cmax == 0)
			{
				hsvs = 0;
			}else{
				ds = delta / cmax;
				fs = ds * 65535.0;
				hsvs = (int)fs;
			}
			break;
		
		case MKVS_RGB48:
			rp = (double)pixel->rgb48.r / 65536.0;
			gp = (double)pixel->rgb48.g / 65536.0;
			bp = (double)pixel->rgb48.b / 65536.0;
			x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
			y = -1;
			//This is a maximum and minimum function
			if(rp < gp){
				if(bp > gp){
					y = 1;
					cmin = rp;
					x = 3;
					cmax = bp;
				}else if(bp < rp){
					y = 3;
					cmin = bp;
					x = 2;
				}else{
					y = 1;
					cmin = rp;
					x = 2;
					cmax = gp;
				}
			}else{
				if(bp > rp){
					y = 2;
					cmin = gp;
					x = 3;
					cmax = bp;
				}else if(bp < gp){
					y = 3;
					cmin = bp;
					x = 1;
					cmax = rp;
				}else{
					y = 2;
					cmin = gp;
					x = 1;
					cmax = rp;
				}
			}
			delta = cmax - cmin;
			if(cmax == 0)
			{
				hsvs = 0;
			}else{
				ds = delta / cmax;
				fs = ds * 65535.0;
				hsvs = (int)fs;
			}
			break;
		
		case MKVS_YUV444_24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV24:
			hsvs = pixel->hsv24.s;
			hsvs *= 256;
			break;
		
		case MKVS_HSV48:
			hsvs = pixel->hsv48.s;
			break;
		
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
	}
	return hsvs;
}



uint16_t getValue(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t hsvv = 0;
	double rp = 0;
	double bp = 0;
	double gp = 0; //these variable represent the portion of the red, green, or blue value that is present in the color
	double cmin, cmax;
	int x, y;
	switch(metaData->colorspace){
		case MKVS_RGB24:
		rp = (double)pixel->rgb24.r / 256.0;
		gp = (double)pixel->rgb24.g / 256.0;
		bp = (double)pixel->rgb24.b / 256.0;
		x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
		y = -1;
		//This is a maximum and minimum function
		if(rp < gp){
			if(bp > gp){
				y = 1;
				cmin = rp;
				x = 3;
				cmax = bp;
			}else if(bp < rp){
				y = 3;
				cmin = bp;
				x = 2;
				cmax = gp;
			}else{
				y = 1;
				cmin = rp;
				x = 2;
				cmax = gp;
			}
		}else{
			if(bp > rp){
				y = 2;
				cmin = gp;
				x = 3;
				cmax = bp;
			}else if(bp < gp){
				y = 3;
				cmin = bp;
				x = 1;
				cmax = rp;
			}else{
				y = 2;
				cmin = gp;
				x = 1;
				cmax = rp;
			}
		}
		hsvv = cmax * 65535.0;
		break;
		
		case MKVS_RGB48:
		rp = (double)pixel->rgb48.r / 65536.0;
		gp = (double)pixel->rgb48.g / 65536.0;
		bp = (double)pixel->rgb48.b / 65536.0;
		x = -1; //x and y represent which variable (r,g,b) is the maximum and minimum, respectively
		y = -1;
		//This is a maximum and minimum function
		if(rp < gp){
			if(bp > gp){
				y = 1;
				cmin = rp;
				x = 3;
				cmax = bp;
			}else if(bp < rp){
				y = 3;
				cmin = bp;
				x = 2;
				cmax = gp;
			}else{
				y = 1;
				cmin = rp;
				x = 2;
				cmax = gp;
			}
		}else{
			if(bp > rp){
				y = 2;
				cmin = gp;
				x = 3;
				cmax = bp;
			}else if(bp < gp){
				y = 3;
				cmin = bp;
				x = 1;
				cmax = rp;
			}else{
				y = 2;
				cmin = gp;
				x = 1;
				cmax = rp;
			}
		}
		hsvv = cmax * 65535.0;
		break;
		
		case MKVS_YUV444_24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSV24:
			hsvv = pixel->hsv24.v;
			hsvv *= 256;
			break;
		
		case MKVS_HSV48:
			hsvv = pixel->hsv48.v;
			break;
			
		case MKVS_HSL24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_HSL48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
	}
	return hsvv;
}



uint16_t getHSLSaturation(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t hsls = 0;
	switch(metaData->colorspace){
		case MKVS_RGB48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_RGB24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			hsls = pixel->hsl24.s;
			hsls *= 256;
			break;
			
		case MKVS_HSL48:
			hsls = pixel->hsl24.s;
			break;
	}
	return hsls;
}



uint16_t getLightness(MkvsynthPixel *pixel, MkvsynthMetaData *metaData){
	uint16_t hsll = 0;
	switch(metaData->colorspace){
		case MKVS_RGB48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_RGB24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
		
		case MKVS_YUV444_48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSV24:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSV48:
			MkvsynthError("This colorspace interaction is not yet supported");
			break;
			
		case MKVS_HSL24:
			hsll = pixel->hsl24.l;
			hsll *= 256;
			break;
			
		case MKVS_HSL48:
			hsll = pixel->hsl24.l;
			break;
	}
	return hsll;
}






