#include "delbrot.h"

Value* bilinearResize_AST(argList *);
Value* colorspacingTests_AST(argList *);
Value* convertColorspace_AST(argList *);
Value* crop_AST(argList *);
Value* ffmpegDecode_AST(argList *);
Value* go_AST(argList *);
Value* gradientVideoGenerate_AST(argList *);
Value* removeRange_AST(argList *);
Value* testingGradient_AST(argList *);
Value* writeRawFile_AST(argList *);
Value* x264Encode_AST(argList *);

Fn internalFilters[] = {
#ifndef DELBROT
	{ fnCore, "bilinearResize",        bilinearResize_AST,        NULL, NULL, NULL },
	{ fnCore, "colorspacingTests",     colorspacingTests_AST,     NULL, NULL, NULL },
	{ fnCore, "convertColorspace",     convertColorspace_AST,     NULL, NULL, NULL },
	{ fnCore, "crop",                  crop_AST,                  NULL, NULL, NULL },
	{ fnCore, "ffmpegDecode",          ffmpegDecode_AST,          NULL, NULL, NULL },
	{ fnCore, "go",                    go_AST,                    NULL, NULL, NULL },
	{ fnCore, "gradientVideoGenerate", gradientVideoGenerate_AST, NULL, NULL, NULL },
	{ fnCore, "removeRange",           removeRange_AST,           NULL, NULL, NULL },
	{ fnCore, "testingGradient",       testingGradient_AST,       NULL, NULL, NULL },
	{ fnCore, "writeRawFile",          writeRawFile_AST,          NULL, NULL, NULL },
	{ fnCore, "x264Encode",            x264Encode_AST,            NULL, NULL, NULL },
	{ 0,      0,                       0,                         0,    0,    0    },
#endif
};
