#include "../../jarvis/jarvis.h"
#include <string.h>
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
		uint8_t *payload = malloc(getBytes(params->output->metaData));

		int i;
		for(i = 0; i < params->output->metaData->height; i++) {
			int sourceOffset = (double)((double)params->left / (double)params->input->metaData->width) * (double)getLinesize(params->input->metaData);
			sourceOffset += (i + params->top) * getLinesize(params->input->metaData);
			int offsetSize = getLinesize(params->output->metaData);
			int destOffset = i * offsetSize;
			memcpy(payload+destOffset, workingFrame->payload+sourceOffset, offsetSize);
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

ASTnode *crop_AST(ASTnode *p, argList *a) {
	struct CropParams *params = malloc(sizeof(struct CropParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs(a, 5, typeClip, typeNum, typeNum, typeNum, typeNum);
	MkvsynthOutput *input = MANDCLIP(0);
	params->left = (unsigned long long)MANDNUM(1);
	params->top = (unsigned long long)MANDNUM(2);
	params->right = (unsigned long long)MANDNUM(3);
	params->bottom = (unsigned long long)MANDNUM(4);

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = input->metaData->width - params->left - params->right;
	params->output->metaData->height = input->metaData->height - params->top - params->bottom;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	////////////////////
	// Error Checking //
	////////////////////
	if(isMetaDataValid(params->input->metaData) != 1)
		MkvsynthError("invalid input!");

	if(isMetaDataValid(params->output->metaData) != 1)
		MkvsynthError("invalid ouput resolution!");

	MkvsynthMetaData temp = *params->output->metaData;
	temp.width = params->left;
	if(isMetaDataValid(&temp) != 1)
		MkvsynthError("invalid crop value (left)");

	temp.width = params->right;
	if(isMetaDataValid(&temp) != 1)
		MkvsynthError("invalid crop value (right)");

	if((params->left + params->right) > input->metaData->width)
		MkvsynthError("cannot crop that many columns! Insufficient video width!");
   
	if((params->top + params->bottom) > input->metaData->height)
		MkvsynthError("cannot crop that many rows! Insufficient video height!");

	mkvsynthQueue((void *)params, crop);
	RETURNCLIP(params->output);
}
