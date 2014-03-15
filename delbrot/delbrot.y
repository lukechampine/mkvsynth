%{
    #include <dlfcn.h>
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "delbrot.h"
    /* prototypes to please -Wall */
    void yyerror(char *error, ...);
    int yylex();
    /* script file */
    extern FILE *yyin;
    /* debug */
    #define YYDEBUG 1
%}

%token NUM BOOL STRING CLIP TRUE FALSE
%token CONSTANT IDENTIFIER OPTARG
%token ASSIGN BINOP
%token ADDEQ SUBEQ MULEQ DIVEQ POWEQ MODEQ CHNEQ CHAIN CNCAT
%token IF ELSE TERN
%token FNCT FNDEF RETURN DEFAULT OTHER IMPORT
%token LOR LAND EQ NE GT LT GE LE

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE
%right NEG

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
    | import_stmt
    ;

function_declaration
    : FNDEF primary_expr '(' param_list ')' '{' stmt_list '}' { $$ = mkOpNode(FNDEF, 3, $2, $4, $7);   }
    ;

default_stmt
    : DEFAULT primary_expr ':' expr ';'                       { $$ = mkOpNode(DEFAULT, 2, $2, $4);     }
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

import_stmt
    : IMPORT primary_expr ';'                                 { $$ = mkOpNode(IMPORT, 1, $2);          }
    ;

param_list
    : /* empty */                                             { $$ = NULL;                             }
    | param                                                   { $$ = $1;                               }
    | param_list ',' param                                    { $$ = append($1, $3);                   }
    ;

param
    : type primary_expr                                       { $$ = mkParamNode(0, $1->num, $2);      }
    | ':' type primary_expr                                   { $$ = mkParamNode(1, $2->num, $3);      }
    ;

type
    : NUM | BOOL | STRING | CLIP
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
    : '=' | ADDEQ | SUBEQ | MULEQ | DIVEQ | POWEQ | MODEQ | CHNEQ
    ;

ternary_expr
    : boolean_or_expr
    | boolean_or_expr '?' ternary_expr '|' ternary_end        { $$ = mkOpNode(TERN, 3, $1, $3, $5);    } 
    ;

ternary_end
    : ternary_expr                                            { $$ = $1;                               }
    | OTHER '?' ternary_expr                                  { $$ = $3;                               }
    ;

