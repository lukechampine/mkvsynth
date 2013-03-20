#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

int resizeFrame(int width, int height, enum AVPixelFormat outputFormat, struct DecodeContext *decodeContext) {
	struct SwsContext *resizeContext = NULL;
	resizeContext = sws_getContext (
		decodeContext->codecContext->width,
		decodeContext->codecContext->height,
		decodeContext->codecContext->pix_fmt,
		width,
		height,
		outputFormat,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL
	);

	sws_scale (
		resizeContext,
		(uint8_t const * const *)decodeContext->frame->data,
		decodeContext->frame->linesize,
		0,
		decodeContext->codecContext->height,
		decodeContext->activeFrame->data,
		decodeContext->activeFrame->linesize
	);

	return 1;	
}
