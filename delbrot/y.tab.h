
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     STRING = 259,
     FNCT = 260,
     VAR = 261,
     WHILE = 262,
     IF = 263,
     ELSE = 264,
     ADD_ASSIGN = 265,
     SUB_ASSIGN = 266,
     MUL_ASSIGN = 267,
     DIV_ASSIGN = 268,
     MOD_ASSIGN = 269,
     IFX = 270,
     LOR = 271,
     LAND = 272,
     NE = 273,
     EQ = 274,
     LE = 275,
     GE = 276,
     NEG = 277,
     DEC = 278,
     INC = 279
   };
#endif
/* Tokens.  */
#define NUM 258
#define STRING 259
#define FNCT 260
#define VAR 261
#define WHILE 262
#define IF 263
#define ELSE 264
#define ADD_ASSIGN 265
#define SUB_ASSIGN 266
#define MUL_ASSIGN 267
#define DIV_ASSIGN 268
#define MOD_ASSIGN 269
#define IFX 270
#define LOR 271
#define LAND 272
#define NE 273
#define EQ 274
#define LE 275
#define GE 276
#define NEG 277
#define DEC 278
#define INC 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 21 "delbrot.y"

    ASTnode *ASTptr; /* pointer to node in the AST */
    double  val;     /* number */
    char    *str;    /* string */
    func    fptr;    /* function */
    var     *vptr;   /* variable */



/* Line 1676 of yacc.c  */
#line 110 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


