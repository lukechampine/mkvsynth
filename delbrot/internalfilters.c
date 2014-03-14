#include "delbrot.h"

ASTnode* bilinearResize_AST(ASTnode *, argList *);
ASTnode* colorspacingTests_AST(ASTnode *, argList *);
ASTnode* convertColorspace_AST(ASTnode *, argList *);
ASTnode* crop_AST(ASTnode *, argList *);
ASTnode* ffmpegDecode_AST(ASTnode *, argList *);
ASTnode* go_AST(ASTnode *, argList *);
ASTnode* gradientVideoGenerate_AST(ASTnode *, argList *);
ASTnode* removeRange_AST(ASTnode *, argList *);
ASTnode* testingGradient_AST(ASTnode *, argList *);
ASTnode* writeRawFile_AST(ASTnode *, argList *);
ASTnode* x264Encode_AST(ASTnode *, argList *);

fnEntry internalFilters[] = {
#ifndef DELBROT
	{ "bilinearResize",        bilinearResize_AST        },
	{ "colorspacingTests",     colorspacingTests_AST     },
	{ "convertColorspace",     convertColorspace_AST     },
	{ "crop",                  crop_AST                  },
	{ "ffmpegDecode",          ffmpegDecode_AST          },
	{ "go",                    go_AST                    },
	{ "gradientVideoGenerate", gradientVideoGenerate_AST },
	{ "removeRange",           removeRange_AST           },
	{ "testingGradient",       testingGradient_AST       },
	{ "writeRawFile",          writeRawFile_AST          },
	{ "x264Encode",            x264Encode_AST            },
	{ 0,                       0                         },
#endif
};
