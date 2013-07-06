# General Overview

This file explains how filters work in mkvsynth. There is a design goal to 
maximize parallelism and to minimize the total amount of RAM used. In most
cases, minimizing RAM has taken precedence.

# input and output buffer

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
filters that still need to view the frame. When a filter is done collecting a
frame, it will subract 1 from that number. (this causes race-condition issues,
but that will be properly handled)

// Names could proabably be improved
// Eventually, support will be added for custom fields
mkvsynthControlNode {
	int maxBufferSize;
	int currentBufferSize;
	int numFiltersUsingAsInput;
	int width;
	int height;
	int colorDepth;
	int colorSpace;
	mkvsynthFrame *mostRecentOutputFrame;
}

mkvsynthFrame {
	uint8_t *data;
	int numFiltersRemaining;
	int frameNumber;
	frameStreamFrame *nextFrame;
}

# clearFrame() overview

An advanced feature is the function clearFrame(). When multiple filters are
using the same frame, the frame must be read-only so that the data is not
corrupted. In the nieve case, getFrame() will make a copy of the frame it is
fetching if there are other filters that need to use the frame. It is then
expected that the frame will make good use of the memory and do as much
computation as possible using the memory it already has. The ideal situation
is that a filter can use the same memory for output as it used for input, and
such a case is easily achieveable for simple filters that do something like
add 10 to the red value of every single pixel. This may not be feasible for
some of the more advanced filters, and these filters have an additional tool
available to them, getReadOnlyFrame(). getReadOnlyFrame() should only be used
if the input frame is not used as a workspace. Using getReadOnlyFrame() allows
mkvsynth to let multiple filters use the same frame, thus keeping the overall
memory footprint of the program to a minimum.

If getReadOnlyFrame() is used, the frame still has to be deallocated, but
instead of manually deallocating the frame the filter must call the function
clearFrame(), which will insure that the memory is not deallocated while other
filters are still reading from the same memory. clearFrame() should be called
as soon as the input frame is no longer necessary for computation.

As stated before, this is an advanced feature that will not be mentioned in the
beginner tutorials, because it is easy to misuse and in general complicates the
process of writing a filter.

# input and output filters

There are two special types of filters, an input filter and an output filter.
An input filter creates its own frames, and therefore does not have an input
buffer. It may get frames from a file, or it may just algorithmically generate
frame. The point is that there is no input buffer for this type of filter.

The other type of filter is an output filter, which instead of writing to an
output buffer does something else with the frame, like writing it to a file,
uploading it over the internet, or who knows what.

These two types of filters do not need to do anything special internally, but
the interpreter needs to be able to recognize them to help with warnings and
errors, and to make sure buffers are not created if they will not be used.

# Funtion specifics

getFrame:
1. getFrame has a static variable (value doesn't change when it goes out of
   scope) pointing to the next frame in the buffer. If the next frame is not
   in the buffer, sleep until there is a frame to consume
2. when there is a frame to consume, check the 'filtersRemaining' count. If
   the count is greater than 1, copy the frame into a new place in memory,
   lock the counter, decrement the counter, unlock the counter, and pass the
   copied frame back to the filter
   
   If the count is just 1, pass the frame (without allocating more memory) to
   the filter and decrement the 'currentBufferSize' counter in the control
   node. It is up to the filter calling getFrame to deallocate the memory.
   
getReadOnlyFrame:
1. see part 1 of getFrame
2. the frame is pointer is given to the filter, nothimg more happens. The
   filter recieving the frame is trusted not to corrupt the data.

clearFrame:
0. Used to deallocate frames. clearFrame must be called if getReadOnlyFrame
   was used to read the frame. Can also be used if the frame was retrieved
   via getFrame(), though free() can also be used.
1. All it does is perform the 'filtersRemaining' check/update/deallocate that
   happens when getFrame is called.
   
putFrame:
1. if the buffer is full, sleep until the buffer is not full
2. change mostRecentOutputFrame from having null data to pointing at the
   output frame generated by the filter.
3. Create new node with null data and point nextFrame of mostRecentOutputFrame
   at the new null-data frame.
4. fill out the numFiltersRemaining value in the new frame (from control node)
5. point mostRecentOutputFrame at the frame with null-data.
6. update the bufferSize to reflect that a frame has been added (use locks)

# Spawning

Each filter operates from its own thread. The interpreter has to spawn the
thread. Each filter also has a buffer (most have 2 buffers, input and out).
The interpreter has to allocate the control node for the buffer, as well as
the very first null-data frame for each buffer. The spawning process will
have to make sure that all the starting variables are correct but then the
filter stream will do everything else itself.

What is probably going to happen is that Delbrot is going to have a template
for writing a parent process that spawns all the filter threads and insures
that they terminate successfully and without error.

# Termination

I'm not happy with my current solution for termination, but what currently
happens is that I'm adding a value to the frame struct that accounts for
frame number, a negative number signifying that this is the last frame.

Primarily because you have to accept the frames in order anway, I don't see
a particular use for counting frames from within the actual frame struct
itself, but the filter needs some way to recognize that it's time to finish
processing and output it's last few frames and terminate.

I would like to add some functionality where a 'novice' filter builder would
not have to deal with checking for the last frame and the program woud just
automatically terminate when the last frame had been processed, but I haven't
yet thought of a good way to do this that wouldn't also make life harder for
experienced filter writers. At this point, I'm more worried about making life
simple for advanced developers than I am for novice developers, because it's
the advanced filters that will make this program worth using.

The parent thread will then confirm that the whole stream got processed
without error. Which means that the runtime thread will need some way to
pass an error message to the parent in the even that something happens such
as 'input resolution must be evenly divisible by 16'.

And now that I'm thinking about it, it would be nice to have a way to catch
simpler errors like that compile-time instead of run-time. This might not be
an MVP feature though

# Parallel Filters and Other Optimizations

These will not be supported in the mvp but are worth being mentioned. For
slow single-frame filters, we can allocate multiple instead of having a
single one be a bottleneck. We could also potentially do this with multi-
frame filters, but the implimentation will be trickier. We could also
take multiple fast filters and slam them together into a single filter,
something that might also be tricky to implement but may have some speed
and memory benefit - basically force only 1 to be running at a time and
instead of having 1 buffer each, have one buffer on either end of the
whole chain.
