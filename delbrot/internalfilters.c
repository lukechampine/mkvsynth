#include "delbrot.h"

ASTnode* bilinearResize_AST(ASTnode *p, ASTnode *args);
ASTnode* colorspacingTests_AST(ASTnode *p, ASTnode *args);
ASTnode* convertColorspace_AST(ASTnode *p, ASTnode *args);
ASTnode* crop_AST(ASTnode *p, ASTnode *args);
ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args);
ASTnode* go_AST(ASTnode *, ASTnode *);
ASTnode* gradientVideoGenerate_AST(ASTnode *p, ASTnode *args);
ASTnode* removeRange_AST(ASTnode *p, ASTnode *args);
ASTnode* testingGradient_AST(ASTnode *p, ASTnode *args);
ASTnode* writeRawFile_AST(ASTnode *p, ASTnode *args);
ASTnode* x264Encode_AST(ASTnode *p, ASTnode *args);

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
