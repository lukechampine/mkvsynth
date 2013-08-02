#include "../delbrot/delbrot.h"
#include <stdio.h>

/* standard function definition */
MkvsynthOutput *fakeFilter(MkvsynthInput *input, int frames) {
    /* do stuff here... */
    return NULL;
}

/* ASTnode function definition */
ASTnode* fakeFilter_AST(ASTnode *p, ASTnode *args) {
    /* check that (mandatory) arguments are valid */
    checkArgs("fakeFilter", args, 1, typeClip);

    /* get arguments */
    MkvsynthInput *input = MANDCLIP();
    /* get optional arguments */
    double frames = OPTVAL("frames", -1);

    /* pass arguments to standard definition */
    MkvsynthOutput *result = fakeFilter(input, frames);

    /* return value */
	RETURNCLIP(result);
}
