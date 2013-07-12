/* types */
typedef enum { typeVal, typeStr, typeFn, typeVar, typeParam, typeOp } nodeType;

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
    int readonly;           /* determines whether ex() will consume node */
    union {
        double  val;        /* value */
        char   *str;        /* string */
        funcRec *fn;        /* function pointer */
        varRec *var;        /* variable */
        opNode   op;        /* operator */
    };
    struct ASTnode *next;   /* used for argument linked lists */
};

#define YYSTYPE ASTnode*

/* ASTnode prototypes */
ASTnode* newNode(int);
ASTnode* mkIdNode(char *);
ASTnode* mkValNode(double);
ASTnode* mkStrNode(char *);
ASTnode* mkTypeNode(int);
ASTnode* mkOpNode(int, int, ...);
ASTnode* mkParamNode(char *);
ASTnode* append(ASTnode *, ASTnode *);
ASTnode* ex(ASTnode *);
void     freeNodes(int);
void     setReadOnly(ASTnode *);

/* a function */
/* arg1 is passed by reference, and will contain result of the function */
/* arg2 is a linked list of ASTnodes, allowing an arbitrary number of arguments to be passed */
typedef ASTnode * (*func) (ASTnode *, ASTnode *);
struct funcRec {
    char *name;             /* function name */
    func ptr;               /* function pointer */
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

/* the variable table */
extern varRec *varTable;

/* variable/function access prototypes */
varRec* putVar(char const *);
varRec* getVar(char const *);
funcRec* putFn(funcRec *);
funcRec* getFn(char const *);
void* getOptArg(ASTnode *args, char *name, int type);

/* standard mathematical function prototypes */
ASTnode* nsin(ASTnode *, ASTnode *);
ASTnode* ncos(ASTnode *, ASTnode *);
ASTnode* nlog(ASTnode *, ASTnode *);
ASTnode* nsqrt(ASTnode *, ASTnode *);
ASTnode* modvar(ASTnode *, char, double);

/* mkvsynth function prototypes */
ASTnode* ffmpegDecode(ASTnode *, ASTnode *);
ASTnode* print(ASTnode *, ASTnode *);

/* user-defined functions */
extern funcRec pluginFunctions[];

/* helpful plugin macros */
#define OPTSTR(name, default) getOptArg(args, name, typeStr) ?     (char *) getOptArg(args, name, typeStr)  : default
#define OPTVAL(name, default) getOptArg(args, name, typeVal) ? *((double *) getOptArg(args, name, typeVal)) : default