#ifndef ffmpegDecode_c_
#define ffmpegDecode_c_

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

/////////////////////////////////////
// The Threaded Function Arguments //
/////////////////////////////////////
struct ffmpegDecode {
	int videoStream;
	AVFormatContext *formatContext;
	AVCodecContext *codecContext;
	AVFrame *frame;
	struct SwsContext *resizeContext;
	MkvsynthOutput *output;
};

MkvsynthOutput* ffmpegDecodeDefinition(ASTnode *p, ASTnode *args) {

	///////////////////////////
	// Parse Input Arguments //
	///////////////////////////
	checkArgs("ffmpegDecode", args, 1, typeStr);
	char *filename = MANDSTR();

	struct ffmpegDecode *params = malloc(sizeof(struct ffmpegDecode));
	params->output = createOutputBuffer();
	
	//////////////////////////////////////
	// Error Checking And Initializtion //
	//////////////////////////////////////
	AVFormatContext *formatContext = NULL;
	AVCodecContext *codecContext = NULL;
	AVCodec *codec = NULL;
	AVDictionary *optionsDictionary = NULL;
	int videoStream = -1;
	int frameFinished;
	struct SwsContext *resizeContext;
	
	av_register_all();
	
	int avOpen = avformat_open_input(
		&formatContext,
		(struct ffmpegDecode *)filterParams->filename,
		NULL,
		NULL);

	if(avOpen != 0) {
		filterError("Input file could not be opened.");
	}
	
	if(avformat_find_stream_info(formatContext, NULL) < 0) {
		filterError("Input file does not seem to be a video.");
	}
	
	int i;
	for(i=0; i<formatContext->nb_streams; i++) {
		if(formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	
	if(videoStream == -1) {
		filterError("Input file does not seem to have a video stream.");
	}

	codecContext = formatContext->streams[videoStream]->codec;

	codec = avcodec_find_decoder(codecContext->codec_id);
	if(decodeContext->codec == NULL) {
		filterError("Unrecognized video codec.");
	}

	int openCodec = avcodec_open2(
		codecContext,
		codec,
		&optionsDictionary);

	if(openCodec < 0) {
		filterError("Failed to open codec.");
	}
	
	resizeContext = sws_getContext (
		codecContext->width,
		codecContext->height,
		codecContext->pix_fmt,
		codecContext->width,
		codecContext->height,
		PIX_FMT_RGB24,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL);

	///////////////////////
	// Memory Allocation //
	///////////////////////
	AVFrame *frame = avcodec_alloc_frame();
	if(frame == NULL) {
		filterError("Failed to allocate the decoding frame.");
	}
	
	///////////////
	// Meta Data //
	///////////////
	ffmpegParams->output->metaData->width = codecContext->width;
	ffmpegParams->output->metaData->height = codecContext->height;
	ffmpegParams->output->metaData->channels = 3;
	ffmpegParams->output->metaData->depth = 8;
	ffmpegParams->output->metaData->colorspace = 0;
	ffmpegParams->output->metaData->bytes = 3*codecContext->width*codecContext->height;
	
	/////////////////////////
	// Pass Decode Context //
	/////////////////////////
	params->videoStream = videoStream;
	params->formatContext = formatContext;
	params->codecContext = codecContext;
	params->frame = frame;
	params->resizeContext = resizeContext;
	
	//////////////////////
	// Queue and Return //
	//////////////////////
	mkvsynthQueue((void *)ffmpegParams, ffmpegDecode);
	return ffmpegParams->output;
}

void ffmpegDecode(void *filterParams) {
	//////////////////////
	// Decode Variables //
	//////////////////////
	int frameFinished;
	int newLinesize = 3*width;
	AVPacket packet;
	
	struct ffmpegDecode *params = (ffmpegDecode *)filterParams;
	
	/////////////////
	// Decode Loop //
	/////////////////
	while(av_read_frame(params->formatContext, &packet) >= 0) {
		if(packet.stream_index == params->videoStream) {
			avcodec_decode_video2(
				params->codecContext,
				params->frame,
				&frameFinished,
				&packet);

			if(frameFinished) {
				uint8_t *payload = malloc(3*width*height);
				sws_scale (
					params->resizeContext,
					(uint8_t const * const *)params->frame->data,
					params->frame->linesize,
					0,
					params->frame->height,
					payload,
					newLinesize);
			
				putFrame(params->output, payload);
			}
		}
		
		av_free_packet(&packet);
	}

	////////////////////////
	// Stream Termination //
	////////////////////////
	putFrame(NULL);
	
	/////////////////////////
	// Memory Deallocation //
	/////////////////////////
	av_free(params->frame);
	avcodec_close(params->codecContext);
	avformat_close_input(&params->formatContext);
	free(params);
}

#endif
