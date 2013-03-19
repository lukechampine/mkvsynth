#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

int resizeFrame(int width, int height, AVCodecContext *codecContext, AVFrame *frame, x264_picture_t *picIn)
{
	struct SwsContext *resizeContext = NULL;

	resizeContext =
		sws_getContext (
			codecContext->width,
			codecContext->height,
			codecContext->pix_fmt,
			width,
			height,
			PIX_FMT_YUV420P,
			SWS_SPLINE,
			NULL,
			NULL,
			NULL
		);

	sws_scale(
		resizeContext,
			(uint8_t const * const *)frame->data,
			frame->linesize,
			0,
			codecContext->height,
			picIn->img.plane,
			picIn->img.i_stride
		);

	return 1;
}
