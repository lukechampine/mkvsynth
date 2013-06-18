%{
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    ASTnode *mkOpNode(int, int, ...);   /* create an operation node in the AST */
    ASTnode *mkVarNode(symRec *);       /* create a variable node in the AST */
    ASTnode *mkValNode(double);         /* create a value node in the AST */             
    void freeNode(ASTnode *);           /* destroy a node in the AST */
    ASTnode *ex(ASTnode *);             /* execute a section of the AST */
    void yyerror (char *);

    extern int linenumber;
%}

%union {
    ASTnode *ASTptr;                    /* pointer to node in the AST */
    double  val;                        /* number */
    symRec  *tptr;                      /* symbol-table pointer */
}

%type  <ASTptr> stmt stmtlist           /* statements are "parent" nodes in the AST */
%type  <ASTptr> expr arglist            /* expressions are "child" nodes in the AST */
%token <val>  NUM                       /* all numbers are doubles */
%token <tptr> VAR FNCT                  /* variables and functions are symbol-table pointers */

%token WHILE IF ELSE                    /* keywords don't have a type */


%right '='
%left GE LE EQ NE '>' '<'
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

%% /* grammar definition section  */

input:
        /* empty */
        | input stmt              { ex($2); /*freeNode($2);*/                 }
        ;

stmt:
        ';'                       { $$ = mkOpNode(';', 2, NULL, NULL);        }
        | expr ';'                { $$ = $1;                                  }
        | error ';'               { yyerrok;                                  }
        | IF '(' expr ')' stmt    { $$ = mkOpNode(IF, 2, $3, $5);             }
        | WHILE '(' expr ')' stmt { $$ = mkOpNode(WHILE, 2, $3, $5);          }
        | '{' stmtlist '}'        { $$ = $2;                                  }
        ;

stmtlist:
        stmt                      { $$ = $1;                                  }
        | stmtlist stmt           { $$ = mkOpNode(';', 2, $1, $2);            }
        ;

expr:
        NUM                       { $$ = mkValNode($1);                       }
        | VAR                     { $$ = mkVarNode($1);                       }
        | VAR '=' expr            { $$ = mkOpNode('=', 2, mkVarNode($1), $3); }
        | FNCT '(' arglist ')'    { $$ = mkOpNode(FNCT,2, mkVarNode($1), $3); }
        | '-' expr %prec NEG      { $$ = mkOpNode(NEG, 1, $2);                }
        | expr '+' expr           { $$ = mkOpNode('+', 2, $1, $3);            }
        | expr '-' expr           { $$ = mkOpNode('-', 2, $1, $3);            }
        | expr '*' expr           { $$ = mkOpNode('*', 2, $1, $3);            }
        | expr '/' expr           { $$ = mkOpNode('/', 2, $1, $3);            }
        | expr '^' expr           { $$ = mkOpNode('^', 2, $1, $3);            }
        | expr '>' expr           { $$ = mkOpNode('>', 2, $1, $3);            }
        | expr '<' expr           { $$ = mkOpNode('<', 2, $1, $3);            }
        | expr EQ expr            { $$ = mkOpNode(EQ, 2, $1, $3);             }
        | expr NE expr            { $$ = mkOpNode(NE, 2, $1, $3);             }
        | expr GE expr            { $$ = mkOpNode(GE, 2, $1, $3);             }
        | expr LE expr            { $$ = mkOpNode(LE, 2, $1, $3);             }
        | '(' expr ')'            { $$ = $2;                                  }
        ;

arglist:
        expr                      { $$ = $1;                                  }
        | arglist ',' expr        { $$ = append($1, $3);                      }
        ;

%% /* end of grammar */

#define SIZEOF_ASTNODE ((char *)&p->val - (char *)p)

/* create a value node in the AST */
ASTnode *mkValNode(double val) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeVal;
    p->val = val;
    return p;
}

/* create a variable node in the AST */
ASTnode *mkVarNode(symRec *tptr) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeVar;
    p->var = tptr;
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    if ((p->op.ops = malloc(nops * sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeOp;
    p->op.oper = oper;
    p->op.nops = nops;
    int i;
    va_list ap;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->op.ops[i] = va_arg(ap, ASTnode*);
    va_end(ap);
    return p;
}

/* destroy a node in the AST */
void freeNode(ASTnode *p) {
    int i;
    if (!p)
        return;
    if (p->type == typeOp) {
        for (i = 0; i < p->op.nops; i++) {
            freeNode(p->op.ops[i]);
        }
        
        free(p->op.ops);
    }
    free(p);
}

/* add an AST node to the end of a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
    ASTnode *traverse = root;
    while (traverse->next != NULL) traverse = traverse->next;
    traverse->next = node;
    return root;
}

/* map strings to their corresponding function pointers */
static struct {
    char const *fname;
    ASTnode * (*fnct) (ASTnode *, ASTnode *);
} coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode,
    "print", print,
    "sin", nsin,
    "cos", ncos,
    "ln", nlog,
    "sqrt", nsqrt,
    0, 0
};

void initFunctionMap (void) {
    int i;
    for (i = 0; coreFunctions[i].fname != 0; i++) {
        symRec *ptr = putSym (coreFunctions[i].fname, FNCT);
        ptr->fnPtr = coreFunctions[i].fnct;
    }
}

/* the symbol table: a chain of `struct symRec' */
symRec *symTable;

/* add a new symbol to the table */
symRec *putSym (char const *symName, int symType) {
    symRec *ptr = (symRec *) malloc (sizeof (symRec));
    ptr->name = (char *) malloc (strlen (symName) + 1);
    strcpy (ptr->name,symName);
    ptr->type = symType;
    ptr->val = 0; /* Set value to 0 even if function */
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

/* Called by yyparse on error. */
void yyerror(char *s) {
    fprintf(stderr, "delbrot:%d error: %s\n", linenumber, s);
}

int main () {
    initFunctionMap();
    return yyparse();
}