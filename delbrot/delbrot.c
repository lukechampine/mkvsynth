#include <dlfcn.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delbrot.h"
#include "y.tab.h"

/* function declarations */
static argList* argify(Env *, ASTnode *);
static ASTnode* assign(Env *, ASTnode *, ASTnode *);
static ASTnode* assignOp(Env *, ASTnode *, ASTnode *, ASTnode *);
static ASTnode* binaryOp(ASTnode *, ASTnode *, int, ASTnode *);
static void     chain(ASTnode *, ASTnode *);
       void     checkArgs(char *, argList *, int, ...);
static ASTnode* copy(ASTnode *);
static ASTnode* dereference(ASTnode *);
       ASTnode* ex(Env *, ASTnode *p);
static ASTnode* fnctCall(Env *, ASTnode *, ASTnode *, argList *);
static void     funcDefine(Env *, ASTnode *, ASTnode *, ASTnode *);
       void*    getOptArg(argList *, char *, int);
static ASTnode* identify(Env *, ASTnode *);
static void     ifelse(Env *, ASTnode *, ASTnode *, ASTnode *, ASTnode *);
static void     import(ASTnode *);
       void     MkvsynthError(char *error, ...);
static ASTnode* setDefault(Env *, ASTnode *, ASTnode *);
static ASTnode* ternary(Env *, ASTnode *, ASTnode *, ASTnode *);
static ASTnode* unaryOp(ASTnode *, ASTnode *, int);
static ASTnode* userDefFnCall(Env *, ASTnode *, argList *);

/* global variables */
char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "variable", "function", "operation", "argument", "optional argument", "parameter", "optional parameter"};
extern int linenumber;

/* function definitions */
/* transform a linked list of nodes into an argList */
argList* argify(Env *e, ASTnode *p) {
    argList *a = malloc(sizeof(argList));
    if (!p)
        return a;

    /* count # of arguments */
    a->nargs = 1;
    ASTnode *traverse = p;
    while ((traverse = traverse->next)) a->nargs++;
    /* allocate space */
    a->args = malloc(a->nargs * sizeof(varNode));

    /* place arguments in args array */
    int i;
    for (i = 0, traverse = p; traverse; traverse = traverse->next, i++) {
        if (traverse->type == typeOptArg)
            a->args[i] = traverse->var;
        else {
            a->args[i].type = typeArg;
            a->args[i].name = NULL;
            a->args[i].value = traverse;
        }
    }
    /* evaluate arguments */
    for (i = 0; i < a->nargs; i++)
        a->args[i].value = ex(e, a->args[i].value);

    return a;
}

/* create or modify a variable */
ASTnode* assign(Env *e, ASTnode *varNode, ASTnode *valueNode) {
    if (varNode->type != typeVar && varNode->type != typeId)
        MkvsynthError("can't assign to a constant value (got %s)", typeNames[ex(e, varNode)->type]);
    if (valueNode->type > typeClip)
        MkvsynthError("can't assign type %s to variable", typeNames[valueNode->type]);
    /* new variable */
    if (varNode->type == typeId)
        varNode = putVar(e, varNode->id);
    /* copy new value */
    return memcpy(varNode->var.value, valueNode, sizeof(ASTnode));
}

/* handle assignment operators */
ASTnode* assignOp(Env *e, ASTnode *varNode, ASTnode *op, ASTnode *valueNode) {
    /* special cases */
    if (op->num == '=')
        return assign(e, varNode, valueNode);

    ASTnode *RHS;
    if (op->num == CHNEQ)
        RHS = mkOpNode(CHAIN, 2, varNode, valueNode);
    else
        RHS = mkOpNode(BINOP, 3, varNode, op, valueNode);

    /* unfold syntactic sugar and reprocess */
    return ex(e, mkOpNode(ASSIGN, 3, varNode, mkNumNode('='), RHS));
}

