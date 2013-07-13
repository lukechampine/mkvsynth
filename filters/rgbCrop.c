// Cropping in RGB colorspace is pretty easy, because each pixel has the same
// volume of data and the data is organized well. The YUV colorspaces can be
// harder because they can either have 'macropixels' or multiple images within
// a single frame.

// For now, the mkvsynth standard is going to be RGB. Even picking that standard,
// there are multiple different ways to do things. For example, you have RGB24,
// RGB32, RGB36, RGB48, RGB64, each with a different implementation of how to do
// things. RGB24 is the simplest: 3 channels per pixel, each is 1 byte. You can do
// bitwise operations and have a party, and things will be okay. RGB32 has a 4th
// channel, the alpha channel, which is left blank because there's no reason to use
// and alpha channel. But RGB32 is nice because 32bits makes a lot more sense to a
// processor than 24bits, and you can get substantially more optimized code. RGB36 is
// the colorspace that I would like to use, except that you'd need 12bit fields,
// which aren't portable and you can't do bitwise operations on them because
// different compilers will organize the bits differently (yeah, weird, eh?).

// Generally speaking though, it'll be understood that all RGB will be bit field
// values, to prevent filters from making mistakes. Then, it'll be understood that
// if there are 4 channels, the 4th channel should just be ignored. That puts a generic
// framework up that allows filters to operate on basic assuptions about the colorspace
// and be correct simply by following the colorspace conventions. The same code should
// work just fine in all situations.

// Well, maybe not though. It depends on how you treat the potency. If you add '10' to
// an RGB24 pixel, you might notice the difference. If you add '10' to an RGB36 pixel
// though, the difference will be much less noticeable because the magnitude of the
// change will be smaller. So some structure has to be in place that will allow
// filters to keep the same magnitude as they perform the same mathematical operations
// to colorspaces with different depths. Luckily this is a crop filter so pixels are
// only being eliminated, and it can be a problem for a later date.

// Because 'Mkvsynth RGB' is the standard, it will be set to '1' in the colorspace
// field.

// And another new problem: how does the parent node know what the meta data for each
// buffer will be? I guess that each filter will need to have incoming meta data and
// outgoing metadata, which means that metaData should actually be stored in getParams
// and putParams.

// That might also mean a mutex will be needed, to make sure that during the startup
// phase a filter doesn't read inputData before the previous filter writes it.

// Instead I'm going to add a function in jarvis that will signal completion of
// startup.

// These following lines define the input params for the interpreter
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

  if((filterParams->left + filterParams->right) > getParams->metaData->width)
    filterError("You cannot crop that many columns! Insufficient video width!");
   
  if((filterParams->top + filterParams->bottom) > getParams->metaData->height)
    filterError("You cannot crop that many rows! Insufficient video height!");
   
  if(getParams->metaData->colorspace != RGB_GENERIC)
    filterError("rgbCrop does not recognize the colorspace of the input video!");

  // All metaData fields must be set here
  putParams->metaData->colorspace = RGB_GENERIC;
  putParams->metaData->payloadBytes = ___
  putParams->metaData->width = ___
  putParams->metaData->height = ___
  putParams->metaData->depth = ___
  putParams->metaData->channels = ___
 
  signalStartupCompletion(); 

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
