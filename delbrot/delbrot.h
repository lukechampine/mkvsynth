#ifndef delbrot_h_
#define delbrot_h_

/* includes */
#include <setjmp.h>
#include "../jarvis/jarvis.h"

/* macros */
#define YYSTYPE ASTnode* /* all tokens are ASTnode */
#define bool_t int       /* boolean type alias */
/* useful abstractions for writing plugins */
#define MANDNUM(n)  a->args[n].value->num
#define MANDBOOL(n) a->args[n].value->bool
#define MANDSTR(n)  a->args[n].value->str
#define MANDCLIP(n) a->args[n].value->clipOut
#define OPTNUM(name, default)  getOptArg(a, name, typeNum)  ?      *((double *) getOptArg(a, name, typeNum))  : default
#define OPTBOOL(name, default) getOptArg(a, name, typeBool) ?        *((bool_t) getOptArg(a, name, typeBool)) : default
#define OPTSTR(name, default)  getOptArg(a, name, typeStr)  ?          (char *) getOptArg(a, name, typeStr)   : default
#define OPTCLIP(name, default) getOptArg(a, name, typeClip) ? (MkvsynthInput *) getOptArg(a, name, typeClip)  : default
#define RETURNNUM(_num)   p->type = typeNum;  p->num     = _num;  return p
#define RETURNBOOL(_bool) p->type = typeBool; p->bool    = _bool; return p
#define RETURNSTR(_str)   p->type = typeStr;  p->str     = _str;  return p
#define RETURNCLIP(clip)  p->type = typeClip; p->clipOut = clip;  return p
#define RETURNNULL()      p->type = typeFn; return p

/* enums */
typedef enum { fnCore, fnUser } fnType; /* function types */
typedef enum { typeNum, typeBool, typeStr, typeClip,
    typeId, typeVar, typeFn, typeOp, typeArg, typeOptArg, typeParam, typeOptParam
} nodeType; /* internal node types */

/* forward definitions */
typedef struct ASTnode ASTnode;
typedef struct Env Env;
typedef struct argList argList;

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
    ASTnode * (*fnPtr) (ASTnode *, argList *);
} coreFn;

/* a user-defined function */
typedef struct {
    argList *params;
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
    ASTnode * (*fnPtr) (ASTnode *, argList *);
} fnEntry;

/* a variable node (also used for function arguments) */
typedef struct {
    nodeType type; /* either variable, (opt)arg, or (opt)param */
    char *name;
    ASTnode *value;
} varNode;

/* an argument list */
struct argList {
    int nargs;     /* number of arguments */
    varNode *args; /* arguments */
};

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

/* a loaded plugin */
typedef struct Plugin Plugin;
struct Plugin {
    char *name;
    void *handle;
    Plugin *next;
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
ASTnode* getPluginFn(ASTnode *, ASTnode *);
ASTnode* getVar(Env const *, char const *);
ASTnode* putFn(Env *, fnEntry);
ASTnode* putVar(Env *, char const *);
/* AST evaluation */
ASTnode* ex(Env *, ASTnode *);
void checkArgs(char *, argList *, int, ...);
void* getOptArg(argList *, char *, int);

/* global variables */
Env *global; /* the global execution environment */
Plugin *pluginList; /* loaded plugins */
extern fnEntry coreFunctions[];
extern fnEntry internalFilters[];
extern char *typeNames[];
#endif
