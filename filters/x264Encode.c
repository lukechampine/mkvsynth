#ifndef x264Encode_c_
#define x264Encode_c_

#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/frameControl.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct x264EncodeParams {
	char *filename;
	char *x264params;
	MkvsynthInput *input;
};

void *x264Encode(void *filterParams) {
	struct x264EncodeParams *params = (struct x264EncodeParams*)filterParams;

	char fullCommand[1024];
	
	snprintf(fullCommand, sizeof(fullCommand), "x264 - --input-csp rgb --input-depth 16 --fps %i/%i --input-res %ix%i %s -o %s",
		params->input->metaData->fpsNumerator,
		params->input->metaData->fpsDenominator,
		params->input->metaData->width,
		params->input->metaData->height,
		params->x264params,
		params->filename);

	FILE *x264Proc = popen(fullCommand, "w");

	MkvsynthFrame *workingFrame = getReadOnlyFrame(params->input);

	int i = 0;
	while(workingFrame->payload != NULL) {
		fwrite(workingFrame->payload, 1, params->input->metaData->bytes, x264Proc);
		clearReadOnlyFrame(workingFrame);
		workingFrame = getReadOnlyFrame(params->input);
	}

	pclose(x264Proc);
	free(params);
}

void x264Encode_AST(ASTnode *p, ASTnode *args) {
	struct x264EncodeParams *params = malloc(sizeof(struct x264EncodeParams));

	checkArgs("writeRawFile", args, 2, typeClip, typeStr);
	MkvsynthOutput *output = MANDCLIP();
	params->filename = MANDSTR();
	params->x264params = OPTSTR("params", "");
	params->input = createInputBuffer(output);

	mkvsynthQueue((void *)params, x264Encode);
}

#endif
