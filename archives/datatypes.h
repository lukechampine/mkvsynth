#ifndef _datatypes_h_
#define _datatypes_h_
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <x264.h>

// Everything needed to decode/track a video stream
typedef struct {
	// filename is needed for the function gotoFrame
	char *filename;

	// directly used in decode()
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
	AVPacket packet;
	AVDictionary *optionsDictionary;
	int videoStream;
	int frameFinished;

} DecodeContext;

// All information needed to encode a stream
typedef struct {
	// Directly encodes frames
	x264_t *encoder;

	// Not sure what these do, but you need them
	x264_picture_t picIn;
	x264_picture_t picOut;

	// Used to retrieve raw data
	x264_nal_t *nals;
	int frameSize;
	int i_nals;

	// Used to resize incoming frames
	int width;
	int height;
} EncodeContext;
#endif
