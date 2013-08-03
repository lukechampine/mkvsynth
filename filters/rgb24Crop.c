#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/frameControl.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct rgb24CropParams {
	unsigned long long left;
	unsigned long long top;
	unsigned long long right;
	unsigned long long bottom;
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *rgb24Crop(void *filterParams) {
	struct rgb24CropParams *params = (struct rgb24CropParams*)filterParams;

	MkvsynthFrame *workingFrame = getFrame(params->input);

	while(workingFrame->payload != NULL) {
		uint8_t *payload = malloc(params->output->metaData->bytes);

		int i, j;
		for(i = 0; i < params->output->metaData->height; i++) {
			int sourceOffset = (i + params->top) * 3 * params->input->metaData->width + params->left * 3;
			int offsetSize = 3 * params->output->metaData->width;
			int destOffset = i * offsetSize;
			memcpy(payload+destOffset, workingFrame->payload+sourceOffset, offsetSize);
		}
 
		clearFrame(workingFrame, 1);
		putFrame(params->output, payload);
		workingFrame = getFrame(params->input);
	}

	putFrame(params->output, NULL);

	free(params);
}

ASTnode *rgb24Crop_AST(ASTnode *p, ASTnode *args) {
	struct rgb24CropParams *params = malloc(sizeof(struct rgb24CropParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs("rgb24Crop", args, 5, typeClip, typeVal, typeVal, typeVal, typeVal);
	MkvsynthOutput *input = MANDCLIP();
	params->left = (unsigned long long)MANDVAL();
	params->top = (unsigned long long)MANDVAL();
	params->right = (unsigned long long)MANDVAL();
	params->bottom = (unsigned long long)MANDVAL();

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	////////////////////
	// Error Checking //
	////////////////////
	if((params->left + params->right) > input->metaData->width) {
		printf("You cannot crop that many columns! Insufficient video width!\n");
	}
   
	if((params->top + params->bottom) > input->metaData->height) {
		printf("You cannot crop that many rows! Insufficient video height!\n");
	}

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = input->metaData->width - params->left - params->right;
	params->output->metaData->height = input->metaData->height - params->top - params->bottom;
	params->output->metaData->channels = input->metaData->channels;
	params->output->metaData->depth = input->metaData->depth;
	params->output->metaData->bytes = 3 * params->output->metaData->width * params->output->metaData->height;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	mkvsynthQueue((void *)params, rgb24Crop);
	RETURNCLIP(params->output);
}
