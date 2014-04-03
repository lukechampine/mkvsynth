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
     CONSTANT = 262,
     IDENTIFIER = 263,
     ASSIGN = 264,
     BINOP = 265,
     ADDEQ = 266,
     SUBEQ = 267,
     MULEQ = 268,
     DIVEQ = 269,
     POWEQ = 270,
     MODEQ = 271,
     CHNEQ = 272,
     CHAIN = 273,
     CNCAT = 274,
     IF = 275,
     ELSE = 276,
     TERN = 277,
     FNCT = 278,
     FNDEF = 279,
     RETURN = 280,
     DEFAULT = 281,
     OTHER = 282,
     IMPORT = 283,
     LOR = 284,
     LAND = 285,
     EQ = 286,
     NE = 287,
     GT = 288,
     LT = 289,
     GE = 290,
     LE = 291,
     NEG = 292,
     IFX = 293
   };
#endif
/* Tokens.  */
#define NUM 258
#define BOOL 259
#define STRING 260
#define CLIP 261
#define CONSTANT 262
#define IDENTIFIER 263
#define ASSIGN 264
#define BINOP 265
#define ADDEQ 266
#define SUBEQ 267
#define MULEQ 268
#define DIVEQ 269
#define POWEQ 270
#define MODEQ 271
#define CHNEQ 272
#define CHAIN 273
#define CNCAT 274
#define IF 275
#define ELSE 276
#define TERN 277
#define FNCT 278
#define FNDEF 279
#define RETURN 280
#define DEFAULT 281
#define OTHER 282
#define IMPORT 283
#define LOR 284
#define LAND 285
#define EQ 286
#define NE 287
#define GT 288
#define LT 289
#define GE 290
#define LE 291
#define NEG 292
#define IFX 293




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


