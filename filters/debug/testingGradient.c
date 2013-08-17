#include "../../jarvis/jarvis.h"
#include <stdio.h>

struct TestingGradientParams {
	int frames;
	MkvsynthOutput *output;
};

void *testingGradient(void *filterParams) {
	struct TestingGradientParams *params = (struct TestingGradientParams*)filterParams;

	int i, j;
	for(i = 0; i < params->frames; i++) {
		uint8_t *payload = malloc(getBytes(params->output->metaData));
		uint16_t *shortPayload = (uint16_t *)payload;

		int bytes = getBytes(params->output->metaData);
		for(j = 0; j < bytes / 2; j++)
			shortPayload[j] = (j + i) % 65536;

		putFrame(params->output, payload);
	}

	putFrame(params->output, NULL);
	free(params);
}

ASTnode *testingGradient_AST(ASTnode *p, ASTnode *args) {
	checkArgs("testingGradient", args, 0);
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
	output->metaData->width = width;
	output->metaData->height = height;
	output->metaData->fpsNumerator = 60;
	output->metaData->fpsDenominator = 1;

	////////////////////////
	// Pthread Parameters //
	////////////////////////
	struct TestingGradientParams *params = malloc(sizeof(struct TestingGradientParams));
	params->frames = numFrames;
	params->output = output;

	mkvsynthQueue((void *)params, testingGradient);

	RETURNCLIP(output);
}
