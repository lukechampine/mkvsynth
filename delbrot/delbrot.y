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
%token CONSTANT IDENTIFIER
%token ASSIGN BINOP
%token ADDEQ SUBEQ MULEQ DIVEQ POWEQ MODEQ CHNEQ CHAIN CNCAT
%token IF ELSE TERN
%token FNCT FNDEF RETURN DEFAULT OTHER IMPORT
%token LOR LAND EQ NE GT LT GE LE NEG

%nonassoc IFX  /* avoid shift/reduce conflicts */
%nonassoc ELSE

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
    : FNDEF primary_expr '(' param_list ')' '{' stmt_list '}' { $$ = makeNode(FNDEF, 3, $2, $4, $7);   }
    ;

default_stmt
    : DEFAULT primary_expr ':' expr ';'                       { $$ = makeNode(DEFAULT, 2, $2, $4);     }
    ;

return_stmt
    : RETURN expr ';'                                         { $$ = makeNode(RETURN, 1, $2);          }
    | RETURN ';'                                              { $$ = makeNode(RETURN, 1, NULL);        }
    ;

expression_stmt
    : ';'                                                     { $$ = makeNode(';', 2, NULL, NULL);     }
    | expr ';'                                                { $$ = $1;                               }
    ;

selection_stmt
    : IF '(' expr ')' block %prec IFX                         { $$ = makeNode(IF, 2, $3, $5);          }
    | IF '(' expr ')' block ELSE block                        { $$ = makeNode(IF, 3, $3, $5, $7);      }
    ;

import_stmt
    : IMPORT primary_expr ';'                                 { $$ = makeNode(IMPORT, 1, $2);          }
    ;

param_list
    : /* empty */                                             { $$ = NULL;                             }
    | param                                                   { $$ = $1;                               }
    | param_list ',' param                                    { $$ = append($1, $3);                   }
    ;

param
    : type primary_expr                                       { $$ = makeParam(typeParam, $1, $2);     }
    | ':' type primary_expr                                   { $$ = makeParam(typeOptParam, $2, $3);  }
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
    | stmt_list stmt                                          { $$ = makeNode(';', 2, $1, $2);         }
    ;

expr
    : assignment_expr
    ;

assignment_expr
    : ternary_expr
    | primary_expr assignment_operator assignment_expr        { $$ = makeNode(ASSIGN, 3, $1, $2, $3);  }
    ;

assignment_operator
    : '=' | ADDEQ | SUBEQ | MULEQ | DIVEQ | POWEQ | MODEQ | CHNEQ
    ;

ternary_expr
    : boolean_or_expr
    | boolean_or_expr '?' ternary_expr '|' ternary_end        { $$ = makeNode(TERN, 3, $1, $3, $5);    } 
    ;

ternary_end
    : ternary_expr
    | OTHER '?' ternary_expr                                  { $$ = $3;                               }
    ;

