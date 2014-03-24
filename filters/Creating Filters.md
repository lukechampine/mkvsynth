###Creating Filters###

The purpose of this guide is to walk you through a few examples demonstrating how to write filters in mkvsynth. One of the major design goals of mkvsynth is to make writing filters easy and friendly. Currently, the process is still fairly complex but hopefully things will have greatly simplified by December.

As a caveat, Mkvsynth is not yet in alpha (though it is very close!) and any filters that get written for Mkvsynth may be broken as Mkvsynth continues to evolve. We are however committed to releasing the software in a timely manner, and once there is an official version 1.0, we will commit to not breaking code with future updates.

Having said that, feel free to explore and write whatever you want.

##Example: Darken##

[brief aside, this tutorial uses functions that I will implement _soon_ but don't exist yet]

Let's say we want to write a filter that just darkens everything a little bit (a nieve implementation). As input, the filter takes a video and a number, the number representing how dark the frame should get.

Filters are split up into 2 pieces: the startup piece and the multi-threaded piece. The startup piece is where you should do all error checking and set all the metadata. The filter startup is run very early in the program, before any major computation happens. The multithreaded piece is mostly for editing video frames.

The startup piece must be in the form

```c
Value *filterName_AST(argList *a) {
	// Do stuff
}
```

The first part of the startup piece is writing the arguments for the interpreter. The first function you call is checkArgs(), which verifies that the filter was called the the correct parameters. [Luke should explain this since he wrote it]. The syntax for checkArgs is:

```c
checkArgs(a, numberOfRequiredArguments, {... the types of the arguments});
```

For darken, we would have 2 required arguments, an input clip and a number. That means so far, we have:

```c
Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
}
```

To access the function's arguments, we call delbrot macros. For darken, it would look like:

```c
Value *nieveDarken_AST(argList *) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
}
```

If you'll notice, all input videos are going to be of type MkvsynthInput. MkvsynthInput is different than MkvsynthOuptut. typeVal returns a double, so we have to typecast it to a short, because for darken we want a short.

The next part of writing a filter is error checking. Darken is only going to support a single color space, so it's important to check that the input video is the right colorspace. `MkvsynthError()` is used for error messages, and will cause the program to terminate. It is good practice to preface your errors with the name of your function.

```c
Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
}
```

The next part is creating an output video and filling out the metaData. createMkvsynthOutput(MkvsynthInput *input) will automatically copy the metaData from the input to the output. If you are writing a filter that changes the metaData (for example, crop will change the height and width of the video), then you must make sure to update the metaData in the output. nieveDarken does not change any meta data, so this section is short.

```c
Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
}
```

Now it's time for the tricky part. Mkvsynth is a mutlithreaded program, which means the bulk of the computation should be done inside of a pthread. The pthread needs a function to call separate from the startup phase. So let's add that funciton:

```c
void *nieveDarken(void *filterParams) {
	// Do stuff
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
}
```

You'll notice that it takes a void* as input and returns a void* as output. A void* is a pointer to unknown data. Void* is used to make the functions generic, and are required because the same pthread call is used to call every single filter. Before you can dereference a void*, you have to typecast it.

nieveDarken needs 2 input fields: a video clip and a number. This is unfortunate, because our nieveDarken function only has 1 void star for input. We'll need to make a struct in order to pass data to the nieveDarken function:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
}
```

Now that we have set this up, we can return to our startup funciton. We have to create a struct containing the parameters so it can be passed to nieveDarken. The struct must be allocated dynamically, or it will go out of scope too early.

You might also notice that our struct has both an MkvsynthInput and an MkvsynthOutput. That's because after nieveDarken gets a frame and modifies it, it needs somewhere to output it to.

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
}
```

Finally, we can call mkvsynthQueue and return the output:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

If you'll notice, mkvsynthQueue takes as a first argument the parameters, which must be typecast to a void*. The second argument is a function pointer, which is simply the name of the function to be called.

RETURNCLIP is a macro that will give delbrot the output object.

Now that we've performed all of the overhead, we can get to the fun part: actually doing something with the input video to create an output! To get a frame, you call getFrame(input). Since videos are a bunch of frames, you need a while loop to make sure you get the whole video:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
	
	MkvsynthFrame *currentFrame = getFrame(params->input);
	while(currentFrame->payload != NULL) {
		// Do stuff
		
		currentFrame = getFrame(params->input);
	}
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

You know that you have hit the last frame when the 'payload' value is NULL.

For nieveDarken, all we are doing is going through every pixel and darkening it by 'strenght' amount. All frame data is represented as an array of bytes. For rgb48, each color channel is 2 bytes and each pixel is 3 channels. For rgb48, we can look at a frame as an array of width*height*3 values that are 2 bytes each. We'll have to do some typecasting to see the 1-byte array as 2-byte values.

For darken, we just do some simple subtraction on each pixel and the whole frame will darken. We have to be careful though, because unsigned values will wrap around if they go below 0, so we need to check that we aren't subtracting too much.

In total, our frame editing is going to look like:

