#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "delbrot.h"
#include "y.tab.h"

#define UNDEFINED(n) n->type == typeVar && n->var.value == NULL

/* useful for error messages */
static char *typeNames[] = {"integer", "identifier", "string", "clip", "function", "variable", "optional argument", "operation"};

/* dereference a variable */
ASTnode* dereference(ASTnode *p) {
    if (UNDEFINED(p))
        yyerror("dereference called on uninitialized variable %s", p->var.name);
    ASTnode *d = newNode();
    memcpy(d, p->var.value, sizeof(ASTnode));
    d->next = p->next;
    return d;
}

/* assign or modify a variable */
ASTnode* assign(ASTnode *varNode, char op, ASTnode *valueNode) {
    /* standard assignment */
    if (op == '=') {
        if (varNode->type != typeVar)
            yyerror("can't assign to a constant value (got %s)", typeNames[varNode->type]);
        /* new variable */
        if (UNDEFINED(varNode))
            varNode->var.value = newNode();
        /* copy new value */
        return memcpy(varNode->var.value, valueNode, sizeof(ASTnode));
    }
    /* arithmetic operation + assignment */
    if (varNode->type != typeVar)
        yyerror("can't modify constant value (got %s)", typeNames[varNode->type]);
    if (UNDEFINED(varNode))
        yyerror("reference to uninitialized variable \"%s\"", varNode->var.name);
    if (varNode->var.value->type != typeVal)
        yyerror("can't modify non-numeric variable \"%s\"", varNode->var.name);
    if (valueNode && valueNode->type != typeVal)
        yyerror("can't modify variable %s with non-numeric type (expected integer, got %s)\n", varNode->var.name, typeNames[valueNode->type]);

    int mod;
    if (!valueNode)
        mod = 1;
    else
        mod = valueNode->val;

    switch (op) {
        case '+': varNode->var.value->val += mod; break;
        case '-': varNode->var.value->val -= mod; break;
        case '*': varNode->var.value->val *= mod; break;
        case '/': varNode->var.value->val /= mod; break;
        case '^': varNode->var.value->val = pow(varNode->var.value->val, mod); break;
        case '%': varNode->var.value->val = ((int)varNode->var.value->val % (int)mod); break;
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
            yyerror("out of memory");
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

/* process a run-time function definition */
/* TODO: handle optional arguments */
void funcDefine(Env *e, ASTnode *nameNode, ASTnode *paramNode, ASTnode *bodyNode) {
    if (nameNode->type != typeId)
        yyerror("function name \"%s\" is already in use", nameNode->var.name);

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

/* process the call of a function that was defined at run-time */
ASTnode* userDefFnCall(Env *e, ASTnode *p, ASTnode *fnNode, ASTnode *args) {
    /* create new environment */
    Env *local = (Env *) malloc(sizeof(Env));
    local->parent = e;

    /* check arguments */
    int i = 0, j = 0;
    ASTnode *pTraverse = fnNode->fn.user.params, *aTraverse = args;
    while (1) {
        if (!pTraverse && !aTraverse)
            break;
        if (pTraverse && !aTraverse) {
            while ((pTraverse = pTraverse->next)) i++;
            yyerror("%s expected %d argument(s), got %d", fnNode->fn.name, ++i, j);
        }
        if (!pTraverse && aTraverse) {
            while ((aTraverse = aTraverse->next)) j++;
            yyerror("%s expected %d argument(s), got %d", fnNode->fn.name, i, ++j);
        }
        if (pTraverse->type != aTraverse->type)
            yyerror("type mismatch: arg %d of %s expected %s, got %s", i, fnNode->fn.name, typeNames[pTraverse->type], typeNames[aTraverse->type]);

        /* all is well; record var in local table and assign value */
        assign(putVar(local, pTraverse->id), '=', aTraverse);

        pTraverse = pTraverse->next; i++;
        aTraverse = aTraverse->next; j++;
    }

    /* execute a copy of the function body in the local environment */
    ASTnode *ret;
    if (setjmp(local->returnContext) == 0)
        ex(local, copy(fnNode->fn.user.body));
    else
        ret = local->returnValue;

    /* TODO: free local env without breaking stuff */
    
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
        yyerror("reference to undefined function \"%s\"", fnNode->var.name);
    if (fnNode->type != typeFn)
        yyerror("expected function name before '(' (got %s)", typeNames[fnNode->type]);

    if (fnNode->fn.type == fnCore)
        p = (*(fnNode->fn.core.fnPtr))(p, args);
    else
        p = userDefFnCall(e, p, fnNode, args);

    return p;
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
        return dereference(p);

    /* only nodes with children can be reduced */
    if (p->type != typeOp)
        return p;

    /* for convenience */
    ASTnode **child = p->op.ops;

    switch(p->op.oper) {
        /* declarations */
        case FNDEF:  funcDefine(e, child[0], child[1], child[2]); break;
        /* keywords */
        case IF:     if (ex(e, child[0])->val) ex(e, child[1]); else if (p->op.nops > 2) ex(e, child[2]); p->type = typeOp; break;
        case WHILE:  while(ex(e, copy(child[0]))->val) ex(e, copy(child[1])); p->type = typeOp; break;
        case FOR:    for(ex(e, child[0]); ex(e, copy(child[1]))->val; ex(e, copy(child[2]))) ex(e, copy(child[3])); p->type = typeOp; break;
        /* functions */
        case FNCT:   p = fnctCall(e, p, identify(e, child[0]), reduceArgs(e, child[1])); break;
        case '.':    child[0]->next = child[2]; p = fnctCall(e, p, ex(e, child[1]), ex(e, child[0])); break;
        case RETURN: e->returnValue = ex(e, child[0]); longjmp(e->returnContext, 1); break;
        /* assignment */
        case ASSIGN: p = assign(identify(e, child[0]), child[1]->val, ex(e, child[2])); break;
        case INC:    p = assign(identify(e, child[0]), '+', mkValNode(1)); break; /* bit of a hack, maybe use mkValNode(1) instead? */
        case DEC:    p = assign(identify(e, child[0]), '-', mkValNode(1)); break;
        /* unary operators */
        case NEG:    p = nneg(p, ex(e, child[0])); break;
        case '!':    p = nnot(p, ex(e, child[0])); break;
        /* arithmetic / boolean operators */
        case BINOP:  p = binOp(p, child[1]->val, ex(e, child[0]), ex(e, child[2])); break;
        /* compound statements */
        case ';':    ex(e, child[0]); p = ex(e, child[1]); break;
        /* should never wind up here */
        default: yyerror("Unknown operator");
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
            yyerror("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), i);
        /* check type */
        int argType = va_arg(ap, int);
        if (traverse->type != argType)
            yyerror("type mismatch: arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[traverse->type]);
    }
    va_end(ap);
    /* check for excess arguments */
    if (traverse != NULL && traverse->type != typeOptArg) {
        while ((traverse = traverse->next) != NULL) i++;
        yyerror("%s expected %d argument%s, got %d", funcName, numArgs, (numArgs == 1 ? "" : "s"), ++i);
    }
}

/* helper function to get optional arguments in a function call */
void* getOptArg(ASTnode *args, char *name, int type) {
    ASTnode *traverse = args;
    for (traverse = args; traverse != NULL; traverse = traverse->next)
        if (traverse->type == typeOptArg && !(strncmp(traverse->opt.name, name, strlen(name)))) {
            if (type != traverse->opt.value->type)
                yyerror("type mismatch: optional argument \"%s\" expected %s, got %s", name, typeNames[type], typeNames[traverse->opt.value->type]);
            switch (type) {
                case typeVal: return &traverse->opt.value->val;
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
        /* printable types */
        switch(args->type) {
            case typeVal: printf("%.10g ", args->val); break;
            case typeStr: printf("%s ", unesc(args->str)); break;
            default: printf("[could not print type: %s] ", typeNames[args->type]); break;
        }
        args = args->next;
    }
    printf("\n");
    p->type = typeVal;
    p->val = 0;
    return p;
}

/* create a clip from a filename... eventually */
ASTnode* MKVsource(ASTnode *p, ASTnode *args) {
    checkArgs("MKVsource", args, 1, typeStr);
    p->type = typeClip;
    p->clipIn = NULL;
    return p;
}

/* handle arithmetic / boolean operators */
ASTnode* binOp(ASTnode* p, int op, ASTnode* c1, ASTnode* c2) {
    if (c1->type != typeVal) yyerror("type mismatch: LHS of %c expected integer, got %s", op, typeNames[c1->type]);
    if (c2->type != typeVal) yyerror("type mismatch: RHS of %c expected integer, got %s", op, typeNames[c2->type]);

    p->type = typeVal;
    switch(op) {
        /* arithmetic operators */
        case '+':  p->val = c1->val + c2->val; break;
        case '-':  p->val = c1->val - c2->val; break;
        case '*':  p->val = c1->val * c2->val; break;
        case '/':  p->val = c1->val / c2->val; break;
        case '^':  p->val = pow(c1->val, c2->val); break;
        case '%':  p->val = (double) ((int) c1->val % (int) c2->val); break;
        /* boolean operators */
        case EQ:   p->val = c1->val == c2->val; break;
        case NE:   p->val = c1->val != c2->val; break;
        case GT:   p->val = c1->val  > c2->val; break;
        case LT:   p->val = c1->val  < c2->val; break;
        case GE:   p->val = c1->val >= c2->val; break;
        case LE:   p->val = c1->val <= c2->val; break;
        case LOR:  p->val = c1->val || c2->val; break;
        case LAND: p->val = c1->val && c2->val; break;
        /* should never wind up here */
        default: yyerror("unrecognized binary operator");
    }
    return p;
}

/* handle unary operators */
ASTnode* nneg(ASTnode *p, ASTnode *c1) {
    if (c1->type != typeVal) yyerror("arg 1 of - expected integer, got %s", typeNames[c1->type]);
    p->type = typeVal;
    p->val = -c1->val;
    return p;
}

ASTnode* nnot(ASTnode *p, ASTnode *c1) {
    if (c1->type != typeVal) yyerror("arg 1 of ! expected integer, got %s", typeNames[c1->type]);
    p->type = typeVal;
    p->val = !c1->val;
    return p;
}

/* standard mathematical functions, modified to use ASTnode */
ASTnode* nsin(ASTnode *p, ASTnode *args) {
    checkArgs("sin", args, 1, typeVal);
    p->type = typeVal;
    p->val = sin(args->val); 
    return p;
}
ASTnode* ncos(ASTnode *p, ASTnode *args) {
    checkArgs("cos", args, 1, typeVal);
    p->type = typeVal;
    p->val = cos(args->val); 
    return p;
}
ASTnode* nlog(ASTnode *p, ASTnode *args) {
    checkArgs("log", args, 1, typeVal);
    p->type = typeVal;
    p->val = log(args->val); 
    return p;
}
ASTnode* nsqrt(ASTnode *p, ASTnode *args) {
    checkArgs("sqrt",args, 1, typeVal);
    p->type = typeVal;
    p->val = sqrt(args->val);
    return p;
}
