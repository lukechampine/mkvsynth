#ifndef delbrot_h_
#define delbrot_h_

#include <setjmp.h>
#include "../jarvis/jarvis.h"

#define bool_t int

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

/* the global environment */
extern Env *global;

/* types of nodes */
typedef enum { 
    typeNum, typeBool, typeStr, typeClip,
    typeId, typeFn, typeVar, typeOptArg, typeOp,
    typeOptNum, typeOptBool, typeOptStr, typeOptClip,
} nodeType;

/* a core or plugin function */
typedef struct {
    ASTnode * (*fnPtr) (ASTnode *, ASTnode *);
} coreFn;

/* a user-defined function */
typedef struct {
    ASTnode *params;
    ASTnode *body;
} userFn;

/* a function node */
typedef enum { fnCore, fnUser } fnType;
typedef struct {
    fnType type;
    char *name;
    union {
        coreFn core;
        userFn user;
    };
} fnNode;

/* a variable node (also used for optional arguments) */
typedef struct {
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
        varNode opt;
        opNode   op;
    };
    ASTnode *next;
};

/* all tokens are ASTnodes */
#define YYSTYPE ASTnode*

/* error handling function */
void yyerror(char *, ...);
void MkvsynthError(char *, ...);

/* ASTnode prototypes */
ASTnode* newNode();
void     freeNodes();
ASTnode* mkIdNode(char *);
ASTnode* mkNumNode(double);
ASTnode* mkBoolNode(int);
ASTnode* mkStrNode(char *);
ASTnode* mkOpNode(int, int, ...);
ASTnode* mkOptArgNode(ASTnode *, ASTnode *);
ASTnode* initList(ASTnode *);
ASTnode* append(ASTnode *, ASTnode *);
ASTnode* ex(Env *, ASTnode *);

/* used for defining core/plugin functions */
typedef struct {
    char *name;
    ASTnode * (*fnPtr) (ASTnode *, ASTnode *);
} fnEntry;

/* variable/function access prototypes */
ASTnode* putVar(Env *, char const *);
ASTnode* getVar(Env const *, char const *);
ASTnode* putFn(Env *, fnEntry);
ASTnode* getFn(Env const *, char const *);
void checkArgs(char *funcName, ASTnode *args, int numArgs, ...);
void* getOptArg(ASTnode *args, char *name, int type);

/* standard mathematical function prototypes */
ASTnode* nneg(ASTnode *, ASTnode *);
ASTnode* nnot(ASTnode *, ASTnode *);
ASTnode* nsin(ASTnode *, ASTnode *);
ASTnode* ncos(ASTnode *, ASTnode *);
ASTnode* nlog(ASTnode *, ASTnode *);
ASTnode* nsqrt(ASTnode *, ASTnode *);
ASTnode* binOp(ASTnode *, ASTnode *, int op, ASTnode *);

/* mkvsynth function prototypes */
ASTnode* MKVsource(ASTnode *, ASTnode *);
ASTnode* assert(ASTnode *, ASTnode *);
ASTnode* print(ASTnode *, ASTnode *);

/* plugin functions */
extern fnEntry pluginFunctions[];

/* helpful plugin macros */
#define MANDNUM()  args->num;    args = args->next
#define MANDBOOL() args->bool;   args = args->next
#define MANDSTR()  args->str;    args = args->next
#define MANDCLIP() args->clipOut; args = args->next
#define OPTNUM(name, default)  getOptArg(args, name, typeNum)  ?      *((double *) getOptArg(args, name, typeNum))  : default
#define OPTBOOL(name, default) getOptArg(args, name, typeBool) ?        *((char *) getOptArg(args, name, typeBool)) : default
#define OPTSTR(name, default)  getOptArg(args, name, typeStr)  ?          (char *) getOptArg(args, name, typeStr)   : default
#define OPTCLIP(name, default) getOptArg(args, name, typeClip) ? (MkvsynthInput *) getOptArg(args, name, typeClip)  : default

#define RETURNNUM(num)   p->type = typeNum;  p->num     = num;  return p
#define RETURNBOOL(bool) p->type = typeBool; p->bool    = bool; return p
#define RETURNSTR(str)   p->type = typeStr;  p->str     = str;  return p
#define RETURNCLIP(clip) p->type = typeClip; p->clipOut = clip; return p

#endif
