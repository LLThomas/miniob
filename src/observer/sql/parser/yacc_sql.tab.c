/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"

//bison -d -b yacc_sql yacc_sql.y
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContext {
  Query *ssql;

  size_t query_length;
  Query *queries[MAX_NUM];
  size_t last_condition_length_length;
  // 每次 begin_select 会记录当前的 condition_length 到 last_condition_length
  size_t last_condition_length[MAX_NUM];

  size_t condition_length;
  Condition conditions[MAX_NUM];
  size_t value_length;
  Value values[MAX_NUM];
  size_t tuple_length;
  InsertTuple tuples[MAX_NUM];

  CompOp comp;
  char id[MAX_NUM];
  FuncName func;
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->value_length = 0;
  context->tuple_length = 0;
  context->query_length = 0;
  context->last_condition_length_length = 0;
  printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)


#line 136 "yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_YACC_SQL_TAB_H_INCLUDED
# define YY_YY_YACC_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 133 "yacc_sql.y"
 #include <stdbool.h> 

#line 182 "yacc_sql.tab.c"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    CREATE = 259,
    DROP = 260,
    TABLE = 261,
    TABLES = 262,
    INDEX = 263,
    SELECT = 264,
    DESC = 265,
    SHOW = 266,
    SYNC = 267,
    INSERT = 268,
    DELETE = 269,
    UPDATE = 270,
    LBRACE = 271,
    RBRACE = 272,
    COMMA = 273,
    TRX_BEGIN = 274,
    TRX_COMMIT = 275,
    TRX_ROLLBACK = 276,
    INT_T = 277,
    DATE_T = 278,
    STRING_T = 279,
    FLOAT_T = 280,
    TEXT_T = 281,
    HELP = 282,
    EXIT = 283,
    DOT = 284,
    INTO = 285,
    VALUES = 286,
    FROM = 287,
    WHERE = 288,
    AND = 289,
    SET = 290,
    ON = 291,
    LOAD = 292,
    DATA = 293,
    INFILE = 294,
    EQ = 295,
    LT = 296,
    GT = 297,
    LE = 298,
    GE = 299,
    NE = 300,
    AGGMAX = 301,
    AGGMIN = 302,
    AGGCOUNT = 303,
    AGGAVG = 304,
    NOT = 305,
    NULL_TOK = 306,
    NULLABLE = 307,
    IS = 308,
    UNIQUE = 309,
    INNER = 310,
    JOIN = 311,
    GROUP = 312,
    BY = 313,
    ASC = 314,
    ORDER = 315,
    IN = 316,
    NUMBER = 317,
    FLOAT = 318,
    ID = 319,
    PATH = 320,
    SSS = 321,
    STAR = 322,
    STRING_V = 323
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 135 "yacc_sql.y"

  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position;
  bool *bools;

#line 273 "yacc_sql.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void *scanner);

#endif /* !YY_YY_YACC_SQL_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  134
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  268

#define YYUNDEFTOK  2
#define YYMAXUTOK   323


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   163,   163,   165,   168,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   191,   196,   201,   207,   213,   219,   225,   231,
     237,   244,   249,   255,   256,   261,   268,   276,   278,   282,
     289,   300,   301,   302,   306,   309,   310,   311,   312,   313,
     316,   324,   335,   336,   340,   341,   349,   351,   354,   357,
     360,   364,   370,   380,   391,   403,   413,   418,   423,   428,
     433,   437,   446,   455,   464,   473,   475,   480,   485,   490,
     495,   497,   500,   505,   507,   510,   512,   516,   525,   534,
     543,   552,   564,   574,   583,   592,   603,   617,   629,   641,
     652,   666,   682,   683,   684,   685,   686,   687,   688,   689,
     693,   694,   695,   696,   699,   700,   705,   710,   711,   716,
     722,   723,   731,   739,   747,   755,   763,   771,   772,   780,
     788,   796,   804,   812,   821
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC", "INSERT", "DELETE",
  "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN", "TRX_COMMIT",
  "TRX_ROLLBACK", "INT_T", "DATE_T", "STRING_T", "FLOAT_T", "TEXT_T",
  "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "AND", "SET",
  "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE", "GE", "NE",
  "AGGMAX", "AGGMIN", "AGGCOUNT", "AGGAVG", "NOT", "NULL_TOK", "NULLABLE",
  "IS", "UNIQUE", "INNER", "JOIN", "GROUP", "BY", "ASC", "ORDER", "IN",
  "NUMBER", "FLOAT", "ID", "PATH", "SSS", "STAR", "STRING_V", "$accept",
  "commands", "command", "q", "exit", "help", "sync", "begin", "commit",
  "rollback", "drop_table", "show_tables", "desc_table", "create_index",
  "col_list", "drop_index", "create_table", "attr_def_list", "attr_def",
  "nullable", "number", "type", "ID_get", "insert", "tuple_list", "tuple",
  "value_list", "value", "delete", "update", "select", "begin_select",
  "select_attr", "expression", "attr_list", "rel_list", "where",
  "condition_list", "condition", "comOp", "func", "groupbys", "groupby",
  "orderbys", "orderby", "load_data", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323
};
# endif

