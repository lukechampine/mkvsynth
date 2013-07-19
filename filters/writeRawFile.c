void writeRawFile(ASTParams *filterParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char *outputFileName = checkString(filterParams, "file");
	MkvsynthInput *input = checkInput(filterParams, "input1");

	////////////////////
	// Error Checking //
	////////////////////
	FILE *outputFile = fopen(outputFileName, "w");
	if(outputFile == NULL)
		filterError("Could not open the output file!");

	///////////////
	// Meta Data //
	///////////////
	// Not needed on sink filters

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(input);

	while(workingFrame != NULL) {
		fwrite(workingFrame->payload, 1, input->metaData->bytes, outputFile);
		clearFrame(workingFrame, NULL);
		workingFrame = getFrame(input);
	}

	putFrame(NULL);
	mkvsynthTerminate();
}
