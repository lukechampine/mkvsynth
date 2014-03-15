/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_DELBROT_Y_TAB_H_INCLUDED
# define YY_YY_DELBROT_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

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
     CHNEQ = 275,
     CHAIN = 276,
     CNCAT = 277,
     IF = 278,
     ELSE = 279,
     TERN = 280,
     FNCT = 281,
     FNDEF = 282,
     RETURN = 283,
     DEFAULT = 284,
     OTHER = 285,
     IMPORT = 286,
     LOR = 287,
     LAND = 288,
     EQ = 289,
     NE = 290,
     GT = 291,
     LT = 292,
     GE = 293,
     LE = 294,
     IFX = 295,
     NEG = 296
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
#define CHNEQ 275
#define CHAIN 276
#define CNCAT 277
#define IF 278
#define ELSE 279
#define TERN 280
#define FNCT 281
#define FNDEF 282
#define RETURN 283
#define DEFAULT 284
#define OTHER 285
#define IMPORT 286
#define LOR 287
#define LAND 288
#define EQ 289
#define NE 290
#define GT 291
#define LT 292
#define GE 293
#define LE 294
#define IFX 295
#define NEG 296



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_DELBROT_Y_TAB_H_INCLUDED  */