#define YYPACT_NINF (-217)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -217,    79,  -217,     2,    46,   -50,    21,  -217,    57,    65,
      41,  -217,  -217,  -217,  -217,  -217,    70,  -217,   116,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,   118,  -217,    62,    64,   123,
      76,    81,  -217,  -217,    84,    85,   108,   111,  -217,   139,
     138,   127,   105,  -217,  -217,   146,   147,   119,   134,  -217,
    -217,  -217,  -217,    -6,  -217,   150,   177,   125,   137,   166,
     188,   -24,  -217,   165,   178,    88,   -46,  -217,   143,    51,
    -217,   191,   173,   194,   148,   128,   193,  -217,  -217,  -217,
      80,  -217,   117,   179,   128,   208,    12,   199,   198,   198,
      -2,   189,  -217,  -217,   200,   125,   202,  -217,  -217,  -217,
    -217,  -217,    -5,   156,   205,   204,   188,  -217,   159,  -217,
    -217,  -217,  -217,  -217,  -217,   163,    31,  -217,    -3,    45,
     102,   -24,  -217,   147,   161,    37,  -217,    51,  -217,  -217,
     162,   171,   147,   164,   198,   191,  -217,   167,   180,  -217,
    -217,   212,   168,   128,   216,   193,   131,  -217,   183,  -217,
    -217,  -217,  -217,   184,  -217,   207,  -217,   179,  -217,  -217,
     198,   198,   220,    -2,   174,   182,  -217,  -217,  -217,  -217,
     223,  -217,   181,   224,   225,   204,  -217,  -217,    61,     6,
    -217,   226,  -217,   185,  -217,  -217,  -217,   198,  -217,   210,
     186,   187,    36,   212,  -217,  -217,  -217,   197,  -217,  -217,
     221,  -217,  -217,  -217,  -217,   -24,   190,   195,  -217,  -217,
    -217,  -217,   234,   192,   179,    17,   196,  -217,  -217,    -2,
     201,   203,  -217,     7,  -217,    26,   237,   239,   206,   209,
     239,  -217,   211,  -217,  -217,  -217,    14,    16,  -217,   237,
     239,   213,   239,  -217,   239,   239,  -217,  -217,  -217,    19,
    -217,  -217,  -217,   239,   239,  -217,  -217,  -217
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    65,     1,     0,     0,     0,     0,    24,     0,     0,
       0,    25,    26,    27,    23,    22,     0,     3,     0,    21,
      20,    15,    16,    17,    18,    10,    11,    12,    13,    14,
       9,     6,     8,     7,     5,     0,    19,     0,     0,     0,
       0,     0,    30,    29,     0,     0,     0,     0,     4,     0,
       0,     0,     0,    28,    35,     0,    83,     0,     0,   110,
     111,   112,   113,    75,    66,     0,     0,     0,     0,     0,
      54,     0,    62,     0,     0,     0,     0,    67,     0,     0,
      50,    37,     0,     0,     0,     0,    52,    61,    58,    59,
       0,    60,     0,    85,     0,     0,    75,     0,    75,    75,
      80,    72,    71,    74,     0,     0,     0,    45,    46,    47,
      48,    49,    41,     0,     0,    56,    54,    51,     0,   102,
     103,   104,   105,   106,   107,     0,     0,   108,     0,     0,
       0,     0,    84,    83,     0,     0,    76,     0,    68,    69,
       0,     0,    83,     0,    75,    37,    36,     0,     0,    42,
      40,    33,     0,     0,     0,    52,     0,   109,     0,    87,
      65,    95,    91,     0,    93,    96,    92,    85,    63,   134,
      75,    75,     0,    80,     0,   114,    73,    70,    38,    44,
       0,    43,     0,     0,     0,    56,    55,    53,     0,     0,
      88,     0,    94,     0,    86,    77,    78,    75,    81,     0,
       0,   120,    41,    33,    31,    32,    57,     0,    89,    65,
       0,    97,   100,    98,    79,     0,     0,     0,    64,    39,
      34,    90,     0,     0,    85,   117,     0,   101,    99,    80,
       0,     0,   116,   127,    82,   117,   117,   127,     0,     0,
     127,   121,     0,   119,   115,   123,   127,   127,   122,   117,
     127,     0,   127,   128,   127,   127,   125,   118,   130,   127,
     129,   126,   124,   127,   127,   132,   133,   131
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,    49,  -217,  -217,   113,   154,    59,
    -217,  -217,  -217,  -217,   107,   152,    78,   -79,  -217,  -217,
    -129,  -217,  -217,   129,   -94,  -164,   -71,  -160,  -130,   -89,
     214,  -217,  -216,  -217,  -108,  -217
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,   183,    29,    30,   106,    81,   150,
     180,   112,    82,    31,   117,    86,   154,    92,    32,    33,
      34,    35,    65,   104,    77,   142,    72,   132,    93,   128,
      66,   201,   232,   218,   241,    36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     103,   167,   136,   130,   138,   139,   115,   194,    37,   198,
      38,   147,    75,   160,    42,   133,   140,   237,    98,   243,
     244,    99,   209,    76,   250,   238,   254,    87,    43,   263,
      75,   191,   238,   257,   238,   230,   239,   238,    88,    89,
      90,   135,    91,   251,   230,   148,   231,   149,    87,   162,
     177,   166,    40,   141,    41,   242,    39,    87,   103,    88,
      89,   161,   168,    91,   229,   234,   240,   189,    88,    89,
     210,   175,    91,   252,   185,   255,   195,   196,   264,     2,
     222,   158,   159,     3,     4,   224,   148,    44,   149,     5,
       6,     7,     8,     9,    10,   163,   164,    45,    11,    12,
      13,   170,    87,   214,   171,    46,    14,    15,    47,   118,
     211,   207,   208,    88,    89,   101,    16,    91,   102,    48,
     119,   120,   121,   122,   123,   124,    50,    49,    51,   245,
     125,    52,   248,   126,    59,    60,    61,    62,   253,   256,
      53,   127,   258,    57,   260,    54,   261,   262,    55,    56,
      58,   265,    96,    87,    67,   266,   267,   119,   120,   121,
     122,   123,   124,    68,    88,    89,   165,   125,    91,    69,
     129,   119,   120,   121,   122,   123,   124,    70,   127,    87,
      71,   125,    78,    73,   188,    59,    60,    61,    62,    80,
      88,    89,   127,    79,    91,   107,   108,   109,   110,   111,
      74,    83,    84,    63,    85,    94,    64,   100,    95,   105,
     113,   116,   114,   131,   134,   137,    75,   144,   143,   146,
     151,   152,   153,   156,   157,   169,   173,   174,   176,   179,
     182,   181,   184,   186,   190,   192,   193,   197,   199,   200,
     202,   204,   205,   212,   216,   203,   215,   217,   221,   213,
     223,   227,   220,   226,   225,   230,   228,   238,   178,   145,
     233,   219,   187,   206,     0,   235,   172,   236,   155,     0,
     246,     0,     0,   247,     0,   249,     0,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97
};

