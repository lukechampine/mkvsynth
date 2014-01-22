#include "delbrot.h"
#include "y.tab.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* macros */
#define UNDEFINED(n) n->type == typeVar && n->var.value == NULL

/* function declarations */
static ASTnode* assign(ASTnode *, int, ASTnode *);
static ASTnode* binaryOp(ASTnode *, ASTnode *, int, ASTnode *);
static ASTnode* copy(ASTnode *);
       void     checkArgs(char *, ASTnode *, int, ...);
static ASTnode* dereference(ASTnode *);
       ASTnode* ex(Env *, ASTnode *p);
static ASTnode* fnctCall(Env *, ASTnode *, ASTnode *, ASTnode *);
static void     funcDefine(Env *, ASTnode *, ASTnode *, ASTnode *);
       void*    getOptArg(ASTnode *, char *, int);
static ASTnode* identify(Env *, ASTnode *);
static void     ifelse(Env *, ASTnode *, ASTnode *, ASTnode *, ASTnode *);
       void     MkvsynthError(char *error, ...);
static ASTnode* reduceArgs(Env *, ASTnode *);
static ASTnode* setDefault(Env *, ASTnode *, ASTnode *);
static int      splitMandOpt(ASTnode *, ASTnode **, ASTnode **);
static ASTnode* ternary(Env *, ASTnode *, ASTnode *, ASTnode *);
static ASTnode* unaryOp(ASTnode *, ASTnode *, int);
static ASTnode* userDefFnCall(Env *, ASTnode *, ASTnode *, ASTnode *);

/* global variables */
static char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "variable", "optional argument", "function", "operation"};
extern int linenumber;

/* function definitions */
/* assign or modify a variable */
ASTnode* assign(ASTnode *varNode, int op, ASTnode *valueNode) {
    /* standard assignment */
    if (op == '=') {
        if (varNode->type != typeVar)
            MkvsynthError("can't assign to a constant value (got %s)", typeNames[varNode->type]);
        if (valueNode->type > typeClip)
            MkvsynthError("can't assign type %s to variable", typeNames[valueNode->type]);
        /* new variable */
        if (UNDEFINED(varNode))
            varNode->var.value = newNode();
        /* copy new value */
        return memcpy(varNode->var.value, valueNode, sizeof(ASTnode));
    }

    /* arithmetic operation + assignment */
    if (varNode->type != typeVar)
        MkvsynthError("can't modify constant value (got %s)", typeNames[varNode->type]);
    if (UNDEFINED(varNode))
        MkvsynthError("reference to uninitialized variable \"%s\"", varNode->var.name);
    if (varNode->var.value->type != typeNum)
        MkvsynthError("can't modify non-numeric variable \"%s\"", varNode->var.name);
    if (valueNode && valueNode->type != typeNum)
        MkvsynthError("can't modify variable %s with non-numeric type (expected number, got %s)\n", varNode->var.name, typeNames[valueNode->type]);

    switch (op) {
        case ADDEQ: varNode->var.value->num += valueNode->num; break;
        case SUBEQ: varNode->var.value->num -= valueNode->num; break;
        case MULEQ: varNode->var.value->num *= valueNode->num; break;
        case DIVEQ: varNode->var.value->num /= valueNode->num; break;
        case POWEQ: varNode->var.value->num = pow(varNode->var.value->num, valueNode->num); break;
        case MODEQ: varNode->var.value->num = ((int)varNode->var.value->num % (int)valueNode->num); break;
    }

    return dereference(varNode);
}

/* handle arithmetic / boolean operators */
ASTnode* binaryOp(ASTnode* p, ASTnode* c1, int op, ASTnode* c2) {
    /* arithmetic operator */
    if (op < 100) {
        if (c1->type != typeNum) MkvsynthError("type mismatch: LHS of %c expected number, got %s", op, typeNames[c1->type]);
        if (c2->type != typeNum) MkvsynthError("type mismatch: RHS of %c expected number, got %s", op, typeNames[c2->type]);
        switch(op) {
            case '+':  p->num = c1->num + c2->num; break;
            case '-':  p->num = c1->num - c2->num; break;
            case '*':  p->num = c1->num * c2->num; break;
            case '/':  p->num = c1->num / c2->num; break;
            case '^':  p->num = pow(c1->num, c2->num); break;
            case '%':  p->num = (double) ((int) c1->num % (int) c2->num); break;
            /* should never wind up here */
            default: MkvsynthError("unrecognized binary operator");
        }
        p->type = typeNum;
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
                case EQ:   p->bool = c1->num == c2->num ? TRUE : FALSE; break;
                case NE:   p->bool = c1->num != c2->num ? TRUE : FALSE; break;
                case GT:   p->bool = c1->num  > c2->num ? TRUE : FALSE; break;
                case LT:   p->bool = c1->num  < c2->num ? TRUE : FALSE; break;
                case GE:   p->bool = c1->num >= c2->num ? TRUE : FALSE; break;
                case LE:   p->bool = c1->num <= c2->num ? TRUE : FALSE; break;
                default:  MkvsynthError("type mismatch: operator %s is not defined for numbers", opStrs[op]);
            }
        }
        else if (c1->type == typeBool) {
            switch(op) {
                case EQ:   p->bool = (c1->bool == TRUE) == (c2->bool == TRUE) ? TRUE : FALSE; break;
                case NE:   p->bool = (c1->bool == TRUE) != (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LOR:  p->bool = (c1->bool == TRUE) || (c2->bool == TRUE) ? TRUE : FALSE; break;
                case LAND: p->bool = (c1->bool == TRUE) && (c2->bool == TRUE) ? TRUE : FALSE; break;
                default:  MkvsynthError("type mismatch: operator %s is not defined for booleans", opStrs[op]);
            }
        }
        else if (c1->type == typeStr) {
            switch(op) {
                case EQ:   p->bool = strcmp(c1->str, c2->str) == 0 ? TRUE : FALSE; break;
                case NE:   p->bool = strcmp(c1->str, c2->str) != 0 ? TRUE : FALSE; break;
                default:  MkvsynthError("type mismatch: operator %s is not defined for strings", opStrs[op]);
            }
        }
        else {
            MkvsynthError("comparison operators are not supported for type %s (yet)", typeNames[c1->type]);
        }
        p->type = typeBool;
    }
    return p;
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

