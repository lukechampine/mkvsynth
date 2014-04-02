/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.5"

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

/* Line 268 of yacc.c  */
#line 1 "delbrot/delbrot.y"

	#include <dlfcn.h>
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "delbrot.h"
	/* declarations */
	void yyerror(char *error, ...);
	int yylex();
	FILE *yyin;
	/* debug */
	#define YYDEBUG 1


/* Line 268 of yacc.c  */
#line 87 "delbrot/y.tab.c"

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


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 209 "delbrot/y.tab.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   189

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNRULES -- Number of states.  */
#define YYNSTATES  137

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,     2,     2,     2,    55,     2,     2,
      41,    42,    53,    51,    47,    52,    57,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    46,
       2,    48,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,    50,    44,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    28,    34,    38,    41,    43,    46,    52,    60,    64,
      65,    67,    71,    74,    78,    80,    82,    84,    86,    88,
      92,    94,    97,    99,   101,   105,   107,   109,   111,   113,
     115,   117,   119,   121,   123,   129,   131,   135,   137,   141,
     143,   147,   149,   153,   155,   157,   159,   163,   165,   167,
     169,   171,   173,   177,   179,   181,   183,   187,   189,   191,
     193,   195,   199,   201,   205,   207,   211,   213,   216,   218,
     222,   224,   227,   229,   233,   235,   238,   241,   243,   245
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    -1,    60,    61,    -1,    62,    -1,    63,
      -1,    64,    -1,    65,    -1,    66,    -1,    67,    -1,    26,
      96,    41,    68,    42,    43,    72,    44,    -1,    28,    96,
      45,    73,    46,    -1,    27,    73,    46,    -1,    27,    46,
      -1,    46,    -1,    73,    46,    -1,    22,    41,    73,    42,
      71,    -1,    22,    41,    73,    42,    71,    23,    71,    -1,
      30,    73,    46,    -1,    -1,    69,    -1,    68,    47,    69,
      -1,    70,    96,    -1,    45,    70,    96,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,    61,    -1,    43,    72,
      44,    -1,    61,    -1,    72,    61,    -1,    74,    -1,    76,
      -1,    76,    75,    74,    -1,    48,    -1,    13,    -1,    14,
      -1,    15,    -1,    16,    -1,    17,    -1,    18,    -1,    19,
      -1,    78,    -1,    78,    49,    76,    50,    77,    -1,    76,
      -1,    29,    49,    76,    -1,    79,    -1,    78,    31,    79,
      -1,    80,    -1,    79,    32,    80,    -1,    82,    -1,    80,
      81,    82,    -1,    33,    -1,    34,    -1,    84,    -1,    82,
      83,    84,    -1,    35,    -1,    36,    -1,    37,    -1,    38,
      -1,    86,    -1,    84,    85,    86,    -1,    51,    -1,    52,
      -1,    88,    -1,    86,    87,    88,    -1,    53,    -1,    54,
      -1,    55,    -1,    89,    -1,    88,    56,    89,    -1,    90,
      -1,    89,    21,    90,    -1,    91,    -1,    91,    20,    90,
      -1,    92,    -1,    92,    93,    -1,    95,    -1,    95,    57,
      95,    -1,    94,    -1,    93,    94,    -1,    96,    -1,    96,
      45,    96,    -1,    96,    -1,    52,    96,    -1,    58,    96,
      -1,    10,    -1,     9,    -1,    41,    73,    42,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    29,    29,    31,    35,    36,    37,    38,    39,    40,
      44,    48,    52,    53,    57,    58,    62,    63,    67,    71,
      72,    73,    77,    78,    82,    82,    82,    82,    86,    87,
      91,    92,    96,   100,   101,   105,   105,   105,   105,   105,
     105,   105,   105,   109,   110,   114,   115,   119,   120,   124,
     125,   129,   130,   134,   134,   138,   139,   143,   143,   143,
     143,   147,   148,   152,   152,   156,   157,   161,   161,   161,
     165,   166,   170,   171,   174,   175,   179,   180,   184,   185,
     189,   190,   194,   195,   199,   200,   201,   205,   206,   207
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "BOOL", "STRING", "CLIP", "TRUE",
  "FALSE", "CONSTANT", "IDENTIFIER", "ASSIGN", "BINOP", "ADDEQ", "SUBEQ",
  "MULEQ", "DIVEQ", "POWEQ", "MODEQ", "CHNEQ", "CHAIN", "CNCAT", "IF",
  "ELSE", "TERN", "FNCT", "FNDEF", "RETURN", "DEFAULT", "OTHER", "IMPORT",
  "LOR", "LAND", "EQ", "NE", "GT", "LT", "GE", "LE", "NEG", "IFX", "'('",
  "')'", "'{'", "'}'", "':'", "';'", "','", "'='", "'?'", "'|'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'^'", "'.'", "'!'", "$accept", "program",
  "stmt", "function_declaration", "default_stmt", "return_stmt",
  "expression_stmt", "selection_stmt", "import_stmt", "param_list",
  "param", "type", "block", "stmt_list", "expr", "assignment_expr",
  "assignment_operator", "ternary_expr", "ternary_end", "boolean_or_expr",
  "boolean_and_expr", "boolean_eq_expr", "eq_operator", "boolean_rel_expr",
  "rel_operator", "arithmetic_add_expr", "add_operator",
  "arithmetic_mul_expr", "mul_operator", "arithmetic_exp_expr",
  "concat_expr", "chain_expr", "function_expr", "fn_name_expr", "arg_list",
  "function_arg", "unary_expr", "primary_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,    40,    41,   123,   125,    58,    59,    44,    61,    63,
     124,    43,    45,    42,    47,    37,    94,    46,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    61,    61,    61,    61,
      62,    63,    64,    64,    65,    65,    66,    66,    67,    68,
      68,    68,    69,    69,    70,    70,    70,    70,    71,    71,
      72,    72,    73,    74,    74,    75,    75,    75,    75,    75,
      75,    75,    75,    76,    76,    77,    77,    78,    78,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    90,    90,    91,    91,    92,    92,
      93,    93,    94,    94,    95,    95,    95,    96,    96,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       8,     5,     3,     2,     1,     2,     5,     7,     3,     0,
       1,     3,     2,     3,     1,     1,     1,     1,     1,     3,
       1,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     3,     1,     3,     1,     3,     1,     2,     1,     3,
       1,     2,     1,     3,     1,     2,     2,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    88,    87,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     3,     4,     5,     6,     7,     8,
       9,     0,    32,    33,    43,    47,    49,    51,    55,    61,
      65,    70,    72,    74,    76,    78,    84,     0,     0,    13,
       0,     0,     0,     0,    85,    86,    15,    36,    37,    38,
      39,    40,    41,    42,    35,     0,     0,     0,     0,    53,
      54,     0,    57,    58,    59,    60,     0,    63,    64,     0,
      67,    68,    69,     0,     0,     0,     0,    77,    80,    82,
       0,     0,    19,    12,     0,    18,    89,    34,    48,     0,
      50,    52,    56,    62,    66,    71,    73,    75,    81,     0,
      79,     0,    24,    25,    26,    27,     0,     0,    20,     0,
       0,     0,    83,     0,    28,    16,     0,     0,     0,    22,
      11,     0,    45,    44,    30,     0,     0,    23,     0,    21,
       0,    29,    31,    17,     0,    46,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,   114,    15,    16,    17,    18,    19,    20,   107,
     108,   109,   115,   125,    21,    22,    55,    23,   123,    24,
      25,    26,    61,    27,    66,    28,    69,    29,    73,    30,
      31,    32,    33,    34,    77,    78,    35,    36
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const yytype_int16 yypact[] =
{
     -56,     5,   -56,   -56,   -56,   -37,    28,     9,    28,    68,
      68,   -56,    28,    28,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -36,   -56,   141,   -25,   -19,    19,     6,   -35,    46,
     -30,     7,   -56,    14,    28,   -21,   -56,    68,     4,   -56,
      -6,    13,     3,    12,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,    68,    68,    68,    68,   -56,
     -56,    68,   -56,   -56,   -56,   -56,    68,   -56,   -56,    68,
     -56,   -56,   -56,    68,    68,    68,    68,    28,   -56,    31,
      68,    40,    17,   -56,    68,   -56,   -56,   -56,   -19,    30,
      19,     6,   -35,    46,   -30,     7,   -56,   -56,   -56,    28,
     -56,    38,   -56,   -56,   -56,   -56,   132,   -17,   -56,    28,
      39,   112,   -56,   121,   -56,    36,    28,    49,    17,   -56,
     -56,    44,   -56,   -56,   -56,    61,    38,   -56,   121,   -56,
      68,   -56,   -56,   -56,    88,   -56,   -56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,    -1,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -23,    -3,   -20,   -11,     2,    53,   -56,   -55,   -56,   -56,
      67,    70,   -56,    64,   -56,    73,   -56,    75,   -56,    69,
      71,    -2,   -56,   -56,   -56,    84,    72,    -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      14,    38,    89,    41,    37,     2,    56,    44,    45,    40,
      46,    42,    43,    58,     3,     4,    67,    68,     3,     4,
     102,   103,   104,   105,    57,   117,    74,     5,    75,    79,
     118,     6,     7,     8,    76,     9,    80,     3,     4,    81,
      83,    62,    63,    64,    65,    82,    10,     3,     4,    85,
      10,    11,    59,    60,    86,    39,   122,    12,    84,   126,
       5,    12,   106,    13,     6,     7,     8,    13,     9,    10,
       3,     4,    79,    96,    97,   135,    99,     3,     4,    10,
     111,   113,   101,     5,    11,   120,   110,     6,     7,     8,
      12,     9,   128,   130,   112,   129,    13,     3,     4,    70,
      71,    72,    10,   116,   119,   131,   133,    11,    87,    10,
       5,   127,   124,    12,     6,     7,     8,   134,     9,    13,
      12,     3,     4,    88,   132,    91,    13,   124,    90,    10,
       3,     4,   136,   132,    11,   102,   103,   104,   105,    92,
      12,   121,    94,     5,    93,    95,    13,     6,     7,     8,
       0,     9,   100,    10,    47,    48,    49,    50,    51,    52,
      53,    98,    10,     0,    12,     0,     0,    11,     0,     0,
      13,     0,     0,    12,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-56))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       1,     6,    57,     8,    41,     0,    31,    12,    13,     7,
      46,     9,    10,    32,     9,    10,    51,    52,     9,    10,
       3,     4,     5,     6,    49,    42,    56,    22,    21,    34,
      47,    26,    27,    28,    20,    30,    57,     9,    10,    37,
      46,    35,    36,    37,    38,    41,    41,     9,    10,    46,
      41,    46,    33,    34,    42,    46,   111,    52,    45,    23,
      22,    52,    45,    58,    26,    27,    28,    58,    30,    41,
       9,    10,    77,    75,    76,   130,    45,     9,    10,    41,
      50,    43,    42,    22,    46,    46,    84,    26,    27,    28,
      52,    30,    43,    49,    99,   118,    58,     9,    10,    53,
      54,    55,    41,   106,   109,    44,   126,    46,    55,    41,
      22,   116,   113,    52,    26,    27,    28,   128,    30,    58,
      52,     9,    10,    56,   125,    61,    58,   128,    58,    41,
       9,    10,    44,   134,    46,     3,     4,     5,     6,    66,
      52,    29,    73,    22,    69,    74,    58,    26,    27,    28,
      -1,    30,    80,    41,    13,    14,    15,    16,    17,    18,
      19,    77,    41,    -1,    52,    -1,    -1,    46,    -1,    -1,
      58,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    60,     0,     9,    10,    22,    26,    27,    28,    30,
      41,    46,    52,    58,    61,    62,    63,    64,    65,    66,
      67,    73,    74,    76,    78,    79,    80,    82,    84,    86,
      88,    89,    90,    91,    92,    95,    96,    41,    96,    46,
      73,    96,    73,    73,    96,    96,    46,    13,    14,    15,
      16,    17,    18,    19,    48,    75,    31,    49,    32,    33,
      34,    81,    35,    36,    37,    38,    83,    51,    52,    85,
      53,    54,    55,    87,    56,    21,    20,    93,    94,    96,
      57,    73,    41,    46,    45,    46,    42,    74,    79,    76,
      80,    82,    84,    86,    88,    89,    90,    90,    94,    45,
      95,    42,     3,     4,     5,     6,    45,    68,    69,    70,
      73,    50,    96,    43,    61,    71,    70,    42,    47,    96,
      46,    29,    76,    77,    61,    72,    23,    96,    43,    69,
      49,    44,    61,    71,    72,    76,    44
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
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


