// This function -Hopefully- takes a stream of images and and encodes them into a video

#include <x264.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	x264_param_t param;
	x264_param_default_preset(&param, "veryfast", "zerolatency");
	param.i_threads = 1;
	param.i_width = width;
	param.i_height = height;
	param.i_fps_num = fps;
	param.i_fps_den = 1;
	
	// Intra refres:
	param.i_keyint_max = fps;
	param.b_intra_refresh = 1;
	
	// Rate control:
	param.rc.i_rc_method = X264_RC_CRF;
	param.rc.f_rf_constant = 25;
	param.rc.f_rf_constant_max = 35;
	
	// For streaming:
	param.b_repeat_headers = 1;
	param.b_annexb = 1;
	x264_param_apply_profile(&param, "baseline");

	// Initialize Encoder
	x264_t *encoder = x264_encoder_open(&param);
	x264_picture_t pic_in, pic_out;
	x264_picture_alloc(&pic_in, X264_CSP_I420, w, h)
	
	struct SwsContext* convertCtx = sws_getContext(in_w, in_h, PIX_FMT_RGB24, out_w, out_h, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	for(i=0; i<100; i++) {
		int srcstride = w*3;
		sws_scale(convertCtx, &data, &srcstride, 0, h, pic_in.img.plane, pic_in.img.stride);
		x264_nal_t* nals;
		int i_nals;
		int frame_size = x264_encoder_encode(encoder, &nals, &i_nals, &pic_in, &pic_out);
		if (frame_size >= 0) {
	    	// OK
		}
	}
}
