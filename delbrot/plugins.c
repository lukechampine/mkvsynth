#include "delbrot.h"

ASTnode* myPlugin_AST(ASTnode *p, ASTnode *args);

funcRec pluginFunctions[] = {
	"myPlugin", myPlugin_AST, 0, 0, 0,
	0, 0, 0, 0, 0
};