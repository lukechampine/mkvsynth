#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdio.h>

struct DecodeContext {
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVPacket packet;
	AVDictionary *optionsDictionary;

	int videoStream;
	int frameFinished;
};

int initializeDecoder(struct DecodeContext *decodeContext, char *filename) {
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->optionsDictionary = NULL;

	decodeContext->videoStream = -1;
    
	int i=0;

  av_register_all();

  if(avformat_open_input(&decodeContext->formatContext, filename, NULL, NULL) != 0)
      return -1;
  if(avformat_find_stream_info(decodeContext->formatContext, NULL) < 0)
      return -1;

	for(i=0; i<decodeContext->formatContext->nb_streams; i++) {
			if(decodeContext->formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
					decodeContext->videoStream = i;
					break;
			}
	}

	if(decodeContext->videoStream == -1)
		return -1;

	decodeContext->codecContext = decodeContext->formatContext->streams[decodeContext->videoStream]->codec;
	decodeContext->codec = avcodec_find_decoder(decodeContext->codecContext->codec_id);
	if(decodeContext->codec == NULL)
		return -1; //unsupported codec
	
	if(avcodec_open2(decodeContext->codecContext, decodeContext->codec, &decodeContext->optionsDictionary) < 0)
		return -1;

	decodeContext->frame = avcodec_alloc_frame();

	return 1;
}

int closeDecoder(struct DecodeContext *decodeContext) {
	av_free(decodeContext->frame);
	avcodec_close(decodeContext->codecContext);
	avformat_close_input(&decodeContext->formatContext);
	return 1;
}
	

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a movie file\n");
    return -1;
  }

	struct DecodeContext decodeContext;

	if(initializeDecoder(&decodeContext, argv[1]) != 1) {
		printf("failed to initialize the decoder!\n");
		return -1;
	}

	while(av_read_frame(decodeContext.formatContext, &decodeContext.packet) >= 0) {
		if(decodeContext.packet.stream_index == decodeContext.videoStream) {
			avcodec_decode_video2(decodeContext.codecContext, decodeContext.frame, &decodeContext.frameFinished, &decodeContext.packet);
			if(decodeContext.frameFinished) {
				printf("Decoded a Frame!\n");
			}
		}
	}

	closeDecoder(&decodeContext);

	return 1;
}