boolean_or_expr
    : boolean_and_expr
    | boolean_or_expr LOR boolean_and_expr                    { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

boolean_and_expr
    : boolean_eq_expr
    | boolean_and_expr LAND boolean_eq_expr                   { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

boolean_eq_expr
    : boolean_rel_expr
    | boolean_eq_expr eq_operator boolean_rel_expr            { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

eq_operator
    : EQ | NE
    ;

boolean_rel_expr
    : arithmetic_add_expr
    | boolean_rel_expr rel_operator arithmetic_add_expr       { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

rel_operator
    : GT | LT | GE | LE
    ;

arithmetic_add_expr
    : arithmetic_mul_expr
    | arithmetic_add_expr add_operator arithmetic_mul_expr    { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

add_operator
    : '+' | '-'
    ;

arithmetic_mul_expr
    : arithmetic_exp_expr
    | arithmetic_mul_expr mul_operator arithmetic_exp_expr    { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

mul_operator
    : '*' | '/' | '%'
    ;

arithmetic_exp_expr
    : concat_expr
    | arithmetic_exp_expr '^' concat_expr                     { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }
    ;

concat_expr
    : chain_expr
    | concat_expr CNCAT chain_expr                            { $$ = mkOpNode(BINOP, 3, $1, $2, $3);   }

chain_expr
    : function_expr
    | function_expr CHAIN chain_expr                          { $$ = mkOpNode(CHAIN, 2, $1, $3);       }
    ;

function_expr
    : fn_name_expr
    | fn_name_expr arg_list                                   { $$ = mkOpNode(FNCT, 2, $1, $2);        }
    ;

fn_name_expr
    : unary_expr
    | unary_expr '.' unary_expr                               { $$ = getPluginFn($1, $3);              }
    ;

arg_list
    : function_arg                                            { $$ = $1;                               }
    | arg_list function_arg                                   { $$ = append($1, $2);                   }
    ;

function_arg
    : primary_expr                                            { $$ = $1;                               }
    | primary_expr ':' primary_expr                           { $$ = mkOptArgNode($1, $3);             }
    ;

unary_expr
    : primary_expr
    | '-' primary_expr                                        { $$ = mkOpNode(NEG, 1, $2);             }
    | '!' primary_expr                                        { $$ = mkOpNode('!', 1, $2);             }
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
        MkvsynthError("out of memory");
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

/* create a parameter node in the AST */
ASTnode *mkParamNode(char opt, int type, ASTnode *p) {
    p->type = opt ? typeOptParam : typeParam;
    p->var.name = p->id;
    switch(type) {
        case NUM:    p->var.type = typeNum; break;
        case BOOL:   p->var.type = typeBool; break;
        case STRING: p->var.type = typeStr; break;
        case CLIP:   p->var.type = typeClip; break;
    }
    return p;
}

/* create an optional argument node in the AST */
ASTnode *mkOptArgNode(ASTnode *p, ASTnode *value) {
    p->type = typeOptArg;
    p->var.name = p->id;
    p->var.type = value->type;
    p->var.value = newNode();
    memcpy(p->var.value, value, sizeof(ASTnode));
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p = newNode();
    /* allocate space for ops */
    if ((p->op.ops = malloc(nops * sizeof(ASTnode *))) == NULL)
        MkvsynthError("out of memory");
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

/* add an ASTnode to the end of a linked list */
ASTnode *append(ASTnode *root, ASTnode *node) {
    if (!root)
        MkvsynthError("invalid argument");
    ASTnode *traverse;
    for(traverse = root; traverse->next; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* add a core function to the function table */
ASTnode *putFn(Env *e, fnEntry fn) {
    /* create entry */
    ASTnode *ptr = newNode();
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

/* look up a plugin function */
ASTnode *getPluginFn(ASTnode *pluginName, ASTnode *fnName) {
    /* look up plugin */
    Plugin *pTraverse;
    ASTnode * (*pluginFn) (ASTnode *, argList *);
    for (pTraverse = pluginList; pTraverse != NULL; pTraverse = pTraverse->next) {
        if (strcmp(pTraverse->name, pluginName->id) == 0) {
            /* look up symbol */
            dlerror();
            pluginFn = dlsym(pTraverse->handle, fnName->id);
            if (dlerror() != NULL)
                MkvsynthError("function \"%s\" not found in plugin %s", fnName->id, pluginName->id);
            ASTnode *fnNode = newNode();
            fnNode->type = typeFn;
            fnNode->fn.name = fnName->id;
            fnNode->fn.core.fnPtr = pluginFn;
            return fnNode;
        }
    }
    MkvsynthError("plugin \"%s\" not loaded", pluginName->id);
    return NULL;
}

/* allocate a new variable */
ASTnode *putVar(Env *e, char const *varName) {
    /* create entry */
    ASTnode *ptr = newNode();
    ptr->type = typeVar;
    ptr->var.name = strdup(varName);
    ptr->var.value = newNode();
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

/* alias for MkvsynthError */
void yyerror(char *error, ...) {
    va_list arglist;
    va_start(arglist, error);
    MkvsynthError(error, arglist);
    va_end(arglist);
}

int main(int argc, char **argv) {
    /* help message */
    if ((argc != 1 && argc != 2)
    || (argc > 1 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))) {
        printf("Usage: mkvsynth [FILE]\nInterprets an mkvsynth script.\n\nIf FILE is omitted, STDIN will be used instead.\n\nReport bugs on github.com/mkvsynth/mkvsynth.\n");
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
    for(i = 0; internalFilters[i].name != 0; i++)
        putFn(global, internalFilters[i]);

    /* main parse loop */
    return yyparse();
}