static const yytype_int16 yycheck[] =
{
      79,   131,    96,    92,    98,    99,    85,   167,     6,   173,
       8,    16,    18,    16,    64,    94,    18,    10,    64,   235,
     236,    67,    16,    29,    10,    18,    10,    51,     7,    10,
      18,   160,    18,   249,    18,    18,    29,    18,    62,    63,
      64,    29,    66,    29,    18,    50,    29,    52,    51,   128,
     144,   130,     6,    55,     8,    29,    54,    51,   137,    62,
      63,    64,   133,    66,   224,   229,    59,   156,    62,    63,
      64,   142,    66,    59,   153,    59,   170,   171,    59,     0,
     209,    50,    51,     4,     5,   215,    50,    30,    52,    10,
      11,    12,    13,    14,    15,    50,    51,    32,    19,    20,
      21,    64,    51,   197,    67,    64,    27,    28,    38,    29,
     189,    50,    51,    62,    63,    64,    37,    66,    67,     3,
      40,    41,    42,    43,    44,    45,    64,     9,    64,   237,
      50,     8,   240,    53,    46,    47,    48,    49,   246,   247,
      64,    61,   250,    35,   252,    64,   254,   255,    64,    64,
      39,   259,    64,    51,    16,   263,   264,    40,    41,    42,
      43,    44,    45,    36,    62,    63,    64,    50,    66,    64,
      53,    40,    41,    42,    43,    44,    45,    31,    61,    51,
      33,    50,    32,    64,    53,    46,    47,    48,    49,    64,
      62,    63,    61,    16,    66,    22,    23,    24,    25,    26,
      66,    64,    36,    64,    16,    40,    67,    64,    30,    18,
      16,    18,    64,    34,     6,    16,    18,    17,    29,    17,
      64,    16,    18,    64,    61,    64,    64,    56,    64,    62,
      18,    51,    64,    17,    51,    51,    29,    17,    64,    57,
      17,    17,    17,    17,    58,    64,    36,    60,    51,    64,
      29,    17,   203,    58,    64,    18,    64,    18,   145,   105,
      64,   202,   155,   185,    -1,    64,   137,    64,   116,    -1,
      64,    -1,    -1,    64,    -1,    64,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    70,     0,     4,     5,    10,    11,    12,    13,    14,
      15,    19,    20,    21,    27,    28,    37,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    84,
      85,    92,    97,    98,    99,   100,   114,     6,     8,    54,
       6,     8,    64,     7,    30,    32,    64,    38,     3,     9,
      64,    64,     8,    64,    64,    64,    64,    35,    39,    46,
      47,    48,    49,    64,    67,   101,   109,    16,    36,    64,
      31,    33,   105,    64,    66,    18,    29,   103,    32,    16,
      64,    87,    91,    64,    36,    16,    94,    51,    62,    63,
      64,    66,    96,   107,    40,    30,    64,   109,    64,    67,
      64,    64,    67,    96,   102,    18,    86,    22,    23,    24,
      25,    26,    90,    16,    64,    96,    18,    93,    29,    40,
      41,    42,    43,    44,    45,    50,    53,    61,   108,    53,
     108,    34,   106,    96,     6,    29,   103,    16,   103,   103,
      18,    55,   104,    29,    17,    87,    17,    16,    50,    52,
      88,    64,    16,    18,    95,    94,    64,    61,    50,    51,
      16,    64,    96,    50,    51,    64,    96,   107,   105,    64,
      64,    67,   102,    64,    56,   105,    64,   103,    86,    62,
      89,    51,    18,    83,    64,    96,    17,    93,    53,   108,
      51,    99,    51,    29,   106,   103,   103,    17,   104,    64,
      57,   110,    17,    64,    17,    17,    95,    50,    51,    16,
      64,    96,    17,    64,   103,    36,    58,    60,   112,    88,
      83,    51,    99,    29,   107,    64,    58,    17,    64,   106,
      18,    29,   111,    64,   104,    64,    64,    10,    18,    29,
      59,   113,    29,   111,   111,   113,    64,    64,   113,    64,
      10,    29,    59,   113,    10,    59,   113,   111,   113,    64,
     113,   113,   113,    10,    59,   113,   113,   113
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    70,    71,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    82,    83,    83,    84,    85,    86,    86,    87,
      87,    88,    88,    88,    89,    90,    90,    90,    90,    90,
      91,    92,    93,    93,    94,    94,    95,    95,    96,    96,
      96,    96,    97,    98,    99,   100,   101,   101,   101,   101,
     101,   102,   102,   102,   102,   103,   103,   103,   103,   103,
     104,   104,   104,   105,   105,   106,   106,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     109,   109,   109,   109,   110,   110,   110,   111,   111,   111,
     112,   112,   112,   112,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   113,   114
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       2,     9,     9,     0,     3,     3,     7,     0,     3,     6,
       3,     0,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     6,     0,     3,     0,     4,     0,     3,     1,     1,
       1,     1,     4,     7,     9,     0,     1,     2,     4,     4,
       5,     1,     1,     3,     1,     0,     3,     5,     5,     6,
       0,     3,     7,     0,     3,     0,     3,     3,     4,     5,
       6,     3,     3,     3,     4,     3,     3,     5,     5,     7,
       5,     7,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     0,     6,     4,     0,     5,     3,
       0,     4,     5,     5,     7,     6,     7,     0,     3,     4,
       4,     6,     5,     6,     7
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

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
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
      yychar = yylex (&yylval, scanner);
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 22:
#line 191 "yacc_sql.y"
          {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1651 "yacc_sql.tab.c"
    break;

  case 23:
#line 196 "yacc_sql.y"
          {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1659 "yacc_sql.tab.c"
    break;

  case 24:
#line 201 "yacc_sql.y"
          {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1667 "yacc_sql.tab.c"
    break;

  case 25:
#line 207 "yacc_sql.y"
               {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1675 "yacc_sql.tab.c"
    break;

  case 26:
#line 213 "yacc_sql.y"
                {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1683 "yacc_sql.tab.c"
    break;

  case 27:
#line 219 "yacc_sql.y"
                  {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1691 "yacc_sql.tab.c"
    break;

  case 28:
#line 225 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[0].string));
    }
#line 1700 "yacc_sql.tab.c"
    break;

  case 29:
#line 231 "yacc_sql.y"
                 {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1708 "yacc_sql.tab.c"
    break;

  case 30:
#line 237 "yacc_sql.y"
             {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[0].string));
    }
#line 1717 "yacc_sql.tab.c"
    break;

  case 31:
#line 245 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), false);
		}
#line 1726 "yacc_sql.tab.c"
    break;

  case 32:
#line 250 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-5].string), (yyvsp[-3].string), (yyvsp[-1].string), true);
    		}
#line 1735 "yacc_sql.tab.c"
    break;

  case 34:
#line 256 "yacc_sql.y"
                            {
		append_index(&CONTEXT->ssql->sstr.create_index, (yyvsp[-1].string));
	}
#line 1743 "yacc_sql.tab.c"
    break;

  case 35:
#line 262 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[0].string));
		}
#line 1752 "yacc_sql.tab.c"
    break;

  case 36:
#line 269 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-4].string));
			//临时变量清零
			CONTEXT->value_length = 0;
		}
#line 1763 "yacc_sql.tab.c"
    break;

  case 38:
#line 278 "yacc_sql.y"
                                   {    }
#line 1769 "yacc_sql.tab.c"
    break;

  case 39:
#line 283 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-4].number), (yyvsp[-2].number), (yyvsp[0].bools));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1780 "yacc_sql.tab.c"
    break;

  case 40:
#line 290 "yacc_sql.y"
                {
			AttrInfo attribute;
			// 从 1970-01-01 ~ 2038-03-01 有 24896 天，DATE 类型使用两个字节存储
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), (yyvsp[-1].number) == DATES ? 2 : 4, (yyvsp[0].bools));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1792 "yacc_sql.tab.c"
    break;

  case 41:
