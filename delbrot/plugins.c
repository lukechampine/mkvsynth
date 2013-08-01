#include "delbrot.h"

ASTnode* fakeFilter_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	"fakeFilter", fakeFilter_AST,
	0, 0
};