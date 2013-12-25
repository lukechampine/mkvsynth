L    [a-zA-Z]
D    [0-9]

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

    /* boolean */
"True"              {
                        yylval = mkBoolNode(TRUE);
                        return CONSTANT;
                    }
"False"             {
                        yylval = mkBoolNode(FALSE);
                        return CONSTANT;
                    }
    /* keyword, function, or variable */
{L}({L}|{D})*       {
                        /* reserved word */
                        if (resWord(yytext))
                            return resWord(yytext);
                        /* variable or function */
                        yylval = mkIdNode(yytext);
                        return IDENTIFIER;
                    }
    /* number, with optional decimal */
{D}+(\.{D}+)?       {
                        yylval = mkNumNode(atof(yytext));
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
"+"                 { yylval = mkNumNode('+');  return '+';    }
"-"                 { yylval = mkNumNode('-');  return '-';    }
"*"                 { yylval = mkNumNode('*');  return '*';    }
"/"                 { yylval = mkNumNode('/');  return '/';    }
"^"                 { yylval = mkNumNode('^');  return '^';    }
"%"                 { yylval = mkNumNode('%');  return '%';    }
    /* comparator or boolean operator */
"=="                { yylval = mkNumNode(EQ);   return EQ;     }
"!="                { yylval = mkNumNode(NE);   return NE;     }
">"                 { yylval = mkNumNode(GT);   return GT;     }
"<"                 { yylval = mkNumNode(LT);   return LT;     }
">="                { yylval = mkNumNode(GE);   return GE;     }
"<="                { yylval = mkNumNode(LE);   return LE;     }
"||"                { yylval = mkNumNode(LOR);  return LOR;    }
"&&"                { yylval = mkNumNode(LAND); return LAND;   }
    /*assignment operator */
"="                 { yylval = mkNumNode('=');   return '=';   }
"+="                { yylval = mkNumNode(ADDEQ); return ADDEQ; }
"-="                { yylval = mkNumNode(SUBEQ); return SUBEQ; }
"*="                { yylval = mkNumNode(MULEQ); return MULEQ; }
"/="                { yylval = mkNumNode(DIVEQ); return DIVEQ; }
"^="                { yylval = mkNumNode(POWEQ); return POWEQ; }
"%="                { yylval = mkNumNode(MODEQ); return MODEQ; }
    /* chaining operator */
"->"                { yylval = mkNumNode(CHAIN); return CHAIN; }
    /* miscellaneous character */
[(){}!.,?|:;]           return *yytext;
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
    /* types */
    else if (!strcmp(str, "num"))
        return NUM;
    else if (!strcmp(str, "bool"))
        return BOOL;
    else if (!strcmp(str, "string"))
        return STRING;
    else if (!strcmp(str, "clip"))
        return CLIP;
    /* function definition */
    else if (!strcmp(str, "function"))
        return FNDEF;
    /* misc */
    else if (!strcmp(str, "return"))
        return RETURN;
    else if (!strcmp(str, "default"))
        return DEFAULT;
    else if (!strcmp(str, "otherwise"))
        return OTHER;
    /* not a reserved word */
    else
        return 0;
}
