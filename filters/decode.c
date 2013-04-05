#include "datatypes.h"
#include "resize.c"

// Takes in a filename, returns a DecodeContext
// 	DecodeContext structures are used to get specific frames from a video
DecodeContext* openDecoder(char *filename) {
	// All memory management is done in-file
	DecodeContext *decodeContext = malloc(sizeof(DecodeContext));

	// Initialize struct to null/error values
	decodeContext->filename = filename;
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->optionsDictionary = NULL;
	decodeContext->videoStream = -1;
 
	// Not sure what it does, but it's important
  av_register_all();

	// Turns a filename into a datastruture
	int avOpen = avformat_open_input(
		&decodeContext->formatContext,
		filename,
		NULL,
		NULL
	);
  if(avOpen != 0) {
		printf("Mkvsynth Decoder: input file coule not be opened\n");
		return NULL;
	}
	
	// Also not sure, but also important
  if(avformat_find_stream_info(decodeContext->formatContext, NULL) < 0) {
		printf("Mkvsynth Decoder: input file does not appear to be a video\n");
		return NULL;
	}

	// Find the video stream
	int i;
	for(i=0; i<decodeContext->formatContext->nb_streams; i++) {
			if(decodeContext->formatContext->streams[i]->codec->codec_type
																								== AVMEDIA_TYPE_VIDEO) {
				decodeContext->videoStream = i;
				break; // Found video stream
			}
	}
	if(decodeContext->videoStream == -1) {
		printf("Mkvsynth Decoder: input file does not seem to have a video stream\n");
		return NULL;
	}

	// Create a shortcut
	decodeContext->codecContext = 
		decodeContext->formatContext->streams[decodeContext->videoStream]->codec;
	
	// Get the correct decoder
	decodeContext->codec = 
		avcodec_find_decoder(decodeContext->codecContext->codec_id);
	if(decodeContext->codec == NULL) {
		printf("Mkvsynth Decoder: unknown video codec\n");
		return NULL;
	}
	
	// ??, important, memory allocation?
	int openCodec = avcodec_open2(
		decodeContext->codecContext,
		decodeContext->codec,
		&decodeContext->optionsDictionary
	);
	if(openCodec < 0) {
		printf("Mkvsynth Decoder: failed to open codec\n");
		return NULL;
	}
	
	// Dynamically allocate space for a frame
	decodeContext->frame = avcodec_alloc_frame();

	return decodeContext;
}

int nextFrame(DecodeContext *decodeContext) {
	int flag = 0;
	while(av_read_frame(decodeContext->formatContext, &decodeContext->packet) >= 0) {
		flag = 1;
		if(decodeContext->packet.stream_index == decodeContext->videoStream) {
			avcodec_decode_video2(
				decodeContext->codecContext,
				decodeContext->frame,
				&decodeContext->frameFinished,
				&decodeContext->packet
			);

			if(decodeContext->frameFinished)
				break;
		}
	}

	if(flag == 0)
		return -1;
	return 1;
}

int gotoFrame(int frame, DecodeContext *decodeContext) {
	decodeContext->formatContext = NULL;
	int avOpen = avformat_open_input(
		&decodeContext->formatContext,
		decodeContext->filename,
		NULL,
		NULL
	);
  if(avOpen != 0) {
		printf("Mkvsynth Decoder: input file coule not be opened\n");
		return -1;
	}

	int i = 0;
	while(nextFrame(decodeContext) != -1 && frame > i)
		i++;

	if(frame == i)
		return 1;

	printf("Mkvsynth Decoder: failed to goto frame\n");
	return -1;
}

// We allocated a lot of memory; deallocate it all
int closeDecoder(DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	free(decodeContext);
	return 1;
}
