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

		double xRatio = ((double)params->input->metaData->width - 1) / ((double)params->output->metaData->width - 1);
		double yRatio = ((double)params->input->metaData->height - 1) / ((double)params->output->metaData->height - 1);
		
		int i, j;
		for(i = 0; i < params->output->metaData->height; i++) {
			for(j = 0; j < params->output->metaData->width; j++) {
				double x = (double)j * xRatio;
				double y = (double)i * yRatio;

				int xLeft   = floor(x);
				int xRight  = ceil((float)x);
				int yTop    = floor(y);
				int yBottom = ceil((float)y);

				double rightDiff = xRight - x;
				double leftDiff = x - xLeft;
				double bottomDiff = yBottom - y;
				double topDiff = y - yTop;

				if(leftDiff == 0)
					leftDiff = 1;

				if(topDiff == 0)
					topDiff = 1;

				double topLeftWeight     = rightDiff * bottomDiff;
				double topRightWeight    = leftDiff  * bottomDiff;
				double bottomLeftWeight  = rightDiff * topDiff;
				double bottomRightWeight = leftDiff  * topDiff;

				MkvsynthPixel topLeft     = getPixel(workingFrame->payload, params->input->metaData, xLeft,  yTop);
				MkvsynthPixel topRight    = getPixel(workingFrame->payload, params->input->metaData, xRight, yTop);
				MkvsynthPixel bottomLeft  = getPixel(workingFrame->payload, params->input->metaData, xLeft,  yBottom);
				MkvsynthPixel bottomRight = getPixel(workingFrame->payload, params->input->metaData, xRight, yBottom);

				MkvsynthPixel newPixel = {{{0}}};
				addPixel(&newPixel, &topLeft,     params->output->metaData->colorspace, topLeftWeight);
				addPixel(&newPixel, &topRight,    params->output->metaData->colorspace, topRightWeight);
				addPixel(&newPixel, &bottomLeft,  params->output->metaData->colorspace, bottomLeftWeight);
				addPixel(&newPixel, &bottomRight, params->output->metaData->colorspace, bottomRightWeight);

				putPixel(&newPixel, payload, params->output->metaData, j, i);
			}
		}

		putFrame(params->output, payload);
		clearReadOnlyFrame(workingFrame);
		workingFrame = getReadOnlyFrame(params->input);
	}

	putFrame(params->output, NULL);
	clearReadOnlyFrame(workingFrame);
	free(params);
	return NULL;
}

ASTnode *bilinearResize_AST(ASTnode *p, argList *a) {
	struct BilinearResizeParams *params = malloc(sizeof(struct BilinearResizeParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs(a, 3, typeClip, typeNum, typeNum);
	MkvsynthOutput *input = MANDCLIP(0);

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = (unsigned long long)MANDNUM(1);
	params->output->metaData->height = (unsigned long long)MANDNUM(2);
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	////////////////////
	// Error Checking //
	////////////////////
	if(isMetaDataValid(params->input->metaData) != 1)
		MkvsynthError("invalid input!");

	if(isMetaDataValid(params->output->metaData) != 1)
		MkvsynthError("invalid ouput!");

	mkvsynthQueue((void *)params, bilinearResize);
	RETURNCLIP(params->output);
}

#endif