/*----------.
| yyparse.  |
`----------*/

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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

/* Line 1806 of yacc.c  */
#line 31 "delbrot/delbrot.y"
    { ex(&global, &(yyvsp[(2) - (2)]));                        }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 44 "delbrot/delbrot.y"
    { (yyval) = makeNode(FNDEF, 3, &(yyvsp[(2) - (8)]), &(yyvsp[(4) - (8)]), &(yyvsp[(7) - (8)]));  }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 48 "delbrot/delbrot.y"
    { (yyval) = makeNode(DEFAULT, 2, &(yyvsp[(2) - (5)]), &(yyvsp[(4) - (5)]));     }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 52 "delbrot/delbrot.y"
    { (yyval) = makeNode(RETURN, 1, &(yyvsp[(2) - (3)]));           }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 53 "delbrot/delbrot.y"
    { (yyval) = makeNode(RETURN, 0);                }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 57 "delbrot/delbrot.y"
    { (yyval) = makeLeaf(typeNum, 0); /* no op */   }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 58 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (2)]);                                 }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 62 "delbrot/delbrot.y"
    { (yyval) = makeNode(IF, 2, &(yyvsp[(3) - (5)]), &(yyvsp[(5) - (5)]));          }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 63 "delbrot/delbrot.y"
    { (yyval) = makeNode(IF, 3, &(yyvsp[(3) - (7)]), &(yyvsp[(5) - (7)]), &(yyvsp[(7) - (7)]));     }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 67 "delbrot/delbrot.y"
    { (yyval) = makeNode(IMPORT, 1, &(yyvsp[(2) - (3)]));           }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 71 "delbrot/delbrot.y"
    { (yyval) = (ASTnode){};                        }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 72 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                                 }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 73 "delbrot/delbrot.y"
    { (yyval) = append(&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]));                   }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 77 "delbrot/delbrot.y"
    { (yyval) = makeParam(typeParam, &(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]));     }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 78 "delbrot/delbrot.y"
    { (yyval) = makeParam(typeOptParam, &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 87 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                                 }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 92 "delbrot/delbrot.y"
    { (yyval) = makeNode(';', 2, &(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]));         }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 101 "delbrot/delbrot.y"
    { (yyval) = makeNode(ASSIGN, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)])); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 110 "delbrot/delbrot.y"
    { (yyval) = makeNode(TERN, 3, &(yyvsp[(1) - (5)]), &(yyvsp[(3) - (5)]), &(yyvsp[(5) - (5)]));   }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 115 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(3) - (3)]);                                 }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 120 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 125 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 130 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 139 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 148 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 157 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 166 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 171 "delbrot/delbrot.y"
    { (yyval) = makeNode(BINOP, 3, &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]));  }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 175 "delbrot/delbrot.y"
    { (yyval) = makeNode(CHAIN, 2, &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]));       }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 180 "delbrot/delbrot.y"
    { (yyval) = makeNode(FNCT, 2, &(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]));        }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 185 "delbrot/delbrot.y"
    { (yyval) = addPluginFn(&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]));              }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 190 "delbrot/delbrot.y"
    { (yyval) = append(&(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]));                   }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 194 "delbrot/delbrot.y"
    { (yyval) = makeArg(NULL, &(yyvsp[(1) - (1)]));                 }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 195 "delbrot/delbrot.y"
    { (yyval) = makeArg(&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]));                  }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 200 "delbrot/delbrot.y"
    { (yyval) = makeNode(NEG, 1, &(yyvsp[(2) - (2)]));              }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 201 "delbrot/delbrot.y"
    { (yyval) = makeNode('!', 1, &(yyvsp[(2) - (2)]));              }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 207 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                                 }
    break;



