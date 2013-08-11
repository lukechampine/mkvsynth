#include "delbrot.h"

ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args);
ASTnode* gradientVideoGenerate_AST(ASTnode *p, ASTnode *args);
ASTnode* crop_AST(ASTnode *p, ASTnode *args);
ASTnode* removeRange_AST(ASTnode *p, ASTnode *args);
ASTnode* writeRawFile_AST(ASTnode *p, ASTnode *args);
ASTnode* x264Encode_AST(ASTnode *p, ASTnode *args);
ASTnode* go_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	{ "ffmpegDecode",          ffmpegDecode_AST          },
	{ "gradientVideoGenerate", gradientVideoGenerate_AST },
	{ "crop",                  crop_AST                  },
	{ "removeRange",           removeRange_AST           },
	{ "writeRawFile",          writeRawFile_AST          },
	{ "x264Encode",            x264Encode_AST            },
	{ "go",                    go_AST                    },
	{ 0,                       0                         },
};