1. typecast the array of 1-byte data into 2-byte values.
2. loop through every channel of every pixel
3. check that strenght is not going to cause a wrap-around
4. subtract:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
	
	MkvsynthFrame *currentFrame = getFrame(params->input);
	while(currentFrame->payload != NULL) {
		uint16_t *shortPayload = (uint16_t *)currentFrame->payload;
		
		int numChannels = input->metaData->width * input->metaData->height * 3;
		int i;
		for(i = 0; i < numChannels; i++) {
			if(strength < shortPayload[i])
				shortPayload[i] -= strength;
			else
				shortPayload[i] = 0;
		}
		
		currentFrame = getFrame(params->input);
	}
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

Now we have to output the data somewhere. To do this, we call `putFrame(output, shortPayload)`, execpt that putFrame is expecting an array of bytes, so it needs to be typecasted again.

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
	
	MkvsynthFrame *currentFrame = getFrame(params->input);
	while(currentFrame->payload != NULL) {
		uint16_t *shortPayload = (uint16_t *)currentFrame->payload;
		
		int numChannels = input->metaData->width * input->metaData->height * 3;
		int i;
		for(i = 0; i < numChannels; i++) {
			if(strength < shortPayload[i])
				shortPayload[i] -= strength;
			else
				shortPayload[i] = 0;
		}
		
		putFrame(params->output, (uint8_t *)shortPayload);
		currentFrame = getFrame(params->input);
	}
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

To signal in the output that we have hit the last frame, you need to call putFrame with a null payload:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
	
	MkvsynthFrame *currentFrame = getFrame(params->input);
	while(currentFrame->payload != NULL) {
		uint16_t *shortPayload = (uint16_t *)currentFrame->payload;
		
		int numChannels = input->metaData->width * input->metaData->height * 3;
		int i;
		for(i = 0; i < numChannels; i++) {
			if(strength < shortPayload[i])
				shortPayload[i] -= strength;
			else
				shortPayload[i] = 0;
		}
		
		putFrame(params->output, (uint8_t *)shortPayload);
		currentFrame = getFrame(params->input);
	}
	
	putFrame(params->output, NULL);
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

One last thing: memory management. During the startup phase, we allocated a NieveDarkenParams, so we'll have to free() that.

But we also need to memory manage the frames. As soon as you are done with a frame, you should call clearFrame(frame, payloadClear) to deallocate that frame. Don't simply call free(), because there are several variables that need to be free'd (like a mutex). [author's note: clearFrame is going to be complemented by clearReadOnlyFrame at some point, which is why it only needs to take 1 value].

After we do the memory management, the full filter looks like this:

```c
struct NieveDarkenParams {
	MkvsynthInput *input;
	MkvsynthOutput *output;
	short strength;
}

void *nieveDarken(void *filterParams) {
	struct NieveDarkenParams *params = (struct NieveDarkenParams *)filterParams;
	
	MkvsynthFrame *currentFrame = getFrame(params->input);
	while(currentFrame->payload != NULL) {
		uint16_t *shortPayload = (uint16_t *)currentFrame->payload;
		
		int numChannels = input->metaData->width * input->metaData->height * 3;
		int i;
		for(i = 0; i < numChannels; i++) {
			if(strength < shortPayload[i])
				shortPayload[i] -= strength;
			else
				shortPayload[i] = 0;
		}
		
		clearFrame(currentFrame);
		putFrame(params->output, (uint8_t *)shortPayload);
		currentFrame = getFrame(params->input);
	}
	
	putFrame(params->output, NULL);
	free(params);
}

Value *nieveDarken_AST(argList *a) {
	checkArgs(a, 2, typeClip, typeVal);
	MkvsynthInput *input = MANDCLIP(0);
	unsigned short strength = (unsigned short)MANDVAL(1);
	
	if(input->metaData->colorspace != RGB48)
		MkvsynthError("nieveDarken: input video must be rgb48!");
	
	MkvsynthOutput *output = createMkvsynthOutput(input);
	
	struct NieveDarkenParams *params = malloc(sizeof(struct NieveDarkenParams));
	params->input = input;
	params->output = output;
	params->strength = strength;
	
	mkvsynthQueue((void *)params, nieveDarken);
	RETURNCLIP(output);
}
```

Then you use the delbrot tool `loadplugin` to get your filter recognized by mkvsynth. Luke will have to expand on this part.

##Conventions##

Mkvsynth has decided (at least for alpha) to support primarily a single colorspace: rgb48. A single colorspace was chosen because it was determined to be too diffucult to create a generic way to write filters in many colorspaces. Most video editing tools seem to have implementations on a per-colorspace basis, which is not something that mkvsynth wanted. I would like to note that while rgb48 is what is encouraged, there is still a 'colorspace' field in the meta-data and a void*, enabling writers of filters to use any colorspace that they choose. But as a whole, mkvsynth encourages all filters to accept rgb48 as input and have the ability to output rgb48.

rgb was chosen because it represents all potential theoretical colors that can be produced by a screen. rgb48 was chosen because 8 bits does not provide enough granularity for many computations. 8 bit may be as good as most screens can replicate, but filters can produce much more accurate rgb24 pictures if they compute using a higher bit depth. I would have preferred to use something like RGB36, but there is no C datatype that is 12 bits. Thus, the 'short' is used to represent data. This was to avoid bit-twiddling, which can be a computationally intense process.

For video filters, it is convention for the clip arguments to come first. This allows you to use the chain operator (`->`). In an expression like:
```
clip -> nieveDarken x
```
`clip` becomes the first argument of `nieveDarken`, and `x` becomes the second argument.
