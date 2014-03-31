#include <dlfcn.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delbrot.h"
#include "y.tab.h"

/* function declarations */
static argList* argify(Env *, Var *);
static Value    assign(Env *, Value *, ASTnode *);
static Value    assignOp(Env *, ASTnode *, ASTnode *, ASTnode *);
static Value    binaryOp(Env *, ASTnode *, int, ASTnode *);
static void     chain(ASTnode *, ASTnode *);
	   void     checkArgs(argList *, int, ...);
	   Value    ex(Env *, ASTnode *);
static Value    fnctCall(Env *, Value *, argList *);
static void     funcDefine(Env *, Value *, ASTnode *, ASTnode *);
	   void*    getOptArg(argList *, char *, valueType);
static void     ifelse(Env *, ASTnode *);
static void     import(Value *);
	   void     MkvsynthError(char *error, ...);
	   void     MkvsynthMessage(char *error, ...);
	   void     MkvsynthWarning(char *error, ...);
static void     setDefault(Env *, Value *, ASTnode *);
static Value    ternary(Env *, ASTnode *);
static Value    unaryOp(Env *, ASTnode *, int);
static Value    userDefFnCall(Env *, Fn *, argList *);

/* defined in delbrot.l */
void switchToBuffer(char *, FILE *);

/* global variables */
char *typeNames[] = {"void", "number", "boolean", "string", "clip", "identifier"};
char *currentFunction = "";
extern int linenumber;

/* function definitions */
/* transform a linked list of nodes into an argList */
argList* argify(Env *e, Var *p) {
	argList *a = calloc(1, sizeof(argList));
	if (!p)
		return a;

	/* count # of arguments */
	a->nargs = 1;
	Var *traverse = p;
	while ((traverse = traverse->next))
		a->nargs++;

	/* allocate space */
	a->args = calloc(a->nargs, sizeof(Var));

	/* place arguments in args array */
	int i;
	for (i = 0, traverse = p; traverse; traverse = traverse->next, i++)
		a->args[i] = *traverse;

	/* evaluate arguments */
	for (i = 0; i < a->nargs; i++)
		a->args[i].value = ex(e, &a->args[i].fnArg);

	return a;
}

/* create or modify a variable */
Value assign(Env *e, Value *name, ASTnode *valNode) {
	Value v = ex(e, valNode);
	if (!name)
		MkvsynthError("invalid assignment: can't assign to operation", name);
	if (name->type != typeId)
		MkvsynthError("invalid assignment: can't assign to constant type %s", typeNames[name->type]);
	if (v.type > typeClip)
		MkvsynthError("invalid assignment: can't assign type %s to variable", typeNames[v.type]);
	/* new variable */
	if (getVar(e, name->id) == NULL)
		putVar(e, name->id, typeVar);
	return setVar(e, name->id, &v);
}


/* handle assignment operators */
Value assignOp(Env *e, ASTnode *varNode, ASTnode *opNode, ASTnode *valNode) {
	/* special cases */
	if (opNode->op == '=')
		return assign(e, varNode->value, valNode);

	ASTnode RHS;
	if (opNode->op == CHNEQ)
		RHS = makeNode(CHAIN, 2, varNode, valNode);
	else
		RHS = makeNode(BINOP, 3, varNode, opNode, valNode);

	ASTnode eq = makeNode('=', 0);
	ASTnode a = makeNode(ASSIGN, 3, varNode, &eq, &RHS);

	/* unfold syntactic sugar and reprocess */
	return ex(e, &a);
}

