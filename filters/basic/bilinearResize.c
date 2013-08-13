#include "../jarvis/jarvis.h"
#include <stdio.h>

struct BilinearResizeParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *crop(void *filterParams) {
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
		double widthRatio = params->input->width / params->output->width;
		double heightRatio = params->input->height / params->output->height;

		int i, j;
		for(i = 0; i < params->output->width; i++) {
			for(j = 0; j < params->output->height; j++) {
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

				MkvsynthPixel topLeft     = getPixel(workingFrame, xLeft,  yTop);
				MkvsynthPixel topRight    = getPixel(workingFrame, xRight, yTop);
				MkvsynthPixel bottomLeft  = getPixel(workingFrame, xLeft,  yBottom);
				MkvsynthPixel bottomRight = getPixel(workingFrame, xRight, yBottom);
				printf("debug should go here\n");
				
				// merge them together according to weight.
				double topLeftWeight     = (x - xLeft)  * (y - yTop);
				double topRightWeight    = (xRight - x) * (y - yTop);
				double bottomLeftWeight  = (x - xLeft)  * (yBottom - y);
				double bottomRightWeight = (xRight - x) * (yBottom - y);
				printf("debug should go here too\n");

				// Need some type of plan for resizing based on colorspace...
				// Maybe just a blend/overlayer or something
				// the blend funtion takes 2 pixels of the same colorspace and
				// adds the one to the other accoreding to the double value
				MkvsynthPixel newPixel = {0};
				newPixel->colorspace = output->colorspace;
				layer(&newPixel, &topLeft,     topLeftWeight);
				layer(&newPixel, &topRight,    topRightWeight);
				layer(&newPixel, &bottomLeft,  bottomLeftWeight);
				layer(&newPixel, &bottomRight, bottomRightWeight);

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

ASTnode *crop_AST(ASTnode *p, ASTnode *args) {
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
