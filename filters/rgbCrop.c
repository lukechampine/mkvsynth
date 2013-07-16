// Ideally, the interpreter will be able to look at this struct and figure
// out how to take the parameters input by the user and fill out the struct
// for the filter.

//! Mkvsynth Filter rgbCrop
//# mandatory int left
//# mandatory int top
//# mandatory int right
//# mandatory int bottom

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} RgbCropParams;

void rgbCrop(RgbCropParams *filterParams, MkvsynthGetParams *getParams, MkvsynthPutParams *putParams) {

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

	int bits = putParams->metaData->width * 
	           putParams->metaData->height * 
	           putParams->metaData->channels * 
	           putParams->metaData->depth;

	putParams->metaData->bytes = bits / 8;

	if(bits % 8 != 0)
		putParams->metaData->bytes += 1;
 
	signalStartupCompletion(); 

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(getParams);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(putParams->metaData->bytes);

		int i, j;
		for(i = 0; i < putParams->metaData->height; i++) {
			for(j = 0; j < putParams->metaData->width; j++) {
				long long currentPixel = getPixel(getParams->metaData, workingFrame->payload, i+filterParams->top, j+filterParams->left);
				putPixel(putParams->metaData, payload, currentPixel);
			}
		}
 
		clearFrame(workingFrame, workingFrame->payload);
 
		putFrame(putParams, NULL);

		workingFrame = getFrame(getParams);
	}

	putFrame(NULL);
}
