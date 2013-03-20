#ifndef _resize_h_
#define _resize_h_
#include "datatypes.h"
#include <libswscale/swscale.h>

int resizeFrame(int width, int height, int colorspace, DecodeContext *decodeContext) {
	struct SwsContext *resizeContext = NULL;
	resizeContext = sws_getContext (
		decodeContext->codecContext->width,
		decodeContext->codecContext->height,
		decodeContext->codecContext->pix_fmt,
		width,
		height,
		colorspace,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL
	);

	decodeContext->codecContext->width = width;
	decodeContext->codecContext->height = height;
	decodeContext->codecContext->pix_fmt = colorspace;

	AVFrame *tmp = NULL;
	tmp = avcodec_alloc_frame();
	int numBytes = avpicture_get_size(colorspace, width, height);
	uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)tmp, buffer, colorspace, width, height);

	sws_scale (
		resizeContext,
		(uint8_t const * const *)decodeContext->frame->data,
		decodeContext->frame->linesize,
		0,
		decodeContext->codecContext->height,
		tmp->data,
		tmp->linesize
	);

	av_free(decodeContext->frame);
	decodeContext->frame = tmp;

	return 1;	
}
#endif
