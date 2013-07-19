#include <libswscale/swscale.h>

void rgb24Resize(ASTParams *filterParams, MkvsynthGetParams *input, MkvsynthPutParams *output) {

	///////////////////////
	// Parameter Parsing //
	///////////////////////
	int width = checkInt(filterParams, "width");
	int height = checkInt(filterParams, "height");
	char* algorithm = checkString(filterParams, "algorithm");
	MkvsynthInput *input = checkInput(filterParams, "input1");
	MkvsynthOutput *output = checkOutput(filterParams, "output1");

	////////////////////
	// Error Checking //
	////////////////////
	if(algorithm != "spline" && algorithm != "bilinear") {
		filterError("Resize algorithm is not recognized.");
	}

	///////////////
	// Meta Data //
	///////////////
	output->metaData->colorspace = GENERIC_RGB;
	output->metaData->width = width;
	output->metaData->height = height;
	output->metaData->channels = 3;
	output->metaData->depth = 8;
	output->metaData->bytes = 3*width*height;

	signalStartupCompletion(); 

	int oldLinesize = 3 * input->width;
	int newLinesize = 3 * width;

	int sws_algorithm;
	if(algorithm == "spline") {
		sws_algorithm = SWS_SPLINE;
	} else {
		sws_algorithm = SWS_BILINEAR;
	}

	struct SwsContext *resizeContext = sws_getContext (
		input->metaData->width,
		input->metaData->height,
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
	MkvsynthFrame *workingFrame = getFrame(input);

	while(workingFrame != NULL) {
		uint8_t *payload = malloc(3*height*width);

		sws_scale (
			resizeContext,
			(uint8_t const * const *)workingFrame->payload,
			oldLinesize,
			0,
			input->metaData->height,
			payload,
			newLinesize);
			
		clearFrame(workingFrame, NULL);
		putFrame(output, payload);
		workingFrame = getFrame(input);
	}

	putFrame(NULL);
}
