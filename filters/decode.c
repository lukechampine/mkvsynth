#include "datatypes.h"
#include "resize.c"

// Open decoder, find video stream, allocate space for frames
int initializeDecoder(char *filename, DecodeContext *decodeContext) {
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->optionsDictionary = NULL;
	decodeContext->videoStream = -1;
 
  av_register_all();

	int avOpen = avformat_open_input(
		&decodeContext->formatContext,
		filename,
		NULL,
		NULL
	);

  if(avOpen != 0)
      return -1; // Input file could not be opened
	
  if(avformat_find_stream_info(decodeContext->formatContext, NULL) < 0)
      return -1; // Doesn't appear do be a file

	int i;
	for(i=0; i<decodeContext->formatContext->nb_streams; i++) {
			if(decodeContext->formatContext->streams[i]->codec->codec_type
																								== AVMEDIA_TYPE_VIDEO) {
				decodeContext->videoStream = i;
				break; // Found video stream
			}
	}

	if(decodeContext->videoStream == -1)
		return -1; // Did not find video stream

	decodeContext->codecContext = 
		decodeContext->formatContext->streams[decodeContext->videoStream]->codec;
	
	decodeContext->codec = 
		avcodec_find_decoder(decodeContext->codecContext->codec_id);
	
	if(decodeContext->codec == NULL)
		return -1; // Coudn't figure out codec
	
	int openCodec = avcodec_open2(
		decodeContext->codecContext,
		decodeContext->codec,
		&decodeContext->optionsDictionary
	);

	if(openCodec < 0)
		return -1; // failed to open codec
	
	decodeContext->frame = avcodec_alloc_frame();

	return 1;
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

			decodeContext->frame->format = decodeContext->codecContext->pix_fmt;

			if(decodeContext->frameFinished)
				break;
			
		}
	}

	if(flag == 0)
		return -1;
	return 1;
}

int closeDecoder(DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	return 1;
}
