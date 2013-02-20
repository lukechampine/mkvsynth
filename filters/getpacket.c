#include "libavformat/avformat.h"
#define INBUF_SIZE 4096

int FindStream (AVFormatContext *pFormatCtx) {
    int videoStream = -1; int i;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    return videoStream;
}

static int GetPacketFromFile (const char *filename, int timestamp, AVPacket avpacket) {
    // Initialize packet 
    av_init_packet(&avpacket);

    // Set end of buffer to 0 (to ensure no overreading happens for damaged mpeg streams)
    uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

    // Find decoder
    av_register_all();
    AVFormatContext *pFormatCtx = NULL;
    avformat_open_input(&pFormatCtx, filename, NULL, NULL);

    // Find the first video stream
    int videoStream = FindStream(pFormatCtx);

    // Seek to the desired frame
    av_seek_frame(pFormatCtx, videoStream, timestamp, 0);

    // Read the desired frame
    if(av_read_frame(pFormatCtx, &avpacket) < 0) {
        fprintf(stderr, "Error reading frame");
        exit(1);
    }
    av_free_packet(&avpacket);
    return 0;
}

static int filterStream (const char *filename) {
    AVFormatContext *formatContext = NULL;
    AVCodecContext *codecContext = NULL;
    AVCodec *codec = NULL;
    AVFrame *frame = NULL;
		AVDictionary *optionsDict = NULL;
		AVPacket packet;
		
    int videoStream = -1;
		int frameFinished;

    //register codecs and verify valid input
    av_register_all();
    if(avformat_open_input(&formatContext, *filename, NULL, NULL) != 0)
        return -1;
    if(avformat_find_stream_info(formatContext, NULL) <0)
        return -1;
		
		//decode the first videostream
		for(i=0; i<formatContext->nb_streams; i++) {
				if(formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
						videoStreams = i;
						break;
				}
		}

		if(videoStream == -1)
				return -1;

		codecContext = formatContext->streams[videoStream]->codec;
		codec = av_codec_find_decoder(codecContext->codec_id);
		if(codec == NULL)
				return -1; //unsupported codec

		if(avcodec_open2(codecContext, codec, &optionsDict) < 0)
				return -1;

		frame = avcodec_alloc_frame();

		while(av_read_frame(formatContext, &packet) >= 0) {
				if(packet.stream_index == videoStream) {
						avcodec_decode_video2(codecContext, frame, &frameFinished, &packet);
						
				//magic happens here!
				if(frameFinished) {
						resizeFrame(250, 250, codecContext, frame);
				}
		}
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Please provide a movie file and a packet number\n");
        return -1;
    }

    // Get a packet
    AVPacket test_packet;
    //GetPacketFromFile(argv[1], (int) *argv[2], test_packet);
    return 0;

		filterStream(filename);
}
