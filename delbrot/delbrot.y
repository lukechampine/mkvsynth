%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    void yyerror(char *, ...);
    extern int linenumber;
    int mem = 0;
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
    | program item                                            { ex($2); free($2->op.ops); free($2);                      }
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
    | param_list ',' param                                    { $$ = append($3, $1);                   }
    ;

param
    : INT primary_expr                                        { $2->type = typeVal; $$ = $2;           }
    | DOUBLE primary_expr                                     { $2->type = typeVal; $$ = $2;           }
    | STRING primary_expr                                     { $2->type = typeStr; $$ = $2;           }
    ;

stmt
    /* TODO: add return statement */
    : expression_stmt
    | selection_stmt
    | iteration_stmt
    ;

expression_stmt
    : ';'                                                     { $$ = mkOpNode(';', 2, NULL, NULL);     }
    | expr ';'                                                { $$ = $1;                               }
    ;

selection_stmt
    : IF '(' expr ')' block %prec IFX                         { $$ = mkOpNode(IF, 2, $3, $5);          }
    | IF '(' expr ')' block ELSE block                        { $$ = mkOpNode(IF, 3, $3, $5, $7);      }
    ;

iteration_stmt
    : WHILE '(' expr ')' block                                { $$ = mkOpNode(WHILE, 2, $3, $5);       }
    | FOR '(' expr ';' expr ';' expr ')' block                { $$ = mkOpNode(FOR, 4, $3, $5, $7, $9); }
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
    | OPTARG expr                                             { $1->var->value = $2;                   }
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
    p->next = NULL;
    return p;
}

/* free a node */
void freeNode(ASTnode *p) {
    if (!p)
        return;
    /* recurse to linked node */
    freeNode(p->next);
    free(p);
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
    p->str = strdup(ident);
    p->scope = &globalVars;
    return p;
}

/* create an optional argument node in the AST */
/* reuse the var struct, it's close enough */
ASTnode *mkOptArgNode(char *name) {
    ASTnode *p = newNode();
    /* allocate space for var */
    if ((p->var = malloc(sizeof(varRec))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeOptArg;
    p->var->name = name;
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

/* add an ASTnode to the end of a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
    if (!root || !node)
        yyerror("invalid argument");
    ASTnode *traverse;
    for (traverse = root; traverse->next != NULL; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* the function table */
funcRec *fnTable;

funcRec *putFn(funcRec *fn) {
    fn->next = fnTable;
    fnTable = fn;
    return fn;
}

/* look up a function name's corresponding pointer */
funcRec *getFn(char const *fnName) {
    funcRec *fn;
    for (fn = fnTable; fn != NULL; fn = fn->next)
        if (strcmp (fn->name,fnName) == 0)
            return fn;
    return NULL;
}

/* the global variable table */
varRec *globalVars;

/* allocate a new variable */
varRec *putVar(ASTnode *p) {
    varRec *ptr;
    if ((ptr = malloc(sizeof(varRec))) == NULL)
        yyerror("out of memory");
    ptr->name = strdup(p->str);
    ptr->next = *p->scope;
    *p->scope = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
varRec *getVar(ASTnode *p) {
    varRec *ptr;
    for (ptr = *p->scope; ptr && ptr->name; ptr = ptr->next)
        if (strcmp(ptr->name, p->str) == 0)
            return ptr;
    /* check global scope after local scope */
    if (*p->scope != globalVars)
        for (ptr = globalVars; ptr && ptr->name; ptr = ptr->next)
            if (strcmp(ptr->name, p->str) == 0)
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

/* built-in functions */
static funcRec coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode_AST, NULL, NULL, NULL,
    "print", print, NULL, NULL, NULL,
    "sin", nsin, NULL, NULL, NULL,
    "cos", ncos, NULL, NULL, NULL,
    "ln", nlog, NULL, NULL, NULL,
    "sqrt", nsqrt, NULL, NULL, NULL,
    0, 0, 0, 0, 0
};

int main () {
    int i;
    //yydebug = 1;

    /* initialize global variable table */
    /* TODO: can this be removed? */
    globalVars = (varRec *) malloc(sizeof(varRec));

    /* initialize function table */
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(&coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putFn(&pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}
