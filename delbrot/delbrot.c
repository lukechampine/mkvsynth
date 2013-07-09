#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

/* assign a variable */
ASTnode* assign(ASTnode *ident, ASTnode *valueNode) {
    /* existing variable */
    if (ident->type == typeVar)
        return memcpy(ident->varPtr->value, valueNode, sizeof(ASTnode));

    /* new variable */
    if (ident->type != typeId)
        yyerror("can't assign to a constant value");
    ASTnode *p = newNode();
    p->type = typeVar;
    p->varPtr = putVar(ident->id);
    memcpy(p->varPtr->value, valueNode, sizeof(ASTnode));
    return p;
}

/* dereference a variable */
ASTnode *dereference(ASTnode *p) {
    ASTnode *next = p->next;
    memcpy(p, p->varPtr->value, sizeof(ASTnode));
    p->next = next;
    return p;
}

/* handle function calls */
ASTnode *fnctCall(ASTnode *p, ASTnode *node, ASTnode *args) {
    if (node->type == typeId)
        yyerror("reference to undefined function \"%s\"", node->id);
    if (node->type != typeFn)
        yyerror("expected function name before '('");
    p = (*(node->fnPtr))(p, args);
    return p;
}

/* execute a section of the AST */
ASTnode* ex(ASTnode *p) {
    if (!p)
        return NULL;

    /* for convenience/readability */
    ASTnode **child = p->op.ops;
 
    switch(p->type) {
        case typeVar: return dereference(p);
        case typeOp:
            p->type = typeVal; /* this is almost always the case. For special cases it can be redefined. */
            switch(p->op.oper) {
                /* declarations */
                case FNDEF: return NULL;
                /* keywords */
                case IF:    if (ex(child[0])->val) ex(child[1]); else if (p->op.nops > 2) ex(child[2]); return NULL;
                /* functions */
                case FNCT:  return fnctCall(p, child[0], ex(child[1]));
                /* assignment */
                case '=':   return assign(child[0], ex(child[1])); 
                case ADDEQ: return modvar(child[0], '+', ex(child[1])->val);
                case SUBEQ: return modvar(child[0], '-', ex(child[1])->val);
                case MULEQ: return modvar(child[0], '*', ex(child[1])->val);
                case DIVEQ: return modvar(child[0], '/', ex(child[1])->val);
                case MODEQ: return modvar(child[0], '%', ex(child[1])->val);
                case INC:   return modvar(child[0], '+', 1);
                case DEC:   return modvar(child[0], '-', 1);
                /* arithmetic operators */
                case '%':   p->val = (int) ex(child[0])->val % (int) ex(child[1])->val; return p;
                case '^':   p->val = pow(ex(child[0])->val, ex(child[1])->val); return p;
                case '*':   p->val = ex(child[0])->val * ex(child[1])->val;  return p;
                case '/':   p->val = ex(child[0])->val / ex(child[1])->val;  return p;
                case '+':   p->val = ex(child[0])->val + ex(child[1])->val;  return p;
                case '-':   p->val = ex(child[0])->val - ex(child[1])->val;  return p;
                case NEG:   p->val = -(ex(child[0])->val);                   return p;
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
    int i;
    ASTnode *root = args;
    ASTnode *traverse = args;
    /* check for missing arguments */
    for (i = 0; i < numArgs; i++) {
        if (traverse == NULL) {
            yyerror("%s expected %d arguments, got %d", funcName, numArgs, i);
            break;
        }
        traverse = traverse->next;
    }
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeParam) {
        while ((traverse = traverse->next) != NULL) i++;
        yyerror("%s expected %d arguments, got %d", funcName, numArgs, ++i);
    }
    /* evaluate each argument */
    /* gross, fix this */
    root = ex(root);
    traverse = root->next;
    for (i = 1; i < numArgs; i++) {
        if (traverse->type == typeId)
            yyerror("reference to uninitialized variable %s", args->id);
        traverse = ex(traverse);
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
ASTnode* modvar(ASTnode *varNode, char op, double mod) {
    if(varNode->type == typeId)
        yyerror("reference to uninitialized variable \"%s\"", varNode->id);
    if(varNode->type != typeVar)
        yyerror("can't modify constant");
    if(varNode->varPtr->value->type != typeVal)
        yyerror("can't modify non-numeric variable \"%s\"", varNode->id);

    switch (op) {
        case '+': varNode->varPtr->value->val += mod; break;
        case '-': varNode->varPtr->value->val -= mod; break;
        case '*': varNode->varPtr->value->val *= mod; break;
        case '/': varNode->varPtr->value->val /= mod; break;
        case '%': varNode->varPtr->value->val = ((int)varNode->varPtr->value->val % (int)mod); break;
    }
    return ex(varNode);
}

/* helper function to get optional arguments in a function call */
/* TODO: add type checking */
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
                default: yyerror("unknown literal \"\\%c\"", str[i+1]);
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
        if (args->type == typeId)
            yyerror("reference to uninitialized variable \"%s\"", args->id);
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
