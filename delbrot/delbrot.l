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

    /* keyword, function, or variable */
[a-zA-Z]+           {
                        int i;
                        symRec *j;
                        if ((i = resWord(yytext)) != 0)
                            return i;
                        else if ((j = getSym(yytext)) != 0) {
                            yylval.tptr = j;
                            if (j->value->type == typeFn)  return FNCT; /* TODO: quit lumping functions and variables together, it ain't worth it. */
                            else return VAR;
                        }
                        else {
                            yylval.tptr = putSym(yytext);
                            return VAR;
                        }
                    }
    /* number, with optional decimal */
[0-9]+(\.[0-9]+)?   {
                        yylval.val = atof(yytext);
                        return NUM;
                    }
    /* string (literal -- don't preemptively process escape characters) */
\"([^\\\"]|\\.)*\"  { //"
                        yylval.str = strdup(yytext+1);
                        if (yylval.str[yyleng-2] != '"')
                            yyerror("improperly terminated string");
                        else
                            yylval.str[yyleng-2] = 0;
                        return STRING;
                    }

    /* boolean operator */
">="                    return GE;
"<="                    return LE;
"=="                    return EQ;
"!="                    return NE;

    /* miscellaneous character */
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