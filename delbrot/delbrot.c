#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

#define UNDEFINED(n) n->type == typeVar && n->var->value == NULL

/* assign a variable */
ASTnode* assign(ASTnode *varNode, ASTnode *valueNode) {
    /* existing variable */
    if (varNode->type == typeVar)
        return memcpy(varNode->var->value, valueNode, sizeof(ASTnode));
    else
        yyerror("can't assign to a constant value");
}

/* dereference a variable */
ASTnode* dereference(ASTnode *p) {
    ASTnode *next = p->next;
    memcpy(p, p->var->value, sizeof(ASTnode));
    p->next = next;
    return p;
}

/* handle function calls */
ASTnode* fnctCall(ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    if (UNDEFINED(fnNode))
        yyerror("reference to undefined function \"%s\"", fnNode->id);
    if (fnNode->type != typeFn)
        yyerror("expected function name before '('");

    p = (*(fnNode->fn->ptr))(p, args);
}

/* execute a section of the AST */
ASTnode* ex(ASTnode *n) {
    if (!n)
        return NULL;

    ASTnode *p = n;
    if (n->readonly) {
        /* don't reduce node */
        p = newNode();
        memcpy(p, n, sizeof(ASTnode));
        /* propagate flag to any children */
        if (p->type == typeOp) {
            int i;
            for (i = 0; i < p->op.nops; i++) {
                ASTnode *traverse = p->op.ops[i];
                while (traverse) {
                    traverse->readonly = p->readonly;
                    traverse = traverse->next;
                }
            }                   
        }
    }

    /* only nodes with children should be evaluated */
    if (p->type == typeVar)
        return dereference(p);
    if (p->type != typeOp)
        return p;

    /* for convenience/readability */
    ASTnode **child = p->op.ops;
    p->type = typeVal; /* can be redefined later */

    switch(p->op.oper) {
        /* declarations */
        case FNDEF: /* not implemented yet */ return NULL;
        /* keywords */
        case IF:    if (ex(child[0])->val) ex(child[1]); else if (p->op.nops > 2) ex(child[2]); return NULL;
        case WHILE: while(ex(child[0])->val) ex(child[1]); return NULL;
        case FOR:   for(ex(child[0]); ex(child[1])->val; ex(child[2])) ex(child[3]); return NULL;
        /* functions */
        case FNCT:  return fnctCall(p, child[0], child[1]);
        /* assignment */
        case '=':   return assign(child[0], ex(child[1])); 
        case ADDEQ: return ex(modvar(child[0], '+', ex(child[1])->val));
        case SUBEQ: return ex(modvar(child[0], '-', ex(child[1])->val));
        case MULEQ: return ex(modvar(child[0], '*', ex(child[1])->val));
        case DIVEQ: return ex(modvar(child[0], '/', ex(child[1])->val));
        case MODEQ: return ex(modvar(child[0], '%', ex(child[1])->val));
        case INC:   return ex(modvar(child[0], '+', 1));
        case DEC:   return ex(modvar(child[0], '-', 1));
        /* arithmetic operators */
        /* TODO: make these real function calls, complete with type checking */
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
        case ';':   ex(child[0]); return ex(child[1]);
        case '.':   return fnctCall(p, child[0], ex(child[1]));
    }
    /* should never wind up here */
    yyerror("Unknown operator");
}

/* helper function to ensure that a function call is valid */
/* TODO: use ... to allow type checking */
void checkArgs(char *funcName, ASTnode *args, int numArgs) {
    int i;
    ASTnode *traverse = args;
    /* check for missing arguments */
    for (i = 0; i < numArgs; i++) {
        if (traverse == NULL) {
            yyerror("%s expected %d argument(s), got %d", funcName, numArgs, i);
            break;
        }
        traverse = traverse->next;
    }
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeParam) {
        while ((traverse = traverse->next) != NULL) i++;
        yyerror("%s expected %d argument(s), got %d", funcName, numArgs, ++i);
    }
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nsin (ASTnode *p, ASTnode *args) { checkArgs("sin", args, 1); p->val = sin(ex(args)->val);  return p; }
ASTnode* ncos (ASTnode *p, ASTnode *args) { checkArgs("cos", args, 1); p->val = cos(ex(args)->val);  return p; }
ASTnode* nlog (ASTnode *p, ASTnode *args) { checkArgs("log", args, 1); p->val = log(ex(args)->val);  return p; }
ASTnode* nsqrt(ASTnode *p, ASTnode *args) { checkArgs("sqrt",args, 1); p->val = sqrt(ex(args)->val); return p; }

/* modify the value of a variable */
ASTnode* modvar(ASTnode *varNode, char op, double mod) {
    if(UNDEFINED(varNode))
        yyerror("reference to uninitialized variable \"%s\"", varNode->id);
    if(varNode->type != typeVar)
        yyerror("can't modify constant");
    if(varNode->var->value->type != typeVal)
        yyerror("can't modify non-numeric variable \"%s\"", varNode->id);

    switch (op) {
        case '+': varNode->var->value->val += mod; break;
        case '-': varNode->var->value->val -= mod; break;
        case '*': varNode->var->value->val *= mod; break;
        case '/': varNode->var->value->val /= mod; break;
        case '%': varNode->var->value->val = ((int)varNode->var->value->val % (int)mod); break;
    }
    return varNode;
}

/* helper function to get optional arguments in a function call */
/* TODO: add type checking */
void* getOptArg(ASTnode *args, char *name, int type) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next)
        if (traverse->type == typeParam && !(strncmp(traverse->var->name,name,strlen(name)))) {
            ASTnode *value = ex(traverse->var->value);
            switch (type) {
                case typeVal: return &value->val;
                case typeStr: return value->str;
            }
        }
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
        /* unreduced/unprintable types */
        if (UNDEFINED(args))
            yyerror("reference to uninitialized variable \"%s\"", args->id);
        if (args->type == typeVar || args->type == typeOp)
            args = ex(args);
        /* printable types */
        switch(args->type) {
            case typeVal: printf("%.10g ", args->val); break;
            case typeStr: printf("%s ", unesc(args->str)); break;
            default: /* should never wind up here */ break;
        }
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
    checkArgs("ffmpegDecode", args, 1);
    // get arguments
    char *str = args->str;
    double frames = OPTVAL("frames", -1);
    // main function body
    ffmpegDecodeFinal(str, frames);
    // return value
    p->type = typeVal;
    p->val = 0;
    return p;
}