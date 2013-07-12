#include "delbrot.h"
#include <stdio.h>

/* standard function definition */
void myPlugin(char *filename, int numFrames, char *output) {
    if (numFrames != -1)
    	if (output)
        	printf("applied filter to %d frames of %s and saved result in %s\n", numFrames, filename, output);
        else
        	printf("applied filter to %d frames of %s\n", numFrames, filename);
    else
    	if (output)
    		printf("applied filter to %s and saved result in %s\n", filename, output);
    	else
        	printf("applied filter to %s\n", filename);
}

/* ASTnode function definition */
ASTnode* myPlugin_AST(ASTnode *p, ASTnode *args) {
    /* check that (mandatory) arguments are valid */
    checkArgs("myPlugin", args, 1);

    /* get arguments */
    char *str = args->str;
    /* get optional arguments */
    double frames = OPTVAL("frames", -1);
    char *output  = OPTSTR("output", NULL);

    /* pass arguments to standard definition */
    /* this isn't stricly necessary, but some people may prefer to do things this way */
    myPlugin(str, frames, output);

    /* return value */
    p->type = typeVal;
    p->val = 0;
    return p;
}