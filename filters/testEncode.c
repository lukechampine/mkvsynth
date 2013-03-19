// This function -Hopefully- takes a stream of images and and encodes them into a video

#include <libswscale/swscale.h>
#include <stdint.h>
#include <inttypes.h>
#include <x264.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	x264_param_t param;
	x264_param_default_preset(&param, "veryfast", "zerolatency");
	param.i_threads = 1;
	param.i_width = 100;
	param.i_height = 250;
	param.i_fps_num = 24;
	param.i_fps_den = 1;
	
	// Intra refres:
	param.i_keyint_max = 24;
	param.b_intra_refresh = 1;
	
	// Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 25;
	param.rc.f_rf_constant_max = 35;
	
	// For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;
	x264_param_apply_profile(&param, "baseline");
	
	int w = 100;
	int h = 250;
	// Initialize Encoder
	x264_t *encoder = x264_encoder_open(&param);
	x264_picture_t pic_in, pic_out;
	x264_picture_alloc(&pic_in, X264_CSP_I420, w, h);
	
	// Resize parameters
	int in_w = 100;
	int in_h = 250;
	int out_w = 100;
	int out_h = 250;

	// Instead of doing this silly thing, data should actually point to raw image data
	// But I wasn't sure how to load the image data
	const uint8_t * const data = (const uint8_t * const)1100;
	struct SwsContext* convertCtx = sws_getContext(in_w, in_h, PIX_FMT_RGB24, out_w, out_h, PIX_FMT_YUV420P, SWS_SPLINE, NULL, NULL, NULL);

	int i;
	for(i=0; i<100; i++) {
		int srcstride = w*3;
		sws_scale(convertCtx, &data, &srcstride, 0, in_h, pic_in.img.plane, pic_in.img.i_stride);
		x264_nal_t* nals;
		int i_nals;
		int frame_size = x264_encoder_encode(encoder, &nals, &i_nals, &pic_in, &pic_out);
		if (frame_size >= 0) {
	    	// OK
		}
	}

	return 1;
}
