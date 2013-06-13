/* forward definition of symbol table struct */
typedef struct symrec symrec;

/* linked list data structure, used for function arguments */
struct argNode {
	union {					/* argument value */
		double	dValue;		/* number  */
		symrec	*vValue;	/* variable */
	};
	struct argNode *next;	/* pointer to next argument */
};

typedef struct argNode argNode;

/* argument node function prototypes */
argNode *makeArgNode(double);
argNode *appendArgNode(argNode *, double);

/* Function type */
typedef double (*func_t) (argNode *);

/* Data type for links in the chain of symbols.  */
struct symrec {
	char *name;  /* name of symbol */
	int type;    /* type of symbol: either VAR or FNCT */
	union {
		double var;      /* value of a VAR */
		func_t fnctptr;  /* value of a FNCT */
	} value;
	struct symrec *next;  /* link field */
};

/* The symbol table: a chain of `struct symrec'.  */
extern symrec *sym_table;

/* symbol function prototypes */
symrec *putsym (char const *, int);
symrec *getsym (char const *);

/* mkvsynth function prototypes */
double ffmpegDecode(argNode *);
double print(argNode *);