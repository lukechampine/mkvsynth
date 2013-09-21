//do not be alarmed, this is only a test
//tests the getred,blue,green


#include "pixels.h"



//This will probably be something other than int main in the future, but for now, it is int main.
int main()
{
	MkvsynthPixel pixel_rgb24;
	MkvsynthMetaData data_rgb24;
	MkvsynthPixel pixel_rgb48;
	MkvsynthMetaData data_rgb48;
	MkvsynthPixel pixel_yuv24;
	MkvsynthMetaData data_yuv24;
	MkvsynthPixel pixel_yuv48;
	MkvsynthMetaData data_yuv48;
	
	
	//rgb24 colorspace pixel test
	//expected output:
	//getred:49152
	//getgreen:43008
	//getblue:256
	pixel_rgb24.channel[0] = 192;
	pixel_rgb24.channel[1] = 168;
	pixel_rgb24.channel[2] = 1;
	for(int y = 3; y < 8; y++)
	{
		pixel_rgb.channel[y] = 0;
	}
	data_rgb24.colorspace = 2;
	if(getRed(pixel_rgb24, data_rgb24) == 49152)
	{
		printf("rgb24 getred passed\n");
		//test passed
		
	}else{
		printf("rgb24 getred failed\n");
		//test failed
		
	}
if(getGreen(pixel_rgb24, data_rgb24) == 43008)
	{
		printf("rgb24 getgreen passed\n");
		//test passed
		
	}else{
		printf("rgb24 getgreen failed\n");
		//test failed
		
	}
if(getBlue(pixel_rgb24, data_rgb24) == 256)
	{
		printf("rgb24 getblue passed\n");
		//test passed
		
	}else{
		printf("rgb24 getblue failed\n");
		//test failed
		
	}
	
	//rgb48 colorspace pixel test
	//expected output:
	//getred:4876
	//getgreen:57623
	//getblue:75
	pixel_rgb48.channel[0] = 19;
	pixel_rgb48.channel[1] = 12;
	pixel_rgb48.channel[2] = 225;
	pixel_rgb48.channel[3] = 23;
	pixel_rgb48.channel[4] = 0;
	pixel_rgb48.channel[5] = 75;
	pixel_rgb48.channel[6] = 0;
	pixel_rgb48.channel[7] = 0;
	data_rgb48.colorspace = 1;
	if(getRed(pixel_rgb48, data_rgb48) == 4876)
	{
		printf("rgb48 getred passed\n");
		//test passed
		
	}else{
		printf("rgb48 getred failed\n");
		//test failed
		
	}
	if(getGreen(pixel_rgb48, data_rgb48) == 57623)
	{
		printf("rgb48 getgreen passed\n");
		//test passed
		
	}else{
		printf("rgb48 getgreen failed\n");
		//test failed
		
	}
	if(getBlue(pixel_rgb48, data_rgb48) == 75)
	{
		printf("rgb48 getblue passed\n");
		//test passed
		
	}else{
		printf("rgb48 getblue failed\n");
		//test failed
		
	}
	
	
	//yuv24 colorspace pixel test
	//expected output:
	//getred: 65205
	//getgreen: 13528
	//getblue: 44479
	//input:
	//y = 127 (32512)
	//u = 23 (5888)
	//v = 112 (28672)
	pixel_yuv24.channel[0] = 127;
	pixel_yuv24.channel[1] = 23;
	pixel_yuv24.channel[2] = 112;
	data_yuv24.colorspace = 4;
	if(getRed(pixel_yuv24, data_yuv24) == 65205)
	{
		printf("yuv24 getred passed\n");
		//test passed
	}else{
		printf("yuv24 getred failed\n");
		//test failed
	}
	if(getGreen(pixel_yuv24, data_yuv24) == 13528)
	{
		printf("yuv24 getgreen passed\n");
		//test passed
	}else{
		printf("yuv24 getgreen failed\n");
		//test failed
	}
	if(getBlue(pixel_yuv24, data_yuv24) == 44479)
	{
		printf("yuv24 getblue passed\n");
		//test passed
	}else{
		printf("yuv24 getblue failed\n");
		//test failed
	}
	
	
	//yuv48 colorspace pixel test
	//expected output:
	//getred: 34400
	//getgreen: 8834
	//getblue: 40183
	//input:
	//y = 20055
	//u = 9903
	//v = 12581
	pixel_yuv48.channel[0] = 78;
	pixel_yuv48.channel[1] = 87;
	pixel_yuv48.channel[2] = 38;
	pixel_yuv48.channel[3] = 175;
	pixel_yuv48.channel[4] = 49;
	pixel_yuv48.channel[5] = 37;
	data_yuv48.colorspace = 3;
	if(getRed(pixel_yuv48, data_yuv48) == 34400)
	{
		printf("yuv48 getred passed\n");
		//test passed
	}else{
		printf("yuv48 getred failed\n");
		//test failed
	}
	if(getGreen(pixel_yuv48, data_yuv48) == 8834)
	{
		printf("yuv48 getgreen passed\n");
		//test passed
	}else{
		printf("yuv48 getgreen failed\n");
		//test failed
	}
	if(getBlue(pixel_yuv48, data_yuv48) == 40183)
	{
		printf("yuv48 getblue passed\n");
		//test passed
	}else{
		printf("yuv48 getblue failed\n");
		//test failed
	}
	
	//yuv48 colorspace outlier pixel test
	//expected output:
	//getred: 34400
	//getgreen: 8834
	//getblue: 40183
	//input:
	//y = 20055
	//u = 59903
	//v = 62581
	pixel_yuv48.channel[0] = 78;
	pixel_yuv48.channel[1] = 87;
	pixel_yuv48.channel[2] = 38;
	pixel_yuv48.channel[3] = 175;
	pixel_yuv48.channel[4] = 49;
	pixel_yuv48.channel[5] = 37;
	data_yuv48.colorspace = 3;
	if(getRed(pixel_yuv48, data_yuv48) == 34400)
	{
		printf("yuv48 getred passed\n");
		//test passed
	}else{
		printf("yuv48 getred failed\n");
		//test failed
	}
	if(getGreen(pixel_yuv48, data_yuv48) == 8834)
	{
		printf("yuv48 getgreen passed\n");
		//test passed
	}else{
		printf("yuv48 getgreen failed\n");
		//test failed
	}
	if(getBlue(pixel_yuv48, data_yuv48) == 40183)
	{
		printf("yuv48 getblue passed\n");
		//test passed
	}else{
		printf("yuv48 getblue failed\n");
		//test failed
	}
}