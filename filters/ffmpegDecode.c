#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

void ffmpegDecode(ASTParams *filterParams, MkvsynthPutParams *putParams) {
	
	char* filename = checkArgs(filterParams, "file", string);
	
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
	
	AVFrame *frame = avcodec_alloc_frame();
	int flag = 1
	
	while(flag != 0) {
		int flag = 0;
		while(av_read_frame(formatContext, &packet) >= 0) {
			flag = 1;
			if(packet.stream_index == videoStream) {
				avcodec_decode_video2(
					codecContext,
					frame,
					&frameFinished,
					&packet);

				if(frameFinished)
					break;
			}
		}

		if(flag != 0)
			putFrame(frame->data);
	}

	// Indicate that you are done
	putFrame(NULL);
	
	avcodec_close(codecContext);
	avformat_close_input(&formatContext);
}
