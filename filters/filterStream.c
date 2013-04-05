#include "datatypes.h"
#include "decode.c"
#include "encode.c"
#include "frame.c"
#include "resize.c"
#include <stdio.h>
#include <string.h>

// Structure:
// Make a function call, do an error check
// Rinse and repeat. Some things happen in a complex loop,
// 	this will hopefully be simplified at some point
int main(int argc, char* argv[]) {
	// Call each time you want to decode a video
	DecodeContext *decodeContext = openDecoder("test.mp4");
	if(decodeContext == NULL) {
		printf("Mkvsynth Core: Decoding Failed\n");
		return -1;
	}


	// Call each time you want to encode a video
	EncodeContext *encodeContext = initializeEncoder(200, 200);
	if(encodeContext == NULL) {
		printf("Mkvsynth Core: Unexpected problem initializing encoder\n");
		return -1;
	}

	// Open a file, because for this example we are saving the file
	FILE *output = output = fopen("surprise.h264", "w");
	if (output == NULL) {
		printf("Mkvsynth Core: Could not open output file!\n");
		return -1;
	}

	int counter = 0;
	while(nextFrame(decodeContext) != -1 && counter < 10) {
		// Resize the frame
		int resizeCode = resizeFrame(PIX_FMT_RGB24, 320, 480, decodeContext->frame);
		if(resizeCode == -1) {
			printf("Mkvsynth Core: Frame Resize Failed!\n");
			return -1;
		}

		// Encode the frame; compress the frame
		encodeFrame(decodeContext->frame, encodeContext);
		// No errors are expected, no error handling yet established
		// This will change eventually

		//Write the frame to disk
		int fwriteStatus = writeEncodeToFile(output, encodeContext);
		if(fwriteStatus < 0) {
			printf("Mkvsynth Core: error while writing to file\n");
			return -1;
		}

		counter++;
		printf("Printed frame %i\n", counter);
	}

	int tmp = gotoFrame(8, decodeContext);
	if(tmp == -1) {
		printf("Mkvsynth Core: Error with gotoFrame\n");
		return -1;
	}

	counter = 0;
	while(nextFrame(decodeContext) != -1 && counter < 100) {
		// Resize the frame
		int resizeCode = resizeFrame(PIX_FMT_RGB24, 320, 480, decodeContext->frame);
		if(resizeCode == -1) {
			printf("Mkvsynth Core: Frame Resize Failed!\n");
			return -1;
		}

		// Encode the frame; compress the frame
		encodeFrame(decodeContext->frame, encodeContext);
		// No errors are expected, no error handling yet established
		// This will change eventually

		//Write the frame to disk
		int fwriteStatus = writeEncodeToFile(output, encodeContext);
		if(fwriteStatus < 0) {
			printf("Mkvsynth Core: error while writing to file\n");
			return -1;
		}

		counter++;
		printf("Printed frame %i\n", counter+50);
	}

	closeDecoder(decodeContext);
	return 1;
}
