void genericSourceFilter(int numFramesToGenerate, MkvsynthControlNode *controlNode) {
	// I'm just monkeying around at this point, the goal is to generate some sort of 
}	

void genericFilter(GenericFilterParams *filterParams, MkvsynthMetaData *metaData, MkvsynthGetParams *getParams) {
	MkvsynthFrame workingFrame = getFrame(getParams);
	while(workingFrame != NULL) {
		int i;
		for(i=0; i<getParams->payloadBytes; i++)
			workingFrame->payload[i] += filterParams->adjustmentMagnitude;
		workingFrame = getFrame(getParams);
	}
}

void genericReadOnlyFilter() {

}

// Short and sweet:
// 	1. open a file to write to, based on the input parameter
// 	2. get the first frame
// 	3. as long as there are more frames, write to file and grab next frame
//
// 	Note: This is a rare case, since we are just writing the stream to a file we don't actually need to look at the meta data
void genericSinkFilter(GenericSinkFilterParams *filterParams, MkvsynthMetaData *metaData, MkvsynthGetParams *getParams) {
	FILE *file = fopen(filterParams->filename, "w");

	MkvsynthFrame workingFrame = getFrame(getParams);
	while(workingFrame != NULL) {
		fwrite(workingFrame->payload, getParams->payloadBytes, 1, file);
		workingFrame = getFrame(getParams);
	}
}
