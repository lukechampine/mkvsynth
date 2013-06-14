%{
    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    #include "delbrot.h"
    void yyerror (char *);
    extern int linenumber;
%}

%union {
    double  val;        /* number */
    symRec  *tptr;      /* symbol-table pointer */
    argNode *nPtr;      /* argument list root node */
}
%token <val>  NUM       /* numbers are numbers */
%token <tptr> VAR FNCT  /* variables and functions are symbol-table pointers */
%token WHILE IF ELSE    /* keywords don't have a type */
%type  <val>  expr      /* expressions are numbers */
%type  <nPtr> arglist   /* function arguments are a linked list */

%right '='
%left GE LE EQ NE '>' '<'
%left '-' '+'
%left '*' '/'
%left NEG     /* negation */
%right '^'    /* exponentiation */

%% /* grammar definition section  */

input:
        /* empty */
        | input statement
        ;

statement:
        ';'
        | expr ';'              {                               }
        | error ';'             { yyerrok;                      }
        ;

expr:
        NUM                     { $$ = $1;                      }
        | VAR                   { $$ = $1->value.var;           }
        | VAR '=' expr          { $$ = $3; $1->value.var = $3;  }
        | FNCT '(' arglist ')'  { $$ = (*($1->value.fnPtr))($3->head); }
        | '-' expr %prec NEG    { $$ = -$2;                     }
        | expr '+' expr         { $$ = $1 + $3;                 }
        | expr '-' expr         { $$ = $1 - $3;                 }
        | expr '*' expr         { $$ = $1 * $3;                 }
        | expr '/' expr         { $$ = $1 / $3;                 }
        | expr '^' expr         { $$ = pow($1, $3);             }
        | expr EQ expr          { $$ = $1 == $3;                }
        | expr NE expr          { $$ = $1 != $3;                }
        | expr '>' expr         { $$ = $1 > $3;                 }
        | expr '<' expr         { $$ = $1 < $3;                 }
        | expr GE expr          { $$ = $1 >= $3;                }
        | expr LE expr          { $$ = $1 <= $3;                }
        | '(' expr ')'          { $$ = $2;                      }
        ;

arglist:
        expr                    { $$ = makeArgNode($1);         }
        | arglist ',' expr      { $$ = appendArgNode($1, $3);   }
        ;

%% /* end of grammar */

/* map strings to their corresponding function pointers */
static struct {
    char const *fname;
    double (*fnct) (argNode *);
    } coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode,
    "print", print,
    "sin", nsin,
    "cos", ncos,
    "ln", nlog,
    "exp", nexp,
    "sqrt", nsqrt,
    0, 0
};

void initFunctionMap (void) {
    int i;
    for (i = 0; coreFunctions[i].fname != 0; i++) {
        symRec *ptr = putSym (coreFunctions[i].fname, FNCT);
        ptr->value.fnPtr = coreFunctions[i].fnct;
    }
}

/* the symbol table: a chain of `struct symRec'. */
symRec *symTable;

/* add a new symbol to the table */
symRec *putSym (char const *symName, int symType) {
    symRec *ptr = (symRec *) malloc (sizeof (symRec));
    ptr->name = (char *) malloc (strlen (symName) + 1);
    strcpy (ptr->name,symName);
    ptr->type = symType;
    ptr->value.var = 0; /* Set value to 0 even if fctn. */
    ptr->next = (struct symRec *)symTable;
    symTable = ptr;
    return ptr;
}

/* get the value of a symbol in the table */
symRec *getSym (char const *symName) {
    symRec *ptr;
    for (ptr = symTable; ptr != (symRec *) 0; ptr = (symRec *)ptr->next) {
        if (strcmp (ptr->name,symName) == 0)
            return ptr;
    }
    return 0;
}

/* create the root node of an argument list */
argNode *makeArgNode(double dub) {
    argNode *root = (argNode*)malloc(sizeof(argNode));
    root->dValue = dub;
    root->head = root;
    return root;
}

/* add an argument to the end of a linked list of arguments */
argNode *appendArgNode(argNode *args, double dub) {
    argNode *newArg = (argNode*)malloc(sizeof(argNode));
    newArg->dValue = dub;
    newArg->head = args->head;
    args->next = newArg;
    return newArg;
}

/* Called by yyparse on error. */
void yyerror(char *s) {
    fprintf(stderr, "delbrot:%d error: %s\n", linenumber, s);
}

int main () {
    initFunctionMap();
    return yyparse();
}