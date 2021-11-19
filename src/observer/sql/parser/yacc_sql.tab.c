/* A Bison parser, made by GNU Bison 3.7.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

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
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  Value values[MAX_NUM];
  size_t tuple_length;
  InsertTuple tuples[MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
  char id[MAX_NUM];
  size_t func_length;
  FuncName func[MAX_NUM]; 
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
  context->from_length = 0;
  context->select_length = 0;
  context->value_length = 0;
  context->tuple_length = 0;
  memset(context->tuples, 0, sizeof(context->tuples));
  printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)


#line 133 "yacc_sql.tab.c"

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

#include "yacc_sql.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_SELECT = 9,                     /* SELECT  */
  YYSYMBOL_DESC = 10,                      /* DESC  */
  YYSYMBOL_SHOW = 11,                      /* SHOW  */
  YYSYMBOL_SYNC = 12,                      /* SYNC  */
  YYSYMBOL_INSERT = 13,                    /* INSERT  */
  YYSYMBOL_DELETE = 14,                    /* DELETE  */
  YYSYMBOL_UPDATE = 15,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_COMMA = 18,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 19,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 20,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 21,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 22,                     /* INT_T  */
  YYSYMBOL_DATE_T = 23,                    /* DATE_T  */
  YYSYMBOL_STRING_T = 24,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 25,                   /* FLOAT_T  */
  YYSYMBOL_TEXT_T = 26,                    /* TEXT_T  */
  YYSYMBOL_HELP = 27,                      /* HELP  */
  YYSYMBOL_EXIT = 28,                      /* EXIT  */
  YYSYMBOL_DOT = 29,                       /* DOT  */
  YYSYMBOL_INTO = 30,                      /* INTO  */
  YYSYMBOL_VALUES = 31,                    /* VALUES  */
  YYSYMBOL_FROM = 32,                      /* FROM  */
  YYSYMBOL_WHERE = 33,                     /* WHERE  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_SET = 35,                       /* SET  */
  YYSYMBOL_ON = 36,                        /* ON  */
  YYSYMBOL_LOAD = 37,                      /* LOAD  */
  YYSYMBOL_DATA = 38,                      /* DATA  */
  YYSYMBOL_INFILE = 39,                    /* INFILE  */
  YYSYMBOL_EQ = 40,                        /* EQ  */
  YYSYMBOL_LT = 41,                        /* LT  */
  YYSYMBOL_GT = 42,                        /* GT  */
  YYSYMBOL_LE = 43,                        /* LE  */
  YYSYMBOL_GE = 44,                        /* GE  */
  YYSYMBOL_NE = 45,                        /* NE  */
  YYSYMBOL_AGGMAX = 46,                    /* AGGMAX  */
  YYSYMBOL_AGGMIN = 47,                    /* AGGMIN  */
  YYSYMBOL_AGGCOUNT = 48,                  /* AGGCOUNT  */
  YYSYMBOL_AGGAVG = 49,                    /* AGGAVG  */
  YYSYMBOL_NOT = 50,                       /* NOT  */
  YYSYMBOL_NULL_TOK = 51,                  /* NULL_TOK  */
  YYSYMBOL_NULLABLE = 52,                  /* NULLABLE  */
  YYSYMBOL_IS = 53,                        /* IS  */
  YYSYMBOL_UNIQUE = 54,                    /* UNIQUE  */
  YYSYMBOL_INNER = 55,                     /* INNER  */
  YYSYMBOL_JOIN = 56,                      /* JOIN  */
  YYSYMBOL_GROUP = 57,                     /* GROUP  */
  YYSYMBOL_BY = 58,                        /* BY  */
  YYSYMBOL_ASC = 59,                       /* ASC  */
  YYSYMBOL_ORDER = 60,                     /* ORDER  */
  YYSYMBOL_NUMBER = 61,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 62,                     /* FLOAT  */
  YYSYMBOL_ID = 63,                        /* ID  */
  YYSYMBOL_PATH = 64,                      /* PATH  */
  YYSYMBOL_SSS = 65,                       /* SSS  */
  YYSYMBOL_STAR = 66,                      /* STAR  */
  YYSYMBOL_STRING_V = 67,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 68,                  /* $accept  */
  YYSYMBOL_commands = 69,                  /* commands  */
  YYSYMBOL_command = 70,                   /* command  */
  YYSYMBOL_exit = 71,                      /* exit  */
  YYSYMBOL_help = 72,                      /* help  */
  YYSYMBOL_sync = 73,                      /* sync  */
  YYSYMBOL_begin = 74,                     /* begin  */
  YYSYMBOL_commit = 75,                    /* commit  */
  YYSYMBOL_rollback = 76,                  /* rollback  */
  YYSYMBOL_drop_table = 77,                /* drop_table  */
  YYSYMBOL_show_tables = 78,               /* show_tables  */
  YYSYMBOL_desc_table = 79,                /* desc_table  */
  YYSYMBOL_create_index = 80,              /* create_index  */
  YYSYMBOL_col_list = 81,                  /* col_list  */
  YYSYMBOL_drop_index = 82,                /* drop_index  */
  YYSYMBOL_create_table = 83,              /* create_table  */
  YYSYMBOL_attr_def_list = 84,             /* attr_def_list  */
  YYSYMBOL_attr_def = 85,                  /* attr_def  */
  YYSYMBOL_nullable = 86,                  /* nullable  */
  YYSYMBOL_number = 87,                    /* number  */
  YYSYMBOL_type = 88,                      /* type  */
  YYSYMBOL_ID_get = 89,                    /* ID_get  */
  YYSYMBOL_insert = 90,                    /* insert  */
  YYSYMBOL_tuple_list = 91,                /* tuple_list  */
  YYSYMBOL_tuple = 92,                     /* tuple  */
  YYSYMBOL_value_list = 93,                /* value_list  */
  YYSYMBOL_value = 94,                     /* value  */
  YYSYMBOL_delete = 95,                    /* delete  */
  YYSYMBOL_update = 96,                    /* update  */
  YYSYMBOL_select = 97,                    /* select  */
  YYSYMBOL_select_attr = 98,               /* select_attr  */
  YYSYMBOL_expression = 99,                /* expression  */
  YYSYMBOL_attr_list = 100,                /* attr_list  */
  YYSYMBOL_rel_list = 101,                 /* rel_list  */
  YYSYMBOL_where = 102,                    /* where  */
  YYSYMBOL_condition_list = 103,           /* condition_list  */
  YYSYMBOL_condition = 104,                /* condition  */
  YYSYMBOL_comOp = 105,                    /* comOp  */
  YYSYMBOL_func = 106,                     /* func  */
  YYSYMBOL_groupbys = 107,                 /* groupbys  */
  YYSYMBOL_groupby = 108,                  /* groupby  */
  YYSYMBOL_orderbys = 109,                 /* orderbys  */
  YYSYMBOL_orderby = 110,                  /* orderby  */
  YYSYMBOL_load_data = 111                 /* load_data  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYLAST   280

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  272

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   322


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      65,    66,    67
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   158,   158,   160,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   184,   189,   194,   200,   206,   212,   218,   224,   230,
     237,   242,   248,   249,   254,   261,   269,   271,   275,   282,
     293,   294,   295,   299,   302,   303,   304,   305,   306,   309,
     317,   328,   329,   333,   334,   342,   344,   347,   350,   353,
     357,   363,   373,   383,   402,   407,   412,   417,   422,   426,
     435,   444,   453,   461,   463,   468,   473,   478,   483,   485,
     488,   493,   495,   498,   500,   504,   513,   522,   531,   540,
     551,   560,   568,   576,   596,   607,   617,   628,   642,   643,
     644,   645,   646,   647,   650,   651,   652,   653,   655,   656,
     661,   662,   668,   669,   677,   685,   693,   701,   709,   717,
     718,   726,   734,   742,   750,   758,   767
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "DATE_T", "STRING_T", "FLOAT_T",
  "TEXT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE",
  "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE",
  "GE", "NE", "AGGMAX", "AGGMIN", "AGGCOUNT", "AGGAVG", "NOT", "NULL_TOK",
  "NULLABLE", "IS", "UNIQUE", "INNER", "JOIN", "GROUP", "BY", "ASC",
  "ORDER", "NUMBER", "FLOAT", "ID", "PATH", "SSS", "STAR", "STRING_V",
  "$accept", "commands", "command", "exit", "help", "sync", "begin",
  "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "col_list", "drop_index", "create_table",
  "attr_def_list", "attr_def", "nullable", "number", "type", "ID_get",
  "insert", "tuple_list", "tuple", "value_list", "value", "delete",
  "update", "select", "select_attr", "expression", "attr_list", "rel_list",
  "where", "condition_list", "condition", "comOp", "func", "groupbys",
  "groupby", "orderbys", "orderby", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
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
     315,   316,   317,   318,   319,   320,   321,   322
};
#endif

#define YYPACT_NINF (-176)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -176,    66,  -176,     5,    34,   -25,   -43,    25,    45,    43,
      51,    33,    95,    96,    97,   103,   105,    50,  -176,  -176,
    -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,  -176,  -176,  -176,    49,    52,   108,    54,
      58,  -176,  -176,  -176,  -176,    17,  -176,    81,   118,   135,
     136,  -176,    77,    88,   117,  -176,  -176,  -176,  -176,  -176,
     114,   146,   128,   102,   163,   166,   -19,   -30,  -176,   107,
      67,  -176,  -176,   153,   159,   130,   129,   133,   134,   162,
    -176,  -176,    21,   179,   181,   181,    -8,  -176,  -176,  -176,
     171,  -176,  -176,  -176,   184,   186,    80,   200,   165,   174,
    -176,   188,   164,   191,   145,   -14,  -176,    67,  -176,  -176,
     147,   155,   159,   149,   181,   106,   195,    82,   132,   175,
    -176,   106,   208,   133,   198,  -176,  -176,  -176,  -176,  -176,
      15,   154,   202,   181,   181,   199,    -8,   156,   167,  -176,
    -176,   203,   186,   217,   160,  -176,  -176,  -176,  -176,  -176,
    -176,    10,    85,    18,    93,    80,  -176,   159,   168,   188,
     219,   169,   176,  -176,  -176,   207,   170,  -176,  -176,   181,
    -176,   190,   177,   172,   106,   211,   195,  -176,   138,   178,
    -176,  -176,  -176,   183,  -176,   209,  -176,   175,   233,   234,
    -176,  -176,  -176,   222,  -176,   180,   223,   224,  -176,    80,
     182,   189,   239,   203,  -176,  -176,    40,    98,  -176,  -176,
     185,  -176,  -176,  -176,    22,   207,   241,   243,   175,   220,
     187,  -176,  -176,   201,  -176,   225,  -176,  -176,  -176,  -176,
    -176,  -176,    -8,   192,    -4,  -176,   193,  -176,   235,   240,
     194,   196,   240,  -176,  -176,   197,  -176,  -176,    -3,    -2,
    -176,   232,   240,   204,   240,  -176,   240,   240,  -176,   205,
    -176,    -1,  -176,  -176,  -176,   235,   240,   240,  -176,  -176,
    -176,  -176
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,   104,   105,   106,   107,    73,    64,     0,     0,     0,
       0,    23,     0,     0,     0,    24,    25,    26,    22,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,    29,    28,     0,    81,     0,     0,     0,     0,     0,
      27,    34,    73,     0,    73,    73,    78,    60,    57,    58,
      70,    59,    69,    72,     0,    53,     0,     0,     0,     0,
      49,    36,     0,     0,     0,     0,    74,     0,    66,    67,
       0,     0,    81,     0,    73,     0,    51,     0,     0,    83,
      61,     0,     0,     0,     0,    44,    45,    46,    47,    48,
      40,     0,     0,    73,    73,     0,    78,     0,   108,    71,
      68,    55,    53,     0,     0,    98,    99,   100,   101,   102,
     103,     0,     0,     0,     0,     0,    82,    81,     0,    36,
       0,     0,     0,    41,    39,    32,     0,    75,    76,    73,
      79,     0,     0,   112,     0,     0,    51,    50,     0,     0,
      85,    93,    89,     0,    91,    94,    90,    83,     0,     0,
      37,    35,    43,     0,    42,     0,     0,     0,    77,     0,
       0,     0,     0,    55,    54,    52,     0,     0,    86,    92,
       0,    84,    62,   126,    40,    32,     0,     0,    83,     0,
       0,    63,    56,     0,    87,     0,    95,    96,    38,    33,
      30,    31,    78,     0,   119,    88,     0,    80,   110,   119,
       0,     0,   119,   113,    97,     0,   109,   115,   119,   119,
     114,     0,   119,     0,   119,   120,   119,   119,   117,     0,
     122,   119,   121,   118,   116,   110,   119,   119,   124,   111,
     125,   123
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,    36,  -176,  -176,   104,   139,    55,  -176,
    -176,  -176,  -176,    89,   122,    63,   -70,  -176,  -176,  -176,
    -176,   173,   -80,  -135,   -93,  -175,  -137,  -115,   206,  -176,
       6,  -176,  -147,  -176
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   196,    29,    30,   124,   101,   164,   193,
     130,   102,    31,   143,   116,   175,   118,    32,    33,    34,
      47,    94,    68,   112,    97,   156,   119,   152,    48,   173,
     246,   202,   243,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      93,   170,   106,   154,   108,   109,   239,   252,   256,   266,
     110,    36,   211,    37,   240,   240,   240,   240,   187,   138,
      49,    41,    42,    43,    44,   241,   253,    41,    42,    43,
      44,   161,    50,    84,   140,    66,    85,    93,    45,    66,
      39,    46,    40,   232,    82,   141,    67,   111,    51,   133,
     105,   157,   134,   167,   168,   242,   254,   257,   267,    38,
     179,   180,   218,   207,   188,   162,     2,   163,   183,   184,
       3,     4,   162,    52,   163,     5,     6,     7,     8,     9,
      10,    11,   182,    53,   186,    12,    13,    14,    60,   198,
     223,   224,   247,    15,    16,   250,    54,   237,    55,    56,
      57,   255,   258,    17,   203,   260,    58,   262,    59,   263,
     264,   144,    61,    69,   268,    62,    63,    64,    87,   270,
     271,    65,   145,   146,   147,   148,   149,   150,    88,    89,
      90,    87,    91,    92,    70,   151,    87,   226,    71,    72,
      73,    88,    89,   117,    87,    91,    88,    89,   181,    87,
      91,    74,    75,    76,    88,    89,   185,    87,    91,    88,
      89,   225,    77,    91,    78,    79,    80,    88,    89,    81,
      86,    91,   145,   146,   147,   148,   149,   150,   145,   146,
     147,   148,   149,   150,    95,   153,   125,   126,   127,   128,
     129,   206,    96,    98,    99,   107,   100,   103,   104,    66,
     113,   114,   115,   120,   122,   121,   123,   131,   132,   155,
     136,   137,   139,   142,   158,   160,   169,   165,   166,   171,
     177,   174,   191,   178,   172,   195,   199,   194,   204,   208,
     192,   189,   201,   197,   209,   200,   212,   213,   210,   214,
     216,   217,   221,   215,   230,   219,   231,   220,   227,   233,
     234,   229,   235,   245,   236,   238,   244,   248,   240,   249,
     251,   259,   159,   190,   176,   205,   222,   261,   265,   228,
       0,   269,    83,     0,     0,     0,     0,     0,     0,     0,
     135
};

static const yytype_int16 yycheck[] =
{
      70,   136,    82,   118,    84,    85,    10,    10,    10,    10,
      18,     6,   187,     8,    18,    18,    18,    18,   155,   112,
      63,    46,    47,    48,    49,    29,    29,    46,    47,    48,
      49,    16,     7,    63,   114,    18,    66,   107,    63,    18,
       6,    66,     8,   218,    63,   115,    29,    55,     3,    63,
      29,   121,    66,   133,   134,    59,    59,    59,    59,    54,
      50,    51,   199,   178,   157,    50,     0,    52,    50,    51,
       4,     5,    50,    30,    52,     9,    10,    11,    12,    13,
      14,    15,   152,    32,   154,    19,    20,    21,    38,   169,
      50,    51,   239,    27,    28,   242,    63,   232,     3,     3,
       3,   248,   249,    37,   174,   252,     3,   254,     3,   256,
     257,    29,    63,    32,   261,    63,     8,    63,    51,   266,
     267,    63,    40,    41,    42,    43,    44,    45,    61,    62,
      63,    51,    65,    66,    16,    53,    51,   207,     3,     3,
      63,    61,    62,    63,    51,    65,    61,    62,    63,    51,
      65,    63,    35,    39,    61,    62,    63,    51,    65,    61,
      62,    63,    16,    65,    36,    63,     3,    61,    62,     3,
      63,    65,    40,    41,    42,    43,    44,    45,    40,    41,
      42,    43,    44,    45,    31,    53,    22,    23,    24,    25,
      26,    53,    33,    63,    65,    16,    63,    63,    36,    18,
      29,    17,    16,     3,    30,    40,    18,    16,    63,    34,
      63,    56,    63,    18,     6,    17,    17,    63,    16,    63,
       3,    18,     3,    63,    57,    18,    36,    51,    17,    51,
      61,    63,    60,    63,    51,    58,     3,     3,    29,    17,
      17,    17,     3,    63,     3,    63,     3,    58,    63,    29,
      63,   215,    51,    18,    29,    63,    63,    63,    18,    63,
      63,    29,   123,   159,   142,   176,   203,    63,    63,   214,
      -1,   265,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    69,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    27,    28,    37,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    82,
      83,    90,    95,    96,    97,   111,     6,     8,    54,     6,
       8,    46,    47,    48,    49,    63,    66,    98,   106,    63,
       7,     3,    30,    32,    63,     3,     3,     3,     3,     3,
      38,    63,    63,     8,    63,    63,    18,    29,   100,    32,
      16,     3,     3,    63,    63,    35,    39,    16,    36,    63,
       3,     3,    63,   106,    63,    66,    63,    51,    61,    62,
      63,    65,    66,    94,    99,    31,    33,   102,    63,    65,
      63,    85,    89,    63,    36,    29,   100,    16,   100,   100,
      18,    55,   101,    29,    17,    16,    92,    63,    94,   104,
       3,    40,    30,    18,    84,    22,    23,    24,    25,    26,
      88,    16,    63,    63,    66,    99,    63,    56,   102,    63,
     100,    94,    18,    91,    29,    40,    41,    42,    43,    44,
      45,    53,   105,    53,   105,    34,   103,    94,     6,    85,
      17,    16,    50,    52,    86,    63,    16,   100,   100,    17,
     101,    63,    57,   107,    18,    93,    92,     3,    63,    50,
      51,    63,    94,    50,    51,    63,    94,   104,   102,    63,
      84,     3,    61,    87,    51,    18,    81,    63,   100,    36,
      58,    60,   109,    94,    17,    91,    53,   105,    51,    51,
      29,   103,     3,     3,    17,    63,    17,    17,   104,    63,
      58,     3,    93,    50,    51,    63,    94,    63,    86,    81,
       3,     3,   103,    29,    63,    51,    29,   101,    63,    10,
      18,    29,    59,   110,    63,    18,   108,   110,    63,    63,
     110,    63,    10,    29,    59,   110,    10,    59,   110,    29,
     110,    63,   110,   110,   110,    63,    10,    59,   110,   108,
     110,   110
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    80,    81,    81,    82,    83,    84,    84,    85,    85,
      86,    86,    86,    87,    88,    88,    88,    88,    88,    89,
      90,    91,    91,    92,    92,    93,    93,    94,    94,    94,
      94,    95,    96,    97,    98,    98,    98,    98,    98,    99,
      99,    99,    99,   100,   100,   100,   100,   100,   101,   101,
     101,   102,   102,   103,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   106,   106,   106,   106,   107,   107,
     108,   108,   109,   109,   109,   109,   109,   109,   109,   110,
     110,   110,   110,   110,   110,   110,   111
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
      10,    10,     0,     3,     4,     8,     0,     3,     6,     3,
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       7,     0,     3,     0,     4,     0,     3,     1,     1,     1,
       1,     5,     8,     9,     1,     2,     4,     4,     5,     1,
       1,     3,     1,     0,     3,     5,     5,     6,     0,     3,
       7,     0,     3,     0,     3,     3,     4,     5,     6,     3,
       3,     3,     4,     3,     3,     5,     5,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     6,
       0,     5,     0,     4,     5,     5,     7,     6,     7,     0,
       3,     4,     4,     6,     5,     6,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void *scanner)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

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
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 21: /* exit: EXIT SEMICOLON  */
#line 184 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1431 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 189 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1439 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 194 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1447 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 200 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1455 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 206 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1463 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 212 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1471 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 218 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1480 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 224 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1488 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 230 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1497 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE ID col_list RBRACE SEMICOLON  */
#line 238 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-7].string), (yyvsp[-5].string), (yyvsp[-3].string), false);
		}
