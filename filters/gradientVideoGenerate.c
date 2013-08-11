#include "../jarvis/jarvis.h"
#include <stdio.h>

struct gradientVideoGenerateParams {
	int frames;
	MkvsynthOutput *output;
};

void *gradientVideoGenerate(void *filterParams) {
	struct gradientVideoGenerateParams *params = (struct gradientVideoGenerateParams*)filterParams;

	int i, j;
	for(i = 0; i < params->frames; i++) {
		uint8_t *payload = malloc(getBytes(params->output->metaData));
		uint16_t *shortPayload = (uint16_t *)payload;

		int bytes = getBytes(params->output->metaData);
		for(j = 0; j < bytes / 2; j++)
			shortPayload[j] = (i % 256) << 8;

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

	MkvsynthOutput *output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	output->metaData->colorspace = MKVS_RGB48;
	output->metaData->width = (int)width;
	output->metaData->height = (int)height;
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
