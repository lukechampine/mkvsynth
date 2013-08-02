#include "../delbrot/delbrot.h"
#include <stdio.h>

/* standard function definition */
MkvsynthOutput *fakeFilter(MkvsynthOutput *input) {
	printf("secret value revealed: %i\n", input->outputBreadth);
}

/* ASTnode function definition */
ASTnode* fakeFilter_AST(ASTnode *p, ASTnode *args) {
    /* check that (mandatory) arguments are valid */
    checkArgs("fakeFilter", args, 1, typeClip);

    /* get arguments */
    MkvsynthOutput *input = MANDCLIP();

    /* pass arguments to standard definition */
    fakeFilter(input);
}
