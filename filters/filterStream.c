#include "datatypes.h"
#include "decode.c"
#include "encode.c"
#include "frame.c"
#include "resize.c"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	// Call each time you want to decode a video
	DecodeContext *decodeContext = initializeDecoder("test.mp4");

	// Error checking for previous statment
	// initializeDecoder() has it's own error handling
	if(decodeContext == NULL)
		return -1;

	// Call each time you want to encode a video
	EncodeContext *encodeContext = initializeEncoder(200, 200);

	// Error Checking
	// Erros are unexpected
	if(encodeContext == NULL) {
		printf("Unexpected problem initializing encoder\n");
		return -1;
	}

	// This code is a hack, to get output in the early stages
	FILE *output; 
	output = fopen("surprise.h264", "w");
	if (output == NULL) {
		printf("Could not find output file!\n");
		return 0;
	}

	int counter = 1;
	while(nextFrame(decodeContext) != -1 && counter < 500) {
		int resized = resizeFrame(PIX_FMT_RGB24, 320, 480, decodeContext->frame);
		encodeFrame(decodeContext->frame, encodeContext);

		// This is also a hack, will be expanded and fixed
		if(encodeContext->frameSize >= 0) {
			int j;
			for(j=0; j<encodeContext->i_nals; j++) {
				int warningKiller = fwrite(encodeContext->nals[j].p_payload, encodeContext->nals[j].i_payload, 1, output);
				if(warningKiller < 0)
					printf("Error writing to file.\n");
			}
		}
		counter++;
	}
	
	closeDecoder(decodeContext);
	return 1;
}
