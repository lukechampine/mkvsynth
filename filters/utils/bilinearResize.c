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

		double widthRatio = (double)params->input->metaData->width / (double)params->output->metaData->width;
		double heightRatio = (double)params->input->metaData->height / (double)params->output->metaData->height;

		int i, j;
		for(i = 0; i < params->output->metaData->width; i++) {
			for(j = 0; j < params->output->metaData->height; j++) {
				// Find top left neighbor corner
				double x = (double)j * widthRatio;
				double y = (double)i * heightRatio;

				// get the integral versions
				int xLeft   = floor(x);
				int xRight  = floor(x+1);
				int yTop    = floor(y);
				int yBottom = floor(y+1);

				MkvsynthPixel topLeft     = getPixel(workingFrame->payload, params->input->metaData, xLeft,  yTop);
				MkvsynthPixel topRight    = getPixel(workingFrame->payload, params->input->metaData, xRight, yTop);
				MkvsynthPixel bottomLeft  = getPixel(workingFrame->payload, params->input->metaData, xLeft,  yBottom);
				MkvsynthPixel bottomRight = getPixel(workingFrame->payload, params->input->metaData, xRight, yBottom);
				
				// merge them together according to weight.
				double topLeftWeight     = (xRight - x) * (yBottom - y);
				double topRightWeight    = (x - xLeft)  * (yBottom - y);
				double bottomLeftWeight  = (xRight - x) * (y - yTop);
				double bottomRightWeight = (x - xLeft)  * (y - yTop);

				MkvsynthPixel newPixel = {0};
				overlay(&newPixel, &topLeft,     params->output->metaData->colorspace, topLeftWeight);
				overlay(&newPixel, &topRight,    params->output->metaData->colorspace, topRightWeight);
				overlay(&newPixel, &bottomLeft,  params->output->metaData->colorspace, bottomLeftWeight);
				overlay(&newPixel, &bottomRight, params->output->metaData->colorspace, bottomRightWeight);

				putPixel(&newPixel, payload, params->input->metaData, i, j);
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
