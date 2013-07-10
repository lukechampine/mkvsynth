%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    void freeNode(ASTnode *);         /* destroy a node in the AST */
    void yyerror(char *, ...);
    extern int linenumber;
    #define YYDEBUG 1
%}

%token T_INT T_DOUBLE T_STRING
%token CONSTANT IDENTIFIER PARAM
%token LE GE EQ NE
%token ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token IF ELSE FOR WHILE
%token FNCT FNDEF

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE
%right '^'
%left LAND LOR
%left '+' '-'
%left '*' '/' '%'
%right NEG
%left INC DEC


%% /* grammar definition section  */

    /* TODO: consider delaying AST evaluation until it has been fully constructed */
program
    : /* empty program */
    | program item                                            { ex($2); /*freeNode($2);*/                   }
    ;

item
    : function_declaration
    | stmt
    ;

    /* TODO: make function definitions more Haskell-like */
function_declaration
    : FNDEF primary_expr '(' param_list ')' '{' item_list '}' { $$ = mkOpNode(FNDEF, 4, $1, $2, $4, $6);  }
    | FNDEF primary_expr '(' param_list ')' ';'               { $$ = mkOpNode(FNDEF, 3, $1, $2, $4);      }
    ;

param_list
    : param_decl
    | param_list ',' param_decl
    ;

    /* TODO: optional parameters? */
param_decl
    : /* empty */
    | type primary_expr
    ;

type
    : T_INT                                                   { $$ = mkTypeNode(typeVal);                 }
    | T_DOUBLE                                                { $$ = mkTypeNode(typeVal);                 }
    | T_STRING                                                { $$ = mkTypeNode(typeStr);                 }
    ;

stmt
    : expression_stmt
    | selection_stmt
    | iteration_stmt                                          { $1->readonly = 1; /* set readonly flag */ }
    | increment_stmt
    ;

expression_stmt
    : ';'                                                     { $$ = mkOpNode(';', 2, NULL, NULL);        }
    | expr ';'                                                { $$ = $1;                                  }
    ;

selection_stmt
    : IF '(' expr ')' block %prec IFX                         { $$ = mkOpNode(IF, 2, $3, $5);             }
    | IF '(' expr ')' block ELSE block                        { $$ = mkOpNode(IF, 3, $3, $5, $7);         }
    ;

iteration_stmt
    : WHILE '(' expr ')' block                                { $$ = mkOpNode(WHILE, 2, $3, $5);          }
    | FOR '(' expr ';' expr ';' expr ')' block                { $$ = mkOpNode(FOR, 4, $3, $5, $7, $9);    }
    ;

block
    : item
    | '{' item_list '}'                                       { $$ = $2;                                  }
    ;

item_list
    : item
    | item_list item                                          { $$ = mkOpNode(';', 2, $1, $2);            }
    ;

increment_stmt
    : primary_expr INC ';'                                    { $$ = mkOpNode(INC, 1, $1);                }
    | primary_expr DEC ';'                                    { $$ = mkOpNode(DEC, 1, $1);                }
    ;

expr
    : assignment_expr
    ;

assignment_expr
    : boolean_expr
    | primary_expr  '='  assignment_expr                      { $$ = mkOpNode('=',   2, $1, $3);          }
    | primary_expr ADDEQ assignment_expr                      { $$ = mkOpNode(ADDEQ, 2, $1, $3);          }
    | primary_expr SUBEQ assignment_expr                      { $$ = mkOpNode(SUBEQ, 2, $1, $3);          }
    | primary_expr MULEQ assignment_expr                      { $$ = mkOpNode(MULEQ, 2, $1, $3);          }
    | primary_expr DIVEQ assignment_expr                      { $$ = mkOpNode(DIVEQ, 2, $1, $3);          }
    | primary_expr MODEQ assignment_expr                      { $$ = mkOpNode(MODEQ, 2, $1, $3);          }
    ;

boolean_expr
    : arithmetic_expr
    | boolean_expr EQ arithmetic_expr                         { $$ = mkOpNode(EQ,  2, $1, $3);            }
    | boolean_expr NE arithmetic_expr                         { $$ = mkOpNode(NE,  2, $1, $3);            }
    | boolean_expr GE arithmetic_expr                         { $$ = mkOpNode(GE,  2, $1, $3);            }
    | boolean_expr LE arithmetic_expr                         { $$ = mkOpNode(LE,  2, $1, $3);            }
    | boolean_expr '>' arithmetic_expr                        { $$ = mkOpNode('>', 2, $1, $3);            }
    | boolean_expr '<' arithmetic_expr                        { $$ = mkOpNode('<', 2, $1, $3);            }
    | boolean_expr LOR arithmetic_expr                        { $$ = mkOpNode(LOR, 2, $1, $3);            }
    | boolean_expr LAND arithmetic_expr                       { $$ = mkOpNode(LAND,2, $1, $3);            }
    ;

