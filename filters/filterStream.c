// To start things off, the focus will be on RGB video from x264. Eventually, we will expand to most major colorspaces

#include "decode.c"
#include "encode.c"
#include "resize.c"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a movie file\n");
    return -1;
  }

	struct DecodeContext decodeContext;
	struct EncodeContext encodeContext;

	if(initializeDecoder(&decodeContext, argv[1]) != 1) {
		printf("failed to initialize the decoder!\n");
		return -1;
	}
  
	if(initializeEncoder(&encodeContext, &decodeContext) != 1) {
		printf("failed to initialize the encoder!\n");
		return -1;
	}

	FILE *output; 
	output = fopen("surprise.h264", "w");
	if (output == NULL) {
		printf("Could not find output file!\n");
		return 0;
	}
	
	while(av_read_frame(decodeContext.formatContext, &decodeContext.packet) >= 0) {
		if(decodeContext.packet.stream_index == decodeContext.videoStream) {
			avcodec_decode_video2(decodeContext.codecContext, decodeContext.frame, &decodeContext.frameFinished, &decodeContext.packet);
			if(decodeContext.frameFinished)
				break;
		}
	}

	closeDecoder(&decodeContext);
	fclose(output);
	return 1;
}


				resizeFrame(decodeContext.codecContext->width, decodeContext.codecContext->height, decodeContext.codecContext, decodeContext.frame, &encodeContext.picIn);
				encodeContext.frameSize = x264_encoder_encode(encodeContext.encoder, &encodeContext.nals, &encodeContext.i_nals, &encodeContext.picIn, &encodeContext.picOut);
				if(encodeContext.frameSize >= 0) {
					int j;
					for(j=0; j<encodeContext.i_nals; j++) {
						fwrite(encodeContext.nals[j].p_payload, encodeContext.nals[j].i_payload, 1, output);
					}
				}

				counter++;
				printf("Processing Frame %i\n", counter);