#line 1506 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON  */
#line 243 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), true);
    		}
#line 1515 "yacc_sql.tab.c"
    break;

  case 33: /* col_list: COMMA ID col_list  */
#line 249 "yacc_sql.y"
                            {
		append_index(&CONTEXT->ssql->sstr.create_index, (yyvsp[-1].string));
	}
#line 1523 "yacc_sql.tab.c"
    break;

  case 34: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 255 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1532 "yacc_sql.tab.c"
    break;

  case 35: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 262 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1543 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 271 "yacc_sql.y"
                                   {    }
#line 1549 "yacc_sql.tab.c"
    break;

  case 38: /* attr_def: ID_get type LBRACE number RBRACE nullable  */
#line 276 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-4].number), (yyvsp[-2].number), (yyvsp[0].bools));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1560 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def: ID_get type nullable  */
#line 283 "yacc_sql.y"
                {
			AttrInfo attribute;
			// 从 1970-01-01 ~ 2038-03-01 有 24896 天，DATE 类型使用两个字节存储
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), (yyvsp[-1].number) == DATES ? 2 : 4, (yyvsp[0].bools));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
#line 1572 "yacc_sql.tab.c"
    break;

  case 40: /* nullable: %empty  */
#line 293 "yacc_sql.y"
    { (yyval.bools) = false; }
