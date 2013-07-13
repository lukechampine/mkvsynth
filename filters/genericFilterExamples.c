
void genericSourceFilter(GenericFilterParams *filterParams, MkvsynthMetaData *metaData, MkvsynthPutParams *putParams) {
	
	// Write the meta-data, using filterParams as needed
	
	// Somehow frame payloads are generated. As they are generated putFrame is called
	// Probably this step is happening inside of a loop
	while(filterParams->framesRemaining > 0) {
		// create a payload
		uint8_t *payload = malloc(...);
		putFrame(payload);
	}
	
	// Indicate that you are done
	putFrame(NULL);
}	

void genericFilter(GenericFilterParams *filterParams, MkvsynthMetaData *metaData, MkvsynthGetParams *getParams, MkvsynthPutParams *putParams) {

	// Before starting any frame processing, check the filterParams and metaData to make sure
	// that the colorspace of the frame is supported, that the filterParams are all correct,
	// and any other early-checking to minimize errors in the main loop.

	// To start the main loop, you have to getFrame() at least once
	MkvsynthFrame *workingFrame = getFrame(getParams);
	
	while(workingFrame != NULL) {
		
		// Perform modifications here, using filterParams and metaData as needed
		
		// clearFrame will deallocate workingFrame->payload unless
		// workingFrame->payload is specified as the second parameter
		// clearFrame(workingFrame, NULL) is valid, everything will be deallocated
		clearFrame(workingFrame, workingFrame->payload);
		
		// if the second parameter of putFrame is workingFrame->payload,
		// make sure that clearFrame also had workingFrame->payload as the 
		// second parameter. It's also fine to give putFrame() other memory
		putFrame(putParams, workingFrame->payload);
		
		workingFrame = getFrame(getParams);
	}
	
	// This will tell the next filter that there will be no more frames
	putFrame(NULL);
}

// To be implemented later, jarvis proof-of-concept is not complete
// Read-Only filters are only efficient in the hands of advanced users
void genericReadOnlyFilter() {

}

void genericSinkFilter(GenericSinkFilterParams *filterParams, MkvsynthMetaData *metaData, MkvsynthGetParams *getParams) {

	// Before starting any frame processing, check the filterParams and metaData to make sure
	// that the colorspace of the frame is supported, that the filterParams are all correct,
	// and any other early-checking to minimize errors in the main loop.

	// To start things off, you have to getFrame() at least once
	MkvsynthFrame *workingFrame = getFrame(getParams);
	
	while(workingFrame != NULL) {
		
		// Do something with the frame, using filterParams and metaData as needed
		
		workingFrame = getFrame(getParams);
	}
}
