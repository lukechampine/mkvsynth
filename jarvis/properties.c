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
	}
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
	}
}

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
	}
}

int isMetaDataValid(MkvsynthMetaData *metaData) {
	if(metaData->colorspace >= 1 && metaData->colorspace <= 4) {
		switch(metaData->colorspace) {
			case MKVS_RGB48:
				return 1;
			case MKVS_RGB24:
				return 1;
			case MKVS_YUV444_48:
				return 1;
			case MKVS_YUV444_24:
				return 1;
		}
	} else {
		printf("Meta data checker: colorspace is not recognized\n");
		printf("Color space value: %i\n", metaData->colorspace);
	}

	return 0;
}