#line 1578 "yacc_sql.tab.c"
    break;

  case 41: /* nullable: NULLABLE  */
#line 294 "yacc_sql.y"
               { (yyval.bools) = true; }
#line 1584 "yacc_sql.tab.c"
    break;

  case 42: /* nullable: NOT NULL_TOK  */
#line 295 "yacc_sql.y"
                   { (yyval.bools) = false; }
#line 1590 "yacc_sql.tab.c"
    break;

  case 43: /* number: NUMBER  */
#line 299 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1596 "yacc_sql.tab.c"
    break;

  case 44: /* type: INT_T  */
#line 302 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1602 "yacc_sql.tab.c"
    break;

  case 45: /* type: DATE_T  */
#line 303 "yacc_sql.y"
                { (yyval.number)=DATES; }
#line 1608 "yacc_sql.tab.c"
    break;

  case 46: /* type: STRING_T  */
#line 304 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1614 "yacc_sql.tab.c"
    break;

  case 47: /* type: FLOAT_T  */
#line 305 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1620 "yacc_sql.tab.c"
    break;

  case 48: /* type: TEXT_T  */
#line 306 "yacc_sql.y"
                { (yyval.number)=TEXTS; }
#line 1626 "yacc_sql.tab.c"
    break;

  case 49: /* ID_get: ID  */