/* handle arithmetic / boolean operators */
Value binaryOp(Env *e, ASTnode *lhsNode, int op, ASTnode *rhsNode) {
	Value lhs = ex(e, lhsNode);
	Value rhs = ex(e, rhsNode);
	Value v;
	/* arithmetic operator */
	if (op < 100) {
		if (lhs.type != typeNum) MkvsynthError("type mismatch: LHS of %c expected number, got %s", op, typeNames[lhs.type]);
		if (rhs.type != typeNum) MkvsynthError("type mismatch: RHS of %c expected number, got %s", op, typeNames[rhs.type]);
		switch(op) {
			case '+': v.num = lhs.num + rhs.num; break;
			case '-': v.num = lhs.num - rhs.num; break;
			case '*': v.num = lhs.num * rhs.num; break;
			case '/': v.num = lhs.num / rhs.num; break;
			case '^': v.num = pow(lhs.num, rhs.num); break;
			case '%': v.num = (double) ((int) lhs.num % (int) rhs.num); break;
		}
		v.type = typeNum;
	}
	/* concatenation operator */
	else if (op == CNCAT) {
		if (lhs.type != rhs.type) MkvsynthError("type mismatch: cannot concatenate %s with %s", typeNames[lhs.type], typeNames[rhs.type]);
		if (lhs.type == typeStr) {
			// allocate space for new string
			v.str = malloc(strlen(lhs.str) + strlen(rhs.str) + 1);
			strcpy(v.str, lhs.str);
			strcat(v.str, rhs.str);
			v.type = typeStr;
		}
		else if (lhs.type == typeClip) {
			MkvsynthError("clip concatenation is not supported (yet)");
		}
		else {
			MkvsynthError("the concatenation operator is not defined on %ss", typeNames[lhs.type]);
		}
	}
	/* boolean operators */
	else {
		if (lhs.type != rhs.type) MkvsynthError("type mismatch: cannot compare %s to %s", typeNames[lhs.type], typeNames[rhs.type]);
		/* for error messages */
		char *opStrs[512];
		opStrs[EQ] = "=="; opStrs[NE] = "!="; opStrs[GT] = ">"; opStrs[LT] = "<";
		opStrs[GE] = ">="; opStrs[LE] = "<="; opStrs[LOR] = "||"; opStrs[LAND] = "&&";
		if (lhs.type == typeNum) {
			switch(op) {
				case EQ: v.bool = lhs.num == rhs.num ? TRUE : FALSE; break;
				case NE: v.bool = lhs.num != rhs.num ? TRUE : FALSE; break;
				case GT: v.bool = lhs.num  > rhs.num ? TRUE : FALSE; break;
				case LT: v.bool = lhs.num  < rhs.num ? TRUE : FALSE; break;
				case GE: v.bool = lhs.num >= rhs.num ? TRUE : FALSE; break;
				case LE: v.bool = lhs.num <= rhs.num ? TRUE : FALSE; break;
				default: MkvsynthError("type mismatch: operator %s is not defined for numbers", opStrs[op]);
			}
		}
		else if (lhs.type == typeBool) {
			switch(op) {
				case EQ:   v.bool = (lhs.bool == TRUE) == (rhs.bool == TRUE) ? TRUE : FALSE; break;
				case NE:   v.bool = (lhs.bool == TRUE) != (rhs.bool == TRUE) ? TRUE : FALSE; break;
				case LOR:  v.bool = (lhs.bool == TRUE) || (rhs.bool == TRUE) ? TRUE : FALSE; break;
				case LAND: v.bool = (lhs.bool == TRUE) && (rhs.bool == TRUE) ? TRUE : FALSE; break;
				default:   MkvsynthError("type mismatch: operator %s is not defined for booleans", opStrs[op]);
			}
		}
		else if (lhs.type == typeStr) {
			switch(op) {
				case EQ: v.bool = strcmp(lhs.str, rhs.str) == 0 ? TRUE : FALSE; break;
				case NE: v.bool = strcmp(lhs.str, rhs.str) != 0 ? TRUE : FALSE; break;
				default: MkvsynthError("type mismatch: operator %s is not defined for strings", opStrs[op]);
			}
		}
		else {
			MkvsynthError("comparison operators are not supported for %ss (yet)", typeNames[lhs.type]);
		}
		v.type = typeBool;
	}
	return v;
}

/* append LHS to argument list of RHS */
void chain(ASTnode *val, ASTnode *fnNode) {
	if (fnNode->op == CHAIN)
		return chain(val, &fnNode->child[0]);
	ASTnode arg = makeArg(NULL, val);
	if (fnNode->op == FNCT)
		fnNode->child[1] = append(&arg, &fnNode->child[1]);
	else if (fnNode->value != NULL && fnNode->value->type == typeId)
		*fnNode = makeNode(FNCT, 2, fnNode, &arg);
	else
		MkvsynthError("expected function name, got %s", typeNames[fnNode->value->type]);
}

/* ensure that a function call is valid */
void checkArgs(argList *a, int numArgs, ...) {
	/* check number of arguments */
	int i = 0;
	while (i < a->nargs && a->args[i].type == typeArg)
		i++;
	if (i != numArgs)
		MkvsynthError("expected %d argument%s, got %d", numArgs, (numArgs == 1 ? "" : "s"), i);
	/* check types */
	va_list ap;
	va_start(ap, numArgs);
	for (i = 0; i < numArgs; i++) {
		valueType argType = va_arg(ap, valueType);
		if (a->args[i].value.type != argType)
			MkvsynthError("arg %d expected %s, got %s", i+1, typeNames[argType], typeNames[a->args[i].value.type]);
	}
	va_end(ap);
}

