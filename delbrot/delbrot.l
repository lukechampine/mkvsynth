L    [a-zA-Z_]
D    [0-9]

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "delbrot.h"
    #include "y.tab.h"
    int resWord(char *);
    int linenumber = 1;
%}
    /* don't keep scanning after EOF */
%option noyywrap
    /* suppress gcc warnings */
%option noinput
%option nounput

%%  /* rule definition section */

    /* boolean */
"True"              {
                        yylval = makeLeaf(typeBool, TRUE);
                        return CONSTANT;
                    }
"False"             {
                        yylval = makeLeaf(typeBool, FALSE);
                        return CONSTANT;
                    }
    /* type */
"num"               {
                        yylval = makeNode(NUM, 0);
                        return NUM;
                    }
"bool"              {
                        yylval = makeNode(BOOL, 0);
                        return BOOL;
                    }
"string"            {
                        yylval = makeNode(STRING, 0);
                        return STRING;
                    }
"clip"              {
                        yylval = makeNode(CLIP, 0);
                        return CLIP;
                    }
    /* keyword, function, or variable */
{L}({L}|{D})*       {
                        /* reserved word */
                        if (resWord(yytext))
                            return resWord(yytext);
                        /* variable or function */
                        yylval = makeLeaf(typeId, strdup(yytext));
                        return IDENTIFIER;
                    }
    /* number, with optional decimal */
{D}+(\.{D}+)?       {
                        yylval = makeLeaf(typeNum, atof(yytext));
                        return CONSTANT;
                    }
    /* number, without initial 0 */
\.{D}+              {
                        yylval = makeLeaf(typeNum, atof(yytext));
                        return CONSTANT;
                    }
    /* number, hex literal */
0x[a-fA-F0-9]+      {
                        yylval = makeLeaf(typeNum, strtol(yytext, NULL, 16));
                        return CONSTANT;
                    }
    /* string literal */
\"([^\\\"]|\\.)*\"  { //"
                        /* remove enclosing quotation marks */
                        char *str = strdup(yytext + 1);
                        str[strlen(str) - 1] = 0;
                        yylval = makeLeaf(typeStr, str);
                        return CONSTANT;
                    }
    /* arithmetic operator */
"+"                 { yylval = makeNode('+', 0);   return '+';   }
"-"                 { yylval = makeNode('-', 0);   return '-';   }
"*"                 { yylval = makeNode('*', 0);   return '*';   }
"/"                 { yylval = makeNode('/', 0);   return '/';   }
"^"                 { yylval = makeNode('^', 0);   return '^';   }
"%"                 { yylval = makeNode('%', 0);   return '%';   }
    /* comparator or boolean operator */
"=="                { yylval = makeNode(EQ, 0);    return EQ;    }
"!="                { yylval = makeNode(NE, 0);    return NE;    }
">"                 { yylval = makeNode(GT, 0);    return GT;    }
"<"                 { yylval = makeNode(LT, 0);    return LT;    }
">="                { yylval = makeNode(GE, 0);    return GE;    }
"<="                { yylval = makeNode(LE, 0);    return LE;    }
"||"                { yylval = makeNode(LOR, 0);   return LOR;   }
"&&"                { yylval = makeNode(LAND, 0);  return LAND;  }
    /*assignment operator */
"="                 { yylval = makeNode('=', 0);   return '=';   }
"+="                { yylval = makeNode('+', 0);   return ADDEQ; }
"-="                { yylval = makeNode('-', 0);   return SUBEQ; }
"*="                { yylval = makeNode('*', 0);   return MULEQ; }
"/="                { yylval = makeNode('/', 0);   return DIVEQ; }
"^="                { yylval = makeNode('^', 0);   return POWEQ; }
"%="                { yylval = makeNode('%', 0);   return MODEQ; }
"=>"                { yylval = makeNode(CHNEQ, 0); return CHNEQ; }
    /* chaining operator */
"->"                { yylval = makeNode(CHAIN, 0); return CHAIN; }
    /* concatenation operator */
"++"                { yylval = makeNode(CNCAT, 0); return CNCAT; }
    /* miscellaneous character */
[(){}!.,?|:;]           return *yytext;
    /* comment */
#.*\n                   ;
    /* ignore whitespace (2D languages are pig disgusting) */
[ \t]+                  ;
    /* used for error messages */
\n                      linenumber++;
    /* execute queued filters */
<<EOF>>             {
                        #ifndef DELBROT
                        ex(global, makeLeaf(typeId, "go"));
                        #endif
                        yyterminate();
                    }
    /* anything else is an error */
.                       MkvsynthError("unknown character %s", yytext);

%%

int resWord(char *str) {
    if (!strcmp(str, "default"))
        return DEFAULT;
    else if (!strcmp(str, "else"))
        return ELSE;
    else if (!strcmp(str, "function"))
        return FNDEF;
    else if (!strcmp(str, "if"))
        return IF;
    else if (!strcmp(str, "import"))
        return IMPORT;
    else if (!strcmp(str, "otherwise"))
        return OTHER;
    else if (!strcmp(str, "return"))
        return RETURN;   
    /* not a reserved word */
    else
        return 0;
}
