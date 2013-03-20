#include <x264.h>

// All information needed to encode a stream
typedef struct EncodeContext {
	x264_param_t param;
	x264_t *encoder;
	x264_picture_t picIn;
	x264_picture_t picOut;
	x264_nal_t *nals;

	int frameSize;
	int i_nals;
};

// -Hopefully- takes any chain of frames and encodes them
int initializeEncoder(struct EncodeContext *encodeContext, struct DecodeContext *decodeContext) {
	// Set the parameters for the encode
	x264_param_default_preset(&encodeContext->param, "slow", "");
	encodeContext->param.i_threads = 4;
	encodeContext->param.i_width = decodeContext->codecContext->width;
	encodeContext->param.i_height = decodeContext->codecContext->height;
	encodeContext->param.i_fps_den = 1;
	encodeContext->param.i_keyint_max = 24;
	encodeContext->param.b_intra_refresh = 1;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 18;
	x264_param_apply_profile(&encodeContext->param, "baseline");
	
	encodeContext->encoder = x264_encoder_open(&encodeContext->param);
	x264_encoder_parameters(encodeContext->encoder, &encodeContext->param);

	x264_picture_alloc(&encodeContext->picIn, X264_CSP_I420, decodeContext->codecContext->width, decodeContext->codecContext->height);
	return 1;
}

