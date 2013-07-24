
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
     MODEQ = 269,
     IF = 270,
     ELSE = 271,
     FOR = 272,
     WHILE = 273,
     FNCT = 274,
     FNDEF = 275,
     RETURN = 276,
     IFX = 277,
     LAND = 278,
     LOR = 279,
     LE = 280,
     GE = 281,
     LT = 282,
     GT = 283,
     NE = 284,
     EQ = 285,
     NEG = 286,
     DEC = 287,
     INC = 288
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
#define MODEQ 269
#define IF 270
#define ELSE 271
#define FOR 272
#define WHILE 273
#define FNCT 274
#define FNDEF 275
#define RETURN 276
#define IFX 277
#define LAND 278
#define LOR 279
#define LE 280
#define GE 281
#define LT 282
#define GT 283
#define NE 284
#define EQ 285
#define NEG 286
#define DEC 287
#define INC 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


