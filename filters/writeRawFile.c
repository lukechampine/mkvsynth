#ifndef WriteRawFile_c_
#define writeRawFile_c_

#include "../delbrot/delbrot.h"
#include "../jarvis/bufferAllocation.h"
#include "../jarvis/frameControl.h"
#include "../jarvis/spawn.h"
#include <stdio.h>

struct writeRawFileParams {
	FILE *file;
	MkvsynthInput *input;
};

void *writeRawFile(void *filterParams) {
	struct writeRawFileParams *params = (struct writeRawFileParams *)filterParams;

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(params->input);

	while(workingFrame->payload != NULL) {
		fwrite(workingFrame->payload, 1, params->input->metaData->bytes, params->file);
		clearFrame(workingFrame, 1);
		workingFrame = getFrame(params->input);
	}
}

MkvsynthOutput *writeRawFile_AST(ASTnode *p, ASTnode *args) {
	struct writeRawFileParams *params = malloc(sizeof(struct writeRawFileParams));

	checkArgs("writeRawFile", args, 2, typeStr, typeClip);
	char *filename = MANDSTR();
	MkvsynthOutput *output = MANDCLIP();

	////////////////////
	// Error Checking //
	////////////////////
	params->file = fopen(filename, "w");
	if(params->file == NULL) {
		printf("Could not open the output file!\n");	
		exit(0);
	}

	params->input = createInputBuffer(output);

	mkvsynthQueue((void *)params, writeRawFile);
}

#endif
