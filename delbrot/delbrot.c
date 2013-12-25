#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

#define UNDEFINED(n) n->type == typeVar && n->var.value == NULL

/* useful for error messages */
static char *typeNames[] = {"number", "boolean", "string", "clip", "identifier", "function", "variable", "optional argument", "operation"};

/* dereference a variable */
ASTnode* dereference(ASTnode *p) {
    if (UNDEFINED(p))
        MkvsynthError("dereference called on uninitialized variable %s", p->var.name);
    ASTnode *d = newNode();
    memcpy(d, p->var.value, sizeof(ASTnode));
    d->next = p->next;
    return d;
}

/* assign or modify a variable */
ASTnode* assign(ASTnode *varNode, int op, ASTnode *valueNode) {
    /* standard assignment */
    if (op == '=') {
        if (varNode->type != typeVar)
            MkvsynthError("can't assign to a constant value (got %s)", typeNames[varNode->type]);
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

/* process a function definition */
/* TODO: handle optional arguments */
void funcDefine(Env *e, ASTnode *nameNode, ASTnode *paramNode, ASTnode *bodyNode) {
    if (nameNode->type != typeId)
        MkvsynthError("function name \"%s\" is already in use", nameNode->var.name);

    /* create new function table entry */
    ASTnode *ptr = newNode();
    ptr->type = typeFn;
    ptr->fn.type = fnUser;
    ptr->fn.name = nameNode->id;
    ptr->fn.user.body = bodyNode;
    ptr->fn.user.params = paramNode;
    /* add to local function table */
    ptr->next = e->fnTable;
    e->fnTable = ptr;
}

/* process a user-defined function call */
ASTnode* userDefFnCall(Env *e, ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    /* create new environment */
    Env *local = (Env *) malloc(sizeof(Env));
    local->parent = e;

    /* check arguments */
    int i = 0, j = 0;
    ASTnode *pTraverse = fnNode->fn.user.params, *aTraverse = args;
    /* check for correct no. of arguments */
    while (pTraverse || aTraverse) {
        if (pTraverse) {
            pTraverse = pTraverse->next;
            i++;
        }
        if (aTraverse) {
            aTraverse = aTraverse->next;
            j++;
        }
    }
    if (i != j)
        MkvsynthError("%s expected %d argument%s, got %d", fnNode->fn.name, i, (i == 1) ? "" : "s", j);
    /* check for type mismatches */
    pTraverse = fnNode->fn.user.params;
    aTraverse = args;
    while (pTraverse && aTraverse) {
        if (pTraverse->type != aTraverse->type)
            MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", i, fnNode->fn.name, typeNames[pTraverse->type], typeNames[aTraverse->type]);
        pTraverse = pTraverse->next;
        aTraverse = aTraverse->next;
    }

    /* all is well; record var in local table and assign value */
    pTraverse = fnNode->fn.user.params;
    aTraverse = args;
    while (pTraverse && aTraverse) {
        assign(putVar(local, pTraverse->id), '=', aTraverse);
        pTraverse = pTraverse->next;
        aTraverse = aTraverse->next;
    }

    /* execute a copy of the function body in the local environment */
    ASTnode *ret;
    if (setjmp(local->returnContext) == 0)
        ex(local, copy(fnNode->fn.user.body));
    else
        ret = local->returnValue;

    /* return */
    return ret;
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
        p->opt.value = ex(e, p->opt.value);
    /* link nodes back together */
    p->next = next;
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

/* process an if/else statement */
void ifelse(Env *e, ASTnode *cond, ASTnode *ifNode, ASTnode *elseNode) {
    if (cond->type != typeBool)
        MkvsynthError("if expected boolean, got %s", typeNames[cond->type]);
    if (cond->bool == TRUE)
        ex(e, ifNode);
    else if (p->op.nops == 3)
        ex(e, elseNode);
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

/* process a default statement */
ASTnode* setDefault(Env *e, ASTnode *varNode, ASTnode *valueNode) {
    /* check that varNode is an unset optional parameter */
    if (UNDEFINED(varNode))
        memcpy(varNode->var.value, valueNode, sizeof(ASTnode));
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
        case IF:      ifelse(e, ex(e, child[0]), child[1], child[2]); break;
        /* functions */
        case FNCT:    p = fnctCall(e, p, identify(e, child[0]), reduceArgs(e, child[1])); break;
        case CHAIN:   child[0]->next = child[2]; p = fnctCall(e, p, ex(e, child[1]), ex(e, child[0])); break;
        case DEFAULT: setDefault(e, child[0], ex(e, child[1])); break;
        case RETURN:  e->returnValue = ex(e, child[0]); longjmp(e->returnContext, 1); break;
        /* assignment */
        case ASSIGN:  p = assign(identify(e, child[0]), child[1]->num, ex(e, child[2])); break;
        case INC:     p = assign(identify(e, child[0]), ADDEQ, mkNumNode(1)); break;
        case DEC:     p = assign(identify(e, child[0]), SUBEQ, mkNumNode(1)); break;
        /* unary operators */
        case NEG:     p = nneg(p, ex(e, child[0])); break;
        case '!':     p = nnot(p, ex(e, child[0])); break;
        /* binary operators */
        case BINOP:   p = binOp(p, ex(e, child[0]), child[1]->num, ex(e, child[2])); break;
        /* trinary operator */
        case TERN:    p = ternary(e, ex(e, child[0]), child[1], child[2]); break;
        /* compound statements */
        case ';':     ex(e, child[0]); p = ex(e, child[1]); break;
        /* should never wind up here */
        default: MkvsynthError("Unknown operator");
    }

    return p;
}

/* helper function to ensure that a function call is valid */
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

/* helper function to get optional arguments in a function call */
void* getOptArg(ASTnode *args, char *name, int type) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next)
        if (traverse->type == typeOptArg && !(strncmp(traverse->opt.name, name, strlen(name)))) {
            if (type != traverse->opt.value->type)
                MkvsynthError("type mismatch: optional argument \"%s\" expected %s, got %s", name, typeNames[type], typeNames[traverse->opt.value->type]);
            switch (type) {
                case typeNum: return &traverse->opt.value->num;
                case typeStr: return traverse->opt.value->str;
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
                default: MkvsynthError("unknown literal \"\\%c\"", str[i+1]);
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
        /* printable types */
        switch(args->type) {
            case typeNum: printf("%.10g ", args->num); break;
            case typeBool: printf("%s ", args->bool == TRUE ? "True" : "False"); break;
            case typeStr: printf("%s ", unesc(args->str)); break;
            default: printf("[could not print type %s] ", typeNames[args->type]); break;
        }
        args = args->next;
    }
    printf("\n");
    p->type = typeOp;
    return p;
}

/* create a clip from a filename... eventually */
ASTnode* MKVsource(ASTnode *p, ASTnode *args) {
    checkArgs("MKVsource", args, 1, typeStr);
    p->type = typeClip;
    p->clipIn = NULL;
    return p;
}

/* exit with error message if assertion fails */
/* TODO: expand to allow for printf-formatted error msgs */
ASTnode* assert(ASTnode *p, ASTnode *args) {
    checkArgs("assert", args, 2, typeBool, typeStr);
    if (args->bool == FALSE)
        MkvsynthError(args->next->str);
}

/* handle arithmetic / boolean operators */
ASTnode* binOp(ASTnode* p, ASTnode* c1, int op, ASTnode* c2) {
    /* arithmetic operator argument check */
    if (op < 100) {
        if (c1->type != typeNum) MkvsynthError("type mismatch: LHS of %c expected number, got %s", op, typeNames[c1->type]);
        if (c2->type != typeNum) MkvsynthError("type mismatch: RHS of %c expected number, got %s", op, typeNames[c2->type]);
        p->type = typeNum;
    }

    switch(op) {
        /* arithmetic operators */
        case '+':  p->num = c1->num + c2->num; break;
        case '-':  p->num = c1->num - c2->num; break;
        case '*':  p->num = c1->num * c2->num; break;
        case '/':  p->num = c1->num / c2->num; break;
        case '^':  p->num = pow(c1->num, c2->num); break;
        case '%':  p->num = (double) ((int) c1->num % (int) c2->num); break;
        /* boolean operators */
        /* TODO: polymorphism for non-numeric types */
        case EQ:   p->bool = c1->num == c2->num ? TRUE : FALSE; break;
        case NE:   p->bool = c1->num != c2->num ? TRUE : FALSE; break;
        case GT:   p->bool = c1->num  > c2->num ? TRUE : FALSE; break;
        case LT:   p->bool = c1->num  < c2->num ? TRUE : FALSE; break;
        case GE:   p->bool = c1->num >= c2->num ? TRUE : FALSE; break;
        case LE:   p->bool = c1->num <= c2->num ? TRUE : FALSE; break;
        case LOR:  p->bool = c1->num || c2->num ? TRUE : FALSE; break;
        case LAND: p->bool = c1->num && c2->num ? TRUE : FALSE; break;
        /* should never wind up here */
        default: MkvsynthError("unrecognized binary operator");
    }

    p->type = op < 100 ? typeNum : typeBool;
    return p;
}

/* handle unary operators */
ASTnode* nneg(ASTnode *p, ASTnode *c1) {
    if (c1->type != typeNum) MkvsynthError("arg 1 of - expected number, got %s", typeNames[c1->type]);
    p->type = typeNum;
    p->num = -c1->num;
    return p;
}

ASTnode* nnot(ASTnode *p, ASTnode *c1) {
    if (c1->type != typeBool) MkvsynthError("arg 1 of ! expected boolean, got %s", typeNames[c1->type]);
    p->type = typeBool;
    p->bool = c1->bool ? FALSE : TRUE;
    return p;
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nsin(ASTnode *p, ASTnode *args) {
    checkArgs("sin", args, 1, typeNum);
    p->type = typeNum;
    p->num = sin(args->num); 
    return p;
}
ASTnode* ncos(ASTnode *p, ASTnode *args) {
    checkArgs("cos", args, 1, typeNum);
    p->type = typeNum;
    p->num = cos(args->num); 
    return p;
}
ASTnode* nlog(ASTnode *p, ASTnode *args) {
    checkArgs("log", args, 1, typeNum);
    p->type = typeNum;
    p->num = log(args->num); 
    return p;
}
ASTnode* nsqrt(ASTnode *p, ASTnode *args) {
    checkArgs("sqrt",args, 1, typeNum);
    p->type = typeNum;
    p->num = sqrt(args->num);
    return p;
}
