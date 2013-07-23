#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

#define UNDEFINED(n) n->type == typeVar && n->var->value == NULL

/* useful for error messages */
static char *typeNames[] = {"integer", "identifier", "string", "function", "variable", "opt argument", "operation"};

/* assign a variable */
ASTnode* assign(ASTnode *varNode, ASTnode *valueNode) {
    if (varNode->type != typeVar)
        yyerror("can't assign to a constant value (got %s)", typeNames[varNode->type]);

    /* new variable */
    if (UNDEFINED(varNode)) {
        /* allocate space for ASTnode */
        /* don't use newNode() because we don't want this node to ever be freed */
        if ((varNode->var->value = malloc(sizeof(ASTnode))) == NULL)
            yyerror("out of memory");
    }
    /* copy new value */
    return memcpy(varNode->var->value, valueNode, sizeof(ASTnode));
}

/* modify the value of a numeric variable */
ASTnode* modvar(ASTnode *varNode, char op, ASTnode *modNode) {
    if (UNDEFINED(varNode))
        yyerror("reference to uninitialized variable \"%s\"", varNode->var->name);
    if (varNode->type != typeVar)
        yyerror("can't modify constant");
    if (varNode->var->value->type != typeVal)
        yyerror("can't modify non-numeric variable \"%s\"", varNode->var->name);
    if (modNode && modNode->type != typeVal)
        yyerror("can't modify variable %s with non-numeric type (expected integer, got %s)\n", varNode->var->name, typeNames[modNode->type]);

    int mod;
    if (!modNode)
        mod = 1;
    else
        mod = modNode->val;

    switch (op) {
        case '+': varNode->var->value->val += mod; break;
        case '-': varNode->var->value->val -= mod; break;
        case '*': varNode->var->value->val *= mod; break;
        case '/': varNode->var->value->val /= mod; break;
        case '%': varNode->var->value->val = ((int)varNode->var->value->val % (int)mod); break;
    }
    return ex(varNode);
}

/* resolve an identifier */
ASTnode *identify(ASTnode *p) {
    varRec *v; funcRec *f;
    /* function */
    if ((f = getFn(p->str)) != NULL) {
        p->type = typeFn;
        p->fn = f;
    }
    /* existing variable */
    else if ((v = getVar(p)) != NULL) {
        p->type = typeVar;
        p->var = v;
    }
    /* new variable */
    else {
        p->type = typeVar;
        p->var = putVar(p);
    }
    return p;
}

/* dereference a variable */
ASTnode *dereference(ASTnode *p) {
    if (UNDEFINED(p))
        yyerror("dereference called on uninitialized variable %s", p->var->name);
    ASTnode *next = p->next;
    memcpy(p, p->var->value, sizeof(ASTnode));
    p->next = next;
    return p;
}

/* copy a node and it's children */
ASTnode *copy(ASTnode *p) {
    if (!p)
        return NULL;

    ASTnode *dup = tempNode();
    memcpy(dup, p, sizeof(ASTnode));

    /* recurse to children, if any */
    if (dup->type == typeOp) {
        /* TODO: find a way to free this */
        if ((dup->op.ops = malloc(dup->op.nops * sizeof(ASTnode *))) == NULL)
            yyerror("out of memory");
        int i;
        for (i = 0; i < dup->op.nops; i++)
            dup->op.ops[i] = copy(p->op.ops[i]);
    }

    /* recurse to linked node */
    dup->next = copy(dup->next);
    
    return dup;
}

/* propagate scope to children */
void propagateScope(ASTnode *p, varRec **scope) {
    if (!p)
        return;

    p->scope = scope;
    /* recurse to children, if any */
    if (p->type == typeOp) {
        int i;
        for (i = 0; i < p->op.nops; i++)
            propagateScope(p->op.ops[i], scope);
    }
    /* recurse to linked node */
    propagateScope(p->next, scope);
}

/* process a run-time function definition */
void funcDefine(ASTnode *nameNode, ASTnode *paramNode, ASTnode *bodyNode) {
    if (nameNode->type != typeId)
        yyerror("function name \"%s\" is already in use", nameNode->var->name);

    /* create new function table entry */
    funcRec *fn;
    if ((fn = malloc(sizeof(funcRec))) == NULL)
        yyerror("out of memory");
    fn->name = nameNode->str;

    /* allocate variables */
    for(; paramNode; paramNode = paramNode->next) {
        paramNode->scope = &fn->localVars;
        varRec *v = putVar(paramNode);
        v->value = (ASTnode *) malloc(sizeof(ASTnode));
        v->value->type = paramNode->type;
    }

    /* propagate scope to function body */
    fn->body = bodyNode;
    propagateScope(fn->body, &fn->localVars);

    /* protect function body */
    protect(fn->body);

    putFn(fn);
}

