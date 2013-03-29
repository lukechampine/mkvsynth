#include "datatypes.h"
#include "decode.c"
#include "encode.c"
#include "resize.c"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a movie file\n");
    return -1;
  }

	DecodeContext decodeContext;
	EncodeContext encodeContext;

	if(initializeDecoder(argv[1], &decodeContext) != 1) {
		printf("failed to initialize the decoder!\n");
		return -1;
	}
  
	if(initializeEncoder(&decodeContext, &encodeContext, decodeContext.codecContext->width, decodeContext.codecContext->height) != 1) {
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
	while(nextFrame(&decodeContext) != -1) {
		printf("%i \n", counter);
		encodeFrame(&decodeContext, &encodeContext);
		
		if(encodeContext.frameSize >= 0) {
			int j;
			for(j=0; j<encodeContext.i_nals; j++) {
				int warningKiller = fwrite(encodeContext.nals[j].p_payload, encodeContext.nals[j].i_payload, 1, output);
				if(warningKiller < 0)
					printf("There was a strange error.\n");
			}
		}
		counter++;
	}
	
	closeDecoder(&decodeContext);
	//fclose(output);
	return 1;
}
