# jarvis #

Jarvis contains a set of funcitons and structs that enable filters to run in parallel. Jarvis covers all aspects of the process, from startup to buffering to termination. Jarvis is sill under construction, but is nearing completion. Buffer control is nearly complete, startup is a fully formed concept but not fully implemented, and termination is currently just a call to exit().

The general process for scripts is such:

```
for( each filter ) {
	Create Buffers
	Organize MetaData
	Queue Startup
}

for( startup queue ) {
	<optimization and error checking>
	Begin Computation!
}
	
```

When the buffers get created, the create functions will automatically associate the filters to the correct inputs and outputs.
MetaData must be created before a call to startup is made, because the next filter will need to know what the metadata is.
Filters cannot start immediately because they need to have information about the other filters around them. As the filters are created, they will continue to update each other. 

Once all the filters have done the buffer allocation, jarvis can check that all filters are used in computation, and then will create 1 pthread for each filter. Eventually, there will be optimizations in place that happen based on the filters that are put together, but that's a feature for later.

## Frame Streams ##

A frame stream is a linked list of frames that forms a video. Because videos are too large to store entirely in memory, only a part of the video is in memory at a time. Because frames go in order, mkvsynth can have the filters running in parallel. The result is that while the Decoder is decoding frames 100-105, crop can be editing frames 95-100, and resize can be working on frames 90-95.  The frames are passed from filter to filter in a stream.

## Buffers ##

Some filters work faster than others. For this reason, buffers are used. Right now, a buffer size of 5 frames will always be used. Eventually, there will be ways to optimize this number, both manually and automatically. If ffmpegDecode gets 5 frames ahead of the next filter, it will wait until the buffer has emptied.

This is a variation of the Producer-Consumer problem. In mkvsynth, 1 output can be used by multiple consumers as input. The tricky part is that each consumer needs to consume each unique frame that gets produced, and the consumers may all be at different spots along the stream. For this reason, each consumer has a separate semaphore for the producer. Mutexes are done on a per-frame level, because only 1 thread can be accessing a frame at a time. Producers ignore mutexes because there is no risk of a producer editing a frame that a consumer will have access to. The function for a producor is putFrame(). The function for a consumer is getFrame().

The semaphores are implemented as a linked list, because during the startup phase there is no way to tell how many semaphores will be needed. They are added dynamically.

putFrame() assumes that 'recentFrame' currently exists and contains no data. For each filter using the output (for each consumer), putFrame calls sem_wait(consumedBuffer) to insure that it does not produce a frame until there is room in the buffer. wait() must be called for each consumer, because if 1 consumer is lagging behind then the buffer is still full. putFrame() then fills out 'recentFrame' with the data from the filter, and allocates a new empty frame to point to. Then putFrame() calls sem_post() for each consumer, to let all consumers know that a frame has been added to the buffer.

getFrame() waits until there is a frame in the buffer (using the unique semaphore it shares with the producer), then it locks the frame, then it produces a unique copy of the frame for the filter. If this filter is the last filter to get the frame, getFrame() can just pass a pointer. Otherwise, getFrame() will make a replica of the frame (including a new mutex_init) and pass a pointer to the replica. Then it will update the frame to acknowledge that the frame has been consumed by a filter.

clearFrame() gets called when a filter is done using a frame. clearFrame makes sure that all filters are done using a frame before deallocating it, and makes sure the frame is fully deallocated.

## Pthreads ##

The bulk of the processing is done from pthreads. Before the filters can start working though, they need all the metadata from other filters. Filters set up in serial, and add themselves to a linked list of un-created pthreads. When the final filter has started up, it calls the function that will crawl though the linked list and spawn all of the ptheads.


