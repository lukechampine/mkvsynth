
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
     IFX = 265,
     LOR = 266,
     LAND = 267,
     NE = 268,
     EQ = 269,
     LE = 270,
     GE = 271,
     NEG = 272,
     DEC = 273,
     INC = 274
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
#define IFX 265
#define LOR 266
#define LAND 267
#define NE 268
#define EQ 269
#define LE 270
#define GE 271
#define NEG 272
#define DEC 273
#define INC 274




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
#line 100 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


