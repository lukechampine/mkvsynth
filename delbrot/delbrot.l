%{
	#include <stdlib.h>
	#include "delbrot.h"
	#include "y.tab.h"
	void yyerror(char *);
	int linenumber = 1;
%}
	/* don't keep scanning after EOF */
%option noyywrap

%% 	/* rule definition section */

[a-zA-Z]+ 			{	/* match variable names and function names */
						symRec *i;
						i = getSym(yytext);
						if (i == 0)
							i = putSym(yytext, VAR);
						yylval.tptr = i;
						return i->type;
 					}
[0-9]+(\.[0-9]+)? 	{	/* numbers, with optional decimal */
						yylval.val = atof(yytext);
						return NUM;
					}
						/* miscellaneous characters */
[()+*=/.,;\-\^]			return *yytext;

[ \t]+					; /* ignore whitespace */
\n 						linenumber++; /* used for error messages */

.						yyerror("Unknown character");

%%