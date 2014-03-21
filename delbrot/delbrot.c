#include <dlfcn.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delbrot.h"
#include "y.tab.h"

/* function declarations */
static argList* argify(Env *, Var *);
static Value*   assign(Env *, Value *, Value *);
static Value*   assignOp(Env *, ASTnode *, ASTnode *, ASTnode *);
static Value*   binaryOp(Value *, int, Value *);
static void     chain(ASTnode *, ASTnode *);
       void     checkArgs(argList *, int, ...);
static ASTnode* copy(ASTnode *);
       Value*   ex(Env *, ASTnode *);
static Value*   fnctCall(Env *, Value *, argList *);
static void     funcDefine(Env *, Value *, ASTnode *, ASTnode *);
       void*    getOptArg(argList *, char *, valueType);
static void     ifelse(Env *, ASTnode *, Value *);
static void     import(Value *);
       void     MkvsynthError(char *error, ...);
       void     MkvsynthMessage(char *error, ...);
       void     MkvsynthWarning(char *error, ...);
static void     setDefault(Env *, Value *, Value *);
static Value*   ternary(Env *, Value *, Value *, Value *);
static Value*   unaryOp(Value *, int);
static Value*   userDefFnCall(Env *, Fn *, argList *);

/* global variables */
char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "void"};
extern int linenumber;
char *currentFunction = "";

/* function definitions */
/* transform a linked list of nodes into an argList */
argList* argify(Env *e, Var *p) {
    argList *a = calloc(1, sizeof(argList));
    if (!p)
        return a;

    /* count # of arguments */
    a->nargs = 1;
    Var *traverse = p;
    while ((traverse = traverse->next))
        a->nargs++;

    /* allocate space */
    a->args = calloc(a->nargs, sizeof(Var));

    /* place arguments in args array */
    int i;
    for (i = 0, traverse = p; traverse; traverse = traverse->next, i++)
        a->args[i] = *traverse;

    /* evaluate arguments */
    for (i = 0; i < a->nargs; i++)
        a->args[i].value = ex(e, (ASTnode*)a->args[i].value);

    return a;
}

/* create or modify a variable */
Value* assign(Env *e, Value *name, Value *val) {
    if (!name)
        MkvsynthError("invalid assignment: can't assign to operation", name);
    if (name->type != typeId)
        MkvsynthError("invalid assignment: can't assign to constant type %s", typeNames[name->type]);
    if (val->type > typeClip)
        MkvsynthError("invalid assignment: can't assign type %s to variable", typeNames[val->type]);
    /* new variable */
    if (getVar(e, name->id) == NULL)
        putVar(e, name->id, typeVar);
    return setVar(e, name->id, val);
}


/* handle assignment operators */
Value* assignOp(Env *e, ASTnode *varNode, ASTnode *opNode, ASTnode *valNode) {
    /* special cases */
    if (opNode->op == '=')
        return assign(e, varNode->value, ex(e, valNode));

    ASTnode *RHS;
    if (opNode->op == CHNEQ)
        RHS = makeNode(CHAIN, 2, varNode, valNode);
    else
        RHS = makeNode(BINOP, 3, varNode, opNode, valNode);

    /* unfold syntactic sugar and reprocess */
    return ex(e, makeNode(ASSIGN, 3, varNode, makeNode('=', 0), RHS));
}

