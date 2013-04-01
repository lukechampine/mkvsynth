#ifndef _resize_h_
#define _resize_h_
#include "datatypes.h"
#include "frame.c"
#include <libswscale/swscale.h>

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;
  
  // Open file
	sprintf(szFilename, "zframe%d.ppm", iFrame);
	pFile=fopen(szFilename, "wb");
	if(pFile==NULL)
	  return;
	  
	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);
	
 // Write pixel data
 int a = 0;
 for(y=0; y<height; y++)
 	a = fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
 if(a < 0)
 	printf("fwrite error!\n");
   
	// Close file
	fclose(pFile);
}

int resizeFrame(int colorspace, int width, int height, AVFrame *frame) {
	AVFrame *newFrame = NULL;
	newFrame = avcodec_alloc_frame();
	if(newFrame == NULL)
		return -1;

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

	int linesWritten = sws_scale (
		resizeContext,
		(uint8_t const * const *)frame->data,
		frame->linesize,
		0,
		newFrame->height,
		newFrame->data,
		newFrame->linesize
	);

	printf("Lines Written: %i\n", linesWritten);

	SaveFrame(newFrame, newFrame->width, newFrame->height, 5);

	return 1;
}

#endif