#line 310 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1635 "yacc_sql.tab.c"
    break;

  case 50: /* insert: INSERT INTO ID VALUES tuple tuple_list SEMICOLON  */
#line 318 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-4].string), CONTEXT->tuples, CONTEXT->tuple_length);

      //临时变量清零
      CONTEXT->value_length=0;
      CONTEXT->tuple_length=0;
      memset(CONTEXT->tuples, 0, sizeof(CONTEXT->tuples));
    }
#line 1649 "yacc_sql.tab.c"
    break;

  case 52: /* tuple_list: COMMA tuple tuple_list  */
#line 329 "yacc_sql.y"
                             {
    }
#line 1656 "yacc_sql.tab.c"
    break;

  case 54: /* tuple: LBRACE value value_list RBRACE  */
#line 334 "yacc_sql.y"
                                     {
    	memcpy(CONTEXT->tuples[CONTEXT->tuple_length].values, CONTEXT->values, sizeof(Value)*CONTEXT->value_length);
    	CONTEXT->tuples[CONTEXT->tuple_length].value_num = CONTEXT->value_length;
    	CONTEXT->value_length = 0;
        CONTEXT->tuple_length++;
    }
#line 1667 "yacc_sql.tab.c"
    break;

  case 56: /* value_list: COMMA value value_list  */
