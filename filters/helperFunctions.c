#ifndef _datatypes_h_
#define _datatypes_h_

typedef struct {
	// Number of frames allowed in the buffer
	// More will make mkvsynth faster, but take more ram
	// Diminishing returns apply
	int maxBufferSize;

	// How full is the buffer?
	int framesInBuffer;

	// How many filters are using this buffer as input
	int outputBreadth;

	// width portion of the resolution
	int width;

	// height portion of the resolution
	int height;

	// how many bits deep the colorspace is
	int colorDepth;

	// numerical identifier for the colorspace
	// eventually, global defines will be used, like in other video programs
	int colorSpace;

	// points to the most recently output frame
	mkvsythFrame *mostRecentFrame;
} mkvsynthControlNode;

typedef struct {
	// pointer to the first byte of a frame, which exists in dynamic memory
	uint8_t *data;

	// how many filters need to read this frame before the memory can be freed?
	int filtersRemaining;

	// a negative number indicates that this is the last frame
	int frameNumber;

	// points to the next frame in the stream
	mkvsynthFrame *nextFrame;
} mkvsynthFrame;

#endif
