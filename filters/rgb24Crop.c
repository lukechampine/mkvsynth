void rgbCrop(ASTParams *filterParams, MkvsynthGetParams *getParams, MkvsynthPutParams *putParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	int left = checkInt(filterParams, "left", int);
	int top = checkInt(filterParams, "top", int);
	int right = checkInt(filterParams, "right", int);
	int bottom = checkInt(filterParams, "bottom", int);

	////////////////////
	// Error Checking //
	////////////////////
	if((filterParams->left + filterParams->right) > getParams->metaData->width)
		filterError("You cannot crop that many columns! Insufficient video width!");
   
	if((filterParams->top + filterParams->bottom) > getParams->metaData->height)
		filterError("You cannot crop that many rows! Insufficient video height!");
   
	if(getParams->metaData->colorspace != PACKED_RGB && getParams->metaData->colorspace != STRICT_RGB)
		filterError("rgbCrop does not recognize the colorspace of the input video!");

	///////////////
	// Meta Data //
	///////////////
	putParams->metaData->colorspace = getParams->metaData->colorspace;
	putParams->metaData->width = getParams->metaData->width - filterParams->left - filterParams->right;
	putParams->metaData->height = getParams->metaData->height - filterParams->top - filterParams->bottom;
	putParams->metaData->channels = getParams->metaData->channels;
	putParams->metaData->depth = getParams->metaData->channels;
	putParams->metaData->bytes = 3*putParams->metaData->width*putParams->metaData->height;
	signalStartupCompletion(); 

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(getParams);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(putParams->metaData->bytes);

		int i, j;
		for(i = 0; i < putParams->metaData->height; i++) {
			int sourceOffset = i*3*getParams->width+left*3;
			int destOffset = 3*i*width;
			int offsetSize = 3*(width-left-right);
			memcpy(payload[3*i*width], workingFrame->payload[startOffset], offsetSize);
		}
 
		clearFrame(workingFrame, NULL);
		putFrame(putParams, payload);
		workingFrame = getFrame(getParams);
	}

	putFrame(NULL);
}
