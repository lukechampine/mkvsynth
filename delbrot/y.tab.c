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

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include "delbrot.h"
    /* prototypes */
    Env *global;
    extern int linenumber;
    /* script file */
    extern FILE *yyin;
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


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 205 "delbrot/y.tab.c"

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
#define YYLAST   235

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  82
/* YYNRULES -- Number of states.  */
#define YYNSTATES  127

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,     2,     2,    53,     2,     2,
      39,    40,    51,    49,    45,    50,     2,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    44,
       2,    46,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,    48,    42,     2,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      26,    32,    36,    39,    41,    44,    50,    58,    59,    61,
      65,    68,    72,    74,    76,    78,    80,    82,    86,    88,
      91,    93,    95,    99,   101,   103,   105,   107,   109,   111,
     113,   115,   121,   123,   127,   129,   133,   135,   139,   141,
     145,   147,   149,   151,   155,   157,   159,   161,   163,   165,
     169,   171,   173,   175,   179,   181,   183,   185,   187,   191,
     193,   196,   199,   201,   204,   208,   213,   215,   218,   220,
     224,   226,   228
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      57,     0,    -1,    -1,    57,    58,    -1,    59,    -1,    60,
      -1,    61,    -1,    62,    -1,    63,    -1,    25,    89,    39,
      64,    40,    41,    68,    42,    -1,    27,    89,    43,    69,
      44,    -1,    26,    69,    44,    -1,    26,    44,    -1,    44,
      -1,    69,    44,    -1,    21,    39,    69,    40,    67,    -1,
      21,    39,    69,    40,    67,    22,    67,    -1,    -1,    65,
      -1,    64,    45,    65,    -1,    66,    89,    -1,    43,    66,
      89,    -1,     3,    -1,     4,    -1,     5,    -1,     6,    -1,
      58,    -1,    41,    68,    42,    -1,    58,    -1,    68,    58,
      -1,    70,    -1,    72,    -1,    89,    71,    70,    -1,    46,
      -1,    14,    -1,    15,    -1,    16,    -1,    17,    -1,    18,
      -1,    19,    -1,    74,    -1,    74,    47,    72,    48,    73,
      -1,    72,    -1,    28,    47,    72,    -1,    75,    -1,    74,
      29,    75,    -1,    76,    -1,    75,    30,    76,    -1,    78,
      -1,    76,    77,    78,    -1,    31,    -1,    32,    -1,    80,
      -1,    78,    79,    80,    -1,    33,    -1,    34,    -1,    35,
      -1,    36,    -1,    82,    -1,    80,    81,    82,    -1,    49,
      -1,    50,    -1,    84,    -1,    82,    83,    84,    -1,    51,
      -1,    52,    -1,    53,    -1,    85,    -1,    84,    54,    85,
      -1,    86,    -1,    50,    86,    -1,    55,    86,    -1,    89,
      -1,    89,    87,    -1,    86,    20,    89,    -1,    86,    20,
      89,    87,    -1,    88,    -1,    87,    88,    -1,    89,    -1,
      89,    43,    89,    -1,    10,    -1,     9,    -1,    39,    69,
      40,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    30,    30,    32,    36,    37,    38,    39,    40,    44,
      48,    52,    53,    57,    58,    62,    63,    67,    68,    69,
      73,    74,    78,    78,    78,    78,    82,    83,    87,    88,
      92,    96,    97,   101,   101,   101,   101,   101,   101,   101,
     105,   106,   110,   111,   115,   116,   120,   121,   125,   126,
     130,   130,   134,   135,   139,   139,   139,   139,   143,   144,
     148,   148,   152,   153,   157,   157,   157,   161,   162,   166,
     167,   168,   172,   173,   174,   175,   179,   180,   184,   185,
     189,   190,   191
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "BOOL", "STRING", "CLIP", "TRUE",
  "FALSE", "CONSTANT", "IDENTIFIER", "OPTARG", "ASSIGN", "BINOP", "ADDEQ",
  "SUBEQ", "MULEQ", "DIVEQ", "POWEQ", "MODEQ", "CHAIN", "IF", "ELSE",
  "TERN", "FNCT", "FNDEF", "RETURN", "DEFAULT", "OTHER", "LOR", "LAND",
  "EQ", "NE", "GT", "LT", "GE", "LE", "IFX", "NEG", "'('", "')'", "'{'",
  "'}'", "':'", "';'", "','", "'='", "'?'", "'|'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'^'", "'!'", "$accept", "program", "stmt",
  "function_declaration", "default_stmt", "return_stmt", "expression_stmt",
  "selection_stmt", "param_list", "param", "type", "block", "stmt_list",
  "expr", "assignment_expr", "assignment_operator", "ternary_expr",
  "ternary_end", "boolean_or_expr", "boolean_and_expr", "boolean_eq_expr",
  "eq_operator", "boolean_rel_expr", "rel_operator", "arithmetic_add_expr",
  "add_operator", "arithmetic_mul_expr", "mul_operator",
  "arithmetic_exp_expr", "unary_expr", "function_expr", "arg_list",
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
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,    40,
      41,   123,   125,    58,    59,    44,    61,    63,   124,    43,
      45,    42,    47,    37,    94,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    57,    58,    58,    58,    58,    58,    59,
      60,    61,    61,    62,    62,    63,    63,    64,    64,    64,
      65,    65,    66,    66,    66,    66,    67,    67,    68,    68,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      72,    72,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    78,    79,    79,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    83,    84,    84,    85,
      85,    85,    86,    86,    86,    86,    87,    87,    88,    88,
      89,    89,    89
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     8,
       5,     3,     2,     1,     2,     5,     7,     0,     1,     3,
       2,     3,     1,     1,     1,     1,     1,     3,     1,     2,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     1,     3,     1,
       2,     2,     1,     2,     3,     4,     1,     2,     1,     3,
       1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    81,    80,     0,     0,     0,     0,     0,
      13,     0,     0,     3,     4,     5,     6,     7,     8,     0,
      30,    31,    40,    44,    46,    48,    52,    58,    62,    67,
      69,    72,     0,     0,    12,     0,     0,     0,    70,    72,
      71,    14,     0,     0,     0,    50,    51,     0,    54,    55,
      56,    57,     0,    60,    61,     0,    64,    65,    66,     0,
       0,     0,    34,    35,    36,    37,    38,    39,    33,     0,
      73,    76,    78,     0,    17,    11,     0,    82,    45,     0,
      47,    49,    53,    59,    63,    68,    74,    32,    77,     0,
       0,    22,    23,    24,    25,     0,     0,    18,     0,     0,
       0,    75,    79,     0,    26,    15,     0,     0,     0,    20,
      10,     0,    42,    41,    28,     0,     0,    21,     0,    19,
       0,    27,    29,    16,     0,    43,     9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,   104,    14,    15,    16,    17,    18,    96,    97,
      98,   105,   115,    19,    20,    69,    21,   113,    22,    23,
      24,    47,    25,    52,    26,    55,    27,    59,    28,    29,
      30,    70,    71,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -67
