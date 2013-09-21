#include "delbrot.h"

ASTnode* bilinearResize_AST(ASTnode *p, ASTnode *args);
ASTnode* convertColorspace_AST(ASTnode *p, ASTnode *args);
ASTnode* crop_AST(ASTnode *p, ASTnode *args);
ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args);
ASTnode* gradientVideoGenerate_AST(ASTnode *p, ASTnode *args);
ASTnode* removeRange_AST(ASTnode *p, ASTnode *args);
ASTnode* testingGradient_AST(ASTnode *p, ASTnode *args);
ASTnode* writeRawFile_AST(ASTnode *p, ASTnode *args);
ASTnode* x264Encode_AST(ASTnode *p, ASTnode *args);
ASTnode* go_AST(ASTnode *p, ASTnode *args);
ASTnode* colorspacingTests_AST(ASTnode *p, ASTnode *args);

ASTnode* viewer_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	{ "bilinearResize",        bilinearResize_AST        },
	{ "convertColorspace",     convertColorspace_AST     },
	{ "crop",                  crop_AST                  },
	{ "ffmpegDecode",          ffmpegDecode_AST          },
	{ "gradientVideoGenerate", gradientVideoGenerate_AST },
	{ "removeRange",           removeRange_AST           },
	{ "testingGradient",       testingGradient_AST       },
	{ "writeRawFile",          writeRawFile_AST          },
	{ "x264Encode",            x264Encode_AST            },
	{ "go",                    go_AST                    },
	{ "colorspacingTests",     colorspacingTests_AST     },
	{ "viewer",                viewer_AST                },
	{ 0,                       0                         },
};
