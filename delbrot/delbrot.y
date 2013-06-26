%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    ASTnode *mkOpNode(int, int, ...); /* create an operation node in the AST */
    ASTnode *mkValNode(double);       /* create a value node in the AST */
    ASTnode *mkStrNode(char *);       /* create a string node in the AST */
    ASTnode *mkFnNode(func);          /* create a function node in the AST */
    ASTnode *mkVarNode(var *);        /* create a variable node in the AST */
              
    void freeNode(ASTnode *);         /* destroy a node in the AST */
    ASTnode *ex(ASTnode *);           /* execute a section of the AST */
    void yyerror(char *);

    extern int linenumber;
%}

%union {
    ASTnode *ASTptr; /* pointer to node in the AST */
    double  val;     /* number */
    char    *str;    /* string */
    func    fptr;    /* function */
    var     *vptr;   /* variable */
}

%type  <ASTptr> stmt stmtlist /* statements are "parent" nodes in the AST */
%type  <ASTptr> expr arglist  /* expressions are "child" nodes in the AST */
%token <val>    NUM           /* all numbers are doubles */
%token <str>    STRING        /* strings are char*s */ 
%token <fptr>   FNCT          /* functions are function pointers */
%token <vptr>   VAR           /* variables are variable table pointers */

%token WHILE IF ELSE          /* keywords don't have a type */

%nonassoc IFX
%nonassoc ELSE
%right '='
%right '^'
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%right NEG
%left INC DEC

%% /* grammar definition section  */

input:
        /* empty */
        | input stmt                     { ex($2); freeNode($2);                     }
        ;

stmt:
        ';'                              { $$ = mkOpNode(';', 2, NULL, NULL);        }
        | expr ';'                       { $$ = $1;                                  }
        | IF '(' expr ')' stmt %prec IFX { $$ = mkOpNode(IF, 2, $3, $5);             }
        | IF '(' expr ')' stmt ELSE stmt { $$ = mkOpNode(IF, 3, $3, $5, $7);         }
        | WHILE '(' expr ')' stmt        { $$ = mkOpNode(WHILE, 2, $3, $5);          }
        | '{' stmtlist '}'               { $$ = $2;                                  }
        | error ';'                      { yyerrok;                                  }
        | error '}'                      { yyerrok;                                  }
        ;

stmtlist:
        stmt                             { $$ = $1;                                  }
        | stmtlist stmt                  { $$ = mkOpNode(';', 2, $1, $2);            }
        ;

expr:
        NUM                              { $$ = mkValNode($1);                       }
        |STRING                          { $$ = mkStrNode($1);                       }
        | VAR                            { $$ = mkVarNode($1);                       }
        | VAR '=' expr                   { $$ = mkOpNode('=', 2, mkVarNode($1), $3); }
        | VAR INC                        { $$ = mkOpNode(INC, 1, mkVarNode($1));     }
        | VAR DEC                        { $$ = mkOpNode(DEC, 1, mkVarNode($1));     }
        | FNCT '(' arglist ')'           { $$ = mkOpNode(FNCT,2, mkFnNode($1), $3);  }
        | '-' expr %prec NEG             { $$ = mkOpNode(NEG, 1, $2);                }
        | expr '+' expr                  { $$ = mkOpNode('+', 2, $1, $3);            }
        | expr '-' expr                  { $$ = mkOpNode('-', 2, $1, $3);            }
        | expr '*' expr                  { $$ = mkOpNode('*', 2, $1, $3);            }
        | expr '/' expr                  { $$ = mkOpNode('/', 2, $1, $3);            }
        | expr '%' expr                  { $$ = mkOpNode('%', 2, $1, $3);            }
        | expr '^' expr                  { $$ = mkOpNode('^', 2, $1, $3);            }
        | expr '>' expr                  { $$ = mkOpNode('>', 2, $1, $3);            }
        | expr '<' expr                  { $$ = mkOpNode('<', 2, $1, $3);            }
        | expr EQ expr                   { $$ = mkOpNode(EQ, 2, $1, $3);             }
        | expr NE expr                   { $$ = mkOpNode(NE, 2, $1, $3);             }
        | expr GE expr                   { $$ = mkOpNode(GE, 2, $1, $3);             }
        | expr LE expr                   { $$ = mkOpNode(LE, 2, $1, $3);             }
        | '(' expr ')'                   { $$ = $2;                                  }
        ;

arglist:
        /* empty */                      { $$ = NULL;                                }
        | expr                           { $$ = $1;                                  }
        | arglist ',' expr               { $$ = append($1, $3);                      }
        ;

%% /* end of grammar */

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

/* create a string node in the AST */
ASTnode *mkStrNode(char *str) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeStr;
    p->str = str;
    return p;
}

/* create a function node in the AST */
ASTnode *mkFnNode(func fptr) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeFn;
    p->fnPtr = fptr;
    return p;
}

/* create a variable node in the AST */
ASTnode *mkVarNode(var *vptr) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeVar;
    p->varPtr = vptr;
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
    /* traverse linked list, if it exists */
    while(p) {
        ASTnode* next = p->next;
        if (p->type == typeOp)
            for (i = 0; i < p->op.nops; i++);
                //freeNode(p->op.ops[i]);
        free(p);
        p = next;
    }
}

/* add an AST node to the end of a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
    ASTnode *traverse;
    for (traverse = root; traverse->next != NULL; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* map strings to their corresponding function pointers */
static struct {
    char const *fname;
    func fnPtr;
} coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode,
    "print", print,
    "sin", nsin,
    "cos", ncos,
    "ln", nlog,
    "sqrt", nsqrt,
    0, 0
};

/* look up a function name's corresponding pointer */
func getFn (char const *fnName) {
    int i;
    for (i = 0; coreFunctions[i].fname != 0; i++)
        if (strcmp (coreFunctions[i].fname,fnName) == 0)
            return coreFunctions[i].fnPtr;
    return NULL;
}

/* the variable table */
var *varTable;

/* allocate a new variable */
var *putVar (char const *varName) {
    var *ptr = (var *) malloc(sizeof (var));
    ptr->name = (char *) malloc(strlen (varName) + 1);
    strcpy(ptr->name,varName);
    ptr->value = (ASTnode *) malloc(sizeof (ASTnode)); /* allocate space for ASTnode */
    ptr->value->type = typeVar;
    ptr->next = varTable;
    varTable = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
var *getVar (char const *varName) {
    var *ptr;
    for (ptr = varTable; ptr != NULL; ptr = ptr->next)
        if (strcmp (ptr->name,varName) == 0)
            return ptr;
    return NULL;
}

/* Called by yyparse on error. */
void yyerror(char *s) {
    fprintf(stderr, "delbrot:%d error: %s\n", linenumber, s);
    exit(1);
}

int main () {
    return yyparse();
}