boolean_or_expr
    : boolean_and_expr
    | boolean_or_expr LOR boolean_and_expr                    { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

boolean_and_expr
    : boolean_eq_expr
    | boolean_and_expr LAND boolean_eq_expr                   { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

boolean_eq_expr
    : boolean_rel_expr
    | boolean_eq_expr eq_operator boolean_rel_expr            { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

eq_operator
    : EQ | NE
    ;

boolean_rel_expr
    : arithmetic_add_expr
    | boolean_rel_expr rel_operator arithmetic_add_expr       { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

rel_operator
    : GT | LT | GE | LE
    ;

arithmetic_add_expr
    : arithmetic_mul_expr
    | arithmetic_add_expr add_operator arithmetic_mul_expr    { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

add_operator
    : '+' | '-'
    ;

arithmetic_mul_expr
    : arithmetic_exp_expr
    | arithmetic_mul_expr mul_operator arithmetic_exp_expr    { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

mul_operator
    : '*' | '/' | '%'
    ;

arithmetic_exp_expr
    : concat_expr
    | arithmetic_exp_expr '^' concat_expr                     { $$ = makeNode(BINOP, 3, $1, $2, $3);   }
    ;

concat_expr
    : chain_expr
    | concat_expr CNCAT chain_expr                            { $$ = makeNode(BINOP, 3, $1, $2, $3);   }

chain_expr
    : function_expr
    | function_expr CHAIN chain_expr                          { $$ = makeNode(CHAIN, 2, $1, $3);       }
    ;

function_expr
    : fn_name_expr
    | fn_name_expr arg_list                                   { $$ = makeNode(FNCT, 2, $1, $2);        }
    ;

fn_name_expr
    : unary_expr
    | unary_expr '.' unary_expr                               { $$ = addPluginFn($1, $3);              }
    ;

arg_list
    : function_arg
    | arg_list function_arg                                   { $$ = append($1, $2);                   }
    ;

function_arg
    : primary_expr                                            { $$ = makeArg(NULL, $1);                }
    | primary_expr ':' primary_expr                           { $$ = makeArg($1, $3);                  }
    ;

unary_expr
    : primary_expr
    | '-' primary_expr                                        { $$ = makeNode(NEG, 1, $2);             }
    | '!' primary_expr                                        { $$ = makeNode('!', 1, $2);             }
    ;

primary_expr
    : IDENTIFIER
    | CONSTANT
    | '(' expr ')'                                            { $$ = $2;                               }
    ;

%% /* end of grammar */

/* allocate a node */
ASTnode* newNode() {
    ASTnode *p;
    if ((p = calloc(1, sizeof(ASTnode))) == NULL)
        MkvsynthError("out of memory");
    return p;
}

/* allocate a value */
Value* newValue() {
    Value *v;
    if ((v = calloc(1, sizeof(Value))) == NULL)
        MkvsynthError("out of memory");
    return v;
}

/* create a node in the AST */
ASTnode* makeNode(int op, int nops, ...) {
    ASTnode *p = newNode();
    /* allocate space for children */
    if ((p->child = malloc(nops * sizeof(ASTnode *))) == NULL)
        MkvsynthError("out of memory");
    p->op = op;
    p->nops = nops;
    int i;
    va_list ap;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->child[i] = va_arg(ap, ASTnode *);
    va_end(ap);
    return p;
}

/* create a leaf node */
ASTnode* makeLeaf(valueType type, ...) {
    ASTnode *p = newNode();
    /* create payload */
    p->value = newValue();
    p->value->type = type;
    va_list ap;
    va_start(ap, type);
    switch (type) {
        case typeNum:  p->value->num  = va_arg(ap, double); break;
        case typeBool: p->value->bool = va_arg(ap, bool_t); break;
        case typeStr:  p->value->str  = va_arg(ap, char *); break;
        case typeId:   p->value->id   = va_arg(ap, char *); break;
        default: MkvsynthError("invalid leaf type");
    }
    return p;
}

/* create a parameter */
ASTnode* makeParam(varType type, ASTnode *typeNode, ASTnode *nameNode) {
    ASTnode *p = newNode();
    p->value = newValue();
    Var *v = calloc(1, sizeof(Var));
    v->type = type;
    v->name = nameNode->value->id;
    switch (typeNode->op) {
        case NUM:    v->valType = typeNum;  break;
        case BOOL:   v->valType = typeBool; break;
        case STRING: v->valType = typeStr;  break;
        case CLIP:   v->valType = typeClip; break;
    }
    p->value->arg = v;
    return p;
}

/* create an argument */
ASTnode* makeArg(ASTnode *nameNode, ASTnode *valNode) {
    ASTnode *p = newNode();
    p->op = p->nops = 0;
    p->value = newValue();
    Var *v = calloc(1, sizeof(Var));
    v->type = nameNode ? typeOptArg : typeArg;
    v->name = nameNode ? nameNode->value->id : NULL;
    v->value = (Value*) valNode;
    p->value->arg = v;
    return p;
}

/* link two parameters/optargs together */
ASTnode* append(ASTnode *p, ASTnode *v) {
    if (!p || !v)
        MkvsynthError("invalid argument");
    Var *traverse;
    for (traverse = p->value->arg; traverse->next; traverse = traverse->next);
    traverse->next = v->value->arg;
    return p;
}

/* add a core function to the function table */
void putFn(Env *e, Fn *f) {
    f->next = e->fnTable;
    e->fnTable = f;
}

/* look up a function */
Fn* getFn(Env const *e, char const *fnName) {
    if (!e)
        return NULL;
    Fn *traverse;
    for (traverse = e->fnTable; traverse != NULL; traverse = traverse->next) {
        if (strcmp(traverse->name, fnName) == 0)
            return traverse;
    }
    /* check parent environment */
    return getFn(e->parent, fnName);
}

/* add a plugin function to the global fnTable and return its identifier */
ASTnode* addPluginFn(ASTnode *pluginName, ASTnode *fnName) {
    /* look up plugin */
    Plugin *traverse;
    Value * (*pluginFn) (argList *);
    for (traverse = pluginList; traverse != NULL; traverse = traverse->next) {
        if (strcmp(traverse->name, pluginName->value->id) == 0) {
            /* look up symbol */
            dlerror();
            pluginFn = dlsym(traverse->handle, fnName->value->id);
            if (dlerror() != NULL)
                MkvsynthError("function \"%s\" not found in plugin %s", fnName->value->id, pluginName->value->id);
            /* create identifier */
            char *id = malloc(strlen(pluginName->value->id) + strlen(fnName->value->id) + 1);
            strcat(id, pluginName->value->id);
            strcat(id, ".");
            strcat(id, fnName->value->id);
            /* add function to fnTable */
            Fn *f = calloc(1, sizeof(Fn));
            f->type = fnCore;
            f->name = id;
            f->fnPtr = pluginFn;
            putFn(global, f);
            return makeLeaf(typeId, id);
        }
    }
    MkvsynthError("plugin \"%s\" not loaded", pluginName->value->id);
    return NULL;
}

/* add an entry to the local varTable */
void putVar(Env *e, char const *varName, varType type) {
    /* create entry */
    Var *v = calloc(1, sizeof(Var));
    v->type = type;
    v->name = strdup(varName);
    /* add to variable table */
    v->next = e->varTable;
    e->varTable = v;
}

/* look up a variable in the varTable */
Var* getVar(Env const *e, char const *varName) {
    Var *traverse;
    for (traverse = e->varTable; traverse != NULL; traverse = traverse->next) {
        if (strcmp(traverse->name, varName) == 0)
            return traverse;
    }
    return NULL;
}

Value* setVar(Env const *e, char const *varName, Value const *v) {
    Var *traverse;
    for (traverse = e->varTable; traverse != NULL; traverse = traverse->next) {
        if (strcmp(traverse->name, varName) == 0) {
            traverse->valType = v->type;
            traverse->value = newValue();
            return memcpy(traverse->value, v, sizeof(Value));
        }
    }
    MkvsynthError("could not set value of undefined variable %s", varName);
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
            MkvsynthError("could not open file \"%s\" for reading", argv[1]);
            exit(1);
        }
    }

    /* create global environment */
    global = calloc(1, sizeof(Env));

    if (setjmp(global->returnContext) != 0)
        return 0;

    /* initialize function table */
    int i;
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(global, &coreFunctions[i]);
    for(i = 0; internalFilters[i].name != 0; i++)
        putFn(global, &internalFilters[i]);

    /* main parse loop */
    return yyparse();
}
