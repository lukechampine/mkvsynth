/* types */
typedef enum { typeVal, typeVar, typeOp } nodeType;

typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct ASTnode **ops;       /* operands */
} opNode;

/* a node in the AST */
typedef struct symRec symRec;
typedef struct ASTnode {
    nodeType type;              /* type of node */
    union {
        double  val;            /* value */
        symRec *var;            /* function or variable */
        opNode  op;             /* operator */
    };
    struct ASTnode *next;       /* used for argument linked lists */
} ASTnode;

/* a function */
typedef ASTnode * (*func) (ASTnode *, ASTnode *);

/* Data type for links in the chain of symbols.  */
struct symRec {
    char *name;             /* name of symbol */
    int type;               /* type of symbol: either VAR or FNCT */
    union {
        double val;         /* value of a VAR */
        func fnPtr;         /* value of a FNCT */
    };
    struct symRec *next;    /* link field */
};

/* The symbol table: a chain of `struct symRec'.  */
extern symRec *symTable;

/* symbol function prototypes */
symRec *putSym (char const *, int);
symRec *getSym (char const *);

/* link two nodes together */
ASTnode *append (ASTnode *, ASTnode *);

/* standard mathematical function prototypes */
ASTnode* nadd (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nsub (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nmul (ASTnode *, ASTnode *, ASTnode *);
ASTnode* ndiv (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nexp (ASTnode *, ASTnode *, ASTnode *);
ASTnode* ngtr (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nles (ASTnode *, ASTnode *, ASTnode *);
ASTnode* ngte (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nlte (ASTnode *, ASTnode *, ASTnode *);
ASTnode* neql (ASTnode *, ASTnode *, ASTnode *);
ASTnode* nneq (ASTnode *, ASTnode *, ASTnode *);
ASTnode* npow (ASTnode *, ASTnode *);
ASTnode* nsin (ASTnode *, ASTnode *);
ASTnode* ncos (ASTnode *, ASTnode *);
ASTnode* nlog (ASTnode *, ASTnode *);
ASTnode* nsqrt(ASTnode *, ASTnode *);

/* mkvsynth function prototypes */
ASTnode* ffmpegDecode(ASTnode *, ASTnode *);
ASTnode* print(ASTnode *, ASTnode *);