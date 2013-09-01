#include "../../jarvis/jarvis.h"
#include <stdio.h>

struct ConvertColorspaceParams {
	short colorspace;
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *convertColorspace(void *filterParams) {
	struct ConvertColorspaceParams *params = (struct ConvertColorspaceParams *)filterParams;

	MkvsynthFrame *workingFrame = getReadOnlyFrame(params->input);

	while(workingFrame->payload != NULL) {
		uint8_t *payload = malloc(getBytes(params->output->metaData));

		int i;
		for(i = 0; i < getBytes(params->output->metaData); i++) {
			payload[i] = workingFrame->payload[i*2 + 1];
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

ASTnode *convertColorspace_AST(ASTnode *p, ASTnode *args) {
	struct ConvertColorspaceParams *params = malloc(sizeof(struct ConvertColorspaceParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs("convertColorspace", args, 2, typeClip, typeStr);
	MkvsynthOutput *input = MANDCLIP();
	char *colorspaceStr = MANDSTR();

	if(strcmp(colorspaceStr, "rgb24") || strcmp(colorspaceStr, "MKVS_RGB24")) {
		params->colorspace = MKVS_RGB24;
	} else {
		printf("convertColorspace Error: Currently the only valid output colorspace is rgb24!\n");
		exit(0);
	}

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	if(params->input->metaData->colorspace != MKVS_RGB48) {
		printf("convertColorspace Error: Currently the only valid input colorspace is rgb48!\n");
		exit(0);
	}

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = params->colorspace;
	params->output->metaData->width = input->metaData->width;
	params->output->metaData->height = input->metaData->height;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	mkvsynthQueue((void *)params, convertColorspace);
	RETURNCLIP(params->output);
}
