
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
     CONSTANT = 258,
     STRING_LITERAL = 259,
     IDENTIFIER = 260,
     IFX = 261,
     ELSE = 262,
     LOR = 263,
     LAND = 264,
     NEG = 265,
     DEC = 266,
     INC = 267,
     LE = 268,
     GE = 269,
     EQ = 270,
     NE = 271,
     ADDEQ = 272,
     SUBEQ = 273,
     MULEQ = 274,
     DIVEQ = 275,
     MODEQ = 276,
     IF = 277,
     WHILE = 278,
     FNCT = 279
   };
#endif
/* Tokens.  */
#define CONSTANT 258
#define STRING_LITERAL 259
#define IDENTIFIER 260
#define IFX 261
#define ELSE 262
#define LOR 263
#define LAND 264
#define NEG 265
#define DEC 266
#define INC 267
#define LE 268
#define GE 269
#define EQ 270
#define NE 271
#define ADDEQ 272
#define SUBEQ 273
#define MULEQ 274
#define DIVEQ 275
#define MODEQ 276
#define IF 277
#define WHILE 278
#define FNCT 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 21 "delbrot.y"

    ASTnode *ASTptr; /* pointer to node in the AST */
    double val;      /* a constant */
    char *str;       /* a string, variable, or function */



/* Line 1676 of yacc.c  */
#line 108 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


