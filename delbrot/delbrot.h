/* types */
typedef enum { typeVal, typeStr, typeFn, typeVar, typeParam, typeOp } nodeType;

/* a function */
/* arg1 is passed by reference, and will contain result of the function */
/* arg2 is a linked list of ASTnodes, allowing an arbitrary number of arguments to be passed */
typedef struct ASTnode ASTnode;
typedef ASTnode * (*func) (ASTnode *, ASTnode *);

/* an operator node */
typedef struct {
    int oper;               /* operator */
    int nops;               /* number of operands */
    struct ASTnode **ops;   /* operands */
} opNode;

/* a generic node in the AST */
/* TOOD: add line number field for error messages */
typedef struct var var;
struct ASTnode {
    nodeType type;          /* type of node */
    int readonly;           /* determines whether ex() will consume node */
    union {
        double  val;        /* value */
        char   *str;        /* string */
        char    *id;        /* identifier */
        func  fnPtr;        /* function pointer */
        var *varPtr;        /* variable */
        opNode   op;        /* operator */
    };
    struct ASTnode *next;   /* used for argument linked lists */
};

#define YYSTYPE ASTnode*

/* ASTnode prototypes */
ASTnode *newNode();
ASTnode *mkIdNode(char *);
ASTnode *mkValNode(double);
ASTnode *mkStrNode(char *);
ASTnode *mkTypeNode(int);
ASTnode *mkOpNode(int, int, ...);
ASTnode *mkParamNode(char *);
ASTnode *append(ASTnode *, ASTnode *);
ASTnode *ex(ASTnode *);

/* a variable  */
struct var {
    char *name;             /* variable name */
    ASTnode *value;         /* ASTnode it references */
    struct var *next;       /* link field */
};

/* the variable table */
extern var *varTable;

/* variable/function access prototypes */
var *putVar(char const *);
var *getVar(char const *);
func getFn(char const *);

/* standard mathematical function prototypes */
ASTnode* nsin(ASTnode *, ASTnode *);
ASTnode* ncos(ASTnode *, ASTnode *);
ASTnode* nlog(ASTnode *, ASTnode *);
ASTnode* nsqrt(ASTnode *, ASTnode *);
ASTnode* modvar(ASTnode *, char, double);

/* mkvsynth function prototypes */
ASTnode* ffmpegDecode(ASTnode *, ASTnode *);
ASTnode* print(ASTnode *, ASTnode *);