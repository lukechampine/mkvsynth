#include "datatypes.h"
#include "decode.c"
#include "encode.c"
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
	EncodeContext *encodeContext = openEncoder(160, 120);
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
	while(nextFrame(decodeContext) != -1 && counter < 500) {
		// Resize the frame
		AVFrame *newFrame = resizeFrame(PIX_FMT_RGB24, 160, 120, decodeContext->frame);
		if(newFrame == NULL) {
			printf("Mkvsynth Core: Frame Resize Failed!\n");
			return -1;
		}

		// Encode the frame; compress the frame
		encodeFrame(newFrame, encodeContext);
		// The x264 encoder does all of it's own error handling

		//Write the frame to disk
		int fwriteStatus = writeEncodeToFile(output, encodeContext);
		if(fwriteStatus < 0) {
			printf("Mkvsynth Core: error while writing to file\n");
			return -1;
		}

		counter++;
		printf("Printed frame %i\n", counter);
	}

	closeDecoder(decodeContext);
	return 1;
}