#line 344 "yacc_sql.y"
                              { }
#line 1673 "yacc_sql.tab.c"
    break;

  case 57: /* value: NUMBER  */
#line 347 "yacc_sql.y"
          {
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1681 "yacc_sql.tab.c"
    break;

  case 58: /* value: FLOAT  */
#line 350 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].floats));
		}
#line 1689 "yacc_sql.tab.c"
    break;

  case 59: /* value: SSS  */
#line 353 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1698 "yacc_sql.tab.c"
    break;

  case 60: /* value: NULL_TOK  */
#line 357 "yacc_sql.y"
              {
    		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
    }
#line 1706 "yacc_sql.tab.c"
    break;

  case 61: /* delete: DELETE FROM ID where SEMICOLON  */
#line 364 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1718 "yacc_sql.tab.c"
    break;

  case 62: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 374 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1730 "yacc_sql.tab.c"
    break;

  case 63: /* select: SELECT select_attr FROM ID rel_list where groupbys orderbys SEMICOLON  */
#line 384 "yacc_sql.y"
                {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-5].string));

			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);


			CONTEXT->ssql->flag=SCF_SELECT;//"select";

			//临时变量清零
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;
			CONTEXT->func_length=0;
	}
#line 1750 "yacc_sql.tab.c"
    break;

  case 64: /* select_attr: STAR  */
