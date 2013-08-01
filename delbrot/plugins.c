#include "delbrot.h"

ASTnode* myPlugin_AST(ASTnode *p, ASTnode *args);

fnEntry pluginFunctions[] = {
	"myPlugin", myPlugin_AST,
	0, 0
};