/* process the call of a function that was defined at run-time */
ASTnode* userDefFnCall(ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    /* determine number of arguments */
    int i, numArgs = 0;
    varRec* fnVars;
    for(fnVars = fnNode->fn->localVars; fnVars; fnVars = fnVars->next, numArgs++);
    fnVars = fnNode->fn->localVars;

    /* define and assign local variables */
    for(i = 0; i < numArgs; i++, args = args->next, fnVars = fnVars->next) {
        if (!args)
            yyerror("%s expected %d argument(s), got %d", fnNode->fn->name, numArgs, i);
        if (ex(ex(args))->type != fnVars->value->type)
            yyerror("arg %d of %s expected %s, got %s", i+1, fnNode->fn->name, typeNames[fnVars->value->type], typeNames[args->type]);

        memcpy(fnVars->value, args, sizeof(ASTnode));
    }
    /* excessive arguments */
    if (args) {
        while ((args = args->next) != NULL) i++;
        yyerror("%s expected %d argument(s), got %d", fnNode->fn->name, numArgs, ++i);
    }

    /* make a copy of the function body and execute it */
    p = ex(copy(fnNode->fn->body));

    return p;
}

/* handle function calls */
ASTnode* fnctCall(ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    if (UNDEFINED(fnNode))
        yyerror("reference to undefined function \"%s\"", fnNode->var->name);
    if (fnNode->type != typeFn)
        yyerror("expected function name before '(' (got %s)", typeNames[fnNode->type]);

    if (fnNode->fn->ptr)
        p = (*(fnNode->fn->ptr))(p, args);
    else
        p = userDefFnCall(p, fnNode, args);

    return p;
}

void handleWhile(ASTnode *cond, ASTnode *body) {
    if(!ex(copy(cond))->val)
        return;
    ex(copy(body));
    /* clean up */
    freeTemp();
    /* recurse */
    handleWhile(cond, body);
}

void handleFor(ASTnode *cond, ASTnode *each, ASTnode *body) {
    if(!ex(copy(cond))->val)
        return;
    ex(copy(body));
    ex(copy(each));
    /* clean up */
    freeTemp();
    /* recurse */
    handleFor(cond, each, body);
}

/* execute a section of the AST */
ASTnode* ex(ASTnode *n) {
    if (!n)
        return NULL;

    ASTnode *p = n;

    /* resolve identifiers */
    if (p->type == typeId)
        p = identify(p);

    /* dereference variables */
    if (p->type == typeVar)
        return dereference(p);

    /* only nodes with children should be evaluated */
    if (p->type != typeOp)
        return p;

    /* for convenience/readability */
    ASTnode **child = p->op.ops;
    p->type = typeVal; /* can be redefined later if necessary */

    switch(p->op.oper) {
        /* declarations */
        case FNDEF: funcDefine(child[0], child[1], child[2]); return p;
        /* keywords */
        case IF:    if (ex(child[0])->val) ex(child[1]); else if (p->op.nops > 2) ex(child[2]); p->type = typeOp; return p;
        case WHILE: handleWhile(child[0], child[1]); freeTemp(); p->type = typeOp; return p;
        case FOR:   ex(child[0]); handleFor(child[1], child[2], child[3]); freeTemp(); p->type = typeOp; return p;
        /* functions */
        case FNCT:  return fnctCall(p, ex(child[0]), child[1]);
        case '.':   child[0]->next = child[2]; return fnctCall(p, ex(child[1]), ex(child[0]));
        /* assignment */
        case '=':   return assign(identify(child[0]), ex(child[1])); 
        case ADDEQ: return modvar(identify(child[0]), '+', ex(child[1]));
        case SUBEQ: return modvar(identify(child[0]), '-', ex(child[1]));
        case MULEQ: return modvar(identify(child[0]), '*', ex(child[1]));
        case DIVEQ: return modvar(identify(child[0]), '/', ex(child[1]));
        case MODEQ: return modvar(identify(child[0]), '%', ex(child[1]));
        case INC:   return modvar(identify(child[0]), '+', NULL); /* bit of a hack */
        case DEC:   return modvar(identify(child[0]), '-', NULL);
        /* arithmetic operators */
        case '%':   return nmod(p, ex(child[0]), ex(child[1]));
        case '^':   return npow(p, ex(child[0]), ex(child[1]));
        case '*':   return nmul(p, ex(child[0]), ex(child[1]));
        case '/':   return ndiv(p, ex(child[0]), ex(child[1]));
        case '+':   return nadd(p, ex(child[0]), ex(child[1]));
        case '-':   return nsub(p, ex(child[0]), ex(child[1]));
        case NEG:   return nneg(p, ex(child[0]));
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
        /* compound statements */
        case ';':   ex(child[0]); return ex(child[1]);
    }
    /* should never wind up here */
    yyerror("Unknown operator");
}

