#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

// Everything needed to decode/track a video stream
struct DecodeContext {
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVFrame *activeFrame;
	AVPacket packet;
	AVDictionary *optionsDictionary;

	enum AVPixelForamt activeColorSpace;

	int videoStream;
	int frameFinished;
};

// Open decoder, find video stream, allocate space for frames
int initializeDecoder(char *filename, struct DecodeContext *decodeContext) {
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->activeFrame = NULL;
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
		return -1; // Coudn't figure out codec?
	
	int openCodec = avcodec_open2(
		decodeContext->codecContext,
		decodeContext->codec,
		&decodeContext->optionsDictionary
	);

	if(openCodec < 0)
		return -1; // Codec unavailable?
	
	decodeContext->frame = avcodec_alloc_frame();

	changeColorSpace(PIX_FMT_RGB24, decodeContext);

	return 1;
}

int changeColorSpace(enum AVPixelFormat newColorSpace, struct DecodeContext *decodeContext) {
	int numBytes = avpicture_get_size(newColorSpace, decodeContext->width, decodeContext->height);
	uint8_t *buffer = NULL;
	buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	
	decodeContext->activeFrame = avcodec_alloc_frame();

	avpicture_fill((AVPicture *)decodeContext->activeFrame, buffer, newColorSpace, decodeContext->width, decodeContext->height);

	decodeContext->activeColorSpace = newColorSpace;

	return 1;
}

int nextFrame(struct DecodeContext *decodeContext) {
	while(av_read_frame(decodeContext->formatContext, &decodeContext->packet) >= 0) {
		if(decodeContext->packet.stream_index == decodeContext->videoStream) {
			avcodec_decode_video2(
				decodeContext->codecContext,
				decodeContext->frame,
				&decodeContext->frameFinished,
				&decodeContext->packet
			);

			if(decodeContext->frameFinished) {
				resizeFrame(decodeContext->width, decodeContext->height, decodeContext->activeColorSpace, decodeContext);
				break;
			}
		}
	}
	return 1;
}

int closeDecoder(struct DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	return 1;
}
