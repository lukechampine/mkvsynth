// The following file is hopefully ~equivalent~ to the follwoing mkvs:
//
// video a = ffmpegDecode("test.mp4");
// a = a.rgb24Crop(10, 20, 0, 16); // a.crop() would be an error because crop has output, a is not assumed as the output object
// a = a.rgb24Resize(1280, 720);
// video b = a.rgb24Resize(720, 576);
// a.x264Encode("--preset placebo"); // a = a.encode would be an error because encode() is a sink filter
// b.x264Encode("--preset placebo");
//
// Delbrot needs some way to distinguish source, regular, and sink filters to catch certain errors.

int main() {
	
	//
	// This Fuction does the following:
	//
	//                    | -> Resize to SD -> Encode
	//  Decode -> Crop -> |
	//                    | -> Resize to HD -> Encode
	//
	
	char *decoderFile = "test.mp4";
	int cropLeft = 10;
	int cropTop = 20;
	int cropRight = 0;
	int cropBottom = 16;
	int resizeWidth1 = 720;
	int resizeHeight1 = 576;
	int resizeWidth2 = 1280;
	int resizeHeight2 = 720;
	char *x264Params1 = "--preset placebo";
	char *x264Params2 = "--preset placebo";
	
	MkvsynthOutput *decoderOutput = createOutputBuffer();
	MkvsynthOutput *cropOutput = createOutputBuffer(); // crop feeds 2 filters, so two 'slots' must be signaled
	MkvsynthOutput *resizeOutput1 = createOutputBuffer();
	MkvsynthOutput *resizeOutput2 = createOutputbuffer();
	
	MkvsynthInput *cropInput = createInputBuffer(decoderOutput);
	MkvsynthInput *resizeInput1 = createInputBuffer(cropOutput); // This guy gets slot 0
	MkvsynthInput *resizeInput2 = createInputBuffer(cropOutput); // This guy gets slot 1
	MkvsynthInput *x264Input1 = createInputBuffer(resizeOutput1);
	MkvsynthInput *x264Input2 = createInputBuffer(resizeOutput2);
	
	//
	// This is where delbrot assembles all parameters together into ASTParams...
	// Should look something like the following:
	//
	// decodeAST: decoderFile + decoderOutput
	// cropAST: cropLeft + cropTop + cropRight + cropBottom + cropInput + cropOutput
	// resize1AST: resizeWidth1 + resizeHeight1 + resizeInput1 + resizeOutput2
	// resize2AST: resizeWidth2 + resizeHeight2 + resizeInput2 + resizeOutput2
	// x2641AST: x264Params1 + x264Input1
	// x2642AST: x264Params2 + x264Input2
	//

	// the spawn function isn't written yet, so things might change
	// I think though that these parameters will be sufficient
	spawn(decodeAST, ffmpegDecode);
	spawn(cropAST, rgb24Crop);
	spawn(resize1AST, rgb24Resize);
	spawn(resize2AST, rgb24Resize);
	spawn(x2641AST, x264Encode);
	spawn(x2642AST, x264Encode);

	return 0;
}
