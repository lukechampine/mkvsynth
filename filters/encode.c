#include "datatypes.h"

// Need something (struct? string?) that will help establish settings
int initializeEncoder(DecodeContext *decodeContext, EncodeContext *encodeContext) {
	x264_param_default_preset(&encodeContext->param, "slow", "");
	encodeContext->param.i_width = decodeContext->codecContext->width;
	encodeContext->param.i_height = decodeContext->codecContext->height;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 0;
	x264_param_apply_profile(&encodeContext->param, "high444");

	encodeContext->encoder = x264_encoder_open(&encodeContext->param);
	x264_encoder_parameters(encodeContext->encoder, &encodeContext->param);

	x264_picture_alloc(&encodeContext->picIn, X264_CSP_I420, decodeContext->codecContext->width, decodeContext->codecContext->height);
	return 1;
}

// Encode current frame
int encodeFrame(DecodeContext *decodeContext, EncodeContext *encodeContext) {
	encodeContext->param.i_width = decodeContext->codecContext->width;
	encodeContext->param.i_height = decodeContext->codecContext->width;

	struct SwsContext *asdf = NULL;
	asdf = sws_getContext (
		decodeContext->codecContext->width,
		decodeContext->codecContext->height,
		decodeContext->codecContext->pix_fmt,
		//decodeContext->codecContext->width,
		//decodeContext->codecContext->height,
		encodeContext->param.i_width,
		encodeContext->param.i_height,
		PIX_FMT_YUV420P,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL
	);

	sws_scale (
		asdf,
		(uint8_t const * const *)decodeContext->frame->data,
		decodeContext->frame->linesize,
		0,
		decodeContext->codecContext->height,
		encodeContext->picIn.img.plane,
		encodeContext->picIn.img.i_stride
	);
	
	encodeContext->frameSize = x264_encoder_encode(
		encodeContext->encoder,
		&encodeContext->nals,
		&encodeContext->i_nals,
		&encodeContext->picIn,
		&encodeContext->picOut
	);
	return 1;
}