#line 402 "yacc_sql.y"
         {  
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1760 "yacc_sql.tab.c"
    break;

  case 65: /* select_attr: ID attr_list  */
#line 407 "yacc_sql.y"
                   {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1770 "yacc_sql.tab.c"
    break;

  case 66: /* select_attr: ID DOT ID attr_list  */
#line 412 "yacc_sql.y"
                              {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1780 "yacc_sql.tab.c"
    break;

  case 67: /* select_attr: ID DOT STAR attr_list  */
#line 417 "yacc_sql.y"
                                {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1790 "yacc_sql.tab.c"
    break;

  case 68: /* select_attr: func LBRACE expression RBRACE attr_list  */
#line 422 "yacc_sql.y"
                                                  {
	}
#line 1797 "yacc_sql.tab.c"
    break;

  case 69: /* expression: STAR  */
#line 426 "yacc_sql.y"
             {// *
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func[CONTEXT->func_length-1];
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 1811 "yacc_sql.tab.c"
    break;

  case 70: /* expression: ID  */
#line 435 "yacc_sql.y"
            {// age
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[0].string));
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func[CONTEXT->func_length-1];
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 1825 "yacc_sql.tab.c"
    break;

  case 71: /* expression: ID DOT ID  */
#line 444 "yacc_sql.y"
                   {// age
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-2].string), (yyvsp[0].string));
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func[CONTEXT->func_length-1];
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 1839 "yacc_sql.tab.c"
    break;

  case 72: /* expression: value  */
#line 453 "yacc_sql.y"
               { // 1
			Aggregation aggr;
			aggr.func_name = CONTEXT->func[CONTEXT->func_length-1];
			aggr.is_value = 1;
			aggr.value = CONTEXT->values[CONTEXT->value_length - 1];
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
#line 1851 "yacc_sql.tab.c"
    break;

  case 74: /* attr_list: COMMA ID attr_list  */
#line 463 "yacc_sql.y"
                         {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
      }
#line 1861 "yacc_sql.tab.c"
    break;

  case 75: /* attr_list: COMMA ID DOT ID attr_list  */
#line 468 "yacc_sql.y"
                                {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
  	  }
#line 1871 "yacc_sql.tab.c"
    break;

  case 76: /* attr_list: COMMA ID DOT STAR attr_list  */
#line 473 "yacc_sql.y"
                                      {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 1881 "yacc_sql.tab.c"
    break;

  case 77: /* attr_list: COMMA func LBRACE expression RBRACE attr_list  */
#line 478 "yacc_sql.y"
                                                       {

	}
#line 1889 "yacc_sql.tab.c"
    break;

  case 79: /* rel_list: COMMA ID rel_list  */
#line 485 "yacc_sql.y"
                            {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
	}
#line 1897 "yacc_sql.tab.c"
    break;

  case 80: /* rel_list: INNER JOIN ID ON condition condition_list rel_list  */
#line 489 "yacc_sql.y"
        {
		selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-4].string));
	}
#line 1905 "yacc_sql.tab.c"
    break;

  case 82: /* where: WHERE condition condition_list  */
#line 495 "yacc_sql.y"
                                     {	
			}
#line 1912 "yacc_sql.tab.c"
    break;

  case 84: /* condition_list: AND condition condition_list  */
#line 500 "yacc_sql.y"
                                   {
			}
#line 1919 "yacc_sql.tab.c"
    break;

  case 85: /* condition: ID IS NULL_TOK  */
#line 505 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 1932 "yacc_sql.tab.c"
    break;

  case 86: /* condition: ID IS NOT NULL_TOK  */
#line 514 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-3].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 1945 "yacc_sql.tab.c"
    break;

  case 87: /* condition: ID DOT ID IS NULL_TOK  */
#line 523 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 1958 "yacc_sql.tab.c"
    break;

  case 88: /* condition: ID DOT ID IS NOT NULL_TOK  */
#line 532 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-5].string));

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 1971 "yacc_sql.tab.c"
    break;

  case 89: /* condition: ID comOp value  */
#line 541 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 1986 "yacc_sql.tab.c"
    break;

  case 90: /* condition: value comOp value  */
#line 552 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 1999 "yacc_sql.tab.c"
    break;

  case 91: /* condition: value IS NULL_TOK  */
#line 561 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 0, NULL, left_value, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2011 "yacc_sql.tab.c"
    break;

  case 92: /* condition: value IS NOT NULL_TOK  */
#line 569 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 0, NULL, left_value, 0, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
#line 2023 "yacc_sql.tab.c"
    break;

  case 93: /* condition: ID comOp ID  */
#line 577 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;

		}
