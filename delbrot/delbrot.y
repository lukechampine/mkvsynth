%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    Env *global;
    extern int linenumber;
    /* script file */
    extern FILE *yyin;
    /* debug */
    #define YYDEBUG 1
%}

%token NUM BOOL STRING CLIP TRUE FALSE
%token CONSTANT IDENTIFIER OPTARG
%token ASSIGN BINOP
%token ADDEQ SUBEQ MULEQ DIVEQ POWEQ MODEQ CHAIN
%token IF ELSE
%token FNCT FNDEF RETURN DEFAULT OTHER

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE
%right '^' TERN
%left EQ NE GT LT GE LE LOR LAND
%left '+' '-'
%left '*' '/' '%'
%right NEG
%left INC DEC


%% /* grammar definition section  */

program
    : /* empty program */
    | program stmt                                            { ex(global, $2);                        }
    ;

stmt
    : function_declaration
    | default_stmt
    | return_stmt
    | expression_stmt
    | selection_stmt
    ;

function_declaration
    : FNDEF primary_expr '(' param_list ')' '{' stmt_list '}' { $$ = mkOpNode(FNDEF, 3, $2, $4, $7);   }
    ;

default_stmt
    : DEFAULT primary_expr '=' expr ';'                       { $$ = mkOpNode(DEFAULT, 2, $2, $4);     }
    ;

return_stmt
    : RETURN expr ';'                                         { $$ = mkOpNode(RETURN, 1, $2);          }
    | RETURN ';'                                              { $$ = mkOpNode(RETURN, 1, NULL);        }
    ;

expression_stmt
    : ';'                                                     { $$ = mkOpNode(';', 2, NULL, NULL);     }
    | expr ';'                                                { $$ = $1;                               }
    ;

selection_stmt
    : IF '(' expr ')' block %prec IFX                         { $$ = mkOpNode(IF, 2, $3, $5);          }
    | IF '(' expr ')' block ELSE block                        { $$ = mkOpNode(IF, 3, $3, $5, $7);      }
    ;

param_list
    : /* empty */                                             { $$ = NULL;                             }
    | param                                                   { $$ = $1;                               }
    | param_list ',' param                                    { $$ = append($1, $3);                   }
    ;

param
    : NUM primary_expr                                        { $2->type = typeNum; $$ = $2;           }
    | BOOL primary_expr                                       { $2->type = typeBool; $$ = $2;          }
    | STRING primary_expr                                     { $2->type = typeStr; $$ = $2;           }
    | CLIP primary_expr                                       { $2->type = typeClip; $$ = $2;          }
    | ':' NUM primary_expr                                    { $2->type = typeOptNum; $$ = $2;        }
    | ':' BOOL primary_expr                                   { $2->type = typeOptBool; $$ = $2;       }
    | ':' STRING primary_expr                                 { $2->type = typeOptStr; $$ = $2;        }
    | ':' CLIP primary_expr                                   { $2->type = typeOptClip; $$ = $2;       }
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
    : ternary_expr
    | primary_expr assignment_operator assignment_expr        { $$ = mkOpNode(ASSIGN, 3, $1, $2, $3);  }
    ;

assignment_operator
    : '=' | ADDEQ | SUBEQ | MULEQ | DIVEQ | POWEQ | MODEQ
    ;

ternary_expr
    : boolean_expr
    | boolean_expr '?' ternary_expr '|' ternary_end           { $$ = mkOpNode(TERN, 3, $1, $3, $5);    } 
    ;

ternary_end
    : ternary_expr                                            { $$ = $1;                               }
    | OTHER '?' ternary_expr                                  { $$ = $3;                               }
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
    | primary_expr arg_list                                   { $$ = mkOpNode(FNCT, 2, $1, $2);        }
    | function_expr CHAIN primary_expr arg_list               { $$ = mkOpNode(CHAIN, 3, $1, $3, $4);   }
    ;

arg_list
    : function_arg                                            { $$ = $1;                               }
    | arg_list function_arg                                   { $$ = append($1, $2);                   }
    ;

function_arg
    : primary_expr                                            { $$ = $1;                               }
    | primary_expr ':' primary_expr                           { $$ = mkOptArgNode($1, $3);             }
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

/* create a number node in the AST */
ASTnode *mkNumNode(double num) {
    ASTnode *p = newNode();
    p->type = typeNum;
    p->num = num;
    return p;
}

/* create a boolean node in the AST */
ASTnode *mkBoolNode(int bool) {
    ASTnode *p = newNode();
    p->type = typeBool;
    p->bool = bool;
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
ASTnode *mkOptArgNode(ASTnode *name, ASTnode *value) {
    ASTnode *p = newNode();
    switch(value->type) {
        case typeNum: p->type = typeOptNum; break;
        case typeStr: p->type = typeOptStr; break;
        case typeClip: p->type = typeOptClip; break;
        default: p->type = typeOptArg; break;
    }
    p->opt.name = name->id;
    memcpy(p->opt.value, value, sizeof(ASTnode));
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
ASTnode *putFn(Env *e, fnEntry fn) {
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
    //return getVar(e->parent, varName);
    return NULL;
}

/* Called by yyparse on error, passed through to Mkvsynth Error */
void yyerror(char *error, ...) {
    va_list arglist;
    va_start(arglist, error);
    MkvsynthError(error, arglist);
    va_end(arglist);
}

/* alias for yyerror */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}


/* built-in functions */
static fnEntry coreFunctions[] = {
    { "MKVsource", MKVsource },
    { "assert",    assert    },
    { "print",     print     },
    { "sin",       nsin      },
    { "cos",       ncos      },
    { "ln",        nlog      },
    { "sqrt",      nsqrt     },
    { 0,           0         },
};

int main(int argc, char **argv) {
    //yydebug = 1;

    /* help message */
    if ((argc != 1 && argc != 2)
    || (argc > 1 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))) {
        printf("Usage: mkvsynth [FILE]\nInterprets an mkvsynth script.\n\nIf FILE is omitted, STDIN will be used instead.\n\nReport bugs on github.com/DavidVorick/mkvsynth.\n");
        exit(0);
    }

    /* read script file, if provided */
    if (argc == 2) {
        yyin = fopen(argv[1], "r+");
        if (!yyin) {
            printf("error: could not open file for reading\n");
            exit(1);
        }
    }

    /* create global environment */
    global = (Env *) malloc(sizeof(Env));
    global->varTable = NULL;
    global->parent = NULL;

    if (setjmp(global->returnContext) != 0)
        exit(0);

    /* initialize function table */
    int i;
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(global, coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putFn(global, pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}