/* handle arithmetic / boolean operators */
Value* binaryOp(Value* c1, int op, Value* c2) {
    Value *v = newValue();
    /* arithmetic operator */
    if (op < 100) {
        if (c1->type != typeNum) MkvsynthError("type mismatch: LHS of %c expected number, got %s", op, typeNames[c1->type]);
        if (c2->type != typeNum) MkvsynthError("type mismatch: RHS of %c expected number, got %s", op, typeNames[c2->type]);
        switch(op) {
            case '+': v->num = c1->num + c2->num; break;
            case '-': v->num = c1->num - c2->num; break;
            case '*': v->num = c1->num * c2->num; break;
            case '/': v->num = c1->num / c2->num; break;
            case '^': v->num = pow(c1->num, c2->num); break;
            case '%': v->num = (double) ((int) c1->num % (int) c2->num); break;
        }
        v->type = typeNum;
    }
    /* concatenation operator */
    else if (op == CNCAT) {
        if (c1->type != c2->type) MkvsynthError("type mismatch: cannot concatenate %s with %s", typeNames[c1->type], typeNames[c2->type]);
        if (c1->type == typeStr) {
            // allocate space for new string
            char *newString = malloc(strlen(c1->str) + strlen(c2->str) + 1);
            strcpy(newString, c1->str);
            strcat(newString, c2->str);
            v->str = newString;
            v->type = typeStr;
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
                case EQ: v->bool = c1->num == c2->num ? TRUE : FALSE; break;
                case NE: v->bool = c1->num != c2->num ? TRUE : FALSE; break;
                case GT: v->bool = c1->num  > c2->num ? TRUE : FALSE; break;
                case LT: v->bool = c1->num  < c2->num ? TRUE : FALSE; break;
                case GE: v->bool = c1->num >= c2->num ? TRUE : FALSE; break;
                case LE: v->bool = c1->num <= c2->num ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for numbers", opStrs[op]);
            }
        }
        else if (c1->type == typeBool) {
            switch(op) {
                case EQ:   v->bool = (c1->bool == TRUE) == (c2->bool == TRUE) ? TRUE : FALSE; break;
                case NE:   v->bool = (c1->bool == TRUE) != (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LOR:  v->bool = (c1->bool == TRUE) || (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LAND: v->bool = (c1->bool == TRUE) && (c2->bool == TRUE) ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for booleans", opStrs[op]);
            }
        }
        else if (c1->type == typeStr) {
            switch(op) {
                case EQ: v->bool = strcmp(c1->str, c2->str) == 0 ? TRUE : FALSE; break;
                case NE: v->bool = strcmp(c1->str, c2->str) != 0 ? TRUE : FALSE; break;
                default: MkvsynthError("type mismatch: operator %s is not defined for strings", opStrs[op]);
            }
        }
        else {
            MkvsynthError("comparison operators are not supported for %ss (yet)", typeNames[c1->type]);
        }
        v->type = typeBool;
    }
    return v;
}

/* append LHS to argument list of RHS */
void chain(ASTnode *val, ASTnode *fnNode) {
    if (fnNode->op == CHAIN)
        chain(val, fnNode->child[0]);
    else if (fnNode->op == FNCT)
        fnNode->child[1] = append(val, fnNode->child[1]);
    else if (fnNode->value != NULL && fnNode->value->type == typeId)
        memcpy(fnNode, makeNode(FNCT, 2, copy(fnNode), val), sizeof(ASTnode));
    else
        MkvsynthError("expected function name, got %s", typeNames[fnNode->value->type]);
}

/* ensure that a function call is valid */
void checkArgs(argList *a, int numArgs, ...) {
    /* check number of arguments */
    int i = 0;
    while (i < a->nargs && a->args[i].type == typeArg)
        i++;
    if (i != numArgs)
        MkvsynthError("expected %d argument%s, got %d", numArgs, (numArgs == 1 ? "" : "s"), i);
    /* check types */
    va_list ap;
    va_start(ap, numArgs);
    for (i = 0; i < numArgs; i++) {
        valueType argType = va_arg(ap, valueType);
        if (a->args[i].value->type != argType)
            MkvsynthError("arg %d expected %s, got %s", i+1, typeNames[argType], typeNames[a->args[i].value->type]);
    }
    va_end(ap);
}

/* copy a node and its children */
ASTnode* copy(ASTnode *p) {
    if (!p)
        return NULL;

    ASTnode *dup = newNode();
    memcpy(dup, p, sizeof(ASTnode));

    /* copy payload */
    if (dup->value != NULL)
        memcpy(dup->value, p->value, sizeof(Value));

    /* recurse to children, if any */
    if (dup->nops > 0) {
        if ((dup->child = calloc(dup->nops, sizeof(ASTnode *))) == NULL)
            MkvsynthError("out of memory");
        int i;
        for (i = 0; i < dup->nops; i++)
            dup->child[i] = copy(p->child[i]);
    }

    return dup;
}

/* dereference an identifier */
Value* dereference(Env *e, Value *val) {
    if (val == NULL)
        MkvsynthError("unexpected NULL value");
    if (val->type != typeId)
        return val;
    Var *v; Fn *f;
    if ((v = getVar(e, val->id)) != NULL)
        return v->value;
    if ((f = getFn(e, val->id)) != NULL) {
        return fnctCall(e, val, argify(e, NULL));
    }
    MkvsynthError("reference to undefined variable or function \"%s\"", val->id);
    return NULL;
}

/* execute an ASTnode, producing a constant value */
Value* ex(Env *e, ASTnode *p) {
    if (!p)
        return NULL;

    Value *v = NULL;
    switch (p->op) {
        /* leaf node */
        case 0:       v = dereference(e, p->value); break;
        /* declarations */
        case FNDEF:   funcDefine(e, p->child[0]->value, p->child[1], p->child[2]); break;
        /* blocks */
        case IF:      ifelse(e, p, ex(e, p->child[0])); break;
        /* functions */
        case FNCT:    v = fnctCall(e, p->child[0]->value, argify(e, p->child[1]->value->arg)); break;
        case CHAIN:   chain(makeArg(NULL, p->child[0]), p->child[1]); v = ex(e, p->child[1]); break;
        case DEFAULT: setDefault(e, p->child[0]->value, ex(e, p->child[1])); break;
        case RETURN:  e->returnValue = ex(e, p->child[0]); longjmp(e->returnContext, 1); break;
        /* plugin imports */
        case IMPORT:  import(p->child[0]->value); break;
        /* assignment */
        case ASSIGN:  v = assignOp(e, p->child[0], p->child[1], p->child[2]); break;
        /* unary operators */
        case NEG:     v = unaryOp(ex(e, p->child[0]), '-'); break;
        case '!':     v = unaryOp(ex(e, p->child[0]), '!'); break;
        /* binary operators */
        case BINOP:   v = binaryOp(ex(e, p->child[0]), p->child[1]->op, ex(e, p->child[2])); break;
        /* trinary operator */
        case TERN:    v = ternary(e, ex(e, p->child[0]), ex(e, p->child[1]), ex(e, p->child[2])); break;
        /* compound statements */
        case ';':     ex(e, p->child[0]); v = ex(e, p->child[1]); break;
        /* should never wind up here */
        default:      MkvsynthError("unknown operator %d", p->op);
    }

    /* default return value */
    if (v == NULL) {
        v = newValue();
        v->type = typeOp;
    }

    return v;
}

/* handle function calls */
Value* fnctCall(Env *e, Value *name, argList *a) {
    if (!name)
        MkvsynthError("expected function name, got operation", name);
    if (name->type != typeId)
        MkvsynthError("expected function name, got %s", typeNames[name->type]);
    Fn *f = getFn(e, name->id);
    if (f == NULL)
        MkvsynthError("reference to undefined function \"%s\"", name->id);

    /* set global variable */
    currentFunction = f->name;

    /* check argument order */
    int i;
    for (i = 0; i < a->nargs - 1; i++) {
        if (a->args[i].type == typeOptArg && a->args[i+1].type == typeArg)
            MkvsynthError("optional arguments must follow mandatory arguments");
    }
    
    Value *res;
    if (f->type == fnCore)
        res = (*(f->fnPtr))(a);
    else
        res = userDefFnCall(e, f, a);

    /* unset global variable */
    currentFunction = "";

    return res;
}

/* process a function definition */
void funcDefine(Env *e, Value *name, ASTnode *params, ASTnode *body) {
    if (!name)
        MkvsynthError("expected name of function, got operation", name);
    if (name->type != typeId)
        MkvsynthError("expected name of function, got %s", typeNames[name->type]);
    if (getVar(e, name->id) || getFn(e, name->id))
        MkvsynthWarning("overwrote previous definition of %s", name->id);

    /* create new function table entry */
    Fn *f = calloc(1, sizeof(Fn));
    f->type = fnUser;
    f->name = name->id;
    f->body = copy(body);

    /* create parameter list */
    /* TODO: can this be replaced with argify? */
    f->params = calloc(1, sizeof(argList));
    int i;
    if (params) {
        /* count number of parameters */
        f->params->nargs = 1;
        Var *traverse = params->value->arg;
        while ((traverse = traverse->next))
            f->params->nargs++;
        /* allocate space for parameters */
        f->params->args = calloc(f->params->nargs, sizeof(Var));
        /* copy parameters */
        for (i = 0, traverse = params->value->arg; traverse; traverse = traverse->next, i++)
            memcpy(&f->params->args[i], traverse, sizeof(Var));
    }

    /* check argument ordering */
    for (i = 0; i < f->params->nargs - 1; i++) {
        if (f->params->args[i].type == typeOptParam && f->params->args[i+1].type == typeParam)
            MkvsynthError("optional parameters must follow mandatory parameters in function %s", f->name);
    }

    /* add to local function table */
    putFn(e, f);
}

/* get optional arguments in a function call */
void* getOptArg(argList *a, char *name, valueType type) {
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
                default:       MkvsynthError("invalid argument type");
            }
        }
    }
    return NULL;
}

