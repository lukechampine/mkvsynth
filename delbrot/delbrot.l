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
                        /* reserved word */
                        if (resWord(yytext))
                            return resWord(yytext);
                        /* variable or function */
                        yylval.str = strdup(yytext);
                        return IDENTIFIER;

                    }
    /* number, with optional decimal */
[0-9]+(\.[0-9]+)?   {
                        yylval.val = atof(yytext);
                        return CONSTANT;
                    }
    /* string literal */
    /* TODO: switch to using string states for better error handling */
\"([^\\\"]|\\.)*\"  { //"
                        /* don't include surrounding quotation marks */
                        yylval.str = strdup(yytext+1);
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
[(){}<>+*=/%!.,:;\-\^]  return *yytext;

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
