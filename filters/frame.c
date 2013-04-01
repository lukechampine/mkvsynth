#ifndef _frame_c_
#define _frame_c_
#include "datatypes.h"

int initializeFrame(int colorspace, int width, int height, AVFrame *frame) {
	frame = avcodec_alloc_frame();
	if(frame == NULL)
		return -1;

	int numBytes = avpicture_get_size(colorspace, width, height);
	uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)frame, buffer, colorspace, width, height);

	return 1;
}
#endif
