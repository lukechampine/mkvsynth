#include "datatypes.h"

// Need something (struct? string?) that will help establish settings
EncodeContext* openEncoder(int width, int height) {
	EncodeContext *encodeContext = malloc(sizeof(EncodeContext));
	x264_param_default_preset(&encodeContext->param, "fast", "");
	encodeContext->param.i_width = width;
	encodeContext->param.i_height = height;
	encodeContext->param.rc.i_rc_method = X264_RC_CRF;
	encodeContext->param.rc.f_rf_constant = 20;
	x264_param_apply_profile(&encodeContext->param, "high444");

	encodeContext->encoder = x264_encoder_open(&encodeContext->param);
	x264_encoder_parameters(encodeContext->encoder, &encodeContext->param);

	x264_picture_alloc(&encodeContext->picIn, X264_CSP_I420, width, height);
	return encodeContext;
}

void encodeFrame(AVFrame *frame, EncodeContext *encodeContext) {
	struct SwsContext *finalResize = NULL;
	finalResize = sws_getContext (
		frame->width,
		frame->height,
		frame->format,
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
		(uint8_t const * const *)frame->data,
		frame->linesize,
		0,
		frame->height,
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
}

int writeEncodeToFile(FILE *output, EncodeContext *encodeContext) {
	int j;
	int fwriteStatus = 0;

	if(encodeContext->frameSize >= 0) {
		for(j = 0; j < encodeContext->i_nals; j++) {
			fwriteStatus =
				fwrite(
					encodeContext->nals[j].p_payload,
					encodeContext->nals[j].i_payload,
					1,
					output
				);
			if(fwriteStatus < 0)
				return fwriteStatus;
		}
	}
	return fwriteStatus;
}
