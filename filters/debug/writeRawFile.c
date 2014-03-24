#ifndef writeRawFile_c_
#define writeRawFile_c_

#include "../../jarvis/jarvis.h"
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
	MkvsynthFrame *workingFrame = getReadOnlyFrame(params->input);

	int frame = 1;
	while(workingFrame->payload != NULL) {
		fwrite(workingFrame->payload, 1, getBytes(params->input->metaData), params->file);
		MkvsynthMessage("output frame %i", frame);
		frame++;
		clearReadOnlyFrame(workingFrame);
		workingFrame = getReadOnlyFrame(params->input);
	}

	free(params);
	return NULL;
}

Value *writeRawFile_AST(argList *a) {
	struct writeRawFileParams *params = malloc(sizeof(struct writeRawFileParams));

	checkArgs(a, 2, typeClip, typeStr);
	MkvsynthOutput *output = MANDCLIP(0);
	char *filename = MANDSTR(1);

	////////////////////
	// Error Checking //
	////////////////////
	params->file = fopen(filename, "w");
	if(params->file == NULL)
		MkvsynthError("Could not open the output file!");

	params->input = createInputBuffer(output);

	mkvsynthQueue((void *)params, writeRawFile);
    RETURNNULL();
}

#endif
