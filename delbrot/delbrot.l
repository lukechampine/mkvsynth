%{
	#include <stdlib.h>
	#include "delbrot.h"
	#include "y.tab.h"
	void yyerror(char *);
	int linenumber = 1;
%}
	/* don't keep scanning after EOF */
%option noyywrap

%%

	/* match variable names and function names */
[a-zA-Z]+ 			{
						symrec *i;
						i = getsym(yytext);
						if (i == 0)
							i = putsym(yytext, VAR);
						yylval.tptr = i;
						return i->type;
 					}
[0-9]+(\.[0-9]*)? 	{
						yylval.val = atof(yytext);
						return NUM;
					}
[+\-*\^/()=.,;]			return *yytext;

[ \t]+					; /* ignore whitespace */
\n 						linenumber++; /* used for error messages */

.						yyerror("Unknown character");

%%