/* ensure that a function call is valid */
/* TODO: check optional arguments */
void checkArgs(char *funcName, ASTnode *args, int numArgs, ...) {
    int i;
    va_list ap;
    va_start(ap, numArgs);
    ASTnode *traverse = args;
    /* check for missing/uninitialized/mistyped arguments */
    for (i = 0; i < numArgs; i++, traverse = traverse->next) {
        if (traverse == NULL)
            MkvsynthError("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), i);
        /* check type */
        int argType = va_arg(ap, int);
        if (traverse->type != argType)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[traverse->type]);
    }
    va_end(ap);
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeOptArg) {
        while ((traverse = traverse->next) != NULL) i++;
        MkvsynthError("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), ++i);
    }
}

/* dereference a variable */
ASTnode* dereference(ASTnode *p) {
    if (UNDEFINED(p))
        MkvsynthError("dereference called on uninitialized variable %s", p->var.name);
    ASTnode *d = newNode();
    memcpy(d, p->var.value, sizeof(ASTnode));
    d->next = p->next;
    return d;
}

/* execute a section of the AST */
ASTnode* ex(Env *e, ASTnode *p) {
    if (!p)
        return NULL;

    /* resolve identifiers */
    if (p->type == typeId)
        p = identify(e, p);

    /* dereference variables */
    if (p->type == typeVar)
        p = dereference(p);

    /* catch functions with no arguments */
    if (p->type == typeFn)
        p = fnctCall(e, p, p, NULL);

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
        case FNCT:    p = fnctCall(e, p, identify(e, child[0]), reduceArgs(e, child[1])); break;
        case CHAIN:   child[0]->next = child[2]; p = fnctCall(e, p, identify(e, child[1]), ex(e, child[0])); break;
        case DEFAULT: setDefault(e, child[0], ex(e, child[1])); break;
        case RETURN:  p = ex(e, child[0]); if (p != NULL) e->returnValue = p; longjmp(e->returnContext, 1); break;
        /* assignment */
        case ASSIGN:  p = assign(identify(e, child[0]), child[1]->num, ex(e, child[2])); break;
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
ASTnode* fnctCall(Env *e, ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    if (UNDEFINED(fnNode))
        MkvsynthError("reference to undefined function \"%s\"", fnNode->var.name);
    if (fnNode->type != typeFn)
        MkvsynthError("expected function name before '(' (got %s)", typeNames[fnNode->type]);

    if (fnNode->fn.type == fnCore)
        p = (*(fnNode->fn.core.fnPtr))(p, args);
    else
        p = userDefFnCall(e, p, fnNode, args);

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

    /* split parameters into mandatory and optional */
    if (splitMandOpt(params, &p->fn.user.params, &p->fn.user.opts))
        MkvsynthError("optional parameters must follow mandatory parameters in function %s", p->fn.name);

    /* add to local function table */
    p->next = e->fnTable;
    e->fnTable = p;
}

/* get optional arguments in a function call */
void* getOptArg(ASTnode *args, char *name, int type) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next) {
        if (traverse->type == typeOptArg && !(strncmp(traverse->var.name, name, strlen(name)))) {
            if (type != traverse->var.value->type)
                MkvsynthError("type mismatch: optional argument \"%s\" expected %s, got %s", name, typeNames[type], typeNames[traverse->var.value->type]);
            switch (type) {
                case typeNum:  return &traverse->var.value->num;
                case typeBool: return &traverse->var.value->bool;
                case typeStr:  return traverse->var.value->str;
                case typeClip: return traverse->var.value->clipIn;
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
    /* new variable */
    else
        p = putVar(e, p->id);

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

/* evaluate function arguments */
ASTnode* reduceArgs(Env *e, ASTnode *p) {
    if (!p)
        return NULL;

    /* recurse to next argument */
    ASTnode *next = reduceArgs(e, p->next);
    /* reduce argument */
    p = ex(e, p);
    /* reduce optional argument */
    if (p->type == typeOptArg)
        p->var.value = ex(e, p->var.value);
    /* link nodes back together */
    p->next = next;
    return p;
}

/* process a default statement */
ASTnode* setDefault(Env *e, ASTnode *paramNode, ASTnode *valueNode) {
    /* identify parameter */
    paramNode = getVar(e, paramNode->id);
    /* check that paramNode is an unset optional parameter */
    if (paramNode->var.opt == 1 && paramNode->var.value == NULL)
        assign(paramNode, '=', valueNode);
    return paramNode;
}

/* split a list of nodes into mandatory and optional lists */
/* return 0 on success, 1 if list is improperly ordered */
int splitMandOpt(ASTnode *list, ASTnode **mand, ASTnode **opts) {
    /* no nodes */
    if (list == NULL) {
        *mand = *opts = NULL;
        return 0;
    }
    /* one node */
    if (list->next == NULL) {
        *mand = list->var.opt == 1 ? NULL : list;
        *opts = list->var.opt == 1 ? list : NULL;
        return 0;
    }
    /* more than one node */
    /* check ordering */
    ASTnode *traverse = list;
    char check = 0;
    while (traverse) {
        if (traverse->var.opt == 1)
            check = 1;
        else if (check == 1)
            return 1;
        traverse = traverse->next;
    }
    /* no mandatory arguments */
    if (list->var.opt == 1) {
        *mand = NULL;
        *opts = list;
        return 0;
    }
    *mand = list;
    traverse = list;
    while (traverse->next && traverse->next->var.opt != 1)
        traverse = traverse->next;
    *opts = traverse->next;
    traverse->next = NULL;
    return 0;
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
ASTnode* userDefFnCall(Env *e, ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    /* create new environment */
    Env *local = (Env *) malloc(sizeof(Env));
    local->parent = e;
    local->returnValue = newNode();
    local->returnValue->type = typeFn; /* default return value */
    /* record function arguments in local variable table */
    ASTnode *pTraverse = fnNode->fn.user.params;
    while (pTraverse) {
        putVar(local, pTraverse->var.name);
        pTraverse = pTraverse->next;
    }
    pTraverse = fnNode->fn.user.opts;
    while (pTraverse) {
        putVar(local, pTraverse->var.name)->var.opt = 1;
        pTraverse = pTraverse->next;
    }

    /* split arguments into mandatory and optional */
    ASTnode *mandargs, *optargs;
    if (splitMandOpt(args, &mandargs, &optargs))
        MkvsynthError("optional arguments must follow mandatory arguments in function %s", fnNode->fn.name);

    /* check argument number */
    int i = 0, j = 0;
    pTraverse = fnNode->fn.user.params;
    ASTnode *aTraverse = mandargs;
    while (pTraverse && ++i)
        pTraverse = pTraverse->next;
    while (aTraverse && ++j)
        aTraverse = aTraverse ->next;
    if (i != j)
        MkvsynthError("%s expected %d mandatory argument%s, got %d", fnNode->fn.name, i, (i == 1) ? "" : "s", j);

    /* check for type mismatches */
    pTraverse = fnNode->fn.user.params;
    aTraverse = mandargs;
    while (pTraverse && aTraverse) {
        if (pTraverse->var.type != aTraverse->type)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", 
                i, fnNode->fn.name, typeNames[pTraverse->var.type], typeNames[aTraverse->type]);
        pTraverse = pTraverse->next;
        aTraverse = aTraverse->next;
    }

    /* all is well; record vars in local table and assign value */
    pTraverse = fnNode->fn.user.params;
    aTraverse = args;
    while (pTraverse && aTraverse) {
        assign(getVar(local, pTraverse->var.name), '=', aTraverse);
        pTraverse = pTraverse->next;
        aTraverse = aTraverse->next;
    }

    /* check optional arguments */
    aTraverse = optargs;
    while (aTraverse) {
        /* check that parameter exists */
        pTraverse = fnNode->fn.user.opts;
        while (pTraverse && strcmp(pTraverse->var.name, aTraverse->var.name) != 0)
            pTraverse = pTraverse->next;
        if (pTraverse == NULL)
            MkvsynthError("%s is not an optional parameter of function %s", aTraverse->var.name, fnNode->fn.name);
        /* check that types match */
        if (pTraverse->var.type != aTraverse->var.value->type)
            MkvsynthError("type mismatch: opt arg %s of %s expected %s, got %s",
                pTraverse->var.name, fnNode->fn.name, typeNames[pTraverse->var.type], typeNames[aTraverse->var.type]);
        /* assign value */
        assign(getVar(local, pTraverse->var.name), '=', aTraverse->var.value);
        aTraverse = aTraverse->next;
    }

    /* execute a copy of the function body in the local environment */
    if (setjmp(local->returnContext) == 0)
        ex(local, copy(fnNode->fn.user.body));

    /* return */
    return local->returnValue;
}