#line 2047 "yacc_sql.tab.c"
    break;

  case 94: /* condition: value comOp ID  */
#line 597 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
#line 2062 "yacc_sql.tab.c"
    break;

  case 95: /* condition: ID DOT ID comOp value  */
#line 608 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2076 "yacc_sql.tab.c"
    break;

  case 96: /* condition: value comOp ID DOT ID  */
#line 618 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2091 "yacc_sql.tab.c"
    break;

  case 97: /* condition: ID DOT ID comOp ID DOT ID  */
#line 629 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
#line 2106 "yacc_sql.tab.c"
    break;

  case 98: /* comOp: EQ  */
#line 642 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2112 "yacc_sql.tab.c"
    break;

  case 99: /* comOp: LT  */
#line 643 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2118 "yacc_sql.tab.c"
    break;

  case 100: /* comOp: GT  */
#line 644 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2124 "yacc_sql.tab.c"
    break;

  case 101: /* comOp: LE  */
#line 645 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2130 "yacc_sql.tab.c"
    break;

  case 102: /* comOp: GE  */
#line 646 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2136 "yacc_sql.tab.c"
    break;

  case 103: /* comOp: NE  */
#line 647 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2142 "yacc_sql.tab.c"
    break;

  case 104: /* func: AGGMAX  */
