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
                        int i; var *v; func f;
                        /* reserved word */
                        if ((i = resWord(yytext)) != 0)
                            return i;
                        /* built-in function */
                        if ((f = getFn(yytext)) != 0) {
                            yylval.fptr = f;
                            return FNCT;
                        }
                        /* existing variable */
                        if ((v = getVar(yytext)) != 0) {
                            yylval.vptr = v;
                            return VAR;
                        }
                        /* new variable */
                        yylval.vptr = putVar(yytext);
                        return VAR;
                    }
    /* number, with optional decimal */
[0-9]+(\.[0-9]+)?   {
                        yylval.val = atof(yytext);
                        return CONSTANT;
                    }
    /* string (literal -- don't preemptively process escape characters) */
\"([^\\\"]|\\.)*\"  { //"
                        yylval.str = strdup(yytext+1);
                        if (yylval.str[yyleng-2] != '"')
                            yyerror("improperly terminated string");
                        else
                            yylval.str[yyleng-2] = 0;
                        return STRING_LITERAL;
                    }
    /* increment/decrement -- these are preincrements! Post increments are not supported! */
"++"                    return INC;
"--"                    return DEC;

    /* comparator or boolean operator */
">="                    return GE;
"<="                    return LE;
"=="                    return EQ;
"!="                    return NE;
"&&"                    return LAND;
"||"                    return LOR;

    /* arithmetic assignment operator */
"+="                    return ADDEQ;
"-="                    return SUBEQ;
"*="                    return MULEQ;
"/="                    return DIVEQ;
"%="                    return MODEQ;

    /* miscellaneous character */
[(){}<>+*=/%!.,;\-\^]   return *yytext;

    /* comment */
#.*\n                   ; 

    /* ignore whitespace (2D languages are pig disgusting) */
[ \t]+                  ; 

    /* used for error messages */
\n                      linenumber++; 

    /* anything else is an error */
.                       yyerror("Unknown character");

%%

int resWord(char *str) {
    if      (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "else"))
        return ELSE;
    else if (!strcmp(str, "while"))
        return WHILE;
    else
        return 0;
}
