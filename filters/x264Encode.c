void x264Encode(ASTParams *filterParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char *options = checkString(filterParams, "options");
	char *outputFile = checkString(filterParams, "file"); // this is optional
	MkvsynthInput *input = checkInput(filterParams, "input1");

	////////////////////
	// Error Checking //
	////////////////////
	// Not sure what possible errors are
	
	///////////////////
	// x264 Spawning //
	///////////////////
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
