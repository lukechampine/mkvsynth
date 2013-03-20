// To start things off, the focus will be on RGB video from x264. Eventually, we will expand to most major colorspaces

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <x264.h>
#include <stdio.h>
#include "resize.c"

// -Hopefully- all information needed to decode a video stream
typedef struct DecodeContext {
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVPacket packet;
	AVDictionary *optionsDictionary;

	int videoStream;
	int frameFinished;
};

// -Hopefully- all information needed to encode a video stream
typedef struct EncodeContext {
	x264_param_t param;
	x264_t *encoder;
	x264_picture_t picIn;
	x264_picture_t picOut;
	x264_nal_t *nals;

	int frameSize;
	int i_nals;
};

// THOUGHT: put function in it's own file?
// -Hopefully- takes any ffmpeg-recognized video and prepares it for decoding
int initializeDecoder(struct DecodeContext *decodeContext, char *filename) {
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->optionsDictionary = NULL;

	decodeContext->videoStream = -1;
    
	int i=0;

  av_register_all();

  if(avformat_open_input(&decodeContext->formatContext, filename, NULL, NULL) != 0)
      return -1;
  if(avformat_find_stream_info(decodeContext->formatContext, NULL) < 0)
      return -1;

	for(i=0; i<decodeContext->formatContext->nb_streams; i++) {
			if(decodeContext->formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
					decodeContext->videoStream = i;
					break;
			}
	}

	if(decodeContext->videoStream == -1)
		return -1;

	decodeContext->codecContext = decodeContext->formatContext->streams[decodeContext->videoStream]->codec;
	decodeContext->codec = avcodec_find_decoder(decodeContext->codecContext->codec_id);
	if(decodeContext->codec == NULL)
		return -1; //unsupported codec
	
	if(avcodec_open2(decodeContext->codecContext, decodeContext->codec, &decodeContext->optionsDictionary) < 0)
		return -1;

	decodeContext->frame = avcodec_alloc_frame();

	return 1;
}

// Close memeory, etc.
int closeDecoder(struct DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	return 1;
}

// THOUGHT: put function in it's own file?
// -Hopefully- takes any chain of frames and encodes them
int initializeEncoder(struct EncodeContext *encodeContext, struct DecodeContext *decodeContext) {
	// Set the parameters for the encode
	x264_param_default_preset(&encodeContext->param, "slow", "");
	encodeContext->param.i_threads = 4;
	encodeContext->param.i_width = decodeContext->codecContext->width;
	encodeContext->param.i_height = decodeContext->codecContext->height;
	encodeContext->param.i_fps_den = 1;
	encodeContext->param.i_keyint_max = 24;
	encodeContext->param.b_intra_refresh = 1;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 18;
	x264_param_apply_profile(&encodeContext->param, "baseline");
	
	encodeContext->encoder = x264_encoder_open(&encodeContext->param);
	x264_encoder_parameters(encodeContext->encoder, &encodeContext->param);

	x264_picture_alloc(&encodeContext->picIn, X264_CSP_I420, decodeContext->codecContext->width, decodeContext->codecContext->height);
	return 1;
}

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
		printf("some encoder error!\n");
		return -1;
	}

	FILE *output; 
	output = fopen("surprise.h264", "w");
	if (output == NULL) {
		printf("Could not find output file!\n");
		return 0;
	}
	
	int counter = 0;
	while(av_read_frame(decodeContext.formatContext, &decodeContext.packet) >= 0) {
		if(decodeContext.packet.stream_index == decodeContext.videoStream) {
			avcodec_decode_video2(decodeContext.codecContext, decodeContext.frame, &decodeContext.frameFinished, &decodeContext.packet);
			if(decodeContext.frameFinished) {
				resizeFrameO(decodeContext.codecContext->width, decodeContext.codecContext->height, decodeContext.codecContext, decodeContext.frame, &encodeContext.picIn);
				encodeContext.frameSize = x264_encoder_encode(encodeContext.encoder, &encodeContext.nals, &encodeContext.i_nals, &encodeContext.picIn, &encodeContext.picOut);
				if(encodeContext.frameSize >= 0) {
					int j;
					for(j=0; j<encodeContext.i_nals; j++) {
						fwrite(encodeContext.nals[j].p_payload, encodeContext.nals[j].i_payload, 1, output);
					}
				}

				counter++;
				printf("Processing Frame %i\n", counter);
			}
		}
	}

	closeDecoder(&decodeContext);
	fclose(output);
	return 1;
}
