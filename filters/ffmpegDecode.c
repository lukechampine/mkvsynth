#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

void ffmpegDecode(ASTParams *filterParams, MkvsynthPutParams *putParams) {
	
	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char* filename = checkArgs(filterParams, "file", string);

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
		// Error: input file could not be opened
	}
	
	if(avformat_find_stream_info(formatContext, NULL) < 0) {
		// Error: input file does not seem to be a video
	}
	
	int i;
	for(i=0; i<formatContext->nb_streams; i++) {
		if(formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	
	if(videoStream == -1) {
		// Error: input file does not seem to have a video stream
	}

	codecContext = formatContext->streams[videoStream]->codec;

	codec = avcodec_find_decoder(codecContext->codec_id);
	if(decodeContext->codec == NULL) {
		// Error: unrecognized video codec
	}

	int openCodec = avcodec_open2(
		codecContext,
		codec,
		&optionsDictionary);

	if(openCodec < 0) {
		// Error: failed to open codec
	}
	
	///////////////////////
	// Memory Allocation //
	///////////////////////
	AVFrame *frame = avcodec_alloc_frame();
	AVFrame *newFrame = avcodec_alloc_frame();
	if(frame == NULL || newFrame == NULL) {
		// Error!
	}
	
	uint8_t *buffer;
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
					putParams->metaData->width = frame->width;
					putParams->metaData->height = frame->height;
					putParams->metaData->channels = 3;
					putParams->metaData->depth = 8;
					putParams->metaData->colorspace = GENERIC_RGB;
					signalStartupCompletion();

					
					int numBytes = avpicture_get_size(PIX_FMT_RGB24, frame->width, frame->height);
					buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
					avpicture_fill((AVPicture *)newFrame, buffer, PIX_FMT_RGB24, frame->width, frame->height);
		
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
					newFrame->data,
					newFrame->linesize);
			
				putFrame(newFrame->data);
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
	free(buffer);
	av_free(frame);
	av_free(newFrame);
	avcodec_close(codecContext);
	avformat_close_input(&formatContext);
}
