/* forward definition of symbol table struct */
typedef struct symRec symRec;

/* linked list data structure, used for function arguments */
struct argNode {
	union {					/* argument value */
		double	dValue;		/* number  */
		symRec	*vValue;	/* variable */
	};
	struct argNode *next;	/* pointer to next argument */
};

typedef struct argNode argNode;

/* argument node function prototypes */
argNode *makeArgNode(double);
argNode *appendArgNode(argNode *, double);

/* Function type */
typedef double (*func) (argNode *);

/* Data type for links in the chain of symbols.  */
struct symRec {
	char *name;				/* name of symbol */
	int type;				/* type of symbol: either VAR or FNCT */
	union {
		double var;			/* value of a VAR */
		func fnPtr;			/* value of a FNCT */
	} value;
	struct symRec *next;	/* link field */
};

/* The symbol table: a chain of `struct symRec'.  */
extern symRec *symTable;

/* symbol function prototypes */
symRec *putSym (char const *, int);
symRec *getSym (char const *);

/* mkvsynth function prototypes */
double npow(double, double);
double nsin(argNode *);
double ncos(argNode *);
double nlog(argNode *);
double nexp(argNode *);
double nsqrt(argNode *);
double ffmpegDecode(argNode *);
double print(argNode *);