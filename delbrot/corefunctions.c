#include "delbrot.h"
#include "y.tab.h"
#include <math.h>

/* function declarations */
ASTnode* assert_AST(ASTnode *, ASTnode *);
ASTnode* cos_AST(ASTnode *, ASTnode *);
ASTnode* log_AST(ASTnode *, ASTnode *);
ASTnode* print_AST(ASTnode *, ASTnode *);
ASTnode* read_AST(ASTnode *, ASTnode *);
ASTnode* show_AST(ASTnode *, ASTnode *);
ASTnode* sin_AST(ASTnode *, ASTnode *);
ASTnode* sqrt_AST(ASTnode *, ASTnode *);
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
ASTnode* assert_AST(ASTnode *p, ASTnode *args) {
	checkArgs("assert", args, 2, typeBool, typeStr);
	if (args->bool == FALSE)
		MkvsynthError(args->next->str);
	p->type = typeFn;
	return p;
}

/* cosine */
ASTnode* cos_AST(ASTnode *p, ASTnode *args) {
	checkArgs("cos", args, 1, typeNum);
	p->type = typeNum;
	p->num = cos(args->num); 
	return p;
}

/* natural log */
ASTnode* log_AST(ASTnode *p, ASTnode *args) {
	checkArgs("log", args, 1, typeNum);
	p->type = typeNum;
	p->num = log(args->num); 
	return p;
}

/* generalized print function; will print any number of args */
ASTnode* print_AST(ASTnode *p, ASTnode *args) {
	while(args) {
		/* printable types */
		switch(args->type) {
			case typeNum: printf("%.10g ", args->num); break;
			case typeBool: printf("%s ", args->bool == TRUE ? "True" : "False"); break;
			case typeStr: printf("%s ", unesc(args->str)); break;
			default: printf("[could not print type %s] ", typeNames[args->type]); break;
		}
		args = args->next;
	}
	printf("\n");
	p->type = typeFn;
	return p;
}

/* convert string to number */
ASTnode* read_AST(ASTnode *p, ASTnode *args) {
	checkArgs("read", args, 1, typeStr);
	p->num = atof(args->str);
	p->type = typeNum;
	return p;
}

/* return string representation of any type */
ASTnode* show_AST(ASTnode *p, ASTnode *args) {
	int i = 0;
	ASTnode *traverse;
	for (traverse = args; traverse != NULL; traverse = traverse->next, i++);
	if (i != 1) MkvsynthError("show expected 1 argument, got %d", i);
	switch (args->type) {
		case typeNum:  p->str = malloc(256); sprintf(p->str, "%.10g", args->num); break;
		case typeBool: p->str = args->bool == TRUE ? "True" : "False"; break;
		case typeStr:  p->str = args->str; break;
		default: MkvsynthError("show is not defined for %ss", typeNames[args->type]);
	}
	p->type = typeStr;
	return p;
}

/* sine */
ASTnode* sin_AST(ASTnode *p, ASTnode *args) {
	checkArgs("sin", args, 1, typeNum);
	p->type = typeNum;
	p->num = sin(args->num); 
	return p;
}

/* square root */
ASTnode* sqrt_AST(ASTnode *p, ASTnode *args) {
	checkArgs("sqrt",args, 1, typeNum);
	p->type = typeNum;
	p->num = sqrt(args->num);
	return p;
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