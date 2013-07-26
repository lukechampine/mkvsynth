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

/* return value */
static ASTnode *returnValue;

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
        case '^': varNode->var->value->val = pow(varNode->var->value->val, mod); break;
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
    /* TODO: can this be changed to copy only the value and type? */
    ASTnode *next = p->next;
    memcpy(p, p->var->value, sizeof(ASTnode));
    p->next = next;
    return p;
}

/* copy a node and it's children */
ASTnode *copy(ASTnode *p) {
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
/* TODO: allow variables to be defined inside function definitions */
/* TODO: implement return statements */
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
        if ((v->value = malloc(sizeof(ASTnode))) == NULL)
            yyerror("out of memory");
        v->value->type = paramNode->type;
    }

    /* propagate scope to function body */
    fn->body = copy(bodyNode);
    propagateScope(fn->body, &fn->localVars);

    putFn(fn);
}

/* find a return statement in a function body and set its jump point */
void setReturnPoint(ASTnode *p, jmp_buf *ret) {
    if (!p || p->type != typeOp)
        return;

    if (p->op.oper == RETURN) {
        p->op.ops[0]->returnContext = ret;
        return;
    }

    /* recurse to children */
    int i;
    for (i = 0; i < p->op.nops; i++)
        setReturnPoint(p->op.ops[i], ret);
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
        ex(args);
        if (args->type != fnVars->value->type)
            yyerror("arg %d of %s expected %s, got %s", i+1, fnNode->fn->name, typeNames[fnVars->value->type], typeNames[args->type]);

        memcpy(fnVars->value, args, sizeof(ASTnode));
    }
    /* excessive arguments */
    if (args) {
        while ((args = args->next) != NULL) i++;
        yyerror("%s expected %d argument(s), got %d", fnNode->fn->name, numArgs, ++i);
    }

    /* set return point */
    jmp_buf ret;
    setReturnPoint(fnNode->fn->body, &ret);

    /* make a copy of the function body and execute it */
    if (!setjmp(ret))
        ex(copy(fnNode->fn->body));
    return (p = returnValue);
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

/* execute a section of the AST */
ASTnode* ex(ASTnode *p) {
    if (!p)
        return NULL;

    /* resolve identifiers */
    if (p->type == typeId)
        identify(p);

    /* dereference variables */
    if (p->type == typeVar)
        return dereference(p);

    /* only nodes with children should be evaluated */
    if (p->type != typeOp)
        return p;

    /* make a copy of all the child pointers */
    int nops = p->op.nops;
    ASTnode **child;
    if ((child = malloc(nops * sizeof(ASTnode *))) == NULL)
        yyerror("out of memory");
    memcpy(child, p->op.ops, nops * sizeof(ASTnode *));

    switch(p->op.oper) {
        /* declarations */
        case FNDEF: funcDefine(child[0], child[1], child[2]); break;
        case '=':   p = assign(identify(child[0]), ex(child[1])); break;
        /* keywords */
        case IF:    if (ex(child[0])->val) ex(child[1]); else if (nops > 2) ex(child[2]); p->type = typeOp; break;
        case WHILE: while(ex(copy(child[0]))->val) ex(copy(child[1])); p->type = typeOp; break;
        case FOR:   for(ex(child[0]); ex(copy(child[1]))->val; ex(copy(child[2]))) ex(copy(child[3])); p->type = typeOp; break;
        /* functions */
        case FNCT:  p = fnctCall(p, ex(child[0]), child[1]); break;
        case '.':   child[0]->next = child[2]; p = fnctCall(p, ex(child[1]), ex(child[0])); break;
        case RETURN:if(!child[0]->returnContext) yyerror("encountered return statement outside function body"); returnValue = ex(child[0]); longjmp(*child[0]->returnContext, 1); break;
        /* assignment */
        case ADDEQ: p = modvar(identify(child[0]), '+', ex(child[1])); break;
        case SUBEQ: p = modvar(identify(child[0]), '-', ex(child[1])); break;
        case MULEQ: p = modvar(identify(child[0]), '*', ex(child[1])); break;
        case DIVEQ: p = modvar(identify(child[0]), '/', ex(child[1])); break;
        case POWEQ: p = modvar(identify(child[0]), '^', ex(child[1])); break;
        case MODEQ: p = modvar(identify(child[0]), '%', ex(child[1])); break;
        case INC:   p = modvar(identify(child[0]), '+', NULL); break; /* bit of a hack */
        case DEC:   p = modvar(identify(child[0]), '-', NULL); break;
        /* unary operators */
        case NEG:   p = nneg(p, ex(child[0])); break;
        case '!':   p = nnot(p, ex(child[0])); break;
        /* arithmetic / boolean operators */
        case BINOP: p = binOp(p, child[1]->val, ex(child[0]), ex(child[2])); break;
        /* compound statements */
        case ';':   ex(child[0]); p = ex(child[1]); break;
        /* should never wind up here */
        default: yyerror("Unknown operator");
    }

    /* free children, since they are no longer needed */
    int i;
    for(i = 0; i < nops; i++) {
        freeNode(child[i]);
    }
    free(child);

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
            yyerror("%s expected %d argument(s), got %d", funcName, numArgs, i);
        /* evaluate argument */
        ex(traverse);
        /* check type */
        int argType = va_arg(ap, int);
        if (traverse->type != argType)
            yyerror("arg %d of %s expected %s, got %s", i+1, funcName, typeNames[argType], typeNames[traverse->type]);
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
            ex(traverse->var->value);
            switch (type) {
                case typeVal: return &traverse->var->value->val;
                case typeStr: return traverse->var->value->str;
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
        if (args->type == typeId)
            args = ex(args);
        if(args->type == typeVar || args->type == typeOp)
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
    p->type = typeVal;
    p->val = 0;
    return p;
}

/* toy ffmpeg decoding function, showcasing optional arguments */
ASTnode* ffmpegDecode_AST(ASTnode *p, ASTnode *args) {

    checkArgs("ffmpegDecode", args, 1, typeStr);

    char *filename = MANDSTR();
    double numFrames = OPTVAL("frames", -1);

    if (numFrames != -1)
        printf("decoded %f frames of %s\n", numFrames, filename);
    else
        printf("decoded %s\n", filename);

    RETURNVAL(0);
}

/* handle arithmetic / boolean operators */
ASTnode* binOp(ASTnode* p, int op, ASTnode* c1, ASTnode* c2) {
    if (c1->type != typeVal) yyerror("arg 1 of %c expected integer, got %s", op, typeNames[c1->type]);
    if (c2->type != typeVal) yyerror("arg 2 of %c expected integer, got %s", op, typeNames[c2->type]);

    p->type = typeVal;
    switch(op) {
        /* arithmetic operators */
        case '+':  p->val = c1->val + c2->val; break;
        case '-':  p->val = c1->val + c2->val; break;
        case '*':  p->val = c1->val + c2->val; break;
        case '/':  p->val = c1->val + c2->val; break;
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
ASTnode* nsin (ASTnode *p, ASTnode *args) {
    checkArgs("sin", args, 1, typeVal);
    p->type = typeVal;
    p->val = sin(args->val); 
    return p;
}
ASTnode* ncos (ASTnode *p, ASTnode *args) {
    checkArgs("cos", args, 1, typeVal);
    p->type = typeVal;
    p->val = cos(args->val); 
    return p;
}
ASTnode* nlog (ASTnode *p, ASTnode *args) {
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