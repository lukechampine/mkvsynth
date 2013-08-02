L      [a-zA-Z]
D      [0-9]

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "delbrot.h"
    #include "y.tab.h"
    extern void yyerror(char *, ...);
    int resWord(char *);
    int linenumber = 1;
%}
    /* don't keep scanning after EOF */
%option noyywrap

%%  /* rule definition section */

    /* keyword, function, or variable */
{L}({L}|{D})*       {
                        /* reserved word */
                        if (resWord(yytext))
                            return resWord(yytext);
                        /* variable or function */
                        yylval = mkIdNode(yytext);
                        return IDENTIFIER;
                    }
    /* optional function argument */
{L}({L}|{D})*:      {
                        yylval = mkOptArgNode(yytext);
                        return OPTARG;
                    }
    /* number, with optional decimal */
{D}+(\.{D}+)?       {
                        yylval = mkValNode(atof(yytext));
                        return CONSTANT;
                    }
    /* string literal */
\"([^\\\"]|\\.)*\"  { //"
                        /* TODO: add check for newlines */
                        yylval = mkStrNode(yytext);
                        return CONSTANT;
                    }
    /* increment/decrement -- these are preincrements! Post increments are not supported! */
"++"                    return INC;
"--"                    return DEC;
    /* arithmetic operator */
"+"                 { yylval = mkValNode('+');  return '+';    }
"-"                 { yylval = mkValNode('-');  return '-';    }
"*"                 { yylval = mkValNode('*');  return '*';    }
"/"                 { yylval = mkValNode('/');  return '/';    }
"^"                 { yylval = mkValNode('^');  return '^';    }
"%"                 { yylval = mkValNode('%');  return '%';    }
    /* comparator or boolean operator */
"=="                { yylval = mkValNode(EQ);   return EQ;     }
"!="                { yylval = mkValNode(NE);   return NE;     }
">"                 { yylval = mkValNode(GT);   return GT;     }
"<"                 { yylval = mkValNode(LT);   return LT;     }
">="                { yylval = mkValNode(GE);   return GE;     }
"<="                { yylval = mkValNode(LE);   return LE;     }
"||"                { yylval = mkValNode(LOR);  return LOR;    }
"&&"                { yylval = mkValNode(LAND); return LAND;   }

    /*assignment operator */
"="                 { yylval = mkValNode('=');   return '=';   }
"+="                { yylval = mkValNode(ADDEQ); return ADDEQ; }
"-="                { yylval = mkValNode(SUBEQ); return SUBEQ; }
"*="                { yylval = mkValNode(MULEQ); return MULEQ; }
"/="                { yylval = mkValNode(DIVEQ); return DIVEQ; }
"^="                { yylval = mkValNode(POWEQ); return POWEQ; }
"%="                { yylval = mkValNode(MODEQ); return MODEQ; }

    /* miscellaneous character */
[(){}!.,:;]             return *yytext;

    /* comment */
#.*\n                   ; 

    /* ignore whitespace (2D languages are pig disgusting) */
[ \t]+                  ; 

    /* used for error messages */
\n                      linenumber++; 

    /* anything else is an error */
.                       yyerror("unknown character");

%%

int resWord(char *str) {
    /* blocks */
    if      (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "else"))
        return ELSE;
    else if (!strcmp(str, "while"))
        return WHILE;
    else if (!strcmp(str, "for"))
        return FOR;
    /* types */
    else if (!strcmp(str, "int"))
        return INT;
    else if (!strcmp(str, "double"))
        return DOUBLE;
    else if (!strcmp(str, "string"))
        return STRING;
    /* function definition */
    else if (!strcmp(str, "function"))
        return FNDEF;
    else if (!strcmp(str, "return"))
        return RETURN;
    /* not a reserved word */
    else
        return 0;
}
