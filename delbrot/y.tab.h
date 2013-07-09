
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
     T_INT = 258,
     T_DOUBLE = 259,
     T_STRING = 260,
     CONSTANT = 261,
     IDENTIFIER = 262,
     LE = 263,
     GE = 264,
     EQ = 265,
     NE = 266,
     ADDEQ = 267,
     SUBEQ = 268,
     MULEQ = 269,
     DIVEQ = 270,
     MODEQ = 271,
     IF = 272,
     ELSE = 273,
     WHILE = 274,
     FNCT = 275,
     FNDEF = 276,
     IFX = 277,
     LOR = 278,
     LAND = 279,
     NEG = 280,
     DEC = 281,
     INC = 282
   };
#endif
/* Tokens.  */
#define T_INT 258
#define T_DOUBLE 259
#define T_STRING 260
#define CONSTANT 261
#define IDENTIFIER 262
#define LE 263
#define GE 264
#define EQ 265
#define NE 266
#define ADDEQ 267
#define SUBEQ 268
#define MULEQ 269
#define DIVEQ 270
#define MODEQ 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define FNCT 275
#define FNDEF 276
#define IFX 277
#define LOR 278
#define LAND 279
#define NEG 280
#define DEC 281
#define INC 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


