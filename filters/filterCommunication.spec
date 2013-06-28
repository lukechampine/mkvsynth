This file explains how filters work in mkvsynth. There is a design goal to 
maximize parallelism and to minimize the total amount of RAM used. In most
cases, minimizing RAM has taken precedence.

Filters operate on streams of frames. Each filter has an input buffer and an
output buffer. When loading a frame into memory, the filter will call a
universal function getFrame(), which will pull a frame from the linked list
and handle various aspects of memory management. Once a filter has finished
working with a frame, it will call putFrame(), which will place the frame into
an the output buffer.

Buffers have a size, that in the minimum-viable-product will be defaulted to
3 frames. Eventually, functionality will be added that will allow mkvsynth to
intelligently adjust the size of the frame buffers. There will also be
functionality added that will allow the script-writer to indicate a specific
buffer size for each filter, as some filters may have unique needs and can be
better optimized manually.

Each filter operates inside of its own thread, and the filters communicate
through the buffers. The output buffer of the first filter is the input buffer
for the second filter. This creates the classic producer-consumer problem in
parallel programming, and the threads will be managed according to the best
known solutions.

Each buffer is a singly-linked list of frames (the output filter does not need
to see where the input filter starts) with a 'control' node. The control node
keeps track of meta-data and the total number of frames in the linked-list.

There is a tricky situation where multiple filters could be reading from the
same output. This is handled by a value in each frame indicating the number of
filters that still need to view the frame. A race condition is possible, and
needs to be properly accounted for.

// Names could proabably be improved
// Eventually, support will be added for custom fields
frameStreamControlNode {
	int maxBufferSize;
	int currentBufferSize;
	int width;
	int height;
	int colorDepth;
	int colorSpace;
	frameStreamFrame *mostRecentOutputFrame;
}

frameStreamFrame {
	uint8_t *data;
	int numFiltersRemaining;
	frameStreamFrame *nextFrame;
}

Spec incomplete: clearFrame() needs to be discussed.
Spec incomplete: input and output filters need to be discussed.
