#ifndef _datatypes_h_
#define _datatypes_h_
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <x264.h>

// Everything needed to decode/track a video stream
typedef struct {
	char *filename;
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVFrame *activeFrame;
	AVPacket packet;
	AVDictionary *optionsDictionary;

	int activeColorSpace;
	int videoStream;
	int frameFinished;
} DecodeContext;

// All information needed to encode a stream
typedef struct {
	x264_param_t param;
	x264_t *encoder;
	x264_picture_t picIn;
	x264_picture_t picOut;
	x264_nal_t *nals;

	int frameSize;
	int i_nals;
} EncodeContext;
#endif
