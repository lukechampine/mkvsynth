/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     BOOL = 259,
     STRING = 260,
     CLIP = 261,
     TRUE = 262,
     FALSE = 263,
     CONSTANT = 264,
     IDENTIFIER = 265,
     OPTARG = 266,
     ASSIGN = 267,
     BINOP = 268,
     ADDEQ = 269,
     SUBEQ = 270,
     MULEQ = 271,
     DIVEQ = 272,
     POWEQ = 273,
     MODEQ = 274,
     CHAIN = 275,
     IF = 276,
     ELSE = 277,
     TERN = 278,
     FNCT = 279,
     FNDEF = 280,
     RETURN = 281,
     DEFAULT = 282,
     OTHER = 283,
     LOR = 284,
     LAND = 285,
     EQ = 286,
     NE = 287,
     GT = 288,
     LT = 289,
     GE = 290,
     LE = 291,
     IFX = 292,
     NEG = 293
   };
#endif
/* Tokens.  */
#define NUM 258
#define BOOL 259
#define STRING 260
#define CLIP 261
#define TRUE 262
#define FALSE 263
#define CONSTANT 264
#define IDENTIFIER 265
#define OPTARG 266
#define ASSIGN 267
#define BINOP 268
#define ADDEQ 269
#define SUBEQ 270
#define MULEQ 271
#define DIVEQ 272
#define POWEQ 273
#define MODEQ 274
#define CHAIN 275
#define IF 276
#define ELSE 277
#define TERN 278
#define FNCT 279
#define FNDEF 280
#define RETURN 281
#define DEFAULT 282
#define OTHER 283
#define LOR 284
#define LAND 285
#define EQ 286
#define NE 287
#define GT 288
#define LT 289
#define GE 290
#define LE 291
#define IFX 292
#define NEG 293




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


