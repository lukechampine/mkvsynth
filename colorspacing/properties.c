#include "properties.h"

// Returns the bit depth
int getDepth(MkvsynthMetaData *metaData) {
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			return 16;
		case MKVS_RGB24:
			return 8;
		case MKVS_YUV444_48:
			return 16;
		case MKVS_YUV444_24:
			return 8;
		case MKVS_HSV48:
			return 16;
		case MKVS_HSV24:
			return 8;
		case MKVS_HSL48:
			return 16;
		case MKVS_HSL24:
			return 8;
		case NULL_COLOR:
			return -1;
		default:
			return -1;
	}
	return -1;
}

// Returns the number of bytes in the frame
int getBytes(MkvsynthMetaData *metaData) {
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			return metaData->width * metaData->height * 6;
		case MKVS_RGB24:
			return metaData->width * metaData->height * 3;
		case MKVS_YUV444_48:
			return metaData->width * metaData->height * 6;
		case MKVS_YUV444_24:
			return metaData->width * metaData->height * 3;
		case MKVS_HSV48:
			return metaData->width * metaData->height * 6;
		case MKVS_HSV24:
			return metaData->width * metaData->height * 3;
		case MKVS_HSL48:
			return metaData->width * metaData->height * 6;
		case MKVS_HSL24:
			return metaData->width * metaData->height * 3;
		case NULL_COLOR:
			return -1;
		default:
			return -1;
	}
	return -1;
}

// Returns the number of bytes in 1 line of a frame
int getLinesize(MkvsynthMetaData *metaData) {
	switch(metaData->colorspace) {
		case MKVS_RGB48:
			return metaData->width * 6;
		case MKVS_RGB24:
			return metaData->width * 3;
		case MKVS_YUV444_48:
			return metaData->width * 6;
		case MKVS_YUV444_24:
			return metaData->width * 3;
		case MKVS_HSV48:
			return metaData->width * 6;
		case MKVS_HSV24:
			return metaData->width * 3;
		case MKVS_HSL48:
			return metaData->width * 6;
		case MKVS_HSL24:
			return metaData->width * 3;
		case NULL_COLOR:
			return -1;
		default:
			return -1;
	}
	return -1;
}

// Some colorspaces have limitations like resolution that is divisible by 2
// At the moment, mkvsynth does not support any colorspaces with limitations
int isMetaDataValid(MkvsynthMetaData *metaData) {
	if(metaData->colorspace >= 0 && metaData->colorspace <= 8) {
		switch(metaData->colorspace) {
			case MKVS_RGB48:
				return 1;
			case MKVS_RGB24:
				return 1;
			case MKVS_YUV444_48:
				return 1;
			case MKVS_YUV444_24:
				return 1;
			case MKVS_HSV48:
				return 1;
			case MKVS_HSV24:
				return 1;
			case MKVS_HSL48:
				return 1;
			case MKVS_HSL24:
				return 1;
			case NULL_COLOR:
				printf("Meta data checker: colorspace is not initialized\n");
				printf("Color space value: %i\n", metaData->colorspace);
				return -1;	
		}
	} else {
		printf("Meta data checker: colorspace is not recognized\n");
		printf("Color space value: %i\n", metaData->colorspace);
		return -1;
	}
	return -1;
}
