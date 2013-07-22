%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    void yyerror(char *, ...);
    extern int linenumber;
    ASTnode *unfreed[8192];
    #define YYDEBUG 1
%}

%token T_INT T_DOUBLE T_STRING
%token CONSTANT IDENTIFIER OPTARG
%token LE GE EQ NE
%token ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%token IF ELSE FOR WHILE
%token FNCT FNDEF RETURN

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
    | program item                                            { ex($2); freeNodes(0);                  }
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
    : T_INT primary_expr                                      { $2->type = typeVal; $$ = $2;           }
    | T_DOUBLE primary_expr                                   { $2->type = typeVal; $$ = $2;           }
    | T_STRING primary_expr                                   { $2->type = typeStr; $$ = $2;           }
    ;

stmt
    : expression_stmt
    | selection_stmt
    | iteration_stmt
    | increment_stmt
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

increment_stmt
    : primary_expr INC ';'                                    { $$ = mkOpNode(INC, 1, $1);             }
    | primary_expr DEC ';'                                    { $$ = mkOpNode(DEC, 1, $1);             }
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
    | primary_expr MODEQ assignment_expr                      { $$ = mkOpNode(MODEQ, 2, $1, $3);       }
    ;

boolean_expr
    : arithmetic_expr
    | boolean_expr EQ arithmetic_expr                         { $$ = mkOpNode(EQ,  2, $1, $3);         }
    | boolean_expr NE arithmetic_expr                         { $$ = mkOpNode(NE,  2, $1, $3);         }
    | boolean_expr GE arithmetic_expr                         { $$ = mkOpNode(GE,  2, $1, $3);         }
    | boolean_expr LE arithmetic_expr                         { $$ = mkOpNode(LE,  2, $1, $3);         }
    | boolean_expr '>' arithmetic_expr                        { $$ = mkOpNode('>', 2, $1, $3);         }
    | boolean_expr '<' arithmetic_expr                        { $$ = mkOpNode('<', 2, $1, $3);         }
    | boolean_expr LOR arithmetic_expr                        { $$ = mkOpNode(LOR, 2, $1, $3);         }
    | boolean_expr LAND arithmetic_expr                       { $$ = mkOpNode(LAND,2, $1, $3);         }
    ;

arithmetic_expr
    : prefix_expr
    | arithmetic_expr '+' prefix_expr                         { $$ = mkOpNode('+', 2, $1, $3);         }
    | arithmetic_expr '-' prefix_expr                         { $$ = mkOpNode('-', 2, $1, $3);         }
    | arithmetic_expr '*' prefix_expr                         { $$ = mkOpNode('*', 2, $1, $3);         }
    | arithmetic_expr '/' prefix_expr                         { $$ = mkOpNode('/', 2, $1, $3);         }
    | arithmetic_expr '^' prefix_expr                         { $$ = mkOpNode('^', 2, $1, $3);         }
    | arithmetic_expr '%' prefix_expr                         { $$ = mkOpNode('%', 2, $1, $3);         }
    ;

prefix_expr
    : function_expr
    | '-' prefix_expr                                         { $$ = mkOpNode(NEG, 1, $2);             }
    | '!' prefix_expr                                         { $$ = mkOpNode('!', 1, $2);             }
    ;

function_expr
    : primary_expr
    | primary_expr '(' arg_list ')'                           { $$ = mkOpNode(FNCT, 2, $1, $3);        }
    | function_expr '.' primary_expr '(' arg_list ')'         { $$ = mkOpNode('.',  3, $1, $3, $5);    }
    | function_expr '.' primary_expr                          { $$ = mkOpNode('.',  3, $1, $3, NULL);  }
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

/* allocate space for a new node */
ASTnode *newNode(int i) {
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* seek to open slot (normal and duplicate nodes are interleaved) */
    for (i; unfreed[i]; i += 2);
    unfreed[i] = p;
    return p;
}

/* create a value node in the AST */
ASTnode *mkValNode(double val) {
    ASTnode *p = newNode(0);
    p->type = typeVal;
    p->val = val;
    return p;
}

/* create a string node in the AST */
ASTnode *mkStrNode(char *str) {
    ASTnode *p = newNode(0);
    p->type = typeStr;
    p->str = strdup(str);
    return p;
}

/* create an identifier node in the AST */
ASTnode *mkIdNode(char *ident) {
    ASTnode *p = newNode(0);
    p->type = typeId;
    p->str = strdup(ident);
    p->scope = &globalVars;
    return p;
}

/* create an optional argument node in the AST */
/* reuse the var struct, it's close enough */
ASTnode *mkOptArgNode(char *name) {
    ASTnode *p = newNode(0);
    /* allocate space for var */
    if ((p->var = malloc(sizeof(varRec))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeOptArg;
    p->var->name = name;
    return p;
}

ASTnode *mkTypeNode(int type) {
    ASTnode *p = newNode(0);
    p->type = type;
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p = newNode(0);
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

/* destroy evaluated nodes in the AST */
/* TODO: make this smarter. Maybe attach a value to each node and decide whether to free based on that. */
void freeNodes(int i) {
    // for (i; unfreed[i]; i += 2) {
    //     if(!unfreed[i + 2] /* ridiculous hack to prevent freeing of IF/ELSE resolving token */
    //     || unfreed[i + 2]->type != typeOp
    //     || unfreed[i + 2]->op.oper != IF
    //     || unfreed[i + 2]->op.nops != 2)
    //         free(unfreed[i]);
    //     unfreed[i] = NULL;
    // }
}

/* add an ASTnode to the end of a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
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
    varRec *ptr = (varRec *) malloc(sizeof (varRec));
    ptr->name = (char *) malloc(strlen (p->str) + 1);
    strcpy(ptr->name, p->str);
    ptr->next = *p->scope;
    *p->scope = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
varRec *getVar(ASTnode *p) {
    varRec *ptr;
    for (ptr = *p->scope; ptr->name != NULL; ptr = ptr->next)
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
    globalVars = (varRec *) malloc(sizeof(varRec));

    /* initialize function table */
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(&coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putFn(&pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}
