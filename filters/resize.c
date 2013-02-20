#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

int resizeFrame(int height, int width, AVCodecContext *codecContext, AVFrame *frame)
{
	struct SwsContext resizeContext =     //creates struct with information about the input and output frames
		sws_getContext (codecContext->width,
							codecContext->height,
							codecContext->pix_fmt,
							width,
							height,
							codecContext->pix_fmt,
							SWS_SPLINE,
							NULL,
							NULL,
							NULL );
		
	return sws_scale(resizeContext                        //takes video stream and makes gratuitous use of magic to resize
						(uint8_t const * const *)frame->data,
						frame->linesize,
						0,
						codecContext->height,
						frame->data,
						frame->linesize);
}
