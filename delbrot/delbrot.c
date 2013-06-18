#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

/* execute a section of the AST */
ASTnode* ex(ASTnode *n) {
    if (!n)
        return NULL;

    /* allocate new node to return */
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    memcpy(p, n, sizeof(ASTnode));

    /* for convenience/readability */
    ASTnode **child = p->op.ops;

    switch(p->type) {
        case typeVal: return p;
        case typeVar: p->val = p->var->val; return p; // dereference variable
        case typeOp:
            switch(p->op.oper) {
                /* keywords */
                case WHILE: while (ex(child[0])->val) ex(child[1]); return p;
                case IF:    if    (ex(child[0])->val) ex(child[1]); return p;
                /* functions */
                case FNCT:  return (*((child[0])->var->fnPtr))(p, child[1]);
                /* special syntax */
                case '=':   p->val = child[0]->var->val = ex(child[1])->val; p->type = typeVal; return p;
                case ';':   ex(child[0]); p->val = ex(child[1])->val;        p->type = typeVal; return p;                
                case NEG:   p->val = -(ex(child[0])->val);                   p->type = typeVal; return p;
                /* standard mathematical functions */
                case '+':   return nadd(p, ex(child[0]), ex(child[1]));
                case '-':   return nsub(p, ex(child[0]), ex(child[1]));
                case '*':   return nmul(p, ex(child[0]), ex(child[1]));
                case '/':   return ndiv(p, ex(child[0]), ex(child[1]));
                case '^':   return nexp(p, ex(child[0]), ex(child[1]));
                case '>':   return ngtr(p, ex(child[0]), ex(child[1]));
                case '<':   return nles(p, ex(child[0]), ex(child[1]));
                case GE:    return ngte(p, ex(child[0]), ex(child[1]));
                case LE:    return nlte(p, ex(child[0]), ex(child[1]));
                case EQ:    return neql(p, ex(child[0]), ex(child[1]));
                case NE:    return nneq(p, ex(child[0]), ex(child[1]));
            }
    }
    return NULL;
}

/* helper function to ensure that a function call is valid. Also calls ex() on each argument */
/* TODO: use ... to allow checking of any number of arguments */
void checkArgs(char *funcName, ASTnode *args, int numArgs) {
    char errorMsg[128];
    int i;
    ASTnode *root = args;
    ASTnode *traverse = args;
    /* check for missing arguments */
    for (i = 0; i < numArgs; i++) {
        if (traverse == NULL) {
            sprintf(errorMsg, "%s expected %d arguments, got %d", funcName, numArgs, i);
            yyerror(errorMsg);
            break;
        }
        traverse = traverse->next;
    }
    /* check for excess arguments */
    if (traverse != NULL) {
        while ((traverse = traverse->next) != NULL) i++;
        sprintf(errorMsg, "%s expected %d arguments, got %d", funcName, numArgs, ++i);
        yyerror(errorMsg);
    }
    /* evaluate each argument */
    traverse = root;
    for (i = 0; i < numArgs; i++) {
        traverse = ex(traverse);
        traverse = traverse->next;
    }
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nadd (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val + c2->val; p->type = typeVal; return p; }
ASTnode* nsub (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val - c2->val; p->type = typeVal; return p; }
ASTnode* nmul (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val * c2->val; p->type = typeVal; return p; }
ASTnode* ndiv (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val / c2->val; p->type = typeVal; return p; }
ASTnode* nexp (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = pow(c1->val,c2->val);p->type=typeVal; return p; }
ASTnode* ngtr (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val > c2->val; p->type = typeVal; return p; }
ASTnode* nles (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val < c2->val; p->type = typeVal; return p; }
ASTnode* ngte (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val >=c2->val; p->type = typeVal; return p; }
ASTnode* nlte (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val <=c2->val; p->type = typeVal; return p; }
ASTnode* neql (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val ==c2->val; p->type = typeVal; return p; }
ASTnode* nneq (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val !=c2->val; p->type = typeVal; return p; }
ASTnode* nsin (ASTnode *p, ASTnode *args) { checkArgs("sin", args, 1); p->val = sin(args->val); p->type = typeVal;  return p;                 }
ASTnode* ncos (ASTnode *p, ASTnode *args) { checkArgs("cos", args, 1); p->val = cos(args->val); p->type = typeVal;  return p;                 }
ASTnode* nlog (ASTnode *p, ASTnode *args) { checkArgs("log", args, 1); p->val = log(args->val); p->type = typeVal;  return p;                 }
ASTnode* nsqrt(ASTnode *p, ASTnode *args) { checkArgs("sqrt",args, 1); p->val = sqrt(args->val); p->type = typeVal; return p;                 }
ASTnode* npow (ASTnode *p, ASTnode *args) { checkArgs("pow", args, 2); p->val = pow(args->val, args->next->val); p->type = typeVal; return p; }

/* generalized print function, will print any number of args */
ASTnode* print(ASTnode *p, ASTnode *args) {
    printf ("\t");
    /* iterate through args */
    double num;
    while(args) {
        num = ex(args)->val;
        printf ("%.10g ", num);
        args = args->next;
    }
    printf ("\n");
    return p;
}

/* ffmpeg decoding function */
ASTnode* ffmpegDecode(ASTnode *p, ASTnode *args) {
    // get arguments
    checkArgs("ffmpegDecode", args, 3);
    double x = args->val;
    double y = args->next->val;
    double z = args->next->next->val;

    p->val = sqrt(x*x + y*y + z*z);
    p->type = typeVal;
    return p;
}