#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct gradientVideoGenerateParams {
	int frames;
	MkvsynthOutput *output;
};

void *gradientVideoGenerate(void *filterParams) {
	struct gradientVideoGenerateParams *params = (struct gradientVideoGenerateParams*)filterParams;
	printf("frames:width:height - %i:%i:%i\n", params->frames, params->output->metaData->width, params->output->metaData->height);
}

ASTnode *gradientVideoGenerate_AST(ASTnode *p, ASTnode *args) {
	checkArgs("gradientVideoGenerate", args, 0);
	double numFrames = OPTVAL("frames", 1000);
	double width = OPTVAL("width", 200);
	double height = OPTVAL("height", 200);

	printf("Intent to build %f frames at resolution %fx%f\n", numFrames, width, height);

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

	////////////////////////
	// Pthread Parameters //
	////////////////////////
	struct gradientVideoGenerateParams *params = malloc(sizeof(struct gradientVideoGenerateParams));
	params->frames = numFrames;
	params->output = output;

	mkvsynthQueue((void *)params, gradientVideoGenerate);

	RETURNCLIP(output);
}
