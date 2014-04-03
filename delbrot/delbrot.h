#ifndef delbrot_h_
#define delbrot_h_

/* includes */
#include <setjmp.h>
#include "../jarvis/jarvis.h"

/* macros */
#define YYSTYPE ASTnode /* all tokens are ASTnodes */
#define bool_t int      /* boolean type alias */
/* useful abstractions for writing plugins */
#define MANDNUM(n)  a->args[n].value.num
#define MANDBOOL(n) a->args[n].value.bool
#define MANDSTR(n)  a->args[n].value.str
#define MANDCLIP(n) a->args[n].value.clipOut
#define OPTNUM(name, default)  getOptArg(a, name, typeNum)  ?      *((double *) getOptArg(a, name, typeNum))  : default
#define OPTBOOL(name, default) getOptArg(a, name, typeBool) ?      *((bool_t *) getOptArg(a, name, typeBool)) : default
#define OPTSTR(name, default)  getOptArg(a, name, typeStr)  ?          (char *) getOptArg(a, name, typeStr)   : default
#define OPTCLIP(name, default) getOptArg(a, name, typeClip) ? (MkvsynthInput *) getOptArg(a, name, typeClip)  : default
#define RETURNNUM(_num)   { Value v; v.type = typeNum;  v.num     = _num;  return v; }
#define RETURNBOOL(_bool) { Value v; v.type = typeBool; v.bool    = _bool; return v; }
#define RETURNSTR(_str)   { Value v; v.type = typeStr;  v.str     = _str;  return v; }
#define RETURNCLIP(_clip) { Value v; v.type = typeClip; v.clipOut = _clip; return v; }
#define RETURNNULL()      { Value v; v.type = typeNull;                    return v; }

/* enums */
typedef enum { fnCore, fnUser } fnType; /* Fn types */
typedef enum { typeVar, typeArg, typeOptArg, typeParam, typeOptParam } varType; /* Var types */
typedef enum { typeNull, typeNum, typeBool, typeStr, typeClip, typeId } valueType; /* Value types */

/* forward definitions */
typedef struct Env Env;
typedef struct ASTnode ASTnode;
typedef struct Value Value;
typedef struct Var Var;
typedef struct Fn Fn;

/* payload of an ASTnode */
struct Value {
    valueType type;
    union {
        double num;
        bool_t bool;
        char *str;
        MkvsynthInput *clipIn;
        MkvsynthOutput *clipOut;
        char *id;
        Var *arg;
    };
};

/* an environment */
struct Env {
    Var *varTable;         /* the local variable table */
    Fn *fnTable;           /* the local function table */
    jmp_buf returnContext; /* where to jump to when returning */
    Value returnValue;     /* the return value */
    Env *parent;           /* the calling environment */
};

/* a node in the AST */
struct ASTnode {
    int op;         /* operator -- 0 if leaf node */
    Value value;    /* payload */
    int nops;       /* no. of child nodes -- 0 if leaf node */
    ASTnode *child; /* child nodes -- NULL if leaf node */
};

/* a variable (also used for function arguments) */
struct Var {
    varType type; /* either variable, (opt)arg, or (opt)param */
    valueType valType;
    char *name;
    Value value;
    ASTnode fnArg;
    Var *next; /* for variable tables */
};

/* an argument list */
typedef struct {
    int nargs; /* number of arguments */
    Var *args; /* arguments */
} argList;

/* a function */
struct Fn {
    fnType type; /* either core or user */
    char *name;
    /* if core */
    Value (*fnPtr) (argList *);
    /* if user */
    Env *parent;
    argList *params;
    ASTnode *body;
    Fn *next; /* for function tables */
};

/* a loaded plugin */
typedef struct Plugin {
    char *name;
    void *handle;
    struct Plugin *next;
} Plugin;

/* function declarations */
/* error handling */
void MkvsynthError(char *, ...);
void MkvsynthMessage(char *, ...);
void MkvsynthWarning(char *, ...);
/* AST creation */
ASTnode append(ASTnode *, ASTnode *);
ASTnode makeNode(int, int, ...);
ASTnode makeLeaf(valueType, ...);
ASTnode makeArg(ASTnode *, ASTnode *);
ASTnode makeParam(varType, ASTnode *, ASTnode *);
/* AST destruction */
void freeNode(ASTnode *);
void freeFn(Fn *);
void freeValue(Value *);
void freeVar(Var *);
void freeEnv(Env *);
/* variable/function access */
ASTnode addPluginFn(ASTnode *, ASTnode *);
Fn* getFn(Env const *, char const *);
Var* getVar(Env const *, char const *);
void putFn(Env *, Fn *);
void putVar(Env *, char const *, varType type);
Value setVar(Env const *, char const *, Value const *);
/* AST evaluation */
Value ex(Env *, ASTnode *);
void checkArgs(argList *, int, ...);
void* getOptArg(argList *, char *, valueType);

/* global variables */
Env global; /* the global execution environment */
Plugin *pluginList; /* loaded plugins */
extern Fn coreFunctions[];
extern Fn internalFilters[];
extern char *typeNames[];
#endif
