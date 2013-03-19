// To start things off, the focus will be on RGB video from x264. Eventually, we will expand to most major colorspaces

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <x264.h>
#include <stdio.h>
#include "resize.c"


// The hope was to have the structs be all pointers
// but there were some syntactic errors and the easiest
// fix was eliminating pointers.
struct DecodeContext {
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVPacket packet;
	AVDictionary *optionsDictionary;

	int videoStream;
	int frameFinished;
};

struct EncodeContext {
	x264_param_t param;
	x264_t *encoder;
	x264_picture_t picIn;
	x264_picture_t picOut;
	x264_nal_t *nals;

	int *frameSize;
	int *i_nals;
};

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

int closeDecoder(struct DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	return 1;
}

int initializeEncoder(struct EncodeContext *encodeContext, struct DecodeContext *decodeContext) {
	x264_param_default_preset(&encodeContext->param, "placebo", "");
	encodeContext->param.i_width = decodeContext->codecContext->width;
	encodeContext->param.i_height = decodeContext->codecContext->height;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 0;
	x264_param_apply_profile(&encodeContext->param, "high444");
	
	encodeContext->encoder = x264_encoder_open(&encodeContext->param);

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
		printf("some decoder error!\n");
	}

	int counter = 0;
	while(av_read_frame(decodeContext.formatContext, &decodeContext.packet) >= 0) {
		if(decodeContext.packet.stream_index == decodeContext.videoStream) {
			avcodec_decode_video2(decodeContext.codecContext, decodeContext.frame, &decodeContext.frameFinished, &decodeContext.packet);
			if(decodeContext.frameFinished) {
				x264_picture_alloc(&encodeContext.picIn, X264_CSP_RGB, decodeContext.codecContext->width, decodeContext.codecContext->height);
				
				printf("Decoded theoretical frame %i\n", counter);

				x264_picture_alloc(&encodeContext.picIn, X264_CSP_RGB, decodeContext.codecContext->width, decodeContext.codecContext->height);

				printf("test000\n");
				
				resizeFrame(decodeContext.codecContext->width, decodeContext.codecContext->height, decodeContext.codecContext, decodeContext.frame, &encodeContext.picIn);

				printf("resized it!\n");
				
				x264_nal_t *nals;
				int i_nals;
				x264_picture_t pic_out;
				x264_picture_t pic_in;
				x264_picture_alloc(&pic_in, X264_CSP_RGB, decodeContext.codecContext->width, decodeContext.codecContext->height);

				struct SwsContext *cc = sws_getContext(decodeContext.codecContext->width, decodeContext.codecContext->height, decodeContext.codecContext->pix_fmt, encodeContext->param.i_width, encodeContext->param.i_height, PIX_FMT_RGB24, SWS_SPLINE, NULL, NULL, NULL);

				x264_encoder_encode(encodeContext.encoder, &nals, &i_nals, &encodeContext.picIn, &pic_out);
				
				counter++;
			}
		}
	}

	closeDecoder(&decodeContext);

	return 1;
}
