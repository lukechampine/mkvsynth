#ifndef bilinearResize_c_
#define bilinearResize_c_

#include "../../jarvis/jarvis.h"
#include <math.h>
#include <stdio.h>

struct BilinearResizeParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *bilinearResize(void *filterParams) {
	struct BilinearResizeParams *params = (struct BilinearResizeParams *)filterParams;

	MkvsynthFrame *workingFrame = getReadOnlyFrame(params->input);

	while(workingFrame->payload != NULL) {
		uint8_t *payload = malloc(getBytes(params->output->metaData));

		// for every pixel in the new grid
		// map that pixel to a point on the previous map
		// get the 4 pixels nearest to the point
		// use weight to average them together
		//
		// to get the point... you can get the width percentage by dividing current width by new width
		// and do the same for height
		// then use the 2 numbers to find 4 pixels and use multiplication to find the weight for each
		// then avg the values to get the new pixel value
		double widthRatio = params->input->metaData->width / params->output->metaData->width;
		double heightRatio = params->input->metaData->height / params->output->metaData->height;

		int i, j;
		for(i = 0; i < params->output->metaData->width; i++) {
			for(j = 0; j < params->output->metaData->height; j++) {
				// do stuff
				//
				// Every row will match to an equivalent row, so for each video you can go 1 row at a time
				// to cover all colorspaces, use getPixel and putPixel. Memory management might be tough here

				// Find top left neighbor corner
				double x = j * widthRatio;
				double y = i * heightRatio;

				// get the integral versions
				int xLeft   = floor(x);
				int xRight  = ceil(x);
				int yTop    = floor(y);
				int yBottom = ceil(y);
				printf("xl-xr-yt-yb: %i-%i-%i-%i\n", xLeft, xRight, yTop, yBottom);

				MkvsynthPixel topLeft     = getPixel(workingFrame, xLeft,  yTop);
				MkvsynthPixel topRight    = getPixel(workingFrame, xRight, yTop);
				MkvsynthPixel bottomLeft  = getPixel(workingFrame, xLeft,  yBottom);
				MkvsynthPixel bottomRight = getPixel(workingFrame, xRight, yBottom);
				
				// merge them together according to weight.
				double topLeftWeight     = (x - xLeft)  * (y - yTop);
				double topRightWeight    = (xRight - x) * (y - yTop);
				double bottomLeftWeight  = (x - xLeft)  * (yBottom - y);
				double bottomRightWeight = (xRight - x) * (yBottom - y);
				printf("tlw-trw-blw-brw: %g-%g-%g-%g\n", topLeftWeight, topRightWeight, bottomLeftWeight, bottomRightWeight);
				printf("total: %g\n", topLeftWeight + topRightWeight + bottomLeftWeight + bottomRightWeight);

				// Need some type of plan for resizing based on colorspace...
				// Maybe just a blend/overlayer or something
				// the blend funtion takes 2 pixels of the same colorspace and
				// adds the one to the other accoreding to the double value
				MkvsynthPixel newPixel = {0};
				newPixel.colorspace = params->output->metaData->colorspace;
				overlay(&newPixel, &topLeft,     topLeftWeight);
				overlay(&newPixel, &topRight,    topRightWeight);
				overlay(&newPixel, &bottomLeft,  bottomLeftWeight);
				overlay(&newPixel, &bottomRight, bottomRightWeight);

				putPixel(&newPixel, payload, i, j);
			}
		}

		clearReadOnlyFrame(workingFrame);
		putFrame(params->output, payload);
		workingFrame = getReadOnlyFrame(params->input);
	}

	clearReadOnlyFrame(workingFrame);
	putFrame(params->output, NULL);
	free(params);
}

ASTnode *bilinearResize_AST(ASTnode *p, ASTnode *args) {
	struct BilinearResizeParams *params = malloc(sizeof(struct BilinearResizeParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs("bilinearResize", args, 3, typeClip, typeVal, typeVal);
	MkvsynthOutput *input = MANDCLIP();

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = (unsigned long long)MANDVAL();
	params->output->metaData->height = (unsigned long long)MANDVAL();
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	////////////////////
	// Error Checking //
	////////////////////
	if(isMetaDataValid(params->input->metaData) != 1) {
		printf("Crop Error: invalid input!\n");
		exit(0);
	}

	if(isMetaDataValid(params->output->metaData) != 1) {
		printf("Crop Error: invalid ouput!\n");
		exit(0);
	}

	mkvsynthQueue((void *)params, bilinearResize);
	RETURNCLIP(params->output);
}

#endif
