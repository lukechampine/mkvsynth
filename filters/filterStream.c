#include <libavformat/avformat.h>

#define INBUF_SIZE 4096

struct DecodeContext {
	AVFormatContext *formatContext;
  AVCodecContext *codecContext;
  AVCodec *codec;
  AVFrame *frame;
	AVDictionary *optionsDictionary;
	AVPacket *packet;

	int videoStream;
	int frameFinished;
};

int initializeDecoder(struct DecodeContext *decodeContext, char *filename) {
	decodeContext->formatContext = NULL;
	decodeContext->codecContext = NULL;
	decodeContext->codec = NULL;
	decodeContext->frame = NULL;
	decodeContext->optionsDictionary = NULL;
	decodeContext->packet = NULL;

	decodeContext->videoStream = -1;
    
	//register codecs and verify valid input
  av_register_all();
  if(avformat_open_input(&decodeContext->formatContext, *filename, NULL, NULL) != 0)
      return -1;
  if(avformat_find_stream_info(decodeContext->formatContext, NULL) < 0)
      return -1;
		
	//decode the first videostream
	for(i=0; i<decodeContext->formatContext->nb_streams; i++) {
			if(decodeContext->formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
					decodeContext->videoStream = -1;
					break;
			}
	}

	if(decodeContext->videoStream == -1)
		return -1;

	decodeContext->codecContext = decodeContext->formatContext->streams[videoStream]->codec;
	decodeContext->codec = av_codec_find_decoder(decodeContext->codecContext->codec_id);
	if(codec == NULL)
		return -1; //unsupported codec

	if(avcodec_open2(decodeContext->codecContext, decodeContext->codec, &decodeContext->optionsDictionary) < 0)
		return -1;

	decodeContext->frame = avcodec_alloc_frame();
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a movie file\n");
    return -1;
  }

	struct DecodeContext *decodeContext;
	initializeDecoder(&decodeContext, argv[1]);

	AVFrame *pFrameRGB = NULL;

	while(av_read_frame(decodeContext->formatContext, decodeContext->packet) >= 0) {
		if(decodeContext->packet->stream_index == decodeContext->videoStream) {
			av_codec_decode_video2(decodeContext->codecContext, decodeContext->frame, &decodeContext->frameFinished, decodeContext->packet);
			if(decodeContext->frameFinished) {
				printf("Decoded a Frame!\n");
			}
		}
	}
}
