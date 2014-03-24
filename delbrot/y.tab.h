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
     ASSIGN = 266,
     BINOP = 267,
     ADDEQ = 268,
     SUBEQ = 269,
     MULEQ = 270,
     DIVEQ = 271,
     POWEQ = 272,
     MODEQ = 273,
     CHNEQ = 274,
     CHAIN = 275,
     CNCAT = 276,
     IF = 277,
     ELSE = 278,
     TERN = 279,
     FNCT = 280,
     FNDEF = 281,
     RETURN = 282,
     DEFAULT = 283,
     OTHER = 284,
     IMPORT = 285,
     LOR = 286,
     LAND = 287,
     EQ = 288,
     NE = 289,
     GT = 290,
     LT = 291,
     GE = 292,
     LE = 293,
     NEG = 294,
     IFX = 295
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
#define ASSIGN 266
#define BINOP 267
#define ADDEQ 268
#define SUBEQ 269
#define MULEQ 270
#define DIVEQ 271
#define POWEQ 272
#define MODEQ 273
#define CHNEQ 274
#define CHAIN 275
#define CNCAT 276
#define IF 277
#define ELSE 278
#define TERN 279
#define FNCT 280
#define FNDEF 281
#define RETURN 282
#define DEFAULT 283
#define OTHER 284
#define IMPORT 285
#define LOR 286
#define LAND 287
#define EQ 288
#define NE 289
#define GT 290
#define LT 291
#define GE 292
#define LE 293
#define NEG 294
#define IFX 295




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


