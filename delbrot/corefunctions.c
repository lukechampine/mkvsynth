#include "delbrot.h"
#include "y.tab.h"
#include <math.h>

/* function declarations */
ASTnode* assert_AST(ASTnode *, argList *);
ASTnode* cos_AST(ASTnode *, argList *);
ASTnode* log_AST(ASTnode *, argList *);
ASTnode* print_AST(ASTnode *, argList *);
ASTnode* read_AST(ASTnode *, argList *);
ASTnode* show_AST(ASTnode *, argList *);
ASTnode* sin_AST(ASTnode *, argList *);
ASTnode* sqrt_AST(ASTnode *, argList *);
static char* unesc(char *);

/* function table */
fnEntry coreFunctions[] = {
	{ "assert",    assert_AST },
	{ "cos",       cos_AST    },
	{ "ln",        log_AST    },
	{ "print",     print_AST  },
	{ "read",      read_AST   },
	{ "show",      show_AST   },
	{ "sin",       sin_AST    },
	{ "sqrt",      sqrt_AST   },
	{ 0,           0          },
};

/* function definitions */
/* exit with error message if assertion fails */
ASTnode* assert_AST(ASTnode *p, argList *a) {
	checkArgs(a, 2, typeBool, typeStr);
	if (MANDBOOL(0) == FALSE)
		MkvsynthError(MANDSTR(1));
	RETURNNULL();
}

/* cosine */
ASTnode* cos_AST(ASTnode *p, argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(cos(MANDNUM(0)));
}

/* natural log */
ASTnode* log_AST(ASTnode *p, argList *a) {
	checkArgs(a, 1, typeNum);
    RETURNNUM(log(MANDNUM(0)));
}

/* generalized print function; will print any number of args */
ASTnode* print_AST(ASTnode *p, argList *a) {
    if (a->nargs == 0)
        MkvsynthError("print: expected at least one argument");
    int i;
    for (i = 0; i < a->nargs; i++) {
		switch (a->args[i].value->type) {
			case typeNum: printf("%.10g ", MANDNUM(i)); break;
			case typeBool: printf("%s ", MANDBOOL(i) == TRUE ? "True" : "False"); break;
			case typeStr: printf("%s ", unesc(MANDSTR(i))); break;
            default: MkvsynthWarning("could not print type %s", typeNames[a->args[i].value->type]); break;
		}
	}
	printf("\n");
    RETURNNULL();
}

/* convert string to number */
ASTnode* read_AST(ASTnode *p, argList *a) {
	checkArgs(a, 1, typeStr);
	RETURNNUM(atof(MANDSTR(0)));
}

/* return string representation of any type */
ASTnode* show_AST(ASTnode *p, argList *a) {
    /* can't use checkArgs here */
	if (a->nargs != 1)
	    MkvsynthError("expected 1 argument, got %d", a->nargs);
	switch (a->args[0].value->type) {
		case typeNum:  p->str = malloc(256); sprintf(p->str, "%.10g", MANDNUM(0)); break;
		case typeBool: p->str = MANDBOOL(0) == TRUE ? "True" : "False"; break;
		case typeStr:  p->str = MANDSTR(0); break;
        default: MkvsynthError("not defined for %ss", typeNames[a->args[0].value->type]);
	}
	p->type = typeStr;
	return p;
}

/* sine */
ASTnode* sin_AST(ASTnode *p, argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(sin(MANDNUM(0)));
}

/* square root */
ASTnode* sqrt_AST(ASTnode *p, argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(sqrt(MANDNUM(0)));
}

/* helper function to interpret string literals */
static char* unesc(char* str) {
	int i, j;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\\') {
			switch (str[i+1]) {
				case 't': str[i] = '\t'; break;
				case 'n': str[i] = '\n'; break;
				case 'r': str[i] = '\r'; break;
				case '\\':str[i] = '\\'; break;
				case '\'':str[i] = '\''; break;
				case '\"':str[i] = '\"'; break;
                default: MkvsynthError("unknown literal \"\\%c\"", str[i+1]);
			}
			for (j = i + 1; str[j] != '\0'; j++)
				str[j] = str[j+1];
		}
	}
	return str;
}
