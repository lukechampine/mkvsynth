#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

void ffmpegDecode(ASTParams *filterParams) {
	
	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char* filename = checkString(filterParams, "file");
	MkvsynthOutput *output = checkOutput(filterParams, "output1");

	//////////////////////////////////////
	// Error Checking And Initializtion //
	//////////////////////////////////////
	AVFormatContext *formatContext = NULL;
	AVCodecContext *codecContext = NULL;
	AVCodec *codec = NULL;
	AVPacket packet;
	AVDictionary *optionsDictionary = NULL;
	int videoStream = -1;
	int frameFinished;
	
	av_register_all();
	
	int avOpen = avformat_open_input(
		&formatContext,
		filename,
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
	
	///////////////////////
	// Memory Allocation //
	///////////////////////
	AVFrame *frame = avcodec_alloc_frame();
	if(frame == NULLL) {
		filterError("Failed to allocate the decoding frame.");
	}
	
	struct SwsContext *resizeContext;
	char memoryAllocated = 0;
	
	/////////////////
	// Decode Loop //
	/////////////////
	while(av_read_frame(formatContext, &packet) >= 0) {
		if(packet.stream_index == videoStream) {
			avcodec_decode_video2(
				codecContext,
				frame,
				&frameFinished,
				&packet);

			if(frameFinished) {
				// Meta data was unknown until this point.
				if(memoryAllocated == 0) {
					//////////////////////////////////////////
					// Meta Data and More Memory Allocation //
					//////////////////////////////////////////
					output->metaData->width = frame->width;
					output->metaData->height = frame->height;
					output->metaData->channels = 3;
					output->metaData->depth = 8;
					output->metaData->colorspace = GENERIC_RGB;
					output->metaData->bytes = 3*frame->width*frame->height;
					signalStartupCompletion();
					
					uint8_t *payload = malloc(3*width*height);
					int newLinesize = 3*width;
		
					resizeContext = sws_getContext (
						frame->width,
						frame->height,
						frame->format,
						frame->width,
						frame->height,
						PIX_FMT_RGB24,
						SWS_SPLINE,
						NULL,
						NULL,
						NULL);
					
					memoryAllocated = 1;
				}

				sws_scale (
					resizeContext,
					(uint8_t const * const *)frame->data,
					frame->linesize,
					0,
					frame->height,
					payload,
					newLinesize);
			
				putFrame(output, payload);
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
	av_free(frame);
	avcodec_close(codecContext);
	avformat_close_input(&formatContext);
	terminate();
}
