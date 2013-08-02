#include "delbrot.h"

ASTnode* fakeSource_AST(ASTnode *p, ASTnode *args);
ASTnode* fakeFilter_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	"fakeFilter", fakeFilter_AST,
	"fakeSource", fakeSource_AST,
	0, 0
};