/* Line 1806 of yacc.c  */
#line 1837 "delbrot/y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
      if (!yypact_value_is_default (yyn))
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
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



/* Line 2067 of yacc.c  */
#line 210 "delbrot/delbrot.y"
 /* end of grammar */

/* allocate a value */
Value* newValue() {
	Value *v;
	if ((v = calloc(1, sizeof(Value))) == NULL)
		MkvsynthError("out of memory");
	return v;
}

/* free an ASTnode and its children */
void freeNode(ASTnode *p) {
	if (!p)
		return;
	if (p->value) {
		freeValue(p->value);
		free(p->value);
	}
	if (p->nops > 0 && p->child) {
		while (p->nops > 0)
			freeNode(&p->child[--p->nops]);
		free(p->child);
	}
}

/* free a value */
void freeValue(Value *v) {
	if (!v)
		return;
	if (v->type == typeStr)
		free(v->str);
	if (v->type == typeId)
		free(v->id);
	if (v->type == typeNull) {
		freeVar(v->arg);
		free(v->arg);
	}
}

/* free a linked list of variables */
void freeVar(Var *v) {
	if (!v)
		return;
	if (v->name)
		free(v->name);
	freeValue(&v->value);
	if (v->next) {
		freeVar(v->next);
		free(v->next);
	}
}

