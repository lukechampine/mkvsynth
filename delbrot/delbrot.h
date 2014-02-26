#ifndef delbrot_h_
#define delbrot_h_

/* includes */
#include <setjmp.h>
#include "../jarvis/jarvis.h"
#include "y.tab.h"

/* macros */
#define YYSTYPE ASTnode* /* all tokens are ASTnode */
#define bool_t int       /* boolean type alias */
/* useful abstractions for writing plugins */
#define MANDNUM()  args->num;     args = args->next
#define MANDBOOL() args->bool;    args = args->next
#define MANDSTR()  args->str;     args = args->next
#define MANDCLIP() args->clipOut; args = args->next
#define OPTNUM(name, default)  getOptArg(args, name, typeNum)  ?      *((double *) getOptArg(args, name, typeNum))  : default
#define OPTBOOL(name, default) getOptArg(args, name, typeBool) ?        *((bool_t) getOptArg(args, name, typeBool)) : default
#define OPTSTR(name, default)  getOptArg(args, name, typeStr)  ?          (char *) getOptArg(args, name, typeStr)   : default
#define OPTCLIP(name, default) getOptArg(args, name, typeClip) ? (MkvsynthInput *) getOptArg(args, name, typeClip)  : default
#define RETURNNUM(_num)   p->type = typeNum;  p->num     = _num;  return p
#define RETURNBOOL(_bool) p->type = typeBool; p->bool    = _bool; return p
#define RETURNSTR(_str)   p->type = typeStr;  p->str     = _str;  return p
#define RETURNCLIP(clip)  p->type = typeClip; p->clipOut = clip;  return p
#define RETURNNULL()     p->type = typeFn; return p;

/* enums */
typedef enum { fnCore, fnUser } fnType; /* function types */
typedef enum { typeNum, typeBool, typeStr, typeClip,
    typeId, typeVar, typeOptArg, typeFn, typeOp
} nodeType; /* internal node types */

/* structs */
typedef struct ASTnode ASTnode;
typedef struct Env Env;

/* an environment */
struct Env {
    ASTnode *varTable;      /* the local variable table */
    ASTnode *fnTable;       /* the local function table */
    jmp_buf returnContext;  /* where to jump to when returning */
    ASTnode *returnValue;   /* the return value */
    Env *parent;            /* the calling environment */
};

/* a core function */
typedef struct {
    ASTnode * (*fnPtr) (ASTnode *, ASTnode *);
} coreFn;

/* a user-defined function */
typedef struct {
    ASTnode *params;
    ASTnode *opts;
    ASTnode *body;
} userFn;

/* a function node */
typedef struct {
    fnType type;
    char *name;
    union {
        coreFn core;
        userFn user;
    };
} fnNode;

/* used for defining core functions */
typedef struct {
    char *name;
    ASTnode * (*fnPtr) (ASTnode *, ASTnode *);
} fnEntry;

/* a variable node (also used for function arguments) */
typedef struct {
    char opt;
    nodeType type;
    char *name;
    ASTnode *value;
} varNode;

/* an operator node */
typedef struct {
    int oper;        /* operator symbol */
    int nops;        /* number of operands */
    ASTnode **ops;   /* operands */
} opNode;

/* a node in the AST */
struct ASTnode {
    nodeType type;
    union {
        double  num;
        bool_t bool;
        char    *id;
        char   *str;
        MkvsynthInput *clipIn;
        MkvsynthOutput *clipOut;
        fnNode   fn;
        varNode var;
        opNode   op;
    };
    ASTnode *next;
};

/* a loaded library */
typedef struct Lib Lib;
struct Lib {
    char *name;
    void *handle;
    Lib *next;
};

/* function declarations */
/* error handling */
void MkvsynthError(char *, ...);
/* AST creation */
void freeNodes();
ASTnode* append(ASTnode *, ASTnode *);
ASTnode* initList(ASTnode *);
ASTnode* mkBoolNode(int);
ASTnode* mkIdNode(char *);
ASTnode* mkNumNode(double);
ASTnode* mkOpNode(int, int, ...);
ASTnode* mkOptArgNode(ASTnode *, ASTnode *);
ASTnode* mkParamNode(char, int, ASTnode *);
ASTnode* mkStrNode(char *);
ASTnode* newNode();
/* variable/function access prototypes */
ASTnode* getFn(Env const *, char const *);
ASTnode* getLibFn(ASTnode *, ASTnode *);
ASTnode* getVar(Env const *, char const *);
ASTnode* putFn(Env *, fnEntry);
ASTnode* putVar(Env *, char const *);
/* AST evaluation */
ASTnode* ex(Env *, ASTnode *);
void checkArgs(char *, ASTnode *, int, ...);
void* getOptArg(ASTnode *, char *, int);

/* global variables */
Env *global; /* the global execution environment */
Lib *libList; /* loaded libraries */
extern fnEntry coreFunctions[];
extern fnEntry internalFilters[];
#endif
