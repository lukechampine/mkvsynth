#include "datatypes.h"

// Need something (struct? string?) that will help establish settings
int initializeEncoder(DecodeContext *decodeContext, EncodeContext *encodeContext, int finalWidth, int finalHeight) {
	x264_param_default_preset(&encodeContext->param, "fast", "");
	encodeContext->param.i_width = finalWidth;
	encodeContext->param.i_height = finalHeight;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 0;
	x264_param_apply_profile(&encodeContext->param, "high444");

	encodeContext->encoder = x264_encoder_open(&encodeContext->param);
	x264_encoder_parameters(encodeContext->encoder, &encodeContext->param);

	x264_picture_alloc(&encodeContext->picIn, X264_CSP_I420, decodeContext->codecContext->width, decodeContext->codecContext->height);
	return 1;
}

int encodeFrame(DecodeContext *decodeContext, EncodeContext *encodeContext) {
	struct SwsContext *finalResize = NULL;
	finalResize = sws_getContext (
		decodeContext->codecContext->width,
		decodeContext->codecContext->height,
		decodeContext->codecContext->pix_fmt,
		encodeContext->param.i_width,
		encodeContext->param.i_height,
		PIX_FMT_YUV420P,
		SWS_SPLINE,
		NULL,
		NULL,
		NULL
	);

	sws_scale (
		finalResize,
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
