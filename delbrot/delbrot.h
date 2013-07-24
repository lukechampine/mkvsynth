/* types */
typedef enum { typeVal, typeId, typeStr, typeFn, typeVar, typeOptArg, typeOp } nodeType;

/* an operator node */
typedef struct ASTnode ASTnode;
typedef struct {
    int oper;               /* operator */
    int nops;               /* number of operands */
    struct ASTnode **ops;   /* operands */
} opNode;

/* a generic node in the AST */
/* TOOD: add line number field for error messages */
typedef struct funcRec funcRec;
typedef struct varRec varRec;
struct ASTnode {
    nodeType type;          /* type of node */
    union {
        double  val;        /* value */
        char   *str;        /* string */
        funcRec *fn;        /* function pointer */
        varRec *var;        /* variable */
        opNode   op;        /* operator */
    };
    varRec **scope;         /* variable scope */
    struct ASTnode *next;   /* used for argument linked lists */
};

#define YYSTYPE ASTnode*

/* ASTnode prototypes */
ASTnode* newNode();
void     freeAll();
ASTnode* tempNode();
void     freeTemp();
void     protect(ASTnode *);
ASTnode* mkIdNode(char *);
ASTnode* mkValNode(double);
ASTnode* mkStrNode(char *);
ASTnode* mkOpNode(int, int, ...);
ASTnode* mkOptArgNode(char *);
ASTnode* append(ASTnode *, ASTnode *);
ASTnode* ex(ASTnode *);

/* a function */
/* arg1 is passed by reference, and will contain result of the function */
/* arg2 is a linked list of ASTnodes, allowing an arbitrary number of arguments to be passed */
typedef ASTnode * (*func) (ASTnode *, ASTnode *);
struct funcRec {
    char *name;             /* function name */
    func ptr;               /* BUILT-IN: function pointer */
    ASTnode *body;          /* USER-DEFINED: function body */
    varRec *localVars;      /* USER-DEFINED: local variables */
    struct funcRec *next;   /* link field */
};

/* the function table */
extern funcRec *fnTable;

/* a variable  */
struct varRec {
    char *name;             /* variable name */
    ASTnode *value;         /* ASTnode it references */
    struct varRec *next;    /* link field */
};

/* the global variable table */
extern varRec *globalVars;

/* variable/function access prototypes */
varRec* putVar(ASTnode *);
varRec* getVar(ASTnode *);
funcRec* putFn(funcRec *);
funcRec* getFn(char const *);
void* getOptArg(ASTnode *args, char *name, int type);
void checkArgs(char *funcName, ASTnode *args, int numArgs, ...);

/* standard mathematical function prototypes */
ASTnode* nneg(ASTnode *, ASTnode *);
ASTnode* nnot(ASTnode *, ASTnode *);
ASTnode* nsin(ASTnode *, ASTnode *);
ASTnode* ncos(ASTnode *, ASTnode *);
ASTnode* nlog(ASTnode *, ASTnode *);
ASTnode* nsqrt(ASTnode *, ASTnode *);
ASTnode* binOp(ASTnode *, int op, ASTnode *, ASTnode *);

/* mkvsynth function prototypes */
ASTnode* ffmpegDecode_AST(ASTnode *, ASTnode *);
ASTnode* print(ASTnode *, ASTnode *);

/* user-defined functions */
extern funcRec pluginFunctions[];

/* helpful plugin macros */
#define MANDVAL() args->val; args = args->next
#define MANDSTR() args->str; args = args->next
#define OPTVAL(name, default) getOptArg(args, name, typeVal) ? *((double *) getOptArg(args, name, typeVal)) : default
#define OPTSTR(name, default) getOptArg(args, name, typeStr) ?     (char *) getOptArg(args, name, typeStr)  : default

#define RETURNVAL(value) p->type = typeVal; p->val = value; return p
#define RETURNSTR(str)   p->type = typeStr; p->str = str;   return p