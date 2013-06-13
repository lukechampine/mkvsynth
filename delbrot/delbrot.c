#include <stdio.h>
#include "delbrot.h"

/* ffmpeg decoding function */
double ffmpegDecode(argNode *args) {
	// get arguments
	double x = args->dValue;
	args = args->next;
	double y = args->dValue;

	return (x * y);
}

double print(argNode *args) {
	// get argument
	double num = args->dValue;

	printf ("%.10g\n", num);
	return 0;
}

/* x264 encoding function */
void x264Encode(int numFrames) {
	printf("Processed %d frames\n", numFrames);
}