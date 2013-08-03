#include "delbrot.h"

ASTnode* gradientVideoGenerate_AST(ASTnode *p, ASTnode *args);
ASTnode* writeRawFile_AST(ASTnode *p, ASTnode *args);
ASTnode* go_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	{ "gradientVideoGenerate", gradientVideoGenerate_AST },
	{ "writeRawFile",          writeRawFile_AST          },
	{ "go",                    go_AST                    },
	{ 0,                       0                         },
};
