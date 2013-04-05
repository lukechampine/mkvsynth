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
	DecodeContext *decodeContext = initializeDecoder("test.mp4");
	// Error checking for previous statment
	// initializeDecoder() has it's own error handling
	if(decodeContext == NULL) {
		printf("Mkvsynth Core: Decoding Failed\n");
		return -1;
	}


	// Call each time you want to encode a video
	EncodeContext *encodeContext = initializeEncoder(200, 200);
	// Error Checking
	// Erros are unexpected
	if(encodeContext == NULL) {
		printf("Mkvsynth Core: Unexpected problem initializing encoder\n");
		return -1;
	}

	// Open a file, because for this example we are saving the file
	FILE *output = output = fopen("surprise.h264", "w");
	// Error check the opened file
	if (output == NULL) {
		printf("Mkvsynth Core: Could not open output file!\n");
		return -1;
	}

	// All functions happen on a per frame basis in this loop
	// Still a work in progres...
	while(nextFrame(decodeContext) != -1) {
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
		int fwriteStatus = writeToFile(output, encodeContext);
		// Error Check the write call
		if(fwriteStatus < 0) {
			printf("Mkvsynth Core: error while writing to file\n");
			return -1;
		}
	}

	closeDecoder(decodeContext);
	return 1;
}
