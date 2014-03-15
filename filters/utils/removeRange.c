#include "../../jarvis/jarvis.h"
#include <stdio.h>

// first and last are inclusive
struct RemoveRangeParams {
	unsigned long long first;
	unsigned long long last;
	MkvsynthInput *input;
	MkvsynthOutput *output;
};

void *removeRange(void *filterParams) {
	struct RemoveRangeParams *params = (struct RemoveRangeParams *)filterParams;

	MkvsynthFrame *workingFrame = getFrame(params->input);

	int frame = 1;
	while(workingFrame->payload != NULL) {
		if(frame < params->first || frame > params->last) {
			putFrame(params->output, workingFrame->payload);
			clearFrame(workingFrame);
		} else {
			clearReadOnlyFrame(workingFrame); // This will destroy all parts of the frame
		}

		workingFrame = getFrame(params->input);
		frame++;
	}

	putFrame(params->output, NULL);
	clearFrame(workingFrame);
	free(params);
	return NULL;
}

ASTnode *removeRange_AST(ASTnode *p, argList *a) {
	struct RemoveRangeParams *params = malloc(sizeof(struct RemoveRangeParams));

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	checkArgs(a, 3, typeClip, typeNum, typeNum);
	MkvsynthOutput *input = MANDCLIP(0);
	params->first = (unsigned long long)MANDNUM(1);
	params->last = (unsigned long long)MANDNUM(2);

	params->input = createInputBuffer(input);
	params->output = createOutputBuffer();

	///////////////
	// Meta Data //
	///////////////
	params->output->metaData->colorspace = input->metaData->colorspace;
	params->output->metaData->width = input->metaData->width;
	params->output->metaData->height = input->metaData->height;
	params->output->metaData->fpsNumerator = input->metaData->fpsNumerator;
	params->output->metaData->fpsDenominator = input->metaData->fpsDenominator;

	mkvsynthQueue((void *)params, removeRange);
	RETURNCLIP(params->output);
}
