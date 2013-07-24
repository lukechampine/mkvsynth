
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "delbrot.y"

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    void yyerror(char *, ...);
    extern int linenumber;
    int mem = 0;
    #define YYDEBUG 1


/* Line 189 of yacc.c  */
#line 87 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 197 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   177

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNRULES -- Number of states.  */
#define YYNSTATES  121

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,     2,     2,    37,     2,     2,
      41,    42,    35,    33,    45,    34,    49,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
       2,    47,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    25,
      26,    27,    28,    29,    30,    31,    32,    38,    39,    40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    20,    21,    23,
      27,    30,    33,    36,    38,    40,    42,    44,    47,    53,
      61,    67,    77,    79,    83,    85,    88,    90,    92,    96,
     100,   104,   108,   112,   116,   120,   122,   126,   128,   130,
     132,   134,   136,   138,   140,   142,   144,   148,   150,   152,
     154,   156,   158,   160,   162,   165,   168,   170,   173,   176,
     178,   183,   190,   194,   195,   197,   201,   203,   206,   208,
     210
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    -1,    51,    52,    -1,    53,    -1,    56,
      -1,    21,    73,    41,    54,    42,    43,    61,    44,    -1,
      -1,    55,    -1,    54,    45,    55,    -1,     3,    73,    -1,
       4,    73,    -1,     5,    73,    -1,    57,    -1,    58,    -1,
      59,    -1,    46,    -1,    62,    46,    -1,    16,    41,    62,
      42,    60,    -1,    16,    41,    62,    42,    60,    17,    60,
      -1,    19,    41,    62,    42,    60,    -1,    18,    41,    62,
      46,    62,    46,    62,    42,    60,    -1,    56,    -1,    43,
      61,    44,    -1,    56,    -1,    61,    56,    -1,    63,    -1,
      64,    -1,    73,    47,    63,    -1,    73,    10,    63,    -1,
      73,    11,    63,    -1,    73,    12,    63,    -1,    73,    13,
      63,    -1,    73,    14,    63,    -1,    73,    15,    63,    -1,
      66,    -1,    64,    65,    66,    -1,    32,    -1,    31,    -1,
      30,    -1,    29,    -1,    28,    -1,    27,    -1,    26,    -1,
      25,    -1,    68,    -1,    66,    67,    68,    -1,    33,    -1,
      34,    -1,    35,    -1,    36,    -1,    24,    -1,    37,    -1,
      69,    -1,    34,    68,    -1,    48,    68,    -1,    70,    -1,
      69,    40,    -1,    69,    39,    -1,    73,    -1,    73,    41,
      71,    42,    -1,    70,    49,    73,    41,    71,    42,    -1,
      70,    49,    73,    -1,    -1,    72,    -1,    71,    45,    72,
      -1,    62,    -1,     8,    62,    -1,     7,    -1,     6,    -1,
      41,    62,    42,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    33,    33,    35,    39,    40,    44,    48,    49,    50,
      54,    55,    56,    61,    62,    63,    67,    68,    72,    73,
      77,    78,    82,    83,    87,    88,    92,    96,    97,    98,
      99,   100,   101,   102,   103,   107,   108,   112,   112,   112,
     112,   112,   112,   112,   112,   116,   117,   121,   121,   121,
     121,   121,   121,   125,   126,   127,   131,   132,   133,   137,
     138,   139,   140,   144,   145,   146,   150,   151,   155,   156,
     157
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "DOUBLE", "STRING", "CONSTANT",
  "IDENTIFIER", "OPTARG", "BINOP", "ADDEQ", "SUBEQ", "MULEQ", "DIVEQ",
  "POWEQ", "MODEQ", "IF", "ELSE", "FOR", "WHILE", "FNCT", "FNDEF",
  "RETURN", "IFX", "'^'", "LAND", "LOR", "LE", "GE", "LT", "GT", "NE",
  "EQ", "'+'", "'-'", "'*'", "'/'", "'%'", "NEG", "DEC", "INC", "'('",
  "')'", "'{'", "'}'", "','", "';'", "'='", "'!'", "'.'", "$accept",
  "program", "item", "function_declaration", "param_list", "param", "stmt",
  "expression_stmt", "selection_stmt", "iteration_stmt", "block",
  "stmt_list", "expr", "assignment_expr", "boolean_expr",
  "boolean_operator", "arithmetic_expr", "arithmetic_operator",
  "prefix_expr", "postfix_expr", "function_expr", "arg_list",
  "function_arg", "primary_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    94,   279,   280,   281,   282,   283,
     284,   285,   286,    43,    45,    42,    47,    37,   287,   288,
     289,    40,    41,   123,   125,    44,    59,    61,    33,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    51,    52,    52,    53,    54,    54,    54,
      55,    55,    55,    56,    56,    56,    57,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    62,    63,    63,    63,
      63,    63,    63,    63,    63,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65,    66,    66,    67,    67,    67,
      67,    67,    67,    68,    68,    68,    69,    69,    69,    70,
      70,    70,    70,    71,    71,    71,    72,    72,    73,    73,
      73
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     8,     0,     1,     3,
       2,     2,     2,     1,     1,     1,     1,     2,     5,     7,
       5,     9,     1,     3,     1,     2,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     2,     1,
       4,     6,     3,     0,     1,     3,     1,     2,     1,     1,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    69,    68,     0,     0,     0,     0,     0,
       0,    16,     0,     3,     4,     5,    13,    14,    15,     0,
      26,    27,    35,    45,    53,    56,    59,     0,     0,     0,
       0,    54,    59,     0,    55,    17,    44,    43,    42,    41,
      40,    39,    38,    37,     0,    51,    47,    48,    49,    50,
      52,     0,    58,    57,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     7,    70,    36,    46,
      62,    29,    30,    31,    32,    33,    34,     0,    66,     0,
      64,    28,     0,     0,     0,     0,     0,     0,     0,     8,
      63,    67,    60,     0,     0,    22,    18,     0,    20,    10,
      11,    12,     0,     0,     0,    65,    24,     0,     0,     0,
       0,     9,    61,    23,    25,    19,     0,     0,     0,     6,
      21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    13,    14,    88,    89,    95,    16,    17,    18,
      96,   107,    19,    20,    21,    44,    22,    51,    23,    24,
      25,    79,    80,    26
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -74
static const yytype_int16 yypact[] =
{
     -74,     7,   -74,   -74,   -74,   -31,   -24,   -17,    -4,     9,
       9,   -74,     9,   -74,   -74,   -74,   -74,   -74,   -74,   -10,
     -74,   145,    42,   -74,    33,   -20,   122,     9,     9,     9,
      -3,   -74,     1,     5,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,     9,   -74,   -74,   -74,   -74,   -74,
     -74,     9,   -74,   -74,    -4,     9,     9,     9,     9,     9,
       9,    -2,     9,    23,    -6,    50,    91,   -74,    42,   -74,
      19,   -74,   -74,   -74,   -74,   -74,   -74,     9,   -74,    17,
     -74,   -74,    15,     9,    15,    -4,    -4,    -4,    22,   -74,
      -2,   -74,   -74,    -2,   107,   -74,    80,    57,   -74,   -74,
     -74,   -74,    61,    91,    46,   -74,   -74,    64,    15,     9,
     107,   -74,   -74,   -74,   -74,   -74,    65,    83,    15,   -74,
     -74
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -74,   -74,   -74,   -74,   -74,     8,    -1,   -74,   -74,   -74,
     -73,    10,    -9,    87,   -74,   -74,    71,   -74,    18,   -74,
     -74,    28,    26,     0
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      15,    33,     3,     4,     3,     4,    77,     2,    30,    32,
      27,    98,    32,     3,     4,     3,     4,    28,    63,    64,
      65,     3,     4,     5,    29,     6,     7,    31,     8,    54,
      34,     5,     9,     6,     7,   115,    35,    10,    66,    10,
      83,     9,    61,     9,    32,   120,    12,    67,    10,     9,
      10,    32,    78,    11,    70,    12,    10,    12,    94,    92,
      90,    11,    93,    12,   102,    82,    45,   103,    91,    69,
       3,     4,    52,    53,    97,    46,    47,    48,    49,    50,
       5,    78,     6,     7,    78,    99,   100,   101,   112,     3,
       4,    93,    84,   106,    85,    86,    87,   108,     9,     5,
     116,     6,     7,   109,   110,    10,   114,   118,   113,   106,
      11,   111,    12,     3,     4,    68,   114,     9,   104,   105,
     117,     0,     0,     5,    10,     6,     7,   119,     0,    11,
       0,    12,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     9,    71,    72,    73,    74,    75,    76,    10,    81,
       0,     0,     0,    11,     0,    12,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,    62,
      36,    37,    38,    39,    40,    41,    42,    43
};

