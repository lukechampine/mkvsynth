#include <libswscale/swscale.h>

void rgb24Resize(ASTParams *filterParams, MkvsynthGetParams *getParams, MkvsynthPutParams *putParams) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	char* algorithm = checkArgs(filterParams, "algorithm", string);
	int width = checkArgs(filterParams, "width", int);
	int height = checkArgs(filterParams, "height", int);

	////////////////////
	// Error Checking //
	////////////////////
	if(algorithm != "spline" && algorithm != "bilinear") {
		// ERROR
	}

	///////////////
	// Meta Data //
	///////////////
	putParams->metaData->colorspace = GENERIC_RGB;
	putParams->metaData->width = width;
	putParams->metaData->height = height;
	putParams->metaData->channels = 3;
	putParams->metaData->depth = 8;
	putParams->metaData->bytes = 3*width*height;

	signalStartupCompletion(); 

	int oldLinesize = 3 * getParams->width;
	int newLinesize = 3 * width;

	int sws_algorithm;
	if(algorithm == "spline") {
		sws_algorithm = SWS_SPLINE;
	} else {
		sws_algorithm = SWS_BILINEAR;
	}

	struct SwsContext *resizeContext = sws_getContext (
		getParams->metaData->width,
		getParams->metaData->height,
		PIX_FMT_RGB24,
		width,
		height,
		PIX_FMT_RGB24,
		sws_algorithm,
		NULL,
		NULL,
		NULL);

	/////////////////
	// Filter Loop //
	/////////////////
	MkvsynthFrame *workingFrame = getFrame(getParams);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(3*height*width);

		sws_scale (
			resizeContext,
			(uint8_t const * const *)workingFrame->payload,
			oldLinesize,
			0,
			getParams->metaData->height,
			payload,
			newLinesize);
			
		clearFrame(workingFrame, NULL);
		putFrame(putParams, payload);
		workingFrame = getFrame(getParams);
	}

	putFrame(NULL);
}
