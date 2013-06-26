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
    p->type = typeVal; /* this is almost always the case. For special cases it can be redefined. */

    /* for convenience/readability */
    ASTnode **child = p->op.ops;

    switch(n->type) {
        case typeVal: return p;
        case typeStr: p->type = typeStr; return p;
        case typeVar: p = p->varPtr->value; p->next = n->next; return p;
        case typeOp:
            switch(n->op.oper) {
                /* keywords */
                case IF:    if (ex(child[0])->val) ex(child[1]); else if (p->op.nops > 2) ex(child[2]); return NULL;
                case WHILE: while (ex(child[0])->val) ex(child[1]); return NULL;
                /* functions */
                case FNCT:  return (*((child[0])->fnPtr))(p, ex(child[1]));
                /* special syntax */
                case '=':   p = child[0]->varPtr->value = ex(child[1]); return p;
                case ';':   ex(child[0]); p = ex(child[1]);   return p;
                case NEG:   p->val = -(ex(child[0])->val);    return p;
                case INC:   return ninc(p, child[0]);
                case DEC:   return ndec(p, child[0]);
                /* standard mathematical functions */
                case '+':   return nadd(p, ex(child[0]), ex(child[1]));
                case '-':   return nsub(p, ex(child[0]), ex(child[1]));
                case '*':   return nmul(p, ex(child[0]), ex(child[1]));
                case '/':   return ndiv(p, ex(child[0]), ex(child[1]));
                case '^':   return npow(p, ex(child[0]), ex(child[1]));
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
/* TODO: use ... to allow type checking */
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
ASTnode* nadd (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val + c2->val;  return p; }
ASTnode* nsub (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val - c2->val;  return p; }
ASTnode* nmul (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val * c2->val;  return p; }
ASTnode* ndiv (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val / c2->val;  return p; }
ASTnode* npow (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val =pow(c1->val,c2->val);return p; }
ASTnode* ngtr (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val > c2->val;  return p; }
ASTnode* nles (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val < c2->val;  return p; }
ASTnode* ngte (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val >=c2->val;  return p; }
ASTnode* nlte (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val <=c2->val;  return p; }
ASTnode* neql (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val ==c2->val;  return p; }
ASTnode* nneq (ASTnode *p, ASTnode *c1, ASTnode *c2) { p->val = c1->val !=c2->val;  return p; }
/* these have to use checkArgs() because they are called like ordinary functions */
ASTnode* nsin (ASTnode *p, ASTnode *args) { checkArgs("sin", args, 1); p->val = sin(args->val);  return p; }
ASTnode* ncos (ASTnode *p, ASTnode *args) { checkArgs("cos", args, 1); p->val = cos(args->val);  return p; }
ASTnode* nlog (ASTnode *p, ASTnode *args) { checkArgs("log", args, 1); p->val = log(args->val);  return p; }
ASTnode* nsqrt(ASTnode *p, ASTnode *args) { checkArgs("sqrt",args, 1); p->val = sqrt(args->val); return p; }

ASTnode* ninc (ASTnode *p, ASTnode *c1) { if(c1->varPtr->value->type != typeVal) yyerror("non-numeric values cannot be incremented");
                                          c1->varPtr->value->val++; ASTnode *next = p->next; p = ex(c1); p->next = next; return p; }
ASTnode* ndec (ASTnode *p, ASTnode *c1) { if(c1->varPtr->value->type != typeVal) yyerror("non-numeric values cannot be incremented");
                                          c1->varPtr->value->val--; ASTnode *next = p->next; p = ex(c1); p->next = next; return p; }

/* helper function to interpret string literals */
char* unesc(char* str) {
    int i, j;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\') {
            switch (str[i+1]) {
                case 't': str[i] = '\t'; break;
                case 'n': str[i] = '\n'; break;
                case 'r': str[i] = '\r'; break;
                case '\\':str[i] = '\\'; break;
                case '\'':str[i] = '\''; break;
                case '\"':str[i] = '\"'; break;
                default: yyerror("unknown literal");
            }
            for (j = i + 1; str[j] != '\0'; j++)
                str[j] = str[j+1];
        }
    }
    return str;
}

/* generalized print function; will print any number of args */
ASTnode* print(ASTnode *p, ASTnode *args) {
    while(args) {
        /* reduce any unevaluated arguments */
        if (args->type == typeVar || args->type == typeOp)
            args = ex(args);
        /* print according to argument type */
        if (args->type == typeVal)
            printf("%.10g ", args->val);
        if (args->type == typeStr)
            printf("%s ", unesc(args->str));

        args = args->next;
    }
    printf("\n");
    return p;
}

/* ffmpeg decoding function */
ASTnode* ffmpegDecode(ASTnode *p, ASTnode *args) {
    // get arguments
    checkArgs("ffmpegDecode", args, 1);
    p->val = printf("\tdecoded %s\n", args->str);
    p->type = typeVal;
    return p;
}