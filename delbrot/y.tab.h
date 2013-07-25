/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
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
     INT = 258,
     DOUBLE = 259,
     STRING = 260,
     CONSTANT = 261,
     IDENTIFIER = 262,
     OPTARG = 263,
     BINOP = 264,
     ADDEQ = 265,
     SUBEQ = 266,
     MULEQ = 267,
     DIVEQ = 268,
     POWEQ = 269,
     MODEQ = 270,
     IF = 271,
     ELSE = 272,
     FOR = 273,
     WHILE = 274,
     FNCT = 275,
     FNDEF = 276,
     RETURN = 277,
     IFX = 278,
     LAND = 279,
     LOR = 280,
     LE = 281,
     GE = 282,
     LT = 283,
     GT = 284,
     NE = 285,
     EQ = 286,
     NEG = 287,
     DEC = 288,
     INC = 289
   };
#endif
/* Tokens.  */
#define INT 258
#define DOUBLE 259
#define STRING 260
#define CONSTANT 261
#define IDENTIFIER 262
#define OPTARG 263
#define BINOP 264
#define ADDEQ 265
#define SUBEQ 266
#define MULEQ 267
#define DIVEQ 268
#define POWEQ 269
#define MODEQ 270
#define IF 271
#define ELSE 272
#define FOR 273
#define WHILE 274
#define FNCT 275
#define FNDEF 276
#define RETURN 277
#define IFX 278
#define LAND 279
#define LOR 280
#define LE 281
#define GE 282
#define LT 283
#define GT 284
#define NE 285
#define EQ 286
#define NEG 287
#define DEC 288
#define INC 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


