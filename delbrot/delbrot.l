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
    /* type */
"num"               {
                        yylval = mkNumNode(NUM);
                        return NUM;
                    }
"bool"              {
                        yylval = mkNumNode(BOOL);
                        return BOOL;
                    }
"string"            {
                        yylval = mkNumNode(STRING);
                        return STRING;
                    }
"clip"              {
                        yylval = mkNumNode(CLIP);
                        return CLIP;
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
    /* number, without initial 0 */
\.{D}+              {
                        yylval = mkNumNode(atof(yytext));
                        return CONSTANT;
                    }
    /* string literal */
\"([^\\\"]|\\.)*\"  {
                        yylval = mkStrNode(yytext);
                        return CONSTANT;
                    }
    /* arithmetic operator */
"+"                 { yylval = mkNumNode('+');   return '+';   }
"-"                 { yylval = mkNumNode('-');   return '-';   }
"*"                 { yylval = mkNumNode('*');   return '*';   }
"/"                 { yylval = mkNumNode('/');   return '/';   }
"^"                 { yylval = mkNumNode('^');   return '^';   }
"%"                 { yylval = mkNumNode('%');   return '%';   }
    /* comparator or boolean operator */
"=="                { yylval = mkNumNode(EQ);    return EQ;    }
"!="                { yylval = mkNumNode(NE);    return NE;    }
">"                 { yylval = mkNumNode(GT);    return GT;    }
"<"                 { yylval = mkNumNode(LT);    return LT;    }
">="                { yylval = mkNumNode(GE);    return GE;    }
"<="                { yylval = mkNumNode(LE);    return LE;    }
"||"                { yylval = mkNumNode(LOR);   return LOR;   }
"&&"                { yylval = mkNumNode(LAND);  return LAND;  }
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
[(){}!,?|:;]            return *yytext;
    /* comment */
#.*\n                   ; 
    /* ignore whitespace (2D languages are pig disgusting) */
[ \t]+                  ; 
    /* used for error messages */
\n                      linenumber++; 
    /* execute queued up filters
<<EOF>>             { go_AST(); yyterminate();                 } */
    /* anything else is an error */
.                       yyerror("unknown character");

%%

int resWord(char *str) {
    /* blocks */
    if      (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "else"))
        return ELSE;
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
