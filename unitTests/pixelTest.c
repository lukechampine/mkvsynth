#include "../jarvis/jarvis.h"

void testGetPixelAndPutPixel(int *tests, int*testsPassed) {
	// make a generic metaData to use for testing
	MkvsynthMetaData metaData;
	metaData.colorspace = 1;
	metaData.width = 80;
	metaData.height = 200;
	metaData.fpsNumerator = 60;
	metaData.fpsDenominator = 1;

	// Test 1: getBytes

	uint8_t *payload = malloc(getBytes(&metaData));
	uint16_t *shortPayload = (uint16_t *)payload;
	int bytes = getBytes(&metaData);
	int j;
	for(j = 0; j < bytes / 2; j++) {
		shortPayload[j] = j % 65536;
	}
	
	MkvsynthPixel test = getPixel(payload, &metaData, 35, 0);
	*tests += 1;
	if((uint16_t)test.channel[0] != 35 * 3) {
		printf("ERROR: getPixel failed test 1, expecting %i but got %i\n", 35*3, (uint16_t)test.channel[0]);
	} else {
		*testsPassed += 1;
	}

	test = getPixel(payload, &metaData, 25, 44);
	*tests += 1;
	uint16_t *deepChannel = (uint16_t *)test.channel;
	if(deepChannel[1] != 44 * 80 * 3 + 25 * 3 + 1) {
		printf("ERROR: getPixel failed test 2, expecting %i but got %i\n", 44 * 80 * 3 + 25 * 3 + 1, deepChannel[1]);
	} else {
		*testsPassed += 1;
	}
}

// Test a normal case and an odd case for each colorspace
void testGetBytes(int *tests, int *testsPassed) {
	MkvsynthMetaData metaData;
	metaData.fpsNumerator = 60;
	metaData.fpsDenominator = 1;
	
	metaData.width = 80;
	metaData.height = 200;

	metaData.colorspace = MKVS_RGB48;
	*tests += 1;
	if(getBytes(&metaData) != 80*200*6) {
		printf("ERROR: getBytes failed test 1!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_RGB24;
	*tests += 1;
	if(getBytes(&metaData) != 80*200*3) {
		printf("ERROR: getBytes failed test 2!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_YUV444_48;
	*tests += 1;
	if(getBytes(&metaData) != 80*200*6) {
		printf("ERROR: getBytes failed test 3!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_YUV444_24;
	*tests += 1;
	if(getBytes(&metaData) != 80*200*3) {
		printf("ERROR: getBytes failed test 4!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.width = 19987;
	metaData.height = 10999;

	metaData.colorspace = MKVS_RGB48;
	*tests += 1;
	if(getBytes(&metaData) != 19987*10999*6) {
		printf("ERROR: getBytes failed test 5!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_RGB24;
	*tests += 1;
	if(getBytes(&metaData) != 19987*10999*3) {
		printf("ERROR: getBytes failed test 6!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_YUV444_48;
	*tests += 1;
	if(getBytes(&metaData) != 19987*10999*6) {
		printf("ERROR: getBytes failed test 7!\n");
	} else {
		*testsPassed += 1;
	}

	metaData.colorspace = MKVS_YUV444_24;
	*tests += 1;
	if(getBytes(&metaData) != 19987*10999*3) {
		printf("ERROR: getBytes failed test 8!\n");
	} else {
		*testsPassed += 1;
	}
}
