%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    Env *global;
    void yyerror(char *, ...);
    extern int linenumber;
    #define YYDEBUG 1
%}

%token INT DOUBLE STRING
%token CONSTANT IDENTIFIER OPTARG
%token BINOP
%token ADDEQ SUBEQ MULEQ DIVEQ POWEQ MODEQ
%token IF ELSE FOR WHILE
%token FNCT FNDEF RETURN

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE
%right '^'
%left EQ NE GT LT GE LE LOR LAND
%left '+' '-'
%left '*' '/' '%'
%right NEG
%left INC DEC


%% /* grammar definition section  */

program
    : /* empty program */
    | program item                                            { ex(global, $2);                        }
    ;

item
    : function_declaration
    | stmt
    ;

function_declaration
    : FNDEF primary_expr '(' param_list ')' '{' stmt_list '}' { $$ = mkOpNode(FNDEF, 3, $2, $4, $7);   }
    ;

param_list
    : /* empty */                                             { $$ = NULL;                             }
    | param                                                   { $$ = $1;                               }
    | param_list ',' param                                    { $$ = append($1, $3);                   }
    ;

param
    : INT primary_expr                                        { $2->type = typeVal; $$ = $2;           }
    | DOUBLE primary_expr                                     { $2->type = typeVal; $$ = $2;           }
    | STRING primary_expr                                     { $2->type = typeStr; $$ = $2;           }
    ;

stmt
    : expression_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    ;

expression_stmt
    : ';'                                                     { $$ = mkOpNode(';', 2, NULL, NULL);     }
    | expr ';'                                                { $$ = mkOpNode(';', 2, $1, NULL);       }
    ;

selection_stmt
    : IF '(' expr ')' block %prec IFX                         { $$ = mkOpNode(IF, 2, $3, $5);          }
    | IF '(' expr ')' block ELSE block                        { $$ = mkOpNode(IF, 3, $3, $5, $7);      }
    ;

iteration_stmt
    : WHILE '(' expr ')' block                                { $$ = mkOpNode(WHILE, 2, $3, $5);       }
    | FOR '(' expr ';' expr ';' expr ')' block                { $$ = mkOpNode(FOR, 4, $3, $5, $7, $9); }
    ;

return_stmt
    : RETURN expr ';'                                         { $$ = mkOpNode(RETURN, 1, $2);          }
    | RETURN ';'                                              { $$ = mkOpNode(RETURN, 1, NULL);        }
    ;

block
    : stmt
    | '{' stmt_list '}'                                       { $$ = $2;                               }
    ;

stmt_list
    : stmt
    | stmt_list stmt                                          { $$ = mkOpNode(';', 2, $1, $2);         }
    ;

expr
    : assignment_expr
    ;

assignment_expr
    : boolean_expr
    | primary_expr  '='  assignment_expr                      { $$ = mkOpNode('=',   2, $1, $3);       }
    | primary_expr ADDEQ assignment_expr                      { $$ = mkOpNode(ADDEQ, 2, $1, $3);       }
    | primary_expr SUBEQ assignment_expr                      { $$ = mkOpNode(SUBEQ, 2, $1, $3);       }
    | primary_expr MULEQ assignment_expr                      { $$ = mkOpNode(MULEQ, 2, $1, $3);       }
    | primary_expr DIVEQ assignment_expr                      { $$ = mkOpNode(DIVEQ, 2, $1, $3);       }
    | primary_expr POWEQ assignment_expr                      { $$ = mkOpNode(POWEQ, 2, $1, $3);       }
    | primary_expr MODEQ assignment_expr                      { $$ = mkOpNode(MODEQ, 2, $1, $3);       }
    ;