/* dereference an identifier */
Value dereference(Env *e, Value *val) {
	if (val == NULL)
		MkvsynthError("unexpected NULL value");
	if (val->type != typeId)
		return *val;
	Var *v; Fn *f;
	if ((v = getVar(e, val->id)) != NULL)
		return v->value;
	if ((f = getFn(e, val->id)) != NULL) {
		return fnctCall(e, val, argify(e, NULL));
	}
	MkvsynthError("reference to undefined variable or function \"%s\"", val->id);
	return *val;
}

/* execute an ASTnode, producing a constant value */
Value ex(Env *e, ASTnode *p) {
	/* default return type */
	Value v;
	v.type = typeNull;

	if (!p)
		return v;

	switch (p->op) {
		/* leaf node */
		case 0:       v = dereference(e, p->value); break;
		/* declarations */
		case FNDEF:   funcDefine(e, p->child[0].value, &p->child[1], &p->child[2]); break;
		/* blocks */
		case IF:      ifelse(e, p); break;
		/* functions */
		case FNCT:    v = fnctCall(e, p->child[0].value, argify(e, p->child[1].value->arg)); break;
		case CHAIN:   chain(&p->child[0], &p->child[1]); v = ex(e, &p->child[1]); break;
		case DEFAULT: setDefault(e, p->child[0].value, &p->child[1]); break;
		case RETURN:  e->returnValue = p->nops > 0 ? ex(e, &p->child[0]) : v; longjmp(e->returnContext, 1); break;
		/* plugin imports */
		case IMPORT:  import(p->child[0].value); break;
		/* assignment */
		case ASSIGN:  v = assignOp(e, &p->child[0], &p->child[1], &p->child[2]); break;
		/* unary operators */
		case NEG:     v = unaryOp(e, &p->child[0], '-'); break;
		case '!':     v = unaryOp(e, &p->child[0], '!'); break;
		/* binary operators */
		case BINOP:   v = binaryOp(e, &p->child[0], p->child[1].op, &p->child[2]); break;
		/* trinary operator */
		case TERN:    v = ternary(e, p); break;
		/* compound statements */
		case ';':     ex(e, &p->child[0]); ex(e, &p->child[1]); break;
		/* should never wind up here */
		default:      MkvsynthError("unknown operator %d", p->op);
	}

	return v;
}

/* handle function calls */
Value fnctCall(Env *e, Value *name, argList *a) {
	if (!name)
		MkvsynthError("expected function name, got operation", name);
	if (name->type != typeId)
		MkvsynthError("expected function name, got %s", typeNames[name->type]);
	Fn *f = getFn(e, name->id);
	if (f == NULL)
		MkvsynthError("reference to undefined function \"%s\"", name->id);

	/* set global variable */
	currentFunction = f->name;

	/* check argument order */
	int i;
	for (i = 0; i < a->nargs - 1; i++) {
		if (a->args[i].type == typeOptArg && a->args[i+1].type == typeArg)
			MkvsynthError("optional arguments must follow mandatory arguments");
	}
	
	Value res;
	if (f->type == fnCore)
		res = (*(f->fnPtr))(a);
	else
		res = userDefFnCall(e, f, a);

	free(a->args);
	free(a);

	/* unset global variable */
	currentFunction = "";

	return res;
}

/* process a function definition */
void funcDefine(Env *e, Value *name, ASTnode *params, ASTnode *body) {
	if (!name)
		MkvsynthError("expected name of function, got operation", name);
	if (name->type != typeId)
		MkvsynthError("expected name of function, got %s", typeNames[name->type]);
	if (getVar(e, name->id) || getFn(e, name->id))
		MkvsynthWarning("overwrote previous definition of %s", name->id);

	/* create new function table entry */
	Fn *f = calloc(1, sizeof(Fn));
	f->type = fnUser;
	f->name = name->id;
	f->body = body;

	/* create parameter list */
	/* TODO: can this be replaced with argify? */
	f->params = calloc(1, sizeof(argList));
	int i = 0;
	if (params->value) {
		/* count number of parameters */
		Var *traverse = params->value->arg;
		while (traverse != NULL) {
			f->params->nargs++;
			traverse = traverse->next;
		}

		/* allocate space for parameters */
		f->params->args = calloc(f->params->nargs, sizeof(Var));
		/* copy parameters */
		for (traverse = params->value->arg; traverse; traverse = traverse->next) {
			f->params->args[i] = *traverse;
			f->params->args[i].name = strdup(traverse->name);
			i++;
		}
	}

	/* check argument ordering */
	for (i = 0; i < f->params->nargs - 1; i++) {
		if (f->params->args[i].type == typeOptParam && f->params->args[i+1].type == typeParam)
			MkvsynthError("optional parameters must follow mandatory parameters in function %s", f->name);
	}

	/* add to local function table */
	putFn(e, f);
}