#line 300 "yacc_sql.y"
    { (yyval.bools) = false; }
#line 1798 "yacc_sql.tab.c"
    break;

  case 42:
#line 301 "yacc_sql.y"
               { (yyval.bools) = true; }
#line 1804 "yacc_sql.tab.c"
    break;

  case 43:
#line 302 "yacc_sql.y"
                   { (yyval.bools) = false; }
#line 1810 "yacc_sql.tab.c"
    break;

  case 44:
#line 306 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1816 "yacc_sql.tab.c"
    break;

  case 45:
#line 309 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1822 "yacc_sql.tab.c"
    break;

  case 46:
#line 310 "yacc_sql.y"
                { (yyval.number)=DATES; }
#line 1828 "yacc_sql.tab.c"
    break;

  case 47:
#line 311 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1834 "yacc_sql.tab.c"
    break;

  case 48:
#line 312 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1840 "yacc_sql.tab.c"
    break;

  case 49:
#line 313 "yacc_sql.y"
                { (yyval.number)=TEXTS; }
#line 1846 "yacc_sql.tab.c"
    break;

  case 50:
#line 317 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string);
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1855 "yacc_sql.tab.c"
    break;

  case 51:
#line 325 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-3].string), CONTEXT->tuples, CONTEXT->tuple_length);

      //临时变量清零
      CONTEXT->value_length=0;
      CONTEXT->tuple_length=0;
      memset(CONTEXT->tuples, 0, sizeof(CONTEXT->tuples));
    }
