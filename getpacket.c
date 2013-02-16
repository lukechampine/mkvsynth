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

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Please provide a movie file and a packet number\n");
        return -1;
    }

    // Get a packet
    AVPacket test_packet;
    GetPacketFromFile(argv[1], (int) *argv[2], test_packet);
    return 0;
}
