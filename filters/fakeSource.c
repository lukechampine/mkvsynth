#include "../delbrot/delbrot.h"
#include <stdio.h>

/* standard function definition */
MkvsynthOutput *fakeSource() {
		MkvsynthOutput *test = malloc(sizeof(MkvsynthOutput));
		test->outputBreadth = 25;
		printf("Made an ouptut and hid a value!\n");
    return test;
}

/* ASTnode function definition */
ASTnode* fakeSource_AST(ASTnode *p, ASTnode *args) {
    /* check that (mandatory) arguments are valid */
    checkArgs("fakeSource", args, 1, typeStr);

		char* string = MANDSTR();
		
		printf("got string %s\n", string);

    /* pass arguments to standard definition */
    MkvsynthOutput *result = fakeSource();

    /* return value */
		RETURNCLIP(result);
}
