#include "datatypes.h"
#include "resize.c"

/*****
 * used for decoding video and accessing frames within decoded video
 ****/

// Takes in a filename, returns a DecodeContext
// 	DecodeContext structures are used to get specific frames from a video
// Expansion is planned so that it can take in raw data as well as a filename
DecodeContext * openDecoder(char *filename) {
	// All memory management is done in-file
	DecodeContext *decodeContext = malloc(sizeof(DecodeContext));

	// Initialize struct to null/error values
	decodeContext->filename = filename;
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
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
	
	// ??, important, I think it brings the correct decoder into memory
	int openCodec = avcodec_open2(
		decodeContext->codecContext,
		decodeContext->codec,
		&decodeContext->optionsDictionary
	);
	
	if(openCodec < 0) {
		printf("Mkvsynth Decoder: failed to open codec\n");
		return NULL;
	}
	
	return decodeContext;
}

// Allocates memeory for a frame, and then fills it with
// the next frame from decodeContext
AVFrame * nextFrame(DecodeContext *decodeContext) {
	AVFrame *frame = NULL;
	frame = avcodec_alloc_frame();

	int flag = 0;
	while(av_read_frame(decodeContext->formatContext, &decodeContext->packet) >= 0) {
		flag = 1;
		if(decodeContext->packet.stream_index == decodeContext->videoStream) {
			avcodec_decode_video2(
				decodeContext->codecContext,
				frame,
				&decodeContext->frameFinished,
				&decodeContext->packet
			);

			if(decodeContext->frameFinished)
				break;
		}
	}

	if(flag == 0)
		return NULL;
	return frame;
}

// Uses nextFrame to start from a certain frame and go up to the given frame
// Properly deallocates all frames that aren't returned
AVFrame * gotoFrame(int frameNumber, DecodeContext *decodeContext) {
	// All memory allocation included inside this file
	AVFrame *frame = NULL;
	// Destructor can be caaled with av_free(frame)

	decodeContext->formatContext = NULL;

	int avOpen = avformat_open_input(
		&decodeContext->formatContext,
		decodeContext->filename,
		NULL,
		NULL
	);

  if(avOpen != 0) {
		printf("Mkvsynth Decoder: input file coule not be opened\n");
		return NULL;
	}

	int i = 0;
	while((frame = nextFrame(decodeContext)) && frame != NULL && frameNumber > i) {
		frame = nextFrame(decodeContext);
		if(frame == NULL)
			break;

		i++;

		// free the memory allocated by nextFrame
		if(frameNumber != i)
			av_free(frame);
	}

	if(frameNumber == i)
		return frame;

	printf("Mkvsynth Decoder: failed to goto frame\n");
	return NULL;
}

// We allocated a lot of memory; deallocate it all
int closeDecoder(DecodeContext *decodeContext) {
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	free(decodeContext);
	return 1;
}