/* helper function to ensure that a function call is valid */
void checkArgs(char *funcName, ASTnode *args, int numArgs, ...) {
    int i;
    va_list ap;
    va_start(ap, numArgs);
    ASTnode *traverse = args;
    /* check for missing/uninitialized/mistyped arguments */
    for (i = 0; i < numArgs; i++) {
        if (traverse == NULL)
            yyerror("%s expected %d argument(s), got %d", funcName, numArgs, i);
        /* evaluate argument */

        int argType = va_arg(ap, int);
        if (ex(traverse)->type != argType)
            yyerror("arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[traverse->type]);
        traverse = traverse->next;
    }
    va_end(ap);
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeOptArg) {
        while ((traverse = traverse->next) != NULL) i++;
        yyerror("%s expected %d argument(s), got %d", funcName, numArgs, ++i);
    }
}

/* helper function to get optional arguments in a function call */
/* TODO: add type checking */
void* getOptArg(ASTnode *args, char *name, int type) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next)
        if (traverse->type == typeOptArg && !(strncmp(traverse->var->name,name,strlen(name)))) {
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
        if (args->type == typeId || args->type == typeVar || args->type == typeOp)
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

void ffmpegDecode(char *filename, int numFrames) {
    if (numFrames != -1)
        printf("decoded %d frames of %s\n", numFrames, filename);
    else
        printf("decoded %s\n", filename);
}

/* toy ffmpeg decoding function, showcasing optional arguments */
ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args) {
    checkArgs("ffmpegDecode", args, 1, typeStr);

    char *str = args->str;
    double frames = OPTVAL("frames", -1);

    ffmpegDecode(str, frames);

    RETURNVAL(0);
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nmod(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of %% expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of %% expected integer, got %s", typeNames[c2->type]); 
    p->val = (double) ((int) c1->val % (int) c2->val);
    return p;
} 
ASTnode* npow(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of ^ expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of ^ expected integer, got %s", typeNames[c2->type]); 
    p->val = pow(c1->val, c2->val);
    return p;
} 
ASTnode* nmul(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of * expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of * expected integer, got %s", typeNames[c2->type]); 
    p->val = c1->val * c2->val;
    return p;
} 
ASTnode* ndiv(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of / expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of / expected integer, got %s", typeNames[c2->type]); 
    p->val = c1->val / c2->val;
    return p;
} 
ASTnode* nadd(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of + expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of + expected integer, got %s", typeNames[c2->type]); 
    p->val = c1->val + c2->val;
    return p;
} 
ASTnode* nsub(ASTnode *p, ASTnode *c1, ASTnode *c2) {
    if (c1->type != typeVal) yyerror("arg 1 of - expected integer, got %s", typeNames[c1->type]); 
    if (c2->type != typeVal) yyerror("arg 2 of - expected integer, got %s", typeNames[c2->type]); 
    p->val = c1->val - c2->val;
    return p;
} 
ASTnode* nneg(ASTnode *p, ASTnode *c1) {
    if (c1->type != typeVal) yyerror("arg 1 of - expected integer, got %s", typeNames[c1->type]); 
    p->val = -c1->val;
    return p;
}  

ASTnode* nsin (ASTnode *p, ASTnode *args) {
    checkArgs("sin", args, 1, typeVal);
    p->val = sin(args->val); 
    return p;
}
ASTnode* ncos (ASTnode *p, ASTnode *args) {
    checkArgs("cos", args, 1, typeVal);
    p->val = cos(args->val); 
    return p;
}
ASTnode* nlog (ASTnode *p, ASTnode *args) {
    checkArgs("log", args, 1, typeVal);
    p->val = log(args->val); 
    return p;
}
ASTnode* nsqrt(ASTnode *p, ASTnode *args) {
    checkArgs("sqrt",args, 1, typeVal);
    p->val = sqrt(args->val);
    return p;
}