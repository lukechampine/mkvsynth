
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
     ASSIGN = 264,
     BINOP = 265,
     ADDEQ = 266,
     SUBEQ = 267,
     MULEQ = 268,
     DIVEQ = 269,
     POWEQ = 270,
     MODEQ = 271,
     IF = 272,
     ELSE = 273,
     FOR = 274,
     WHILE = 275,
     FNCT = 276,
     FNDEF = 277,
     RETURN = 278,
     IFX = 279,
     LAND = 280,
     LOR = 281,
     LE = 282,
     GE = 283,
     LT = 284,
     GT = 285,
     NE = 286,
     EQ = 287,
     NEG = 288,
     DEC = 289,
     INC = 290
   };
#endif
/* Tokens.  */
#define INT 258
#define DOUBLE 259
#define STRING 260
#define CONSTANT 261
#define IDENTIFIER 262
#define OPTARG 263
#define ASSIGN 264
#define BINOP 265
#define ADDEQ 266
#define SUBEQ 267
#define MULEQ 268
#define DIVEQ 269
#define POWEQ 270
#define MODEQ 271
#define IF 272
#define ELSE 273
#define FOR 274
#define WHILE 275
#define FNCT 276
#define FNDEF 277
#define RETURN 278
#define IFX 279
#define LAND 280
#define LOR 281
#define LE 282
#define GE 283
#define LT 284
#define GT 285
#define NE 286
#define EQ 287
#define NEG 288
#define DEC 289
#define INC 290




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


