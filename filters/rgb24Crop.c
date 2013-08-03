#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/frameControl.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct rgb24CropParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
}

void rgb24Crop_AST(ASTParams *filterParams) {
	///////////////////////
	// Parameter Parsing //
	///////////////////////
	int left = checkInt(filterParams, "left");
	int top = checkInt(filterParams, "top");
	int right = checkInt(filterParams, "right");
	int bottom = checkInt(filterParams, "bottom");
	MkvsynthInput *input = checkInput(filterParams, "input1");
	MkvsynthOutput *output = checkOutput(filterParams, "output1");

	////////////////////
	// Error Checking //
	////////////////////
	if((left + right) > input->metaData->width)
		filterError("You cannot crop that many columns! Insufficient video width!");
   
	if((top + bottom) > input->metaData->height)
		filterError("You cannot crop that many rows! Insufficient video height!");
   
	if(input->metaData->colorspace != PACKED_RGB && input->metaData->colorspace != STRICT_RGB)
		filterError("rgbCrop does not recognize the colorspace of the input video!");

	///////////////
	// Meta Data //
	///////////////
	output->metaData->colorspace = input->metaData->colorspace;
	output->metaData->width = input->metaData->width - left - right;
	output->metaData->height = input->metaData->height - top - bottom;
	output->metaData->channels = input->metaData->channels;
	output->metaData->depth = input->metaData->channels;
	output->metaData->bytes = 3*output->metaData->width*output->metaData->height;
	signalStartupCompletion(output); 

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(input);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(output->metaData->bytes);

		int i, j;
		for(i = 0; i < output->metaData->height; i++) {
			int sourceOffset = i*3*input->width+left*3;
			int destOffset = 3*i*width;
			int offsetSize = 3*(width-left-right);
			memcpy(payload[3*i*width], workingFrame->payload[startOffset], offsetSize);
		}
 
		clearFrame(workingFrame, NULL);
		putFrame(output, payload);
		workingFrame = getFrame(input);
	}

	putFrame(NULL);
	mkvsynthTerminate();
}
struct gradientVideoGenerateParams {
	int frames;
	MkvsynthOutput *output;
};

void *gradientVideoGenerate(void *filterParams) {
	struct gradientVideoGenerateParams *params = (struct gradientVideoGenerateParams*)filterParams;

	printf("frames:width:height - %i:%i:%i\n",
	       params->frames,
				 params->output->metaData->width,
				 params->output->metaData->height);

	int i, j;
	for(i = 0; i < params->frames; i++) {
		uint8_t *payload = malloc(params->output->metaData->bytes);

		for(j = 0; j < params->output->metaData->bytes; j++)
			payload[j] = i % 256;

		putFrame(params->output, payload);
	}

	putFrame(params->output, NULL);
	free(params);
}

ASTnode *gradientVideoGenerate_AST(ASTnode *p, ASTnode *args) {
	checkArgs("gradientVideoGenerate", args, 0);
	long long numFrames = (long long)OPTVAL("frames", 1000);
	long long width = (long long)OPTVAL("width", 200);
	long long height = (long long)OPTVAL("height", 200);

	////////////////////
	// Error Checking //
	////////////////////
	if(numFrames < 0 || width < 0 || height < 0) {
		printf("Error: width, height, and frames must be numbers greater than 0\n");
		exit(0);
	}

	printf("Intent to build %lld frames at resolution %lldx%lld\n", numFrames, width, height);

	MkvsynthOutput *output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	output->metaData->colorspace = 0;
	output->metaData->width = (int)width;
	output->metaData->height = (int)height;
	output->metaData->channels = 3;
	output->metaData->depth = 8;
	output->metaData->bytes = width*height*3;
	output->metaData->fpsNumerator = 60;
	output->metaData->fpsDenominator = 1;

	////////////////////////
	// Pthread Parameters //
	////////////////////////
	struct gradientVideoGenerateParams *params = malloc(sizeof(struct gradientVideoGenerateParams));
	params->frames = numFrames;
	params->output = output;

	mkvsynthQueue((void *)params, gradientVideoGenerate);

	RETURNCLIP(output);
}
