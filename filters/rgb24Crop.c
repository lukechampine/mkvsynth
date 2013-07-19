void rgb24Crop(ASTParams *filterParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	int left = checkInt(filterParams, "left");
	int top = checkInt(filterParams, "top");
	int right = checkInt(filterParams, "right");
	int bottom = checkInt(filterParams, "bottom");
	MkvsynthInput *input = checkInput(filterParams, "input1");
	MkvsynthOutput *output = checkOutput(filterParams, "output1");

	////////////////////
	// Error Checking //
	////////////////////
	if((left + right) > input->metaData->width)
		filterError("You cannot crop that many columns! Insufficient video width!");
   
	if((top + bottom) > input->metaData->height)
		filterError("You cannot crop that many rows! Insufficient video height!");
   
	if(input->metaData->colorspace != PACKED_RGB && input->metaData->colorspace != STRICT_RGB)
		filterError("rgbCrop does not recognize the colorspace of the input video!");

	///////////////
	// Meta Data //
	///////////////
	output->metaData->colorspace = input->metaData->colorspace;
	output->metaData->width = input->metaData->width - left - right;
	output->metaData->height = input->metaData->height - top - bottom;
	output->metaData->channels = input->metaData->channels;
	output->metaData->depth = input->metaData->channels;
	output->metaData->bytes = 3*output->metaData->width*output->metaData->height;
	signalStartupCompletion(); 

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(input);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(output->metaData->bytes);

		int i, j;
		for(i = 0; i < output->metaData->height; i++) {
			int sourceOffset = i*3*input->width+left*3;
			int destOffset = 3*i*width;
			int offsetSize = 3*(width-left-right);
			memcpy(payload[3*i*width], workingFrame->payload[startOffset], offsetSize);
		}
 
		clearFrame(workingFrame, NULL);
		putFrame(output, payload);
		workingFrame = getFrame(input);
	}

	putFrame(NULL);
	mkvsynthTerminate();
}
