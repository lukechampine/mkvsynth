#include "../../jarvis/jarvis.h"
#include <string.h>
#include <stdio.h>

struct ColorspacingTestsParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *colorspacingTests(void *filterParams) {
	struct ColorspacingTestsParams *params = (struct ColorspacingTestsParams *)filterParams;

	MkvsynthFrame *workingFrame = getFrame(params->input);

	while(workingFrame->payload != NULL) {
		int i, j;
		for(i = 0; i < params->input->metaData->width; i++) {
			for(j = 0; j < params->input->metaData->height; j++) {
				MkvsynthPixel oldPixel = getPixel(workingFrame->payload, params->input->metaData, i, j);
				MkvsynthPixel newPixel = {{{0}}};
				newPixel.rgb48.r = getRed(&oldPixel, params->input->metaData);
				newPixel.rgb48.g = getGreen(&oldPixel, params->input->metaData);
				newPixel.rgb48.b = getBlue(&oldPixel, params->input->metaData);
				putPixel(&newPixel, workingFrame->payload, params->output->metaData, i, j);
			}
		}
		
		putFrame(params->output, workingFrame->payload);
		clearFrame(workingFrame); //Memeory Management
		workingFrame = getFrame(params->input);
	}

	putFrame(params->output, NULL);
	clearFrame(workingFrame);
	free(params);
	return NULL;
}

ASTnode *colorspacingTests_AST(ASTnode *p, argList *a) {
	struct ColorspacingTestsParams *params = malloc(sizeof(struct ColorspacingTestsParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs(a, 1, typeClip);
	MkvsynthOutput *input = MANDCLIP(0);

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	MkvsynthMessage("COLORSPACE: %i", input->metaData->colorspace);

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = input->metaData->width;
	params->output->metaData->height = input->metaData->height;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	mkvsynthQueue((void *)params, colorspacingTests);
	RETURNCLIP(params->output);
}
