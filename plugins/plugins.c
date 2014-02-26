#include <stdarg.h>
#include "../delbrot/delbrot.h"

static char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "variable", "optional argument", "function", "operation"};
int linenumber = 2;

/* display error message in red and exit */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}

/* ensure that a function call is valid */
/* TODO: check optional arguments */
void checkArgs(char *funcName, ASTnode *args, int numArgs, ...) {
    int i;
    va_list ap;
    va_start(ap, numArgs);
    ASTnode *traverse = args;
    /* check for missing/uninitialized/mistyped arguments */
    for (i = 0; i < numArgs; i++, traverse = traverse->next) {
        if (traverse == NULL)
            MkvsynthError("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), i);
        /* check type */
        int argType = va_arg(ap, int);
        if (traverse->type != argType)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[traverse->type]);
    }
    va_end(ap);
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeOptArg) {
        while ((traverse = traverse->next) != NULL) i++;
        MkvsynthError("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), ++i);
    }
}