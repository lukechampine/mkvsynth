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
    ASTnode *mkParamNode(var *, ASTnode *); /* create a parameter node in the AST */
              
    void freeNode(ASTnode *);         /* destroy a node in the AST */
    ASTnode *ex(ASTnode *);           /* execute a section of the AST */
    void yyerror(char *);
    extern int linenumber;
    #define YYDEBUG 1
%}

%union {
    ASTnode *ASTptr; /* pointer to node in the AST */
    double  val;     /* number */
    char    *str;    /* string */
    func    fptr;    /* function */
    var     *vptr;   /* variable */
}

%type  <ASTptr> stmt compound_stmt expression_stmt selection_stmt iteration_stmt stmt_list arg_list arg
%type  <ASTptr> expr function_expr assignment_expr arithmetic_expr boolean_expr primary_expr
%token <val>    CONSTANT
%token <str>    STRING_LITERAL
%token <fptr>   FNCT
%token <vptr>   VAR

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE
%right '^'
%left LAND LOR
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%right NEG
%left INC DEC

%token LE GE EQ NE
%token ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token IF ELSE WHILE

%% /* grammar definition section  */

input
    : /* empty program */
    | input stmt                         { ex($2); freeNode($2);                     }
    ;

stmt
    : compound_stmt
    | expression_stmt
    | selection_stmt
    | iteration_stmt
    ;

compound_stmt
    : '{' stmt_list '}'                  { $$ = $2;                                  }
    ;

expression_stmt
    : ';'                                { $$ = mkOpNode(';', 2, NULL, NULL);        }
    | expr ';'
    ;

selection_stmt
    : IF '(' expr ')' stmt %prec IFX     { $$ = mkOpNode(IF, 2, $3, $5);             }
    | IF '(' expr ')' stmt ELSE stmt     { $$ = mkOpNode(IF, 3, $3, $5, $7);         }
    ;

iteration_stmt
    : WHILE '(' expr ')' stmt            { $$ = mkOpNode(WHILE, 2, $3, $5);          }
    ;

stmt_list
    : stmt
    | stmt_list stmt                     { $$ = mkOpNode(';', 2, $1, $2);            }
    ;

expr
    : function_expr
    ;

function_expr
    : assignment_expr
    | FNCT '(' arg_list ')'              { $$ = mkOpNode(FNCT, 2, mkFnNode($1), $3); }
    ;

arg_list
    : arg
    | arg_list ',' arg                   { $$ = append($1, $3);                      }
    ;

arg
    : /* empty */                        { $$ = NULL;                                }
    | expr                               { $$ = $1;                                  }
    | VAR ':' primary_expr               { $$ = mkParamNode($1, $3);                 }

assignment_expr
    : boolean_expr
    | primary_expr '=' assignment_expr   { $$ = mkOpNode('=',   2, $1, $3);          }
    | primary_expr ADDEQ assignment_expr { $$ = mkOpNode(ADDEQ, 2, $1, $3);          }
    | primary_expr SUBEQ assignment_expr { $$ = mkOpNode(SUBEQ, 2, $1, $3);          }
    | primary_expr MULEQ assignment_expr { $$ = mkOpNode(MULEQ, 2, $1, $3);          }
    | primary_expr DIVEQ assignment_expr { $$ = mkOpNode(DIVEQ, 2, $1, $3);          }
    | primary_expr MODEQ assignment_expr { $$ = mkOpNode(MODEQ, 2, $1, $3);          }
    ;

boolean_expr
    : arithmetic_expr
    | '!' arithmetic_expr                { $$ = mkOpNode('!', 1, $2);                }
    | boolean_expr EQ arithmetic_expr    { $$ = mkOpNode(EQ,  2, $1, $3);            }
    | boolean_expr NE arithmetic_expr    { $$ = mkOpNode(NE,  2, $1, $3);            }
    | boolean_expr GE arithmetic_expr    { $$ = mkOpNode(GE,  2, $1, $3);            }
    | boolean_expr LE arithmetic_expr    { $$ = mkOpNode(LE,  2, $1, $3);            }
    | boolean_expr '>' arithmetic_expr   { $$ = mkOpNode('>', 2, $1, $3);            }
    | boolean_expr '<' arithmetic_expr   { $$ = mkOpNode('<', 2, $1, $3);            }
    | boolean_expr LOR arithmetic_expr   { $$ = mkOpNode(LOR, 2, $1, $3);            }
    | boolean_expr LAND arithmetic_expr  { $$ = mkOpNode(LAND,2, $1, $3);            }
    ;

arithmetic_expr
    : primary_expr
    | primary_expr INC                   { $$ = mkOpNode(INC, 1, $1);                }
    | primary_expr DEC                   { $$ = mkOpNode(DEC, 1, $1);                }
    | arithmetic_expr '+' primary_expr   { $$ = mkOpNode('+', 2, $1, $3);            }
    | arithmetic_expr '-' primary_expr   { $$ = mkOpNode('-', 2, $1, $3);            }
    | arithmetic_expr '*' primary_expr   { $$ = mkOpNode('*', 2, $1, $3);            }
    | arithmetic_expr '/' primary_expr   { $$ = mkOpNode('/', 2, $1, $3);            }
    | arithmetic_expr '^' primary_expr   { $$ = mkOpNode('^', 2, $1, $3);            }
    | arithmetic_expr '%' primary_expr   { $$ = mkOpNode('%', 2, $1, $3);            }
    ;

primary_expr
    : CONSTANT                           { $$ = mkValNode($1);                       }
    | STRING_LITERAL                     { $$ = mkStrNode($1);                       }
    | VAR                                { $$ = mkVarNode($1);                       }
    | '(' expr ')'                       { $$ = $2;                                  }
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

/* create a param node in the AST */
/* TODO: switch from using a var to a more general identifier */
ASTnode *mkParamNode(var *vptr, ASTnode *node) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeParam;
    p->varPtr = vptr;
    p->varPtr->value = node;
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
                freeNode(p->op.ops[i]);
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
func getFn(char const *fnName) {
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
    //yydebug = 1;
    return yyparse();
}