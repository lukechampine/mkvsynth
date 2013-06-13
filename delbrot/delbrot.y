%{
	#include <stdio.h>
	#include <string.h>
	#include "delbrot.h"  /* Contains definitions for symbol table and argument list */
	void yyerror (char *);
	extern int linenumber;
%}

%union {
	double 	val;		/* For returning numbers.  */
	symrec 	*tptr; 		/* For returning symbol-table pointers.  */
	argNode *nPtr;
}
%token <val>  NUM 		/* Simple double precision number.  */
%token <tptr> VAR FNCT 	/* Variable and function.  */
%type  <val>  expr
%type  <nPtr> arglist	/* argument list, in the form of a linked list */

%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation */

%% /* The grammar follows.  */
input:
		/* empty */
		| input statement
		;

statement:
		';'
		| expr ';'   			{									}
		| error ';'				{ yyerrok;							}
		;

expr:
		NUM 					{ $$ = $1;							}
		| VAR 					{ $$ = $1->value.var;				}
		| VAR '=' expr			{ $$ = $3; $1->value.var = $3;		}
		| FNCT '(' arglist ')'	{ $$ = (*($1->value.fnctptr))($3);	}
		| expr '+' expr			{ $$ = $1 + $3;						}
		| expr '-' expr			{ $$ = $1 - $3;						}
		| expr '*' expr			{ $$ = $1 * $3;						}
		| expr '/' expr			{ $$ = $1 / $3;						}
		| '-' expr  %prec NEG 	{ $$ = -$2;							}
		//| expr '^' expr 		{ $$ = pow($1, $3);					}
		| '(' expr ')' 			{ $$ = $2;							}
		;

arglist:
		expr					{ $$ = makeArgNode($1);				}
		| arglist ',' expr		{ $$ = appendArgNode($1, $3);		}
		;


/* End of grammar.  */
%%

struct init {
	char const *fname;
	double (*fnct) (argNode *);
};

struct init const arith_fncts[] =
{
	"ffmpegDecode", ffmpegDecode,
	"print", print,
	// "sin",  sin,
	// "cos",  cos,
	// "atan", atan,
	// "ln",   log,
	// "exp",  exp,
	// "sqrt", sqrt,
	0, 0
};

/* The symbol table: a chain of `struct symrec'.  */
symrec *sym_table;

/* Put arithmetic functions in table.  */
void init_table (void) {
	int i;
	for (i = 0; arith_fncts[i].fname != 0; i++)	{
		symrec *ptr = putsym (arith_fncts[i].fname, FNCT);
		ptr->value.fnctptr = arith_fncts[i].fnct;
	}
}
     
symrec *putsym (char const *sym_name, int sym_type) {
	symrec *ptr = (symrec *) malloc (sizeof (symrec));
	ptr->name = (char *) malloc (strlen (sym_name) + 1);
	strcpy (ptr->name,sym_name);
	ptr->type = sym_type;
	ptr->value.var = 0; /* Set value to 0 even if fctn.  */
	ptr->next = (struct symrec *)sym_table;
	sym_table = ptr;
	return ptr;
}

symrec *getsym (char const *sym_name) {
	symrec *ptr;
	for (ptr = sym_table; ptr != (symrec *) 0; ptr = (symrec *)ptr->next) {
		if (strcmp (ptr->name,sym_name) == 0)
			return ptr;
	}
	return 0;
}

argNode *makeArgNode(double dub) {
	argNode *root = (argNode*)malloc(sizeof(argNode));
	root->dValue = dub;
	return root;
}

argNode *appendArgNode(argNode *root, double dub) {
	argNode *newArg = (argNode*)malloc(sizeof(argNode));
	newArg->dValue = dub;
	root->next = newArg;
	return root;
}

/* Called by yyparse on error.  */
void yyerror(char *s) {
	fprintf(stderr, "delbrot:%d error: %s\n", linenumber, s);
}

int main () {
	init_table ();
	return yyparse ();
}