/* free a linked list of functions */
void freeFn(Fn *f) {
	if (!f)
		return;
	free(f->name);
	if (f->type == fnCore) {
		free(f->fnPtr);
	}
	else {
		freeNode(f->body);
		/* free the argument list */
		int i;
		for (i = 0; i < f->params->nargs; i++)
			freeVar(f->params->args);
		free(f->params->args);
		free(f->params);
	}
	if (f->next) {
		freeFn(f->next);
		free(f->next);
	}
}

/* free an environment */
void freeEnv(Env *e) {
	freeVar(e->varTable);
	free(e->varTable);
	freeFn(e->fnTable);
	free(e->fnTable);
}

/* create a node in the AST */
ASTnode makeNode(int op, int nops, ...) {
	ASTnode p = {};
	p.op = op;
	p.nops = nops;
	if (p.nops == 0)
		return p;
	/* allocate space for children */
	if ((p.child = calloc(nops, sizeof(ASTnode))) == NULL)
		MkvsynthError("out of memory");
	int i;
	va_list ap;
	va_start(ap, nops);
	for (i = 0; i < nops; i++)
		p.child[i] = *va_arg(ap, ASTnode *);
	va_end(ap);
	return p;
}

/* create a leaf node */
ASTnode makeLeaf(valueType type, ...) {
	ASTnode p = {};
	/* create payload */
	p.value = newValue();
	p.value->type = type;
	va_list ap;
	va_start(ap, type);
	switch (type) {
		case typeNum:  p.value->num  = va_arg(ap, double); break;
		case typeBool: p.value->bool = va_arg(ap, bool_t); break;
		case typeStr:  p.value->str  = va_arg(ap, char *); break;
		case typeId:   p.value->id   = va_arg(ap, char *); break;
		default:       MkvsynthError("invalid leaf type");
	}
	return p;
}

