#ifndef writeRawFile_c_
#define writeRawFile_c_

#include "../jarvis/datatypes.h"

struct writeRawFileParams {
	FILE *file;
	MkvsynthInput *input;
};

MkvsynthOutput *writeRawFileDefinition(ASTnode *p, ASTnode *args) {
	struct writeRawFileParams *params = malloc(sizeof(struct writeRawFileParams));
	
	checkArgs("writeRawFile", args, 2, typeStr, typeOut);
	MkvsynthOutput *output = MANDOUT();
	char *filename = MANDSTR();
	
	////////////////////
	// Error Checking //
	////////////////////
	params->file = fopen(outputFileName, "w");
	if(outputFile == NULL)
		filterError("Could not open the output file!");	

	params->output = createInputBuffer(output);
	
	mkvsynthQueue((void *)params, writeRawFile);
	return NULL;
}

void writeRawFile(void *filterParams) {
	struct writeRawFileParams *params = (struct writeRawFileParams *)filterParams;
	
	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(params->input);

	while(workingFrame != NULL) {
		fwrite(workingFrame->payload, 1, params->input->metaData->bytes, params->file);
		clearFrame(workingFrame, NULL);
		workingFrame = getFrame(input);
	}

	putFrame(NULL);
}

#endif