/* process an if/else statement */
void ifelse(Env *e, ASTnode *p, Value *cond) {
    if (cond->type != typeBool)
        MkvsynthError("if expected boolean, got %s", typeNames[cond->type]);
    if (cond->bool == TRUE)
        ex(e, p->child[1]);
    else if (p->nops == 3)
        ex(e, p->child[2]);
}

/* import a plugin */
void import(Value *pluginName) {
    if (!pluginName)
        MkvsynthError("expected name of plugin, got operation", pluginName);
    if (pluginName->type != typeId)
        MkvsynthError("expected name of plugin, got %s", typeNames[pluginName->type]);
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
        MkvsynthError("could not load plugin %s: %s not found", pluginName->id, pluginPath);
    /* create and append new plugin entry */
    Plugin *newPlugin = malloc(sizeof(Plugin));
    newPlugin->name = pluginName->id;
    newPlugin->handle = handle;
    newPlugin->next = pluginList;
    pluginList = newPlugin;
}

/* display error and current line number in red and exit */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot:%d error: %s%s", linenumber, currentFunction, currentFunction[0] ? ": " : "");
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}

/* display message in blue */
void MkvsynthMessage(char *message, ...) {
    fprintf(stderr, "\x1B[36m");
    va_list arglist;
    va_start(arglist, message);
    vfprintf(stdout, message, arglist);
    va_end(arglist);
    fprintf(stdout, "\x1B[0m\n");
}