/* create a parameter */
ASTnode makeParam(varType type, ASTnode *typeNode, ASTnode *nameNode) {
	ASTnode p = {};
	Var *v = calloc(1, sizeof(Var));
	v->type = type;
	v->value.type = typeNull;
	v->name = nameNode->value->id;
	switch (typeNode->op) {
		case NUM:    v->valType = typeNum;  break;
		case BOOL:   v->valType = typeBool; break;
		case STRING: v->valType = typeStr;  break;
		case CLIP:   v->valType = typeClip; break;
	}
	p.value = newValue();
	p.value->arg = v;
	return p;
}

/* create an argument */
ASTnode makeArg(ASTnode *nameNode, ASTnode *valNode) {
	ASTnode p = {};
	p.value = newValue();
	p.value->type = typeNull;
	Var *v = calloc(1, sizeof(Var));
	v->type = nameNode ? typeOptArg : typeArg;
	v->name = nameNode ? nameNode->value->id : NULL;
	v->fnArg = *valNode;
	p.value->arg = v;
	return p;
}

/* link two parameters/optargs together */
ASTnode append(ASTnode *p, ASTnode *v) {
	if (!p || !v)
		MkvsynthError("invalid argument");
	Var *traverse;
	for (traverse = p->value->arg; traverse->next; traverse = traverse->next);
	traverse->next = v->value->arg;
	return *p;
}

/* add a core function to the function table */
void putFn(Env *e, Fn *f) {
	f->next = e->fnTable;
	e->fnTable = f;
}

/* look up a function */
Fn* getFn(Env const *e, char const *fnName) {
	if (!e)
		return NULL;
	Fn *traverse;
	for (traverse = e->fnTable; traverse != NULL; traverse = traverse->next) {
		if (strcmp(traverse->name, fnName) == 0)
			return traverse;
	}
	/* check parent environment */
	return getFn(e->parent, fnName);
}

/* add a plugin function to the global fnTable and return its identifier */
ASTnode addPluginFn(ASTnode *pluginName, ASTnode *fnName) {
	/* create identifier */
	char *id = malloc(strlen(pluginName->value->id) + strlen(fnName->value->id) + 1);
	strcat(id, pluginName->value->id);
	strcat(id, ".");
	strcat(id, fnName->value->id);
	/* check if function is already in fnTable */
	if (getFn(&global, id) != NULL)
		return makeLeaf(typeId, id);
	/* look up plugin */
	Plugin *traverse;
	Value (*pluginFn) (argList *);
	for (traverse = pluginList; traverse != NULL; traverse = traverse->next) {
		if (strcmp(traverse->name, pluginName->value->id) == 0) {
			/* look up symbol */
			dlerror();
			pluginFn = dlsym(traverse->handle, fnName->value->id);
			if (dlerror() != NULL)
				MkvsynthError("function \"%s\" not found in plugin %s", fnName->value->id, pluginName->value->id);
			/* add function to fnTable */
			Fn *f = calloc(1, sizeof(Fn));
			f->type = fnCore;
			f->name = id;
			f->fnPtr = pluginFn;
			putFn(&global, f);
			return makeLeaf(typeId, id);
		}
	}
	MkvsynthError("plugin \"%s\" not loaded", pluginName->value->id);
	return (ASTnode){};
}

/* add an entry to the local varTable */
void putVar(Env *e, char const *varName, varType type) {
	/* create entry */
	Var *v = calloc(1, sizeof(Var));
	v->type = type;
	v->name = strdup(varName);
	/* add to variable table */
	v->next = e->varTable;
	e->varTable = v;
}

/* look up a variable in the varTable */
Var* getVar(Env const *e, char const *varName) {
	Var *traverse;
	for (traverse = e->varTable; traverse != NULL; traverse = traverse->next) {
		if (strcmp(traverse->name, varName) == 0)
			return traverse;
	}
	return NULL;
}

Value setVar(Env const *e, char const *varName, Value const *v) {
	Var *traverse;
	for (traverse = e->varTable; traverse != NULL; traverse = traverse->next) {
		if (strcmp(traverse->name, varName) == 0) {
			traverse->valType = v->type;
			traverse->value = *v;
			return traverse->value;
		}
	}
	MkvsynthError("could not set value of undefined variable %s", varName);
	return *newValue();
}

/* alias for MkvsynthError */
void yyerror(char *error, ...) {
	va_list arglist;
	va_start(arglist, error);
	MkvsynthError(error, arglist);
	va_end(arglist);
}

int main(int argc, char **argv) {
	/* help message */
	if ((argc != 1 && argc != 2)
	|| (argc > 1 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))) {
		printf("Usage: mkvsynth [FILE]\nInterprets an mkvsynth script.\n\nIf FILE is omitted, STDIN will be used instead.\n\nReport bugs on github.com/mkvsynth/mkvsynth.\n");
		exit(0);
	}

	/* read script file, if provided */
	if (argc == 2) {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			MkvsynthError("could not open file \"%s\" for reading", argv[1]);
			exit(1);
		}
	}

	if (setjmp(global.returnContext) != 0)
		return 0;

	/* initialize function table */
	int i;
	for(i = 0; coreFunctions[i].name != 0; i++)
		putFn(&global, &coreFunctions[i]);
	for(i = 0; internalFilters[i].name != 0; i++)
		putFn(&global, &internalFilters[i]);

	/* main parse loop */
	return yyparse();
}