arithmetic_expr
    : function_expr
    | arithmetic_expr '+' function_expr                       { $$ = mkOpNode('+', 2, $1, $3);            }
    | arithmetic_expr '-' function_expr                       { $$ = mkOpNode('-', 2, $1, $3);            }
    | arithmetic_expr '*' function_expr                       { $$ = mkOpNode('*', 2, $1, $3);            }
    | arithmetic_expr '/' function_expr                       { $$ = mkOpNode('/', 2, $1, $3);            }
    | arithmetic_expr '^' function_expr                       { $$ = mkOpNode('^', 2, $1, $3);            }
    | arithmetic_expr '%' function_expr                       { $$ = mkOpNode('%', 2, $1, $3);            }
    ;

function_expr
    : prefix_expr
    | function_composition '(' arg_list ')'                   { $$ = mkOpNode(FNCT, 2, $1, $3);           }
    ;

function_composition
    : prefix_expr
    | function_composition '.' prefix_expr                    { $$ = append($3, $1);                      }
    ;

arg_list
    : /* empty */                                             { $$ = NULL;                                }
    | function_arg                                            { $$ = $1;                                  }
    | arg_list ',' function_arg                               { $$ = append($1, $3);                      }
    ;

function_arg
    : expr
    | PARAM expr                                              { $1->varPtr->value = $2;                   }
    ;

prefix_expr
    : primary_expr
    | '-' prefix_expr                                         { $$ = mkOpNode(NEG, 1, $2);                }
    | '!' prefix_expr                                         { $$ = mkOpNode('!', 1, $2);                }
    ;

primary_expr
    : IDENTIFIER
    | CONSTANT
    | '(' expr ')'                                            { $$ = $2;                                  }
    ;

%% /* end of grammar */

/* allocate space for a new node */
ASTnode *newNode() {
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    else
        return p;
}

/* create a value node in the AST */
ASTnode *mkValNode(double val) {
    ASTnode *p = newNode();
    p->type = typeVal;
    p->val = val;
    return p;
}

/* create a string node in the AST */
ASTnode *mkStrNode(char *str) {
    ASTnode *p = newNode();
    p->type = typeStr;
    p->str = strdup(str);
    return p;
}

/* create a variable or function node in the AST */
ASTnode *mkIdNode(char *ident) {
    ASTnode *p = newNode();
    var *v; func f;
    /* function */
    if ((f = getFn(ident)) != NULL) {
        p->type = typeFn;
        p->fnPtr = f;
    }
    /* existing variable */
    else if ((v = getVar(ident)) != NULL) {
        p->type = typeVar;
        p->varPtr = v;
    }
    /* new variable */
    else {
        p->type = typeVar;
        p->varPtr = putVar(ident);
    }
    return p;
}

/* create a param node in the AST */
/* reuse the var struct, it's close enough */
ASTnode *mkParamNode(char *name) {
    ASTnode *p = newNode();
    /* allocate space for var */
    if ((p->varPtr = malloc(sizeof(var))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeParam;
    p->varPtr->name = name;
    return p;
}

ASTnode *mkTypeNode(int type) {
    ASTnode *p = newNode();
    p->type = type;
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p = newNode();
    /* allocate space for ops */
    if ((p->op.ops = malloc(nops * sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
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
    while(p) {
        ASTnode* next = p->next;
        if (p->type == typeOp)
            for (i = 0; i < p->op.nops; i++)
                freeNode(p->op.ops[i]);
        if (p->type != typeVar)
            free(p);
        p = next; /* traverse linked list */
    }
}

/* add an ASTnode to the end of a linked list of arguments */
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
var *putVar(char const *varName) {
    var *ptr = (var *) malloc(sizeof (var));
    ptr->name = (char *) malloc(strlen (varName) + 1);
    strcpy(ptr->name,varName);
    ptr->value = (ASTnode *) malloc(sizeof (ASTnode)); /* allocate space for ASTnode */
    ptr->next = varTable;
    varTable = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
var *getVar(char const *varName) {
    var *ptr;
    for (ptr = varTable; ptr != NULL; ptr = ptr->next)
        if (strcmp (ptr->name,varName) == 0)
            return ptr;
    return NULL;
}

/* Called by yyparse on error. */
void yyerror(char *error, ...) {
    fprintf(stderr, "delbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\n");
    exit(1);
}

int main () {
    // yydebug = 1;
    var *v = putVar("pi");
    v->value->val = 3.141592653589793238462;
    return yyparse();
}