/* display warning and current line number in yellow */
void MkvsynthWarning(char *warning, ...) {
    fprintf(stderr, "\x1B[33mdelbrot:%d warning: %s%s", linenumber, currentFunction, currentFunction[0] ? ": " : "");
    va_list arglist;
    va_start(arglist, warning);
    vfprintf(stderr, warning, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
}

/* process a default statement */
void setDefault(Env *e, Value *name, Value *val) {
    if (currentFunction[0] == 0)
        MkvsynthError("can't set defaults outside of function body");
    if (!name)
        MkvsynthError("expected optional parameter, got operation", name);
    if (name->type != typeId)
        MkvsynthError("expected optional parameter, got %s", typeNames[name->type]);

    Var *v = getVar(e, name->id);
    if (v == NULL)
        MkvsynthError("could not set default of undefined parameter %s", name->id);
    if (v->type != typeOptParam)
        MkvsynthError("could not set default of mandatory parameter %s", name->id);

    if (v->value == NULL)
        v->value = val;
}

/* process a ternary expression */
Value* ternary(Env *e, Value *cond, Value *ifVal, Value *elseVal) {
    if (cond->type != typeBool)
        MkvsynthError("arg 1 of ?| expected boolean, got %s", typeNames[cond->type]);
    if (cond->bool == TRUE)
        return ifVal;
    else
        return elseVal;
}

/* handle negation operators */
Value* unaryOp(Value* val, int op) {
    Value *v = newValue();
    v->type = typeNum;
    if (op == '-') {
        if (val->type != typeNum)
            MkvsynthError("%c expected number, got %s", op, typeNames[val->type]);
        v->num = -val->num;
    }
    else if (op == '!') {
        if (val->type != typeBool)
            MkvsynthError("%c expected boolean, got %s", op, typeNames[val->type]);
        v->bool = val->bool == TRUE ? FALSE : TRUE;
    }
    return v;
}

/* process a user-defined function call */
Value* userDefFnCall(Env *e, Fn *f, argList *a) {
    /* create new environment */
    Env *local = (Env *) calloc(1, sizeof(Env));
    local->parent = e;
    
    /* for convenience */
    Var *args = a->args;
    Var *params = f->params->args;

    /* add function parameters to local variable table */
    int i;
    for (i = 0; i < f->params->nargs; i++)
        putVar(local, params[i].name, params[i].type);

    /* check argument number */
    int nMandArgs = 0, nMandParams = 0;
    while (nMandArgs < a->nargs && args[nMandArgs].type == typeArg)
        nMandArgs++;
    while (nMandParams < f->params->nargs && params[nMandParams].type == typeParam)
        nMandParams++;
    if (nMandArgs != nMandParams)
        MkvsynthError("%s expected %d mandatory argument%s, got %d", f->name, nMandParams, (nMandParams == 1) ? "" : "s", nMandArgs);
    /* check mandatory args for type mismatches */
    for (i = 0; i < nMandParams; i++) {
        if (args[i].value->type != params[i].valType)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", 
                i+1, f->name, typeNames[params[i].valType], typeNames[args[i].value->type]);
        /* all is well; set value of local var */
        setVar(local, params[i].name, args[i].value);
    }

    /* check optional arguments */
    for (i = nMandArgs; i < a->nargs; i++) {
        /* check that optional parameter exists */
        int j = nMandParams;
        for (j = nMandParams; j < f->params->nargs; j++) {
            if (!strcmp(args[i].name, params[j].name))
                break;
        }
        if (j == f->params->nargs)
            MkvsynthError("%s is not an optional parameter of function %s", args[i].name, f->name);
        /* check that types match */
        if (args[i].value->type != params[i].valType)
            MkvsynthError("type mismatch: optional argument \"%s\" of function \"%s\" expected %s, got %s",
                params[i].name, f->name, typeNames[params[i].valType], typeNames[args[i].value->type]);
        /* assign value */
        setVar(local, params[i].name, args[i].value);
    }

    /* execute a copy of the function body in the local environment */
    if (setjmp(local->returnContext) == 0)
        ex(local, copy(f->body));

    /* default return value */
    Value *v = newValue();
    v->type = typeOp;
    if (!local->returnValue)
        return v;
    else
        return memcpy(v, local->returnValue, sizeof(Value));
}