/* get optional arguments in a function call */
void* getOptArg(argList *a, char *name, valueType type) {
	int i;
	for (i = 0; i < a->nargs; i++) {
		if (a->args[i].type == typeArg)
			continue;
		if (!(strcmp(a->args[i].name, name))) {
			if (a->args[i].value.type != type)
				MkvsynthError("type mismatch: optional argument \"%s\" expected %s, got %s", name, typeNames[type], typeNames[a->args[i].value.type]);
			switch (type) {
				case typeNum:  return &a->args[i].value.num;
				case typeBool: return &a->args[i].value.bool;
				case typeStr:  return a->args[i].value.str;
				case typeClip: return a->args[i].value.clipIn;
				default:       MkvsynthError("invalid argument type");
			}
		}
	}
	return NULL;
}

/* process an if/else statement */
void ifelse(Env *e, ASTnode *p) {
	Value cond = ex(e, &p->child[0]);
	if (cond.type != typeBool)
		MkvsynthError("if expected boolean, got %s", typeNames[cond.type]);
	if (cond.bool == TRUE)
		ex(e, &p->child[1]);
	else if (p->nops == 3)
		ex(e, &p->child[2]);
}

/* import a plugin */
void import(Value *importName) {
	if (!importName)
		MkvsynthError("expected name of script or plugin, got operation", importName);
	if (importName->type != typeId)
		MkvsynthError("expected name of script or plugin, got %s", typeNames[importName->type]);

	char *home = getenv("HOME");

	/* attempt to load plugin */
	char *pluginPath = malloc(strlen(home) + 25 + strlen(importName->id));
	strcpy(pluginPath, home);
	strcat(pluginPath, "/.config/mkvsynth/lib");
	strcat(pluginPath, importName->id);
	strcat(pluginPath, ".so");
	void *pHandle = dlopen(pluginPath, RTLD_NOW);
	free(pluginPath);

	/* attempt to load script */
	char *scriptPath = malloc(strlen(home) + 24 + strlen(importName->id));
	strcpy(scriptPath, home);
	strcat(scriptPath, "/.config/mkvsynth/");
	strcat(scriptPath, importName->id);
	strcat(scriptPath, ".mkvs");
	FILE *sHandle = fopen(scriptPath, "r");
	free(scriptPath);

	/* only one handle should be valid */
	if (pHandle == NULL && sHandle == NULL)
		MkvsynthError("could not load script or plugin \"%s\": file not found", importName->id);
	if (pHandle != NULL && sHandle != NULL)
		MkvsynthError("both a script and a plugin with name \"%s\" exist: rename or remove one of them");

	if (sHandle != NULL) {
		/* redirect Flex's input buffer to the script file */
		switchToBuffer(importName->id, sHandle);
	}
	else {
		/* create and append new plugin entry */
		Plugin *newPlugin = malloc(sizeof(Plugin));
		newPlugin->name = importName->id;
		newPlugin->handle = pHandle;
		newPlugin->next = pluginList;
		pluginList = newPlugin;
	}
}

/* display error and current line number in red and exit */
void MkvsynthError(char *error, ...) {
	fprintf(stderr, "\x1B[31mdelbrot:%d error: %s%s", linenumber, currentFunction, currentFunction[0] ? ": " : "");
	va_list arglist;
	va_start(arglist, error);
	vfprintf(stderr, error, arglist);
	va_end(arglist);
	fprintf(stderr, "\x1B[0m\n");
	exit(1);
}

/* display message in blue */
void MkvsynthMessage(char *message, ...) {
	fprintf(stderr, "\x1B[36m");
	va_list arglist;
	va_start(arglist, message);
	vfprintf(stdout, message, arglist);
	va_end(arglist);
	fprintf(stdout, "\x1B[0m\n");
}