#line 650 "yacc_sql.y"
                 { CONTEXT->func[CONTEXT->func_length++] = AGG_MAX; }
#line 2148 "yacc_sql.tab.c"
    break;

  case 105: /* func: AGGMIN  */
#line 651 "yacc_sql.y"
             { CONTEXT->func[CONTEXT->func_length++] = AGG_MIN; }
#line 2154 "yacc_sql.tab.c"
    break;

  case 106: /* func: AGGCOUNT  */
#line 652 "yacc_sql.y"
               { CONTEXT->func[CONTEXT->func_length++] = AGG_COUNT; }
#line 2160 "yacc_sql.tab.c"
    break;

  case 107: /* func: AGGAVG  */
#line 653 "yacc_sql.y"
             { CONTEXT->func[CONTEXT->func_length++] = AGG_AVG; }
#line 2166 "yacc_sql.tab.c"
    break;

  case 109: /* groupbys: GROUP BY ID DOT ID groupby  */
#line 656 "yacc_sql.y"
                                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2176 "yacc_sql.tab.c"
    break;

  case 111: /* groupby: COMMA ID DOT ID groupby  */
#line 662 "yacc_sql.y"
                                  {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
#line 2186 "yacc_sql.tab.c"
    break;

  case 113: /* orderbys: ORDER BY ID orderby  */
#line 669 "yacc_sql.y"
                              {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2199 "yacc_sql.tab.c"
    break;

  case 114: /* orderbys: ORDER BY ID ASC orderby  */
#line 677 "yacc_sql.y"
                                  {
        	RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
        }
#line 2212 "yacc_sql.tab.c"
    break;

  case 115: /* orderbys: ORDER BY ID DESC orderby  */
#line 685 "yacc_sql.y"
                                   {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2225 "yacc_sql.tab.c"
    break;

  case 116: /* orderbys: ORDER BY ID DOT ID ASC orderby  */
#line 693 "yacc_sql.y"
                                        {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2238 "yacc_sql.tab.c"
    break;

  case 117: /* orderbys: ORDER BY ID DOT ID orderby  */
#line 701 "yacc_sql.y"
                                    {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2251 "yacc_sql.tab.c"
    break;

  case 118: /* orderbys: ORDER BY ID DOT ID DESC orderby  */
#line 709 "yacc_sql.y"
                                         {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2264 "yacc_sql.tab.c"
    break;

  case 120: /* orderby: COMMA ID orderby  */
#line 718 "yacc_sql.y"
                          {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2277 "yacc_sql.tab.c"
    break;

  case 121: /* orderby: COMMA ID ASC orderby  */
#line 726 "yacc_sql.y"
                              {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2290 "yacc_sql.tab.c"
    break;

  case 122: /* orderby: COMMA ID DESC orderby  */
#line 734 "yacc_sql.y"
                                {
		RelAttr attr;
		relation_attr_init(&attr, NULL, (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2303 "yacc_sql.tab.c"
    break;

  case 123: /* orderby: COMMA ID DOT ID ASC orderby  */
#line 742 "yacc_sql.y"
                                     {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2316 "yacc_sql.tab.c"
    break;

  case 124: /* orderby: COMMA ID DOT ID orderby  */
#line 750 "yacc_sql.y"
                                 {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2329 "yacc_sql.tab.c"
    break;

  case 125: /* orderby: COMMA ID DOT ID DESC orderby  */
#line 758 "yacc_sql.y"
                                      {
		RelAttr attr;
		relation_attr_init(&attr, (yyvsp[-4].string), (yyvsp[-2].string));

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
#line 2342 "yacc_sql.tab.c"
    break;

  case 126: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 768 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2351 "yacc_sql.tab.c"
    break;


#line 2355 "yacc_sql.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (scanner, YY_("syntax error"));
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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 773 "yacc_sql.y"

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