#line 1869 "yacc_sql.tab.c"
    break;

  case 53:
#line 336 "yacc_sql.y"
                             {
    }
#line 1876 "yacc_sql.tab.c"
    break;

  case 55:
#line 341 "yacc_sql.y"
                                     {
    	memcpy(CONTEXT->tuples[CONTEXT->tuple_length].values, CONTEXT->values, sizeof(Value)*CONTEXT->value_length);
    	CONTEXT->tuples[CONTEXT->tuple_length].value_num = CONTEXT->value_length;
    	CONTEXT->value_length = 0;
        CONTEXT->tuple_length++;
    }
#line 1887 "yacc_sql.tab.c"
    break;

  case 57:
#line 351 "yacc_sql.y"
                              { }
#line 1893 "yacc_sql.tab.c"
    break;

  case 58:
#line 354 "yacc_sql.y"
          {
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1901 "yacc_sql.tab.c"
    break;

  case 59:
#line 357 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
		}
#line 1909 "yacc_sql.tab.c"
    break;

  case 60:
#line 360 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1918 "yacc_sql.tab.c"
    break;

  case 61:
#line 364 "yacc_sql.y"
              {
    		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
    }
#line 1926 "yacc_sql.tab.c"
    break;

  case 62:
#line 371 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-1].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion,
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
    }
