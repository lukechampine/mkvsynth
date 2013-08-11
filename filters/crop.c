#include "../jarvis/jarvis.h"
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

		int i, j;
		for(i = 0; i < params->output->metaData->height; i++) {
			int sourceOffset = (double)((double)params->left / (double)params->input->metaData->width) * (double)getLinesize(params->input->metaData);
			sourceOffset += (i + params->top) * getLinesize(params->input->metaData);
			int offsetSize = getLinesize(params->output->metaData);
			int destOffset = i * offsetSize;
			memcpy(payload+destOffset, workingFrame->payload+sourceOffset, offsetSize);
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
	struct CropParams *params = malloc(sizeof(struct CropParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs("crop", args, 5, typeClip, typeVal, typeVal, typeVal, typeVal);
	MkvsynthOutput *input = MANDCLIP();
	params->left = (unsigned long long)MANDVAL();
	params->top = (unsigned long long)MANDVAL();
	params->right = (unsigned long long)MANDVAL();
	params->bottom = (unsigned long long)MANDVAL();

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
	if(isMetaDataValid(params->input->metaData) != 1) {
		printf("Crop Error: invalid input!\n");
		exit(0);
	}

	if(isMetaDataValid(params->output->metaData) != 1) {
		printf("Crop Error: invalid ouput resolution!\n");
		exit(0);
	}

	MkvsynthMetaData temp = *params->output->metaData;
	temp.width = params->left;
	if(isMetaDataValid(&temp) != 1) {
		printf("Crop Error: invalid crop value (left)\n");
		exit(0);
	}

	temp.width = params->right;
	if(isMetaDataValid(&temp) != 1) {
		printf("Crop Error: invalid crop value (right)\n");
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

	mkvsynthQueue((void *)params, crop);
	RETURNCLIP(params->output);
}
