%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    void freeNode(ASTnode *);         /* destroy a node in the AST */
    ASTnode *ex(ASTnode *);           /* execute a section of the AST */
    void yyerror(char *);
    extern int linenumber;
    #define YYDEBUG 1
%}

%token T_INT T_DOUBLE T_STRING
%token CONSTANT STRING_LITERAL IDENTIFIER
%token LE GE EQ NE
%token ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token IF ELSE WHILE
%token FNCT FNDEF VARDEF

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
    | program item                                           { ex($2); /*freeNode($2);*/                 }
    ;

item
    : declaration
    | stmt
    ;

declaration
    : function_declaration
    | variable_declaration
    ;

function_declaration
    : type primary_expr '(' param_list ')' '{' stmt_list '}' { $$ = mkOpNode(FNDEF, 4, $1, $2, $4, $7);  }
    | type primary_expr '(' param_list ')' ';'               { $$ = mkOpNode(FNDEF, 3, $1, $2, $4);      }
    ;

variable_declaration
    : type primary_expr '=' expr ';'                         { $$ = mkOpNode(VARDEF, 3, $1, $2, $4);     }
    | type primary_expr ';'                                  /* TODO: make this work */
    ;

type
    : T_INT                                                  { $$ = mkTypeNode(typeVal);                 }
    | T_DOUBLE                                               { $$ = mkTypeNode(typeVal);                 }
    | T_STRING                                               { $$ = mkTypeNode(typeStr);                 }
    ;

param_list
    : param_decl
    | param_list ',' param_decl
    ;

    /* TODO: optional parameters? */
param_decl
    : type primary_expr
    | type
    ;

stmt_list
    : stmt
    | stmt_list stmt                                         { $$ = mkOpNode(';', 2, $1, $2);            }
    ;

stmt
    : expression_stmt
    | selection_stmt    
    | iteration_stmt
    | increment_stmt
    ;

expression_stmt
    : ';'                                                    { $$ = mkOpNode(';', 2, NULL, NULL);        }
    | expr ';'                                               { $$ = $1;                                  }
    ;

selection_stmt
    : IF '(' expr ')' comp_stmt %prec IFX                    { $$ = mkOpNode(IF, 2, $3, $5);             }
    | IF '(' expr ')' comp_stmt ELSE comp_stmt               { $$ = mkOpNode(IF, 3, $3, $5, $7);         }
    ;

iteration_stmt
    : WHILE '(' expr ')' comp_stmt                           { $$ = mkOpNode(WHILE, 2, $3, $5);          }
    ;

comp_stmt
    : stmt
    | '{' stmt_list '}'                                      { $$ = $2;                                  }
    ;

increment_stmt
    : primary_expr INC ';'                                   { $$ = mkOpNode(INC, 1, $1);                }
    | primary_expr DEC ';'                                   { $$ = mkOpNode(DEC, 1, $1);                }
    ;

expr
    : assignment_expr
    ;

assignment_expr
    : boolean_expr
    | primary_expr  '='  assignment_expr                     { $$ = mkOpNode('=',   2, $1, $3);          }
    | primary_expr ADDEQ assignment_expr                     { $$ = mkOpNode(ADDEQ, 2, $1, $3);          }
    | primary_expr SUBEQ assignment_expr                     { $$ = mkOpNode(SUBEQ, 2, $1, $3);          }
    | primary_expr MULEQ assignment_expr                     { $$ = mkOpNode(MULEQ, 2, $1, $3);          }
    | primary_expr DIVEQ assignment_expr                     { $$ = mkOpNode(DIVEQ, 2, $1, $3);          }
    | primary_expr MODEQ assignment_expr                     { $$ = mkOpNode(MODEQ, 2, $1, $3);          }
    ;

boolean_expr
    : arithmetic_expr
    | boolean_expr EQ arithmetic_expr                        { $$ = mkOpNode(EQ,  2, $1, $3);            }
    | boolean_expr NE arithmetic_expr                        { $$ = mkOpNode(NE,  2, $1, $3);            }
    | boolean_expr GE arithmetic_expr                        { $$ = mkOpNode(GE,  2, $1, $3);            }
    | boolean_expr LE arithmetic_expr                        { $$ = mkOpNode(LE,  2, $1, $3);            }
    | boolean_expr '>' arithmetic_expr                       { $$ = mkOpNode('>', 2, $1, $3);            }
    | boolean_expr '<' arithmetic_expr                       { $$ = mkOpNode('<', 2, $1, $3);            }
    | boolean_expr LOR arithmetic_expr                       { $$ = mkOpNode(LOR, 2, $1, $3);            }
    | boolean_expr LAND arithmetic_expr                      { $$ = mkOpNode(LAND,2, $1, $3);            }
    ;

arithmetic_expr
    : function_expr
    | arithmetic_expr '+' function_expr                      { $$ = mkOpNode('+', 2, $1, $3);            }
    | arithmetic_expr '-' function_expr                      { $$ = mkOpNode('-', 2, $1, $3);            }
    | arithmetic_expr '*' function_expr                      { $$ = mkOpNode('*', 2, $1, $3);            }
    | arithmetic_expr '/' function_expr                      { $$ = mkOpNode('/', 2, $1, $3);            }
    | arithmetic_expr '^' function_expr                      { $$ = mkOpNode('^', 2, $1, $3);            }
    | arithmetic_expr '%' function_expr                      { $$ = mkOpNode('%', 2, $1, $3);            }
    ;

function_expr
    : prefix_expr
    | function_expr '(' arg_list ')'                         { $$ = mkOpNode(FNCT, 2, $1, $3);           }
    ;

arg_list
    : /* empty */                                            { $$ = NULL;                                }
    | function_arg                                           { $$ = $1;                                  }
    | arg_list ',' function_arg                              { $$ = appendArg($1, $3);                   }
    ;

function_arg
    : expr
    | primary_expr ':' expr                                  { $$ = mkParamNode($1, $3);                 }
    ;

prefix_expr
    : primary_expr
    | '-' prefix_expr                                        { $$ = mkOpNode(NEG, 1, $2);                }
    | '!' prefix_expr                                        { $$ = mkOpNode('!', 1, $2);                }
    ;

primary_expr
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expr ')'                                           { $$ = $2;                                  }
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
    p->str = strdup(str);
    return p;
}

/* create a variable or function node in the AST */
ASTnode *mkIdNode(char *ident) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* look up identifier in variable and function tables */
    var *v; func f;
    if ((f = getFn(ident)) != NULL) {
        p->type = typeFn;
        p->fnPtr = f;
    }
    else if ((v = getVar(ident)) != NULL) {
        p->type = typeVar;
        p->varPtr = v;
    }
    /* could be a new variable or a parameter, so don't do anything yet */
    else {
        p->type = typeId;
        p->id = strdup(ident);
    }
    return p;
}

/* create a param node in the AST */
/* reuse the var struct, it's close enough */
ASTnode *mkParamNode(ASTnode *ident, ASTnode *node) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    if ((p->varPtr = malloc(sizeof(var))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeParam;
    p->varPtr->name = ident->str;
    p->varPtr->type = node->type;
    p->varPtr->value = node;
    return p;
}

ASTnode *mkTypeNode(int type) {
    ASTnode *p;
    /* allocate node */
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = type;
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

/* add an AST node to the end of a linked list of arguments */
ASTnode *appendArg(ASTnode *root, ASTnode *node) {
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
void yyerror(char *s) {
    fprintf(stderr, "delbrot:%d error: %s\n", linenumber, s);
    exit(1);
}

int main () {
    //yydebug = 1;
    return yyparse();
}
