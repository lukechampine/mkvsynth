#include <stdio.h>
#include <math.h>
#include "delbrot.h"

/* helper function to ensure that function arguments exist and have the right type */
/* TODO: use ... to allow checking of any number of arguments */
int checkArgs(char *funcName, argNode *args, int numArgs) {
    char errorMsg[128];
    int i = 0;
    /* check for missing arguments */
    for (i = 0; i < numArgs; i++) {
        if (args == NULL) {
            sprintf(errorMsg, "%s expected %d arguments, got %d", funcName, numArgs, i);
            yyerror(errorMsg);
            break;
        }
        args = args->next;
    }
    /* check for excess arguments */
    if (args != NULL) {
        while ((args = args->next) != NULL) i++;
        sprintf(errorMsg, "%s expected %d arguments, got %d", funcName, numArgs, ++i);
        yyerror(errorMsg);
    }
}

/* standard mathematical functions, modified to use argNode */
double nsin(argNode *args)  { checkArgs("sin", args, 1); return sin(args->dValue);  }
double ncos(argNode *args)  { checkArgs("cos", args, 1); return cos(args->dValue);  }
double nlog(argNode *args)  { checkArgs("log", args, 1); return log(args->dValue);  }
double nexp(argNode *args)  { checkArgs("exp", args, 1); return exp(args->dValue);  }
double nsqrt(argNode *args) { checkArgs("sqrt",args, 1); return sqrt(args->dValue); }
double npow(double x, double y) { return pow(x, y); }

/* ffmpeg decoding function */
double ffmpegDecode(argNode *args) {
    // get arguments
    checkArgs("ffmpegDecode", args, 3);
    double x = args->dValue;
    double y = args->next->dValue;
    double z = args->next->next->dValue;

    return sqrt(x*x + y*y + z*z);
}

double print(argNode *args) {
    // get argument
    checkArgs("print", args, 1);
    double num = args->dValue;

    printf ("%.10g\n", num);
    return 0;
}

/* x264 encoding function */
void x264Encode(int numFrames) {
    printf("Processed %d frames\n", numFrames);
}