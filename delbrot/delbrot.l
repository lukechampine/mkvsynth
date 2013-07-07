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
                        yylval = mkIdNode(yytext);
                        return IDENTIFIER;

                    }
    /* number, with optional decimal */
[0-9]+(\.[0-9]+)?   {
                        yylval = mkValNode(atof(yytext));
                        return CONSTANT;
                    }
    /* string literal */
\"([^\\\"]|\\.)*\"  { //"
                        /* TODO: add check for newlines */
                        /* don't include surrounding quotation marks */
                        char *str = strdup(yytext+1);
                        str[yyleng-2] = 0;
                        yylval = mkStrNode(str);
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
    /* blocks */
    if      (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "else"))
        return ELSE;
    else if (!strcmp(str, "while"))
        return WHILE;
    /* types */
    else if (!strcmp(str, "int"))
        return T_INT;
    else if (!strcmp(str, "double"))
        return T_DOUBLE;
    else if (!strcmp(str, "string"))
        return T_STRING;
    /* not a reserved word */
    else
        return 0;
}
