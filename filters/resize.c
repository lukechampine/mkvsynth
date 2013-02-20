#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

int resizeFrame(int height, int width, AVCodecContext *codecContext, AVFrame *frame) {
	struct SwsContext resizeContext = sws_getContext (codecContext->width,
																										codecContext->height,
																										codecContext->pix_fmt,
																										width,
																										height,
																										codecContext->pix_fmt,
																										SWS_SPLINE,
																										NULL,
																										NULL,
																										NULL );
		
	sws_scale(resizeContext
						(uint8_t const * const *)frame->data,
						frame->linesize,
						0,
						codecContext->height,
						frame->data,
						frame->linesize);
}
