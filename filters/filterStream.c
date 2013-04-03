#include "datatypes.h"
#include "decode.c"
#include "encode.c"
#include "frame.c"
#include "resize.c"
#include <stdio.h>
#include <string.h>
int main() {
	DecodeContext *decodeContext = malloc(sizeof(DecodeContext));
	if(initializeDecoder("test.mp4", decodeContext) != 1) {
		printf("failed to initialize the decoder!\n");
		return -1;
	}

	EncodeContext *encodeContext = malloc(sizeof(EncodeContext));
	if(initializeEncoder(200, 200, encodeContext) != 1) {
		printf("failed to initialize the encoder!\n");
		return -1;
	}

	FILE *output; 
	output = fopen("surprise.h264", "w");
	if (output == NULL) {
		printf("Could not find output file!\n");
		return 0;
	}

	int counter = 1;
	while(nextFrame(decodeContext) != -1 && counter < 500) {
		printf("%i \n", counter);
		int resized = resizeFrame(PIX_FMT_RGB24, 320, 480, decodeContext->frame);
		encodeFrame(decodeContext->frame, encodeContext);
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
	return 0;
}
