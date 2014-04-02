#include "delbrot.h"
#include "y.tab.h"
#include <math.h>

/* function declarations */
Value assert_AST(argList *);
Value cos_AST(argList *);
Value log_AST(argList *);
Value print_AST(argList *);
Value read_AST(argList *);
Value show_AST(argList *);
Value sin_AST(argList *);
Value sqrt_AST(argList *);
static char* unesc(char *);

/* function table */
Fn coreFunctions[] = {
	{ fnCore, "assert", assert_AST, NULL, NULL, NULL },
	{ fnCore, "cos",    cos_AST,    NULL, NULL, NULL },
	{ fnCore, "ln",     log_AST,    NULL, NULL, NULL },
	{ fnCore, "print",  print_AST,  NULL, NULL, NULL },
	{ fnCore, "read",   read_AST,   NULL, NULL, NULL },
	{ fnCore, "show",   show_AST,   NULL, NULL, NULL },
	{ fnCore, "sin",    sin_AST,    NULL, NULL, NULL },
	{ fnCore, "sqrt",   sqrt_AST,   NULL, NULL, NULL },
	{ 0,      0,        0,          0,    0,    0    },
};

/* function definitions */
/* exit with error message if assertion fails */
Value assert_AST(argList *a) {
	checkArgs(a, 2, typeBool, typeStr);
	extern char* currentFunction;
	currentFunction = "assertion failed";
	if (MANDBOOL(0) == FALSE)
		MkvsynthError(MANDSTR(1));
	RETURNNULL();
}

/* cosine */
Value cos_AST(argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(cos(MANDNUM(0)));
}

/* natural log */
Value log_AST(argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(log(MANDNUM(0)));
}

/* generalized print function; will print any number of args */
Value print_AST(argList *a) {
	if (a->nargs == 0)
		MkvsynthError("expected at least one argument");
	int i;
	for (i = 0; i < a->nargs; i++) {
		switch (a->args[i].value.type) {
			case typeNum: printf("%.10g ", MANDNUM(i)); break;
			case typeBool: printf("%s ", MANDBOOL(i) == TRUE ? "True" : "False"); break;
			case typeStr: printf("%s ", unesc(MANDSTR(i))); break;
			default: MkvsynthWarning("could not print type %s", typeNames[a->args[i].value.type]);
		}
	}
	printf("\n");
	RETURNNULL();
}

/* convert string to number */
Value read_AST(argList *a) {
	checkArgs(a, 1, typeStr);
	RETURNNUM(atof(MANDSTR(0)));
}

/* return string representation of any type */
Value show_AST(argList *a) {
	/* can't use checkArgs here */
	if (a->nargs != 1)
		MkvsynthError("expected 1 argument, got %d", a->nargs);
	Value v;
	switch (a->args[0].value.type) {
		case typeNum:  v.str = malloc(256); sprintf(v.str, "%.10g", MANDNUM(0)); break;
		case typeBool: v.str = MANDBOOL(0) == TRUE ? "True" : "False"; break;
		case typeStr:  v.str = MANDSTR(0); break;
		default: MkvsynthError("not defined for %ss", typeNames[a->args[0].value.type]);
	}
	v.type = typeStr;
	return v;
}

/* sine */
Value sin_AST(argList *a) {
	checkArgs(a, 1, typeNum);
	RETURNNUM(sin(MANDNUM(0)));
}

/* square root */
Value sqrt_AST(argList *a) {
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
