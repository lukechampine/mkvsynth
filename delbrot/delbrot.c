#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

/* assign a variable */
ASTnode *assign(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    /* existing variable */
    if (c1->type == typeVar)
        return memcpy(c1->varPtr->value, c2, sizeof(ASTnode));
    /* new variable */
    else if (c1->type == typeId) {
        c1->type = typeVar;
        c1->varPtr = putVar(c1->id);
        return memcpy(c1->varPtr->value, c2, sizeof(ASTnode));
    }
    else
        yyerror("can't assign to a constant value");
}

/* dereference a variable */
ASTnode *dereference(ASTnode *p) {
    if (p->type != typeVar)
        yyerror("dereference called on non-variable");
    
    memcpy(p, p->varPtr->value, sizeof(ASTnode));
    return p;
}

/* handle function calls */
ASTnode *fnctCall(ASTnode *p, ASTnode *node, ASTnode *args) {
    char errorMsg[128];
    if (node->type == typeId) {
        sprintf(errorMsg, "reference to undefined function \"%s\"", node->id);
        yyerror(errorMsg);
    }
    if (node->type != typeFn)
        yyerror("expected function name before '('");

    p = (*(node->fnPtr))(p, args);
    return p;
}

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

    switch(n->type) {
        case typeVar: return dereference(p);
        case typeOp:
            p->type = typeVal; /* this is almost always the case. For special cases it can be redefined. */
            switch(n->op.oper) {
                /* keywords */
                case IF:    if (ex(child[0])->val) ex(child[1]); else if (p->op.nops > 2) ex(child[2]); return NULL;
                case WHILE: while (ex(child[0])->val) ex(child[1]); return NULL;
                /* functions */
                case FNCT:  return fnctCall(p, child[0], ex(child[1]));
                /* assignment */
                case '=':   return assign(p, child[0], ex(child[1])); 
                case ADDEQ: return modvar(p, child[0], '+', ex(child[1])->val);
                case SUBEQ: return modvar(p, child[0], '-', ex(child[1])->val);
                case MULEQ: return modvar(p, child[0], '*', ex(child[1])->val);
                case DIVEQ: return modvar(p, child[0], '/', ex(child[1])->val);
                case MODEQ: return modvar(p, child[0], '%', ex(child[1])->val);
                /* arithmetic operators */
                case '%':   p->val = (int) ex(child[0])->val % (int) ex(child[1])->val; return p;
                case '^':   p->val = pow(ex(child[0])->val, ex(child[1])->val); return p;
                case '*':   p->val = ex(child[0])->val * ex(child[1])->val; return p;
                case '/':   p->val = ex(child[0])->val / ex(child[1])->val; return p;
                case '+':   p->val = ex(child[0])->val + ex(child[1])->val; return p;
                case '-':   p->val = ex(child[0])->val - ex(child[1])->val; return p;
                case NEG:   p->val = -(ex(child[0])->val);                  return p;
                case INC:   return modvar(p, child[0], '+', 1);
                case DEC:   return modvar(p, child[0], '-', 1);
                /* boolean operators */
                case '!':   p->val = !ex(child[0])->val;                     return p;
                case '>':   p->val = ex(child[0])->val > ex(child[1])->val;  return p;
                case '<':   p->val = ex(child[0])->val < ex(child[1])->val;  return p;
                case GE:    p->val = ex(child[0])->val >= ex(child[1])->val; return p;
                case LE:    p->val = ex(child[0])->val <= ex(child[1])->val; return p;
                case EQ:    p->val = ex(child[0])->val == ex(child[1])->val; return p;
                case NE:    p->val = ex(child[0])->val != ex(child[1])->val; return p;
                case LOR:   p->val = ex(child[0])->val || ex(child[1])->val; return p;
                case LAND:  p->val = ex(child[0])->val && ex(child[1])->val; return p;
                /* misc operations */
                case ';':   ex(child[0]); p = ex(child[1]); return p;  
            }
        default: return p;
    }
    return NULL;
}

/* helper function to ensure that a function call is valid. Also calls ex() on each argument */
/* TODO: use ... to allow type checking */
ASTnode* checkArgs(char *funcName, ASTnode *args, int numArgs) {
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
    if (traverse != NULL && traverse->type != typeParam) {
        while ((traverse = traverse->next) != NULL) i++;
        sprintf(errorMsg, "%s expected %d arguments, got %d", funcName, numArgs, ++i);
        yyerror(errorMsg);
    }
    /* evaluate each argument */
    /* gross, fix this */
    root = ex(root);
    traverse = root->next;
    for (i = 1; i < numArgs; i++) {
        traverse = ex(traverse);
        if (traverse->type == typeId) {
            sprintf(errorMsg, "reference to uninitialized variable %s", args->id);
            yyerror(errorMsg);
        }
        traverse = traverse->next;
    }
    return root;
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nsin (ASTnode *p, ASTnode *args) { checkArgs("sin", args, 1); p->val = sin(args->val);  return p; }
ASTnode* ncos (ASTnode *p, ASTnode *args) { checkArgs("cos", args, 1); p->val = cos(args->val);  return p; }
ASTnode* nlog (ASTnode *p, ASTnode *args) { checkArgs("log", args, 1); p->val = log(args->val);  return p; }
ASTnode* nsqrt(ASTnode *p, ASTnode *args) { checkArgs("sqrt",args, 1); p->val = sqrt(args->val); return p; }

/* modify the value of a variable */
ASTnode* modvar(ASTnode *p, ASTnode *c1, char op, double mod) {
    if(c1->type == typeId) {
        char errorMsg[128];
        sprintf(errorMsg, "reference to uninitialized variable %s", p->id);
        yyerror(errorMsg);
    }
    if(c1->type != typeVar)
        yyerror("can't modify constant");
    if(c1->varPtr->value->type != typeVal)
        yyerror("can't modify non-numeric variable");

    switch (op) {
        case '+': c1->varPtr->value->val += mod; break;
        case '-': c1->varPtr->value->val -= mod; break;
        case '*': c1->varPtr->value->val *= mod; break;
        case '/': c1->varPtr->value->val /= mod; break;
        case '%': c1->varPtr->value->val = (double)((int)c1->varPtr->value->val % (int)mod); break;
    }
    p = ex(c1);
    return p;
}

/* helper function to get optional arguments in a function call */
ASTnode* getOptArg(ASTnode *args, char *name) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next)
        if (traverse->type == typeParam && !(strcmp(traverse->varPtr->name,name)))
            return traverse->varPtr->value;
    return NULL;
}

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
        args = ex(args);
        if (args->type == typeId) {
            char errorMsg[128];
            sprintf(errorMsg, "reference to uninitialized variable %s", args->id);
            yyerror(errorMsg);
        }
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

void ffmpegDecodeFinal(char *filename, int numFrames) {
    if (numFrames != -1)
        printf("decoded %d frames of %s\n", numFrames, filename);
    else
        printf("decoded %s\n", filename);
}

/* ffmpeg decoding function, showcasing optional arguments */
ASTnode* ffmpegDecode(ASTnode *p, ASTnode *args) {
    // check that (mandatory) arguments are valid
    args = checkArgs("ffmpegDecode", args, 1);
    // get arguments
    char *str = args->str;
    double frames = getOptArg(args, "frames")
                  ? ex(getOptArg(args, "frames"))->val
                  : -1; /* default value */
    // main function body
    ffmpegDecodeFinal(str, (int) frames);
    // return value
    p->type = typeVal;
    p->val = 0;
    return p;
}