static const yytype_int16 yypact[] =
{
     -67,    19,   -67,   -67,   -67,   -30,    22,    39,    22,    12,
     -67,    22,    22,   -67,   -67,   -67,   -67,   -67,   -67,   -17,
     -67,   -67,   -24,    -6,   -14,    37,    26,    46,   -18,   -67,
      23,   189,    12,    21,   -67,    20,    25,    40,    23,    22,
      23,   -67,    12,    12,    12,   -67,   -67,    12,   -67,   -67,
     -67,   -67,    12,   -67,   -67,    12,   -67,   -67,   -67,    12,
      12,    22,   -67,   -67,   -67,   -67,   -67,   -67,   -67,    12,
      22,   -67,    49,    42,    10,   -67,    12,   -67,    -6,    18,
     -14,    37,    26,    46,   -18,   -67,    22,   -67,   -67,    22,
      95,   -67,   -67,   -67,   -67,    82,   -20,   -67,    22,    56,
       2,    22,   -67,   147,   -67,    81,    22,    65,    10,   -67,
     -67,    60,   -67,   -67,   -67,   116,    95,   -67,   147,   -67,
      12,   -67,   -67,   -67,   138,   -67,   -67
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -67,   -67,    -1,   -67,   -67,   -67,   -67,   -67,   -67,     0,
      14,    -4,    -8,     1,    44,   -67,   -41,   -67,   -67,    69,
      74,   -67,    72,   -67,    75,   -67,    73,   -67,    70,    64,
      79,    45,   -66,    -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      13,    33,    79,    36,    88,    42,    39,    39,    35,    32,
      37,     3,     4,    91,    92,    93,    94,    45,    46,     2,
     107,     3,     4,    43,    44,   108,    72,    41,     3,     4,
     111,     3,     4,    73,    72,    88,    60,    39,    39,    39,
       5,     9,    39,    61,     6,     7,     8,    39,     3,     4,
      39,     9,    11,    95,    39,    39,    86,    12,     9,   112,
      74,     9,    11,    10,    75,    72,   100,    12,    76,    11,
      48,    49,    50,    51,    12,    53,    54,    99,     9,   125,
      77,    72,    90,    34,   102,    91,    92,    93,    94,    11,
      38,    40,    89,   109,    12,    39,    72,    56,    57,    58,
     110,   117,   114,   116,     3,     4,   118,   120,   119,   106,
     124,    78,   123,    87,   122,    39,     5,   114,    80,    81,
       6,     7,     8,   122,    85,     3,     4,    82,    83,    84,
       0,   101,     0,     0,     9,     0,   103,     5,     0,    10,
       0,     6,     7,     8,     0,    11,     0,     3,     4,     0,
      12,     0,     0,     0,     0,     9,     3,     4,   121,     5,
      10,     0,     0,     6,     7,     8,    11,     0,     5,     0,
       0,    12,     6,     7,     8,     0,     0,     9,     0,     0,
     126,     0,    10,     0,     0,     0,     9,     0,    11,     0,
       0,    10,     0,    12,     0,     0,     0,    11,     3,     4,
       0,     0,    12,    62,    63,    64,    65,    66,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     0,     0,    68
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-67))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       1,     6,    43,     8,    70,    29,    11,    12,     7,    39,
       9,     9,    10,     3,     4,     5,     6,    31,    32,     0,
      40,     9,    10,    47,    30,    45,    31,    44,     9,    10,
      28,     9,    10,    32,    39,   101,    54,    42,    43,    44,
      21,    39,    47,    20,    25,    26,    27,    52,     9,    10,
      55,    39,    50,    43,    59,    60,    61,    55,    39,   100,
      39,    39,    50,    44,    44,    70,    48,    55,    43,    50,
      33,    34,    35,    36,    55,    49,    50,    76,    39,   120,
      40,    86,    40,    44,    89,     3,     4,     5,     6,    50,
      11,    12,    43,    98,    55,   100,   101,    51,    52,    53,
      44,   106,   103,    22,     9,    10,    41,    47,   108,    95,
     118,    42,   116,    69,   115,   120,    21,   118,    44,    47,
      25,    26,    27,   124,    60,     9,    10,    52,    55,    59,
      -1,    86,    -1,    -1,    39,    -1,    41,    21,    -1,    44,
      -1,    25,    26,    27,    -1,    50,    -1,     9,    10,    -1,
      55,    -1,    -1,    -1,    -1,    39,     9,    10,    42,    21,
      44,    -1,    -1,    25,    26,    27,    50,    -1,    21,    -1,
      -1,    55,    25,    26,    27,    -1,    -1,    39,    -1,    -1,
      42,    -1,    44,    -1,    -1,    -1,    39,    -1,    50,    -1,
      -1,    44,    -1,    55,    -1,    -1,    -1,    50,     9,    10,
      -1,    -1,    55,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,     0,     9,    10,    21,    25,    26,    27,    39,
      44,    50,    55,    58,    59,    60,    61,    62,    63,    69,
      70,    72,    74,    75,    76,    78,    80,    82,    84,    85,
      86,    89,    39,    89,    44,    69,    89,    69,    86,    89,
      86,    44,    29,    47,    30,    31,    32,    77,    33,    34,
      35,    36,    79,    49,    50,    81,    51,    52,    53,    83,
      54,    20,    14,    15,    16,    17,    18,    19,    46,    71,
      87,    88,    89,    69,    39,    44,    43,    40,    75,    72,
      76,    78,    80,    82,    84,    85,    89,    70,    88,    43,
      40,     3,     4,     5,     6,    43,    64,    65,    66,    69,
      48,    87,    89,    41,    58,    67,    66,    40,    45,    89,
      44,    28,    72,    73,    58,    68,    22,    89,    41,    65,
      47,    42,    58,    67,    68,    72,    42
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
#line 32 "delbrot/delbrot.y"
    { ex(global, (yyvsp[(2) - (2)]));                        }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 44 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(FNDEF, 3, (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(7) - (8)]));   }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 48 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(DEFAULT, 2, (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]));     }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 52 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(RETURN, 1, (yyvsp[(2) - (3)]));          }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 53 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(RETURN, 1, NULL);        }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 57 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(';', 2, NULL, NULL);     }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 58 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (2)]);                               }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 62 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(IF, 2, (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]));          }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 63 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(IF, 3, (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)]));      }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 67 "delbrot/delbrot.y"
    { (yyval) = NULL;                             }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 68 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 69 "delbrot/delbrot.y"
    { (yyval) = append((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));                   }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 73 "delbrot/delbrot.y"
    { (yyval) = mkParamNode(0, (yyvsp[(1) - (2)])->num, (yyvsp[(2) - (2)]));      }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 74 "delbrot/delbrot.y"
    { (yyval) = mkParamNode(1, (yyvsp[(2) - (3)])->num, (yyvsp[(3) - (3)]));      }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 83 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                               }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 88 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(';', 2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]));         }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 97 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(ASSIGN, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));  }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 106 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(TERN, 3, (yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)]));    }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 110 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 111 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(3) - (3)]);                               }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 116 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 121 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 126 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 135 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 144 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 153 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 162 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(BINOP, 3, (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]));   }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 167 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(NEG, 1, (yyvsp[(2) - (2)]));             }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 168 "delbrot/delbrot.y"
    { (yyval) = mkOpNode('!', 1, (yyvsp[(2) - (2)]));             }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 173 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(FNCT, 2, (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]));        }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 174 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(CHAIN, 3, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), NULL); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 175 "delbrot/delbrot.y"
    { (yyval) = mkOpNode(CHAIN, 3, (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)]));   }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 179 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 180 "delbrot/delbrot.y"
    { (yyval) = append((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]));                   }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 184 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 185 "delbrot/delbrot.y"
    { (yyval) = mkOptArgNode((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]));             }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 189 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 190 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(1) - (1)]);                               }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 191 "delbrot/delbrot.y"
    { (yyval) = (yyvsp[(2) - (3)]);                               }
    break;



