void x264Encode(ASTParams *filterParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char *options = checkString(filterParams, "options");
	char *outputFile = checkString(filterParams, "file"); // this is optional
	MkvsynthInput input = checkInput(filterParams, "input");

	////////////////////
	// Error Checking //
	////////////////////
	// I'm sure there are some limitations like video must me modular 2, but not entirely sure
	
	///////////////////
	// x264 Spawning //
	///////////////////
	// Concatenate options to the string that fills out the raw information
	char fullCommand[1024];
	snprintf(fullCommand, sizeof(fullCommand), "x264 --input-csp rgb24 --width %i --height %i %s --output %s",
		input->metaData->width, input->metaData->height, options, outputFile);

	FILE *x264Proc = popen(fullCommand, "w");

	///////////////
	// Meta Data //
	///////////////
	// Output filter, so nothing is needed here

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(getParams);

	while(workingFrame != NULL) {
		fwrite(workingFrame->payload, 1, workingFrame->bytes, x264Proc);
		clearFrame(workingFrame, NULL);
		putFrame(putParams, payload);
		workingFrame = getFrame(getParams);
	}

	pclose(x264Proc);
}
