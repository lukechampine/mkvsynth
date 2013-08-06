#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/frameControl.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct CropParams {
	unsigned long long left;
	unsigned long long top;
	unsigned long long right;
	unsigned long long bottom;
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *crop(void *filterParams) {
	struct CropParams *params = (struct CropParams *)filterParams;

	MkvsynthFrame *workingFrame = getReadOnlyFrame(params->input);

	while(workingFrame->payload != NULL) {
		uint8_t *payload = malloc(params->output->metaData->bytes);

		int i, j;
		for(i = 0; i < params->output->metaData->height; i++) {
			int sourceOffset = (i + params->top) * 6 * params->input->metaData->width + params->left * 6;
			int offsetSize = 6 * params->output->metaData->width;
			int destOffset = i * offsetSize;
			memcpy(payload+destOffset, workingFrame->payload+sourceOffset, offsetSize);
		}
 
		clearReadOnlyFrame(workingFrame);
		putFrame(params->output, payload);
		workingFrame = getReadOnlyFrame(params->input);
	}

	putFrame(params->output, NULL);
	clearReadOnlyFrame(workingFrame);
	free(params);
}

ASTnode *crop_AST(ASTnode *p, ASTnode *args) {
	struct CropParams *params = malloc(sizeof(struct CropParams));

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
	if(input->metaData->colorspace != MKVS_RGB48) {
		printf("Crop only works with RGB48 colorspace!\n");
		exit(0);
	}

	if((params->left + params->right) > input->metaData->width) {
		printf("You cannot crop that many columns! Insufficient video width!\n");
		exit(0);
	}
   
	if((params->top + params->bottom) > input->metaData->height) {
		printf("You cannot crop that many rows! Insufficient video height!\n");
		exit(0);
	}

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = input->metaData->width - params->left - params->right;
	params->output->metaData->height = input->metaData->height - params->top - params->bottom;
	params->output->metaData->bytes = 6 * params->output->metaData->width * params->output->metaData->height;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	mkvsynthQueue((void *)params, crop);
	RETURNCLIP(params->output);
}