/* Line 1806 of yacc.c  */
#line 1852 "delbrot/y.tab.c"
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
#line 194 "delbrot/delbrot.y"
 /* end of grammar */

/* initialize a new node */
ASTnode *newNode() {
    ASTnode *p;
    if ((p = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    /* record in unfreed table */
    p->next = NULL;
    return p;
}

/* collect garbage */
void freeNodes() {
    /* free */
}

/* create a number node in the AST */
ASTnode *mkNumNode(double num) {
    ASTnode *p = newNode();
    p->type = typeNum;
    p->num = num;
    return p;
}

/* create a boolean node in the AST */
ASTnode *mkBoolNode(int bool) {
    ASTnode *p = newNode();
    p->type = typeBool;
    p->bool = bool;
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
    p->id = strdup(ident);
    return p;
}

/* create a parameter node in the AST */
ASTnode *mkParamNode(char opt, int type, ASTnode *p) {
    p->type = typeVar;
    p->var.name = p->id;
    p->var.opt = opt;
    switch(type) {
        case NUM:    p->var.type = typeNum; break;
        case BOOL:   p->var.type = typeBool; break;
        case STRING: p->var.type = typeStr; break;
        case CLIP:   p->var.type = typeClip; break;
    }
    return p;
}

/* create an optional argument node in the AST */
ASTnode *mkOptArgNode(ASTnode *p, ASTnode *value) {
    p->type = typeOptArg;
    p->var.name = p->id;
    p->var.opt = 1;
    p->var.type = value->type;
    p->var.value = newNode();
    memcpy(p->var.value, value, sizeof(ASTnode));
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

/* add an ASTnode to the end of a linked list */
ASTnode *append(ASTnode *root, ASTnode *node) {
    if (!root)
        yyerror("invalid argument");
    ASTnode *traverse;
    for(traverse = root; traverse->next; traverse = traverse->next);
    traverse->next = node;
    return root;
}

/* add a core function to the function table */
ASTnode *putFn(Env *e, fnEntry fn) {
    /* create entry */
    ASTnode *ptr;
    if ((ptr = malloc(sizeof(ASTnode))) == NULL)
        yyerror("out of memory");
    ptr->type = typeFn;
    ptr->fn.name = fn.name;
    ptr->fn.core.fnPtr = fn.fnPtr;
    /* add to function table */
    ptr->next = e->fnTable;
    e->fnTable = ptr;
    return ptr;
}

/* look up a function */
ASTnode *getFn(Env const *e, char const *fnName) {
    if (!e)
        return NULL;
    ASTnode *traverse;
    for (traverse = e->fnTable; traverse != NULL; traverse = traverse->next)
        if (strcmp(traverse->fn.name, fnName) == 0)
            return traverse;
    /* check parent environment */
    return getFn(e->parent, fnName);
}

/* allocate a new variable */
ASTnode *putVar(Env *e, char const *varName) {
    /* create entry */
    ASTnode *ptr = newNode();
    ptr->type = typeVar;
    ptr->var.name = strdup(varName);
    ptr->var.value = NULL;
    /* add to variable table */
    ptr->next = e->varTable;
    e->varTable = ptr;
    return ptr;
}

/* look up a variable's corresponding ASTnode */
ASTnode *getVar(Env const *e, char const *varName) {
    if (!e)
        return NULL;
    ASTnode *traverse;
    for (traverse = e->varTable; traverse != NULL; traverse = traverse->next)
        if (strcmp(traverse->var.name, varName) == 0)
            return traverse;
    /* check parent environment */
    //return getVar(e->parent, varName);
    return NULL;
}

/* Called by yyparse on error, passed through to Mkvsynth Error */
void yyerror(char *error, ...) {
    va_list arglist;
    va_start(arglist, error);
    MkvsynthError(error, arglist);
    va_end(arglist);
}

/* alias for yyerror */
void MkvsynthError(char *error, ...) {
    fprintf(stderr, "\x1B[31mdelbrot:%d error: ", linenumber);
    va_list arglist;
    va_start(arglist, error);
    vfprintf(stderr, error, arglist);
    va_end(arglist);
    fprintf(stderr, "\x1B[0m\n");
    exit(1);
}

/* built-in functions */
static fnEntry coreFunctions[] = {
    { "MKVsource", MKVsource },
    { "assert",    assert    },
    { "print",     print     },
    { "show",      nshow     },
    { "read",      nread     },
    { "sqrt",      nsqrt     },
    { "sin",       nsin      },
    { "cos",       ncos      },
    { "ln",        nlog      },
    { "go",        go        },
    { 0,           0         },
};

int main(int argc, char **argv) {
    /* help message */
    if ((argc != 1 && argc != 2)
    || (argc > 1 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")))) {
        printf("Usage: mkvsynth [FILE]\nInterprets an mkvsynth script.\n\nIf FILE is omitted, STDIN will be used instead.\n\nReport bugs on github.com/mkvsynth/mkvsynth.\n");
        exit(0);
    }

    /* read script file, if provided */
    if (argc == 2) {
        yyin = fopen(argv[1], "r+");
        if (!yyin) {
            printf("error: could not open file for reading\n");
            exit(1);
        }
    }

    /* create global environment */
    global = (Env *) malloc(sizeof(Env));
    global->varTable = NULL;
    global->parent = NULL;

    if (setjmp(global->returnContext) != 0)
        exit(0);

    /* initialize function table */
    int i;
    for(i = 0; coreFunctions[i].name != 0; i++)
        putFn(global, coreFunctions[i]);
    for(i = 0; pluginFunctions[i].name != 0; i++)
        putFn(global, pluginFunctions[i]);

    /* main parse loop */
    return yyparse();
}