boolean_expr
    : arithmetic_expr
    | boolean_expr boolean_operator arithmetic_expr           { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

boolean_operator
    : EQ | NE | GT | LT | GE | LE | LOR | LAND
    ;

arithmetic_expr
    : prefix_expr
    | arithmetic_expr arithmetic_operator prefix_expr         { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

arithmetic_operator
    : '+' | '-' | '*' | '/' | '^' | '%'
    ;

prefix_expr
    : postfix_expr
    | '-' prefix_expr                                         { $$ = mkOpNode(NEG, 1, $2);             }
    | '!' prefix_expr                                         { $$ = mkOpNode('!', 1, $2);             }
    ;

postfix_expr
    : function_expr
    | postfix_expr INC                                        { $$ = mkOpNode(INC, 1, $1);             }
    | postfix_expr DEC                                        { $$ = mkOpNode(DEC, 1, $1);             }
    ;

function_expr
    : primary_expr
    | primary_expr '(' arg_list ')'                           { $$ = mkOpNode(FNCT, 2, $1, $3);        }
    | function_expr '.' primary_expr '(' arg_list ')'         { $$ = mkOpNode('.',  3, $1, $3, $5);    }
    | function_expr '.' primary_expr  /* sugar */             { $$ = mkOpNode('.',  3, $1, $3, NULL);  }
    ;

arg_list
    : /* empty */                                             { $$ = NULL;                             }
    | function_arg                                            { $$ = $1;                               }
    | arg_list ',' function_arg                               { $$ = append($1, $3);                   }
    ;

function_arg
    : expr                                                    { $$ = $1;                               }
    | OPTARG expr                                             { $1->opt.value = $2;                    }
    ;

primary_expr
    : IDENTIFIER                                              { $$ = $1;                               }
    | CONSTANT                                                { $$ = $1;                               }
    | '(' expr ')'                                            { $$ = $2;                               }
    ;

%% /* end of grammar */

/* initialize a new node */
ASTnode *newNode() {
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* record in unfreed table */
    p->next = NULL;
    return p;
}

/* collect garbage */
void freeNodes() {
    /* free */
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
    /* remove enclosing quotation marks */
    p->str = strdup(str + 1);
    p->str[strlen(str)-2] = 0;
    return p;
}

/* create an identifier node in the AST */
ASTnode *mkIdNode(char *ident) {
    ASTnode *p = newNode();
    p->type = typeId;
    p->id = strdup(ident);
    return p;
}

/* create an optional argument node in the AST */
ASTnode *mkOptArgNode(char *name) {
    ASTnode *p = newNode();
    p->type = typeOptArg;
    p->opt.name = strdup(name);
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p = newNode();
    /* allocate space for ops */
    if ((p->op.ops = malloc(nops * sizeof(ASTnode *))) == NULL)
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

/* add an ASTnode to a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
    if (!root)
        yyerror("invalid argument");
    ASTnode *traverse;
    for(traverse = root; traverse->next; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* add a core function to the function table */
ASTnode *putCoreFn(Env *e, fnEntry fn) {
    /* create entry */
    ASTnode *ptr;
    if ((ptr = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    ptr->type = typeFn;
    ptr->fn.name = fn.name;
    ptr->fn.core.fnPtr = fn.fnPtr;
    /* add to function table */
    ptr->next = e->fnTable;
    e->fnTable = ptr;
    return ptr;
}

/* look up a function */
ASTnode *getFn(Env const *e, char const *fnName) {
    if (!e)
        return NULL;
    ASTnode *traverse;
    for (traverse = e->fnTable; traverse != NULL; traverse = traverse->next)
        if (strcmp(traverse->fn.name, fnName) == 0)
            return traverse;
    /* check parent environment */
    return getFn(e->parent, fnName);
}

/* allocate a new variable */
ASTnode *putVar(Env *e, char const *varName) {
    /* create entry */
    ASTnode *ptr;
    if ((ptr = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    ptr->type = typeVar;
    ptr->var.name = strdup(varName);
    ptr->var.value = NULL;
    /* add to variable table */
    ptr->next = e->varTable;
    e->varTable = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
ASTnode *getVar(Env const *e, char const *varName) {
    if (!e)
        return NULL;
    ASTnode *traverse;
    for (traverse = e->varTable; traverse != NULL; traverse = traverse->next)
        if (strcmp(traverse->var.name, varName) == 0)
            return traverse;
    /* check parent environment */
    return getVar(e->parent, varName);
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

/* built-in functions */
static fnEntry coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode_AST,
    "print", print,
    "sin", nsin,
    "cos", ncos,
    "ln", nlog,
    "sqrt", nsqrt,
    0, 0,
};

int main () {
    int i;
    //yydebug = 1;

    /* create global environment */
    global = (Env *) malloc(sizeof(Env));
    global->varTable = NULL;
    global->parent = NULL;

    if (setjmp(global->returnContext) != 0)
        exit(0);

    /* initialize function table */
    for(i = 0; coreFunctions[i].name != 0; i++)
        putCoreFn(global, coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putCoreFn(global, pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}
