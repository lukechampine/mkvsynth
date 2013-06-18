%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "delbrot.h"
    #include "y.tab.h"
    void yyerror(char *);
    int linenumber = 1;
%}
    /* don't keep scanning after EOF */
%option noyywrap

%%  /* rule definition section */

[a-zA-Z]+           {   /* match a keyword or function; alternatively, assign a variable */
                        int i;
                        symRec *j;
                        if ((i = resWord(yytext)) != 0)
                            return i;
                        else if ((j = getSym(yytext)) == 0)
                            j = putSym(yytext, VAR);
                        yylval.tptr = j;
                        return j->type;
                    }
[0-9]+(\.[0-9]+)?   {   /* numbers, with optional decimal */
                        yylval.val = atof(yytext);
                        return NUM;
                    }
                        /* boolean operators */
">="                    return GE;
"<="                    return LE;
"=="                    return EQ;
"!="                    return NE;

                        /* miscellaneous characters */
[(){}<>+*=/.,;\-\^]     return *yytext;



[ \t]+                  ; /* ignore whitespace */
\n                      linenumber++; /* used for error messages */

.                       yyerror("Unknown character");

%%

int resWord(char *str) {
    if      (!strcmp(str, "while"))
        return WHILE;
    else if (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "else"))
        return ELSE;
    else
        return 0;
}