#ifndef ffmpegDecode_c_
#define ffmpegDecode_c_

#include "../jarvis/jarvis.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <stdio.h>

/////////////////////////////////////
// The Threaded Function Arguments //
/////////////////////////////////////
struct ffmpegDecode {
	AVFormatContext *formatContext;
	AVCodecContext *codecContext;
	AVCodec *codec;
	AVFrame *frame;
	AVFrame *rgbFrame;
	AVDictionary *dictionary;
	struct SwsContext *resizeContext;
	int i;
	int videoStream;
	int frameFinished;
	int bytes;
	uint8_t *payload;
	uint8_t *payload2;

	MkvsynthOutput *output;
};

void *ffmpegDecode(void *filterParams) {
	struct ffmpegDecode *params = (struct ffmpegDecode *)filterParams;

	/////////////////
	// Decode Loop //
	/////////////////
	AVPacket packet;
	while(av_read_frame(params->formatContext, &packet) >= 0) {
		if(packet.stream_index == params->videoStream) {
			avcodec_decode_video2(
				params->codecContext,
				params->frame,
				&params->frameFinished,
				&packet);

			if(params->frameFinished) {
				sws_scale (
					params->resizeContext,
					(uint8_t const * const *)params->frame->data,
					params->frame->linesize,
					0,
					params->codecContext->height,
					params->rgbFrame->data,
					params->rgbFrame->linesize);
			
				int y;
				params->payload2 = malloc(6*params->codecContext->width*params->codecContext->height);
  
				for(y = 0; y < 3*params->codecContext->width*params->codecContext->height; y++) {
					params->payload2[y*2] = 0;
					params->payload2[y*2+1] = *(params->rgbFrame->data[0]+y);
				}
				putFrame(params->output, params->payload2);
			}
		}
		
		av_free_packet(&packet);
	}

	////////////////////////
	// Stream Termination //
	////////////////////////
	putFrame(params->output, NULL);
	
	/////////////////////////
	// Memory Deallocation //
	/////////////////////////
	av_free(params->frame);
	av_free(params->rgbFrame);
	av_free(params->payload);
	avcodec_close(params->codecContext);
	avformat_close_input(&params->formatContext);
	free(params);
}

ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args) {

	///////////////////////////
	// Parse Input Arguments //
	///////////////////////////
	struct ffmpegDecode *params = malloc(sizeof(struct ffmpegDecode));
	checkArgs("ffmpegDecode", args, 1, typeStr);
	char *filename = MANDSTR();
	params->output = createOutputBuffer();

	//////////////////////////////
	// Initialize Stuff To NULL //
	//////////////////////////////
	params->formatContext = NULL;
	params->codecContext = NULL;
	params->codec = NULL;
	params->frame = NULL;
	params->rgbFrame = NULL;
	params->payload = NULL;
	
	//////////////////////////////////////
	// Error Checking And Initializtion //
	//////////////////////////////////////
	av_register_all();
	
	int avOpen = avformat_open_input(&params->formatContext, filename, NULL, NULL);

	if(avOpen != 0) {
		printf("Input file could not be opened.\n");
		exit(0);
	}
	
	if(avformat_find_stream_info(params->formatContext, NULL) < 0) {
		printf("Input file does not seem to be a video.\n");
		exit(0);
	}

	av_dump_format(params->formatContext, 0, filename, 0);
	
	for(params->i=0; params->i<params->formatContext->nb_streams; params->i++) {
		if(params->formatContext->streams[params->i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			params->videoStream = params->i;
			break;
		}
	}
	
	if(params->videoStream == -1) {
		printf("Input file does not seem to have a video stream.\n");
		exit(0);
	}

	params->codecContext = params->formatContext->streams[params->videoStream]->codec;

	params->codec = avcodec_find_decoder(params->codecContext->codec_id);
	if(params->codec == NULL) {
		printf("Unrecognized video codec.\n");
		exit(0);
	}

	int openCodec = avcodec_open2(params->codecContext, params->codec, &params->dictionary);

	if(openCodec < 0) {
		printf("Failed to open codec.\n");
		exit(0);
	}

	params->frame = avcodec_alloc_frame();
	params->rgbFrame = avcodec_alloc_frame();

	params->bytes = avpicture_get_size(PIX_FMT_RGB24, params->codecContext->width, params->codecContext->height);
	params->payload = (uint8_t *)av_malloc(params->bytes*sizeof(uint8_t));
	
	params->resizeContext = sws_getContext (
		params->codecContext->width,
		params->codecContext->height,
		params->codecContext->pix_fmt,
		params->codecContext->width,
		params->codecContext->height,
		PIX_FMT_RGB24,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL);

	avpicture_fill((AVPicture *)params->rgbFrame, params->payload, PIX_FMT_RGB24, params->codecContext->width, params->codecContext->height);

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->width = params->codecContext->width;
	params->output->metaData->height = params->codecContext->height;
	params->output->metaData->colorspace = MKVS_RGB48;
	params->output->metaData->bytes = params->bytes * 2;
	//params->output->metaData->fpsNumerator = params->codecContext->time_base.num;
	//params->output->metaData->fpsDenominator = params->codecContext->time_base.den;
	params->output->metaData->fpsNumerator = 60;
	params->output->metaData->fpsDenominator = 1;
	
	//////////////////////
	// Queue and Return //
	//////////////////////
	mkvsynthQueue((void *)params, ffmpegDecode);
	RETURNCLIP(params->output);
}

#endif