#line 1938 "yacc_sql.tab.c"
    break;

  case 63:
#line 381 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-5].string), (yyvsp[-3].string), value,
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->value_length = 0;
			CONTEXT->condition_length = 0;
		}
#line 1951 "yacc_sql.tab.c"
    break;

  case 64:
#line 392 "yacc_sql.y"
                {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));

			selects_append_conditions(&CONTEXT->ssql->sstr.selection,
				CONTEXT->conditions,
				CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1],
				CONTEXT->condition_length);
			CONTEXT->ssql->flag=SCF_SELECT;//"select";
	}
#line 1965 "yacc_sql.tab.c"
    break;

  case 65:
#line 403 "yacc_sql.y"
              {
	if (CONTEXT->last_condition_length_length != 0) {
		CONTEXT->queries[CONTEXT->query_length++] = CONTEXT->ssql;
		CONTEXT->ssql = query_create();
	}
	CONTEXT->last_condition_length[CONTEXT->last_condition_length_length++] = CONTEXT->condition_length;
}
#line 1977 "yacc_sql.tab.c"
    break;

  case 66:
#line 413 "yacc_sql.y"
         {
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1987 "yacc_sql.tab.c"
    break;

  case 67:
#line 418 "yacc_sql.y"
                   {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1997 "yacc_sql.tab.c"
    break;

  case 68:
#line 423 "yacc_sql.y"
                              {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2007 "yacc_sql.tab.c"
    break;

  case 69:
#line 428 "yacc_sql.y"
                                {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2017 "yacc_sql.tab.c"
    break;

  case 70:
#line 433 "yacc_sql.y"
                                                  {
	}
#line 2024 "yacc_sql.tab.c"
    break;

  case 71:
#line 437 "yacc_sql.y"
             {// *
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 2038 "yacc_sql.tab.c"
    break;

  case 72:
#line 446 "yacc_sql.y"
            {// age
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[0].string));
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 2052 "yacc_sql.tab.c"
    break;

  case 73:
#line 455 "yacc_sql.y"
                   {// age
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string));
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 2066 "yacc_sql.tab.c"
    break;

  case 74:
#line 464 "yacc_sql.y"
               { // 1
			Aggregation aggr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 1;
			aggr.value = CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 2079 "yacc_sql.tab.c"
    break;

  case 76:
#line 475 "yacc_sql.y"
                         {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
      }
#line 2089 "yacc_sql.tab.c"
    break;

  case 77:
#line 480 "yacc_sql.y"
                                {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
  	  }
#line 2099 "yacc_sql.tab.c"
    break;

  case 78:
#line 485 "yacc_sql.y"
                                      {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2109 "yacc_sql.tab.c"
    break;

  case 79:
#line 490 "yacc_sql.y"
                                                       {

	}
#line 2117 "yacc_sql.tab.c"
    break;

  case 81:
#line 497 "yacc_sql.y"
                            {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
	}
#line 2125 "yacc_sql.tab.c"
    break;

  case 82:
#line 501 "yacc_sql.y"
        {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));
	}
#line 2133 "yacc_sql.tab.c"
    break;

  case 84:
#line 507 "yacc_sql.y"
                                     {
			}
#line 2140 "yacc_sql.tab.c"
    break;

  case 86:
#line 512 "yacc_sql.y"
                                   {
			}
#line 2147 "yacc_sql.tab.c"
    break;

  case 87:
#line 517 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2160 "yacc_sql.tab.c"
    break;

  case 88:
#line 526 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-3].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2173 "yacc_sql.tab.c"
    break;

  case 89:
#line 535 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2186 "yacc_sql.tab.c"
    break;

  case 90:
#line 544 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-5].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2199 "yacc_sql.tab.c"
    break;

  case 91:
#line 553 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2215 "yacc_sql.tab.c"
    break;

  case 92:
#line 565 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2229 "yacc_sql.tab.c"
    break;

  case 93:
#line 575 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 0, NULL, left_value, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2242 "yacc_sql.tab.c"
    break;

  case 94:
#line 584 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 0, NULL, left_value, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2255 "yacc_sql.tab.c"
    break;

  case 95:
#line 593 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2270 "yacc_sql.tab.c"
    break;

  case 96:
#line 604 "yacc_sql.y"
                {
			fprintf(stderr, "Enter");
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
#line 2288 "yacc_sql.tab.c"
    break;

  case 97:
#line 618 "yacc_sql.y"
                {
			fprintf(stderr, "Enter");
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2304 "yacc_sql.tab.c"
    break;

  case 98:
#line 630 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2320 "yacc_sql.tab.c"
    break;

  case 99:
#line 642 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2335 "yacc_sql.tab.c"
    break;

  case 100:
#line 653 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-4].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, 0, &CONTEXT->ssql->sstr.selection, NULL, NULL);
			CONTEXT->ssql = CONTEXT->queries[CONTEXT->query_length - 1];
			CONTEXT->query_length--;
			CONTEXT->condition_length = CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1];
			CONTEXT->last_condition_length_length--;
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

                }
#line 2353 "yacc_sql.tab.c"
    break;

  case 101:
#line 667 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, 0, &CONTEXT->ssql->sstr.selection, NULL, NULL);
			CONTEXT->ssql = CONTEXT->queries[CONTEXT->query_length - 1];
			CONTEXT->query_length--;
			CONTEXT->condition_length = CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1];
			CONTEXT->last_condition_length_length--;
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2370 "yacc_sql.tab.c"
    break;

  case 102:
#line 682 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2376 "yacc_sql.tab.c"
    break;

  case 103:
#line 683 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2382 "yacc_sql.tab.c"
    break;

  case 104:
#line 684 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2388 "yacc_sql.tab.c"
    break;

  case 105:
#line 685 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2394 "yacc_sql.tab.c"
    break;

  case 106:
#line 686 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2400 "yacc_sql.tab.c"
    break;

  case 107:
#line 687 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2406 "yacc_sql.tab.c"
    break;

  case 108:
#line 688 "yacc_sql.y"
         { CONTEXT->comp = IN_COMP; }
#line 2412 "yacc_sql.tab.c"
    break;

  case 109:
#line 689 "yacc_sql.y"
             { CONTEXT->comp = NOT_IN_COMP; }
#line 2418 "yacc_sql.tab.c"
    break;

  case 110:
#line 693 "yacc_sql.y"
                 { CONTEXT->func = AGG_MAX; }
#line 2424 "yacc_sql.tab.c"
    break;

  case 111:
#line 694 "yacc_sql.y"
             { CONTEXT->func = AGG_MIN; }
#line 2430 "yacc_sql.tab.c"
    break;

  case 112:
#line 695 "yacc_sql.y"
               { CONTEXT->func = AGG_COUNT; }
#line 2436 "yacc_sql.tab.c"
    break;

  case 113:
#line 696 "yacc_sql.y"
             { CONTEXT->func = AGG_AVG; }
#line 2442 "yacc_sql.tab.c"
    break;

  case 115:
#line 700 "yacc_sql.y"
                                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2452 "yacc_sql.tab.c"
    break;

  case 116:
#line 705 "yacc_sql.y"
                             {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2462 "yacc_sql.tab.c"
    break;

  case 118:
#line 711 "yacc_sql.y"
                                  {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2472 "yacc_sql.tab.c"
    break;

  case 119:
#line 716 "yacc_sql.y"
                           {
		RelAttr attr;
		relation_attr_init(&attr, NULL,(yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2482 "yacc_sql.tab.c"
    break;

  case 121:
#line 723 "yacc_sql.y"
                              {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2495 "yacc_sql.tab.c"
    break;

  case 122:
#line 731 "yacc_sql.y"
                                  {
        	RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
        }
#line 2508 "yacc_sql.tab.c"
    break;

  case 123:
#line 739 "yacc_sql.y"
                                   {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2521 "yacc_sql.tab.c"
    break;

  case 124:
#line 747 "yacc_sql.y"
                                        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2534 "yacc_sql.tab.c"
    break;

  case 125:
#line 755 "yacc_sql.y"
                                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2547 "yacc_sql.tab.c"
    break;

  case 126:
#line 763 "yacc_sql.y"
                                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2560 "yacc_sql.tab.c"
    break;

  case 128:
#line 772 "yacc_sql.y"
                          {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2573 "yacc_sql.tab.c"
    break;

  case 129:
#line 780 "yacc_sql.y"
                              {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2586 "yacc_sql.tab.c"
    break;

  case 130:
#line 788 "yacc_sql.y"
                                {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2599 "yacc_sql.tab.c"
    break;

  case 131:
#line 796 "yacc_sql.y"
                                     {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2612 "yacc_sql.tab.c"
    break;

  case 132:
#line 804 "yacc_sql.y"
                                 {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2625 "yacc_sql.tab.c"
    break;

  case 133:
#line 812 "yacc_sql.y"
                                      {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2638 "yacc_sql.tab.c"
    break;

  case 134:
#line 822 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[0].string), (yyvsp[-3].string));
		}
#line 2647 "yacc_sql.tab.c"
    break;


#line 2651 "yacc_sql.tab.c"

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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (scanner, YY_("syntax error"));
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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
        yyerror (scanner, yymsgp);
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
                      yytoken, &yylval, scanner);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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
                  yystos[yystate], yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, scanner);
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
  return yyresult;
}
#line 827 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));

	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}