/* handle arithmetic / boolean operators */
ASTnode* binaryOp(ASTnode* p, ASTnode* c1, int op, ASTnode* c2) {
    /* arithmetic operator */
    if (op < 100) {
        if (c1->type != typeNum) MkvsynthError("type mismatch: LHS of %c expected number, got %s", op, typeNames[c1->type]);
        if (c2->type != typeNum) MkvsynthError("type mismatch: RHS of %c expected number, got %s", op, typeNames[c2->type]);
        switch(op) {
            case '+': p->num = c1->num + c2->num; break;
            case '-': p->num = c1->num - c2->num; break;
            case '*': p->num = c1->num * c2->num; break;
            case '/': p->num = c1->num / c2->num; break;
            case '^': p->num = pow(c1->num, c2->num); break;
            case '%': p->num = (double) ((int) c1->num % (int) c2->num); break;
        }
        p->type = typeNum;
    }
    /* concatenation operator */
    else if (op == CNCAT) {
        if (c1->type != c2->type) MkvsynthError("type mismatch: cannot concatenate %s with %s", typeNames[c1->type], typeNames[c2->type]);
        if (c1->type == typeStr) {
            // allocate space for new string
            char *newString = malloc(strlen(c1->str) + strlen(c2->str) + 1);
            strcpy(newString, c1->str);
            strcat(newString, c2->str);
            p->str = newString;
            p->type = typeStr;
        }
        else if (c1->type == typeClip) {
            MkvsynthError("clip concatenation is not supported (yet)");
        }
        else {
            MkvsynthError("the concatenation operator is not defined on %ss", typeNames[c1->type]);
        }
    }
    /* boolean operators */
    else {
        if (c1->type != c2->type) MkvsynthError("type mismatch: cannot compare %s to %s", typeNames[c1->type], typeNames[c2->type]);
        /* for error messages */
        char *opStrs[512];
        opStrs[EQ] = "=="; opStrs[NE] = "!="; opStrs[GT] = ">"; opStrs[LT] = "<";
        opStrs[GE] = ">="; opStrs[LE] = "<="; opStrs[LOR] = "||"; opStrs[LAND] = "&&";
        if (c1->type == typeNum) {
            switch(op) {
                case EQ: p->bool = c1->num == c2->num ? TRUE : FALSE; break;
                case NE: p->bool = c1->num != c2->num ? TRUE : FALSE; break;
                case GT: p->bool = c1->num  > c2->num ? TRUE : FALSE; break;
                case LT: p->bool = c1->num  < c2->num ? TRUE : FALSE; break;
                case GE: p->bool = c1->num >= c2->num ? TRUE : FALSE; break;
                case LE: p->bool = c1->num <= c2->num ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for numbers", opStrs[op]);
            }
        }
        else if (c1->type == typeBool) {
            switch(op) {
                case EQ:   p->bool = (c1->bool == TRUE) == (c2->bool == TRUE) ? TRUE : FALSE; break;
                case NE:   p->bool = (c1->bool == TRUE) != (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LOR:  p->bool = (c1->bool == TRUE) || (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LAND: p->bool = (c1->bool == TRUE) && (c2->bool == TRUE) ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for booleans", opStrs[op]);
            }
        }
        else if (c1->type == typeStr) {
            switch(op) {
                case EQ: p->bool = strcmp(c1->str, c2->str) == 0 ? TRUE : FALSE; break;
                case NE: p->bool = strcmp(c1->str, c2->str) != 0 ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for strings", opStrs[op]);
            }
        }
        else {
            MkvsynthError("comparison operators are not supported for %ss (yet)", typeNames[c1->type]);
        }
        p->type = typeBool;
    }
    return p;
}

/* append LHS to argument list of RHS */
void chain(ASTnode *valueNode, ASTnode *fnNode) {
    if (fnNode->type == typeOp && fnNode->op.oper == CHAIN)
        chain(valueNode, fnNode->op.ops[0]);
    else if (fnNode->type == typeOp && fnNode->op.oper == FNCT)
        valueNode->next = fnNode->op.ops[1], fnNode->op.ops[1] = valueNode;
    else if (fnNode->type == typeId || fnNode->type == typeFn)
        memcpy(fnNode, mkOpNode(FNCT, 2, copy(fnNode), valueNode), sizeof(ASTnode));
    else
        MkvsynthError("expected function name, got %s", typeNames[fnNode->type]);
}

/* ensure that a function call is valid */
/* TODO: check optional arguments */
void checkArgs(char *funcName, argList *a, int numArgs, ...) {
    /* check number of arguments */
    if (a->nargs != numArgs)
        MkvsynthError("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), a->nargs);
    /* check types */
    int i;
    va_list ap;
    va_start(ap, numArgs);
    for (i = 0; i < numArgs; i++) {
        nodeType argType = va_arg(ap, nodeType);
        if (a->args[i].value->type != argType)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[a->args[i].value->type]);
    }
    va_end(ap);
}

/* copy a node and its children */
ASTnode* copy(ASTnode *p) {
    if (!p)
        return NULL;

    ASTnode *dup = newNode();
    memcpy(dup, p, sizeof(ASTnode));

    /* recurse to children, if any */
    if (dup->type == typeOp) {
        if ((dup->op.ops = malloc(dup->op.nops * sizeof(ASTnode *))) == NULL)
            MkvsynthError("out of memory");
        int i;
        for (i = 0; i < dup->op.nops; i++)
            dup->op.ops[i] = copy(p->op.ops[i]);
    }

    /* recurse to linked node */
    dup->next = copy(dup->next);
    
    return dup;
}

/* dereference a variable */
ASTnode* dereference(ASTnode *p) {
    if (p->type == typeId)
        MkvsynthError("reference to undefined variable or function \"%s\"", p->id);
    if (p->type != typeVar)
        return p;
    return p->var.value;
}

/* execute a section of the AST */
ASTnode* ex(Env *e, ASTnode *p) {
    if (!p)
        return NULL;

    /* resolve identifiers */
    p = dereference(identify(e, p));

    /* catch functions with no arguments */
    if (p->type == typeFn)
        p = mkOpNode(FNCT, 2, p, NULL);

    /* only nodes with children can be reduced */
    if (p->type != typeOp)
        return p;

    /* for convenience */
    ASTnode **child = p->op.ops;

    switch(p->op.oper) {
        /* declarations */
        case FNDEF:   funcDefine(e, child[0], child[1], child[2]); break;
        /* blocks */
        case IF:      ifelse(e, p, ex(e, child[0]), child[1], child[2]); break;
        /* functions */
        case FNCT:    p = fnctCall(e, p, identify(e, child[0]), argify(e, child[1])); break;
        case CHAIN:   chain(ex(e, child[0]), child[1]); p = ex(e, child[1]); break;
        case DEFAULT: setDefault(e, child[0], ex(e, child[1])); break;
        case RETURN:  p = ex(e, child[0]); if (p != NULL) e->returnValue = p;longjmp(e->returnContext, 1); break;
        /* plugin imports */
        case IMPORT:  import(child[0]); break;
        /* assignment */
        case ASSIGN:  p = assignOp(e, identify(e, child[0]), child[1], ex(e, child[2])); break;
        /* unary operators */
        case NEG:     p = unaryOp(p, ex(e, child[0]), NEG); break;
        case '!':     p = unaryOp(p, ex(e, child[0]), '!'); break;
        /* binary operators */
        case BINOP:   p = binaryOp(p, ex(e, child[0]), child[1]->num, ex(e, child[2])); break;
        /* trinary operator */
        case TERN:    p = ternary(e, ex(e, child[0]), child[1], child[2]); break;
        /* compound statements */
        case ';':     ex(e, child[0]); p = ex(e, child[1]); break;
        /* should never wind up here */
        default: MkvsynthError("unknown operator %d", p->op.oper);
    }

    return p;
}

/* handle function calls */
ASTnode* fnctCall(Env *e, ASTnode *p, ASTnode *fnNode, argList *a) {
    if (fnNode->type == typeId)
        MkvsynthError("reference to undefined function \"%s\"", fnNode->var.name);
    if (fnNode->type != typeFn)
        MkvsynthError("expected function, got %s", typeNames[fnNode->type]);

    /* check argument order */
    int i;
    for (i = 0; i < a->nargs - 1; i++) {
        if (a->args[i].type == typeOptArg && a->args[i+1].type == typeArg)
            MkvsynthError("optional arguments must follow mandatory arguments in function \"%s\"", fnNode->fn.name);
    }
    
    if (fnNode->fn.type == fnCore)
        p = (*(fnNode->fn.core.fnPtr))(p, a);
    else
        p = userDefFnCall(e, fnNode, a);

    return p;
}

/* process a function definition */
void funcDefine(Env *e, ASTnode *name, ASTnode *params, ASTnode *body) {
    if (name->type != typeId)
        MkvsynthError("function name \"%s\" is already in use", name->var.name);

    /* create new function table entry */
    ASTnode *p = newNode();
    p->type = typeFn;
    p->fn.type = fnUser;
    p->fn.name = name->id;
    p->fn.user.body = body;

    /* create parameter list */
    p->fn.user.params = malloc(sizeof(argList));
    argList *fnParams = p->fn.user.params; /* for convenience */
    int i;
    if (params) {
        /* count number of parameters */
        fnParams->nargs = 1;
        ASTnode *traverse = params;
        while ((traverse = traverse->next))
            fnParams->nargs++;
        /* allocate space for parameters */
        fnParams->args = malloc(fnParams->nargs * sizeof(varNode));
        /* copy parameters */
        for (i = 0, traverse = params; traverse; traverse = traverse->next, i++) {
            fnParams->args[i].type = traverse->type;
            fnParams->args[i].name = traverse->var.name;
            fnParams->args[i].value = newNode();
            fnParams->args[i].value->type = traverse->var.type;
        }
    }

    /* check argument ordering */
    for (i = 0; i < fnParams->nargs - 1; i++) {
        printf("%s\n", typeNames[fnParams->args[i].value->type]);
        if (fnParams->args[i].type == typeOptParam && fnParams->args[i+1].type == typeParam)
            MkvsynthError("optional parameters must follow mandatory parameters in function %s", p->fn.name);
    }

    /* add to local function table */
    p->next = e->fnTable;
    e->fnTable = p;
}

/* get optional arguments in a function call */
void* getOptArg(argList *a, char *name, int type) {
    int i;
    for (i = 0; i < a->nargs; i++) {
        if (a->args[i].type == typeArg)
            continue;
        if (!(strcmp(a->args[i].name, name))) {
            if (a->args[i].value->type != type)
                MkvsynthError("type mismatch: optional argument \"%s\" expected %s, got %s", name, typeNames[type], typeNames[a->args[i].value->type]);
            switch (type) {
                case typeNum:  return &a->args[i].value->num;
                case typeBool: return &a->args[i].value->bool;
                case typeStr:  return a->args[i].value->str;
                case typeClip: return a->args[i].value->clipIn;
            }
        }
    }
    return NULL;
}

/* resolve an identifier */
ASTnode* identify(Env *e, ASTnode *p) {
    if (p->type != typeId)
        return p;

    ASTnode *i;
    /* function */
    if ((i = getFn(e, p->id)) != NULL)
        p = copy(i);
    /* existing variable */
    else if ((i = getVar(e, p->id)) != NULL)
        p = copy(i);

    return p;
}

/* process an if/else statement */
void ifelse(Env *e, ASTnode *p, ASTnode *cond, ASTnode *ifNode, ASTnode *elseNode) {
    if (cond->type != typeBool)
        MkvsynthError("if expected boolean, got %s", typeNames[cond->type]);
    if (cond->bool == TRUE)
        ex(e, ifNode);
    else if (p->op.nops == 3)
        ex(e, elseNode);
}

/* import a plugin */
void import(ASTnode *pluginName) {
    if (pluginName->type != typeId)
        MkvsynthError("invalid plugin name");
    /* construct plugin path */
    char *home = getenv("HOME");
    char *pluginPath = malloc(strlen(pluginName->id) + strlen(home) + 24);
    strcpy(pluginPath, home);
    strcat(pluginPath, "/.config/mkvsynth/lib");
    strcat(pluginPath, pluginName->id);
    strcat(pluginPath, ".so");
    /* load plugin */
    void *handle = dlopen(pluginPath, RTLD_NOW);
    if (!handle)
        MkvsynthError("could not load plugin %s: %s", pluginName->id, dlerror());
    /* create and append new plugin entry */
    Plugin *newPlugin = malloc(sizeof(Plugin));
    newPlugin->name = pluginName->id;
    newPlugin->handle = handle;
    newPlugin->next = pluginList;
    pluginList = newPlugin;
}

/* display error message in red and exit */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}

/* process a default statement */
ASTnode* setDefault(Env *e, ASTnode *paramNode, ASTnode *valueNode) {
    /* identify parameter */
    paramNode = getVar(e, paramNode->id);
    if (paramNode == NULL)
        MkvsynthError("could not set default of undefined parameter %s", paramNode->id);
    if (paramNode->type != typeOptArg)
        MkvsynthError("could not set default of mandatory parameter %s", paramNode->id);
    if (paramNode->var.value != NULL)
        MkvsynthError("value of %s already set", paramNode->id); /* TODO: make this a warning */

    assign(e, paramNode, valueNode);
    paramNode->type = typeVar;
    return paramNode;
}

/* process a ternary expression */
ASTnode* ternary(Env *e, ASTnode *cond, ASTnode *ifNode, ASTnode *elseNode) {
    if (cond->type != typeBool)
        MkvsynthError("arg 1 of ?| expected boolean, got %s", typeNames[cond->type]);
    if (cond->bool == TRUE)
        return ex(e, ifNode);
    else
        return ex(e, elseNode);
}

/* handle negation operators */
ASTnode* unaryOp(ASTnode* p, ASTnode* c1, int op) {
    if (c1->type != typeNum) MkvsynthError("arg 1 of %c expected number, got %s", op, typeNames[c1->type]);
    switch (op) {
        case NEG: p->type = typeNum; p->num = -c1->num; break;
        case '!': p->type = typeBool; p->bool = c1->bool ? FALSE : TRUE; break;
    }
    return p;
}

/* process a user-defined function call */
ASTnode* userDefFnCall(Env *e, ASTnode *fnNode, argList *a) {
    /* create new environment */
    Env *local = (Env *) malloc(sizeof(Env));
    local->parent = e;
    local->returnValue = newNode();
    local->returnValue->type = typeFn; /* default return value */
    
    /* for convenience */
    argList *p = fnNode->fn.user.params;

    /* check argument number */
    int nMandArgs = 0, nMandParams = 0;
    while (a->args && a->args[nMandArgs].type == typeArg) nMandArgs++;
    while (p->args && p->args[nMandParams].type == typeParam) nMandParams++;
    if (nMandArgs != nMandParams)
        MkvsynthError("%s expected %d mandatory argument%s, got %d", fnNode->fn.name, nMandParams, (nMandParams == 1) ? "" : "s", nMandArgs);

    /* check mandatory args for type mismatches */
    int i;
    for (i = 0; i < nMandParams; i++) {
        if (a->args[i].value->type != p->args[i].value->type)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", 
                i+1, fnNode->fn.name, typeNames[p->args[i].value->type], typeNames[a->args[i].value->type]);
        /* all is well; create local variable and set value */
        assign(e, putVar(local, p->args[i].name), a->args[i].value);
    }

    /* check optional arguments */
    for (i = nMandArgs; i < a->nargs; i++) {
        /* check that optional parameter exists */
        int j = nMandParams;
        for (j = nMandParams; j < p->nargs; j++) {
            if (!strcmp(a->args[i].name, p->args[j].name))
                break;
        }
        if (j == p->nargs)
            MkvsynthError("%s is not an optional parameter of function %s", a->args[i].name, fnNode->fn.name);
        /* check that types match */
        if (a->args[i].value->type != p->args[i].value->type)
            MkvsynthError("type mismatch: optional argument \"%s\" of function \"%s\" expected %s, got %s",
                p->args[i].name, fnNode->fn.name, typeNames[p->args[i].value->type], typeNames[a->args[i].value->type]);
        /* assign value */
        putVar(local, p->args[i].name)->type = typeOptParam;
        assign(e, getVar(local, p->args[i].name), a->args[i].value);
    }

    /* execute a copy of the function body in the local environment */
    if (setjmp(local->returnContext) == 0)
        ex(local, copy(fnNode->fn.user.body));

    /* return */
    return local->returnValue;
}
