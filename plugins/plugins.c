#include <stdarg.h>
#include "../delbrot/delbrot.h"

char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "operation"};

/* allocate a value */
Value* newValue() {
    Value *v;
    if ((v = malloc(sizeof(Value))) == NULL)
        MkvsynthError("out of memory");
    return v;
}

/* display error and in red and exit */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot: error: ");
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}

/* display message in blue */
void MkvsynthMessage(char *message, ...) {
    fprintf(stderr, "\x1B[36m");
    va_list arglist;
    va_start(arglist, message);
    vfprintf(stdout, message, arglist);
    va_end(arglist);
    fprintf(stdout, "\x1B[0m\n");
}

/* display warning in yellow */
void MkvsynthWarning(char *warning, ...) {
    fprintf(stderr, "\x1B[33mdelbrot: warning: ");
    va_list arglist;
    va_start(arglist, warning);
    vfprintf(stderr, warning, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
}

/* ensure that a function call is valid */
/* TODO: check optional arguments */
void checkArgs(argList *a, int numArgs, ...) {
    /* check number of arguments */
    if ((a ? a->nargs : 0) != numArgs)
        MkvsynthError("expected %d argument%s, got %d", numArgs, (numArgs == 1 ? "" : "s"), a->nargs);
    /* check types */
    int i;
    va_list ap;
    va_start(ap, numArgs);
    for (i = 0; i < numArgs; i++) {
        valueType argType = va_arg(ap, valueType);
        if (a->args[i].value->type != argType)
            MkvsynthError("arg %d expected %s, got %s", i+1, typeNames[argType], typeNames[a->args[i].value->type]);
    }
    va_end(ap);
}