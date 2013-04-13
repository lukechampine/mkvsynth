#ifndef _resize_h_
#define _resize_h_
#include "datatypes.h"
#include <libswscale/swscale.h>

AVFrame * resizeFrame(int colorspace, int width, int height, AVFrame *frame) {
	AVFrame *newFrame = NULL;
	newFrame = avcodec_alloc_frame();
	if(newFrame == NULL) {
		printf("Mkvsynth Resize: Could not allocate frame!\n");
		return NULL;
	}

	int numBytes = avpicture_get_size(colorspace, width, height);
	uint8_t *buffer = NULL;
	buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)newFrame, buffer, colorspace, width, height);

	newFrame->width = width;
	newFrame->height = height;
	newFrame->format = colorspace;

	struct SwsContext *resizeContext = NULL;
	resizeContext = sws_getContext (
		frame->width,
		frame->height,
		frame->format,
		newFrame->width,
		newFrame->height,
		newFrame->format,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL
	);

	sws_scale (
		resizeContext,
		(uint8_t const * const *)frame->data,
		frame->linesize,
		0,
		frame->height,
		newFrame->data,
		newFrame->linesize
	);

	return newFrame;
}

#endif