static const yytype_int8 yycheck[] =
{
       1,    10,     6,     7,     6,     7,     8,     0,     8,     9,
      41,    84,    12,     6,     7,     6,     7,    41,    27,    28,
      29,     6,     7,    16,    41,    18,    19,     9,    21,    49,
      12,    16,    34,    18,    19,   108,    46,    41,    41,    41,
      46,    34,    41,    34,    44,   118,    48,    42,    41,    34,
      41,    51,    61,    46,    54,    48,    41,    48,    43,    42,
      41,    46,    45,    48,    42,    42,    24,    45,    77,    51,
       6,     7,    39,    40,    83,    33,    34,    35,    36,    37,
      16,    90,    18,    19,    93,    85,    86,    87,    42,     6,
       7,    45,    42,    94,     3,     4,     5,    17,    34,    16,
     109,    18,    19,    46,    43,    41,   107,    42,    44,   110,
      46,   103,    48,     6,     7,    44,   117,    34,    90,    93,
     110,    -1,    -1,    16,    41,    18,    19,    44,    -1,    46,
      -1,    48,    10,    11,    12,    13,    14,    15,    -1,    -1,
      -1,    34,    55,    56,    57,    58,    59,    60,    41,    62,
      -1,    -1,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      25,    26,    27,    28,    29,    30,    31,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    51,     0,     6,     7,    16,    18,    19,    21,    34,
      41,    46,    48,    52,    53,    56,    57,    58,    59,    62,
      63,    64,    66,    68,    69,    70,    73,    41,    41,    41,
      73,    68,    73,    62,    68,    46,    25,    26,    27,    28,
      29,    30,    31,    32,    65,    24,    33,    34,    35,    36,
      37,    67,    39,    40,    49,    10,    11,    12,    13,    14,
      15,    41,    47,    62,    62,    62,    41,    42,    66,    68,
      73,    63,    63,    63,    63,    63,    63,     8,    62,    71,
      72,    63,    42,    46,    42,     3,     4,     5,    54,    55,
      41,    62,    42,    45,    43,    56,    60,    62,    60,    73,
      73,    73,    42,    45,    71,    72,    56,    61,    17,    46,
      43,    55,    42,    44,    56,    60,    62,    61,    42,    44,
      60
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1455 of yacc.c  */
#line 35 "delbrot.y"
    { ex((yyvsp[(2) - (2)])); free((yyvsp[(2) - (2)])->op.ops); free((yyvsp[(2) - (2)]));                      }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 44 "delbrot.y"
    { (yyval) = mkOpNode(FNDEF, 3, (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(7) - (8)]));   }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 48 "delbrot.y"
    { (yyval) = NULL;                             }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 49 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 50 "delbrot.y"
    { (yyval) = append((yyvsp[(3) - (3)]), (yyvsp[(1) - (3)]));                   }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 54 "delbrot.y"
    { (yyvsp[(2) - (2)])->type = typeVal; (yyval) = (yyvsp[(2) - (2)]);           }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 55 "delbrot.y"
    { (yyvsp[(2) - (2)])->type = typeVal; (yyval) = (yyvsp[(2) - (2)]);           }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 56 "delbrot.y"
    { (yyvsp[(2) - (2)])->type = typeStr; (yyval) = (yyvsp[(2) - (2)]);           }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 67 "delbrot.y"
    { (yyval) = mkOpNode(';', 2, NULL, NULL);     }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 68 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (2)]);                               }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 72 "delbrot.y"
    { (yyval) = mkOpNode(IF, 2, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]));          }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 73 "delbrot.y"
    { (yyval) = mkOpNode(IF, 3, (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)]));      }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 77 "delbrot.y"
    { (yyval) = mkOpNode(WHILE, 2, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]));       }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 78 "delbrot.y"
    { (yyval) = mkOpNode(FOR, 4, (yyvsp[(3) - (9)]), (yyvsp[(5) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(9) - (9)])); }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 83 "delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                               }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 88 "delbrot.y"
    { (yyval) = mkOpNode(';', 2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]));         }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 97 "delbrot.y"
    { (yyval) = mkOpNode('=',   2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 98 "delbrot.y"
    { (yyval) = mkOpNode(ADDEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 99 "delbrot.y"
    { (yyval) = mkOpNode(SUBEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 100 "delbrot.y"
    { (yyval) = mkOpNode(MULEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 101 "delbrot.y"
    { (yyval) = mkOpNode(DIVEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 102 "delbrot.y"
    { (yyval) = mkOpNode(POWEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 103 "delbrot.y"
    { (yyval) = mkOpNode(MODEQ, 2, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));       }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 108 "delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 117 "delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 126 "delbrot.y"
    { (yyval) = mkOpNode(NEG, 1, (yyvsp[(2) - (2)]));             }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 127 "delbrot.y"
    { (yyval) = mkOpNode('!', 1, (yyvsp[(2) - (2)]));             }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 132 "delbrot.y"
    { (yyval) = mkOpNode(INC, 1, (yyvsp[(1) - (2)]));             }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 133 "delbrot.y"
    { (yyval) = mkOpNode(DEC, 1, (yyvsp[(1) - (2)]));             }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 138 "delbrot.y"
    { (yyval) = mkOpNode(FNCT, 2, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)]));        }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 139 "delbrot.y"
    { (yyval) = mkOpNode('.',  3, (yyvsp[(1) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(5) - (6)]));    }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 140 "delbrot.y"
    { (yyval) = mkOpNode('.',  3, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL);  }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 144 "delbrot.y"
    { (yyval) = NULL;                             }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 145 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 146 "delbrot.y"
    { (yyval) = append((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));                   }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 150 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 151 "delbrot.y"
    { (yyvsp[(1) - (2)])->var->value = (yyvsp[(2) - (2)]);                   }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 155 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 156 "delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 157 "delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                               }
    break;



/* Line 1455 of yacc.c  */
#line 1794 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 160 "delbrot.y"
 /* end of grammar */

/* initialize a new node */
ASTnode *newNode() {
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    p->next = NULL;
    return p;
}

/* free a node */
void freeNode(ASTnode *p) {
    if (!p)
        return;
    /* recurse to linked node */
    freeNode(p->next);
    free(p);
}

/* create a value node in the AST */
ASTnode *mkValNode(double val) {
    ASTnode *p = newNode();
    p->type = typeVal;
    p->val = val;
    return p;
}

/* create a string node in the AST */
ASTnode *mkStrNode(char *str) {
    ASTnode *p = newNode();
    p->type = typeStr;
    /* remove enclosing quotation marks */
    p->str = strdup(str + 1);
    p->str[strlen(str)-2] = 0;
    return p;
}

/* create an identifier node in the AST */
ASTnode *mkIdNode(char *ident) {
    ASTnode *p = newNode();
    p->type = typeId;
    p->str = strdup(ident);
    p->scope = &globalVars;
    return p;
}

/* create an optional argument node in the AST */
/* reuse the var struct, it's close enough */
ASTnode *mkOptArgNode(char *name) {
    ASTnode *p = newNode();
    /* allocate space for var */
    if ((p->var = malloc(sizeof(varRec))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeOptArg;
    p->var->name = name;
    return p;
}

/* create an operation node in the AST */
ASTnode *mkOpNode(int oper, int nops, ...) {
    ASTnode *p = newNode();
    /* allocate space for ops */
    if ((p->op.ops = malloc(nops * sizeof(ASTnode *))) == NULL)
        yyerror("out of memory");
    p->type = typeOp;
    p->op.oper = oper;
    p->op.nops = nops;
    int i;
    va_list ap;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->op.ops[i] = va_arg(ap, ASTnode*);
    va_end(ap);
    return p;
}

/* add an ASTnode to the end of a linked list of arguments */
ASTnode *append(ASTnode *root, ASTnode *node) {
    if (!root || !node)
        yyerror("invalid argument");
    ASTnode *traverse;
    for (traverse = root; traverse->next != NULL; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* the function table */
funcRec *fnTable;

funcRec *putFn(funcRec *fn) {
    fn->next = fnTable;
    fnTable = fn;
    return fn;
}

/* look up a function name's corresponding pointer */
funcRec *getFn(char const *fnName) {
    funcRec *fn;
    for (fn = fnTable; fn != NULL; fn = fn->next)
        if (strcmp (fn->name,fnName) == 0)
            return fn;
    return NULL;
}

/* the global variable table */
varRec *globalVars;

/* allocate a new variable */
varRec *putVar(ASTnode *p) {
    varRec *ptr;
    if ((ptr = malloc(sizeof(varRec))) == NULL)
        yyerror("out of memory");
    ptr->name = strdup(p->str);
    ptr->next = *p->scope;
    *p->scope = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
varRec *getVar(ASTnode *p) {
    varRec *ptr;
    for (ptr = *p->scope; ptr && ptr->name; ptr = ptr->next)
        if (strcmp(ptr->name, p->str) == 0)
            return ptr;
    /* check global scope after local scope */
    if (*p->scope != globalVars)
        for (ptr = globalVars; ptr && ptr->name; ptr = ptr->next)
            if (strcmp(ptr->name, p->str) == 0)
                return ptr;
    return NULL;
}

/* Called by yyparse on error. */
void yyerror(char *error, ...) {
    fprintf(stderr, "delbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\n");
    exit(1);
}

/* built-in functions */
static funcRec coreFunctions[] = {
    "ffmpegDecode", ffmpegDecode_AST, NULL, NULL, NULL,
    "print", print, NULL, NULL, NULL,
    "sin", nsin, NULL, NULL, NULL,
    "cos", ncos, NULL, NULL, NULL,
    "ln", nlog, NULL, NULL, NULL,
    "sqrt", nsqrt, NULL, NULL, NULL,
    0, 0, 0, 0, 0
};

int main () {
    int i;
    //yydebug = 1;

    /* initialize global variable table */
    /* TODO: can this be removed? */
    globalVars = (varRec *) malloc(sizeof(varRec));

    /* initialize function table */
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(&coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putFn(&pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}