/* display warning and current line number in yellow */
void MkvsynthWarning(char *warning, ...) {
	fprintf(stderr, "\x1B[33mdelbrot:%d warning: %s%s", linenumber, currentFunction, currentFunction[0] ? ": " : "");
	va_list arglist;
	va_start(arglist, warning);
	vfprintf(stderr, warning, arglist);
	va_end(arglist);
	fprintf(stderr, "\x1B[0m\n");
}

/* process a default statement */
void setDefault(Env *e, Value *name, ASTnode *valNode) {
	if (currentFunction[0] == 0)
		MkvsynthError("can't set defaults outside of function body");
	if (!name)
		MkvsynthError("default expected optional parameter, got operation", name);
	if (name->type != typeId)
		MkvsynthError("default expected optional parameter, got %s", typeNames[name->type]);

	Var *v = getVar(e, name->id);
	if (v == NULL)
		MkvsynthError("could not set default of undefined parameter %s", name->id);
	if (v->type != typeOptParam)
		MkvsynthError("could not set default of mandatory parameter %s", name->id);

	if (v->value.type == typeNull)
		v->value = ex(e, valNode);
}

/* process a ternary expression */
Value ternary(Env *e, ASTnode *p) {
	Value cond = ex(e, &p->child[0]);
	if (cond.type != typeBool)
		MkvsynthError("arg 1 of ?| expected boolean, got %s", typeNames[cond.type]);
	if (cond.bool == TRUE)
		return ex(e, &p->child[1]);
	else
		return ex(e, &p->child[2]);
}

/* handle negation operators */
Value unaryOp(Env *e, ASTnode* valNode, int op) {
	Value v = ex(e, valNode);
	v.type = typeNum;
	if (op == '-') {
		if (v.type != typeNum)
			MkvsynthError("%c expected number, got %s", op, typeNames[v.type]);
		v.num = -v.num;
	}
	else if (op == '!') {
		if (v.type != typeBool)
			MkvsynthError("%c expected boolean, got %s", op, typeNames[v.type]);
		v.bool = v.bool == TRUE ? FALSE : TRUE;
	}
	return v;
}

/* process a user-defined function call */
Value userDefFnCall(Env *e, Fn *f, argList *a) {
	/* create new environment */
	Env local;
	local.varTable = NULL;
	local.fnTable = NULL;
	local.parent = e;
	
	/* for convenience */
	Var *args = a->args;
	Var *params = f->params->args;

	/* add function parameters to local variable table */
	int i;
	for (i = 0; i < f->params->nargs; i++)
		putVar(&local, params[i].name, params[i].type);

	/* check argument number */
	int nMandArgs = 0, nMandParams = 0;
	while (nMandArgs < a->nargs && args[nMandArgs].type == typeArg)
		nMandArgs++;
	while (nMandParams < f->params->nargs && params[nMandParams].type == typeParam)
		nMandParams++;
	if (nMandArgs != nMandParams)
		MkvsynthError("expected %d mandatory argument%s, got %d", nMandParams, (nMandParams == 1) ? "" : "s", nMandArgs);
	/* check mandatory args for type mismatches */
	for (i = 0; i < nMandParams; i++) {
		if (args[i].value.type != params[i].valType)
			MkvsynthError("type mismatch: arg %d of %s expected %s, got %s", 
				i+1, f->name, typeNames[params[i].valType], typeNames[args[i].value.type]);
		/* all is well; set value of local var */
		setVar(&local, params[i].name, &args[i].value);
	}

	/* check optional arguments */
	for (i = nMandArgs; i < a->nargs; i++) {
		/* check that optional parameter exists */
		int j = nMandParams;
		for (j = nMandParams; j < f->params->nargs; j++) {
			if (!strcmp(args[i].name, params[j].name))
				break;
		}
		if (j == f->params->nargs)
			MkvsynthError("%s is not an optional parameter of function %s", args[i].name, f->name);
		/* check that types match */
		if (args[i].value.type != params[i].valType)
			MkvsynthError("type mismatch: optional argument \"%s\" of function \"%s\" expected %s, got %s",
				params[i].name, f->name, typeNames[params[i].valType], typeNames[args[i].value.type]);
		/* assign value */
		setVar(&local, params[i].name, &args[i].value);
	}

	/* execute function body in the local environment */
	if (setjmp(local.returnContext) == 0)
		ex(&local, f->body);

	Value ret = local.returnValue;
	freeEnv(&local);
	return ret;
}
