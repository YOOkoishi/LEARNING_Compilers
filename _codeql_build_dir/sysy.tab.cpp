/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 8 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"


#include <iostream>
#include <memory>
#include <string>
#include "include/ast.h"




// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

extern int yylineno;
extern char* yytext;
extern FILE* yyin;

using namespace std;


#line 93 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"

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

#include "sysy.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_RETURN = 4,                     /* RETURN  */
  YYSYMBOL_IDENT = 5,                      /* IDENT  */
  YYSYMBOL_CONST = 6,                      /* CONST  */
  YYSYMBOL_INT_CONST = 7,                  /* INT_CONST  */
  YYSYMBOL_LE = 8,                         /* LE  */
  YYSYMBOL_GE = 9,                         /* GE  */
  YYSYMBOL_EQ = 10,                        /* EQ  */
  YYSYMBOL_NE = 11,                        /* NE  */
  YYSYMBOL_LT = 12,                        /* LT  */
  YYSYMBOL_GT = 13,                        /* GT  */
  YYSYMBOL_14_ = 14,                       /* '='  */
  YYSYMBOL_LOR = 15,                       /* LOR  */
  YYSYMBOL_LAND = 16,                      /* LAND  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_ELSE = 18,                      /* ELSE  */
  YYSYMBOL_WHILE = 19,                     /* WHILE  */
  YYSYMBOL_BREAK = 20,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 21,                  /* CONTINUE  */
  YYSYMBOL_22_ = 22,                       /* '+'  */
  YYSYMBOL_23_ = 23,                       /* '-'  */
  YYSYMBOL_24_ = 24,                       /* '*'  */
  YYSYMBOL_25_ = 25,                       /* '/'  */
  YYSYMBOL_26_ = 26,                       /* '%'  */
  YYSYMBOL_27_ = 27,                       /* '('  */
  YYSYMBOL_28_ = 28,                       /* ')'  */
  YYSYMBOL_29_ = 29,                       /* '{'  */
  YYSYMBOL_30_ = 30,                       /* '}'  */
  YYSYMBOL_31_ = 31,                       /* ';'  */
  YYSYMBOL_32_ = 32,                       /* ','  */
  YYSYMBOL_33_ = 33,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_CompUnit = 35,                  /* CompUnit  */
  YYSYMBOL_FuncDef = 36,                   /* FuncDef  */
  YYSYMBOL_FuncType = 37,                  /* FuncType  */
  YYSYMBOL_Block = 38,                     /* Block  */
  YYSYMBOL_BlockItems = 39,                /* BlockItems  */
  YYSYMBOL_BlockItem = 40,                 /* BlockItem  */
  YYSYMBOL_Decl = 41,                      /* Decl  */
  YYSYMBOL_VarDecl = 42,                   /* VarDecl  */
  YYSYMBOL_VarDefs = 43,                   /* VarDefs  */
  YYSYMBOL_VarDef = 44,                    /* VarDef  */
  YYSYMBOL_InitVal = 45,                   /* InitVal  */
  YYSYMBOL_ConstDecl = 46,                 /* ConstDecl  */
  YYSYMBOL_BType = 47,                     /* BType  */
  YYSYMBOL_ConstDefs = 48,                 /* ConstDefs  */
  YYSYMBOL_ConstDef = 49,                  /* ConstDef  */
  YYSYMBOL_ConstInitVal = 50,              /* ConstInitVal  */
  YYSYMBOL_ConstExp = 51,                  /* ConstExp  */
  YYSYMBOL_Stmt = 52,                      /* Stmt  */
  YYSYMBOL_Exp = 53,                       /* Exp  */
  YYSYMBOL_LOrExp = 54,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 55,                   /* LAndExp  */
  YYSYMBOL_EqExp = 56,                     /* EqExp  */
  YYSYMBOL_RelExp = 57,                    /* RelExp  */
  YYSYMBOL_AddExp = 58,                    /* AddExp  */
  YYSYMBOL_MulExp = 59,                    /* MulExp  */
  YYSYMBOL_PrimaryExp = 60,                /* PrimaryExp  */
  YYSYMBOL_UnaryExp = 61,                  /* UnaryExp  */
  YYSYMBOL_UnaryOp = 62,                   /* UnaryOp  */
  YYSYMBOL_Number = 63,                    /* Number  */
  YYSYMBOL_Lval = 64                       /* Lval  */
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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

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
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   143

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   275


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
       2,     2,     2,    33,     2,     2,     2,    26,     2,     2,
      27,    28,    24,    22,    32,    23,     2,    25,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    31,
       2,    14,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    15,
      16,    17,    18,    19,    20,    21
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    82,    82,    91,   102,   112,   117,   127,   133,   144,
     151,   163,   168,   178,   188,   193,   204,   209,   218,   231,
     241,   251,   257,   269,   281,   291,   302,   307,   311,   316,
     320,   325,   331,   337,   344,   350,   354,   364,   375,   380,
     392,   397,   409,   414,   421,   434,   439,   446,   453,   460,
     473,   478,   485,   498,   503,   510,   517,   530,   535,   540,
     551,   556,   568,   571,   574,   583,   592
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "RETURN",
  "IDENT", "CONST", "INT_CONST", "LE", "GE", "EQ", "NE", "LT", "GT", "'='",
  "LOR", "LAND", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'('", "')'", "'{'", "'}'", "';'", "','", "'!'",
  "$accept", "CompUnit", "FuncDef", "FuncType", "Block", "BlockItems",
  "BlockItem", "Decl", "VarDecl", "VarDefs", "VarDef", "InitVal",
  "ConstDecl", "BType", "ConstDefs", "ConstDef", "ConstInitVal",
  "ConstExp", "Stmt", "Exp", "LOrExp", "LAndExp", "EqExp", "RelExp",
  "AddExp", "MulExp", "PrimaryExp", "UnaryExp", "UnaryOp", "Number",
  "Lval", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-92)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      25,   -92,     2,   -92,    27,   -92,    19,    28,    37,     0,
     -92,   -92,   103,   -92,    67,   -92,    50,    51,    48,    49,
     -92,   -92,   110,   -92,   -92,   -92,   -92,    38,   -92,   -92,
     -92,   -92,    76,   -92,    52,    71,    66,    -1,    39,     3,
     -10,   -92,   -92,   110,   -92,    74,   -92,    59,   -92,    86,
     110,   110,   -92,   -92,    64,   -92,   -92,    79,    18,   -92,
     -92,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   -92,   110,   -92,    82,    22,   -92,
      70,    77,   -92,   110,   -92,    76,    66,    -1,    39,    39,
       3,     3,     3,     3,   -10,   -10,   -92,   -92,   -92,    73,
     110,   -92,    86,    80,    80,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,    94,   -92,    80,   -92
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     2,     0,     1,     0,     0,     0,     0,
       3,    20,     0,    66,     0,    65,     0,     0,     0,     0,
      62,    63,     0,     6,    29,    64,    30,     0,     7,     9,
      12,    11,     0,    10,     0,    37,    38,    40,    42,    45,
      50,    60,    53,     0,    58,    59,    27,     0,    59,     0,
       0,     0,    35,    36,     0,     5,     8,    16,     0,    14,
      28,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    26,     0,     0,    21,
       0,     0,    57,     0,    13,     0,    39,    41,    43,    44,
      48,    49,    46,    47,    51,    52,    54,    55,    56,     0,
       0,    19,     0,     0,     0,    17,    18,    15,    31,    23,
      24,    25,    22,    32,    34,     0,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -92,   -92,   -92,   -92,   106,   -92,    89,   -92,   -92,   -92,
      33,   -92,   -92,   105,   -92,    20,   -92,   -92,   -91,   -11,
     -92,    60,    58,   -29,     8,    -7,   -92,   -35,   -92,   -92,
      -9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,    26,    27,    28,    29,    30,    58,
      59,   105,    31,    32,    78,    79,   109,   110,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      45,    47,     5,    11,    12,    13,    14,    15,    74,    63,
      64,    54,   113,   114,    71,    72,    73,    16,    45,    17,
      18,    19,    20,    21,   116,    69,    70,    22,     1,     9,
      23,    24,     6,    25,    88,    89,    96,    97,    98,    80,
      81,    11,    12,    13,    14,    15,     7,    65,    66,    84,
      85,    67,    68,   101,   102,    16,     8,    17,    18,    19,
      20,    21,    94,    95,    99,    22,     9,     9,    55,    24,
      11,    25,   106,    90,    91,    92,    93,    50,    51,    52,
      53,    57,    62,    60,    12,    13,    61,    15,    75,   111,
      76,    77,    82,    83,    45,    45,   100,    16,   103,    17,
      18,    19,    20,    21,   108,   104,    45,    22,    13,     9,
      15,    24,   115,    25,    10,    13,    56,    15,   107,    49,
      87,    86,   112,     0,     0,    20,    21,     0,     0,     0,
      22,     0,    20,    21,    46,     0,    25,    22,     0,     0,
       0,     0,     0,    25
};

static const yytype_int8 yycheck[] =
{
       9,    12,     0,     3,     4,     5,     6,     7,    43,    10,
      11,    22,   103,   104,    24,    25,    26,    17,    27,    19,
      20,    21,    22,    23,   115,    22,    23,    27,     3,    29,
      30,    31,     5,    33,    63,    64,    71,    72,    73,    50,
      51,     3,     4,     5,     6,     7,    27,     8,     9,    31,
      32,    12,    13,    31,    32,    17,    28,    19,    20,    21,
      22,    23,    69,    70,    75,    27,    29,    29,    30,    31,
       3,    33,    83,    65,    66,    67,    68,    27,    27,    31,
      31,     5,    16,    31,     4,     5,    15,     7,    14,   100,
      31,     5,    28,    14,   103,   104,    14,    17,    28,    19,
      20,    21,    22,    23,    31,    28,   115,    27,     5,    29,
       7,    31,    18,    33,     8,     5,    27,     7,    85,    14,
      62,    61,   102,    -1,    -1,    22,    23,    -1,    -1,    -1,
      27,    -1,    22,    23,    31,    -1,    33,    27,    -1,    -1,
      -1,    -1,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    35,    36,    37,     0,     5,    27,    28,    29,
      38,     3,     4,     5,     6,     7,    17,    19,    20,    21,
      22,    23,    27,    30,    31,    33,    38,    39,    40,    41,
      42,    46,    47,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    31,    53,    64,    47,
      27,    27,    31,    31,    53,    30,    40,     5,    43,    44,
      31,    15,    16,    10,    11,     8,     9,    12,    13,    22,
      23,    24,    25,    26,    61,    14,    31,     5,    48,    49,
      53,    53,    28,    14,    31,    32,    55,    56,    57,    57,
      58,    58,    58,    58,    59,    59,    61,    61,    61,    53,
      14,    31,    32,    28,    28,    45,    53,    44,    31,    50,
      51,    53,    49,    52,    52,    18,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    36,    37,    38,    38,    39,    39,    40,
      40,    41,    41,    42,    43,    43,    44,    44,    45,    46,
      47,    48,    48,    49,    50,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    53,    54,    54,
      55,    55,    56,    56,    56,    57,    57,    57,    57,    57,
      58,    58,    58,    59,    59,    59,    59,    60,    60,    60,
      61,    61,    62,    62,    62,    63,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     1,     3,     2,     1,     2,     1,
       1,     1,     1,     3,     1,     3,     1,     3,     1,     4,
       1,     1,     3,     3,     1,     1,     3,     2,     2,     1,
       1,     4,     5,     7,     5,     2,     2,     1,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     1,
       1,     2,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (ast, YY_("syntax error: cannot back up")); \
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (ast);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, ast);
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
                 int yyrule, std::unique_ptr<BaseAST> &ast)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ast); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
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
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YY_USE (yyvaluep);
  YY_USE (ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (std::unique_ptr<BaseAST> &ast)
{
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

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
      yychar = yylex ();
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
  case 2: /* CompUnit: FuncDef  */
#line 82 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
            {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit -> fun_def = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast = move(comp_unit);
  }
#line 1501 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 3: /* FuncDef: FuncType IDENT '(' ')' Block  */
#line 91 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                                 {
    auto ast = new FunDefAST();
    ast -> fun_type = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast -> ident = *unique_ptr<string>((yyvsp[-3].str_val));
    ast -> block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1513 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 4: /* FuncType: INT  */
#line 102 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
        {
    auto ast = new FunTypeAST();
    ast -> type = "int";
    (yyval.ast_val) = ast;

  }
#line 1524 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 5: /* Block: '{' BlockItems '}'  */
#line 112 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                       {
    auto block = new BlockAST();
    block -> blockitems = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = block;
  }
#line 1534 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 6: /* Block: '{' '}'  */
#line 117 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
            {
    auto block = new BlockAST();
    (yyval.ast_val) = block;
  }
#line 1543 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 7: /* BlockItems: BlockItem  */
#line 127 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
              {
    auto blockitems = new BlockItemsAST();
    blockitems -> item.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = blockitems;    

  }
#line 1554 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 8: /* BlockItems: BlockItems BlockItem  */
#line 133 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                         {
    auto blockitems = dynamic_cast<BlockItemsAST*>((yyvsp[-1].ast_val));
    blockitems -> item.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = blockitems;
  }
#line 1564 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 9: /* BlockItem: Decl  */
#line 144 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
         {
    auto blockitem = new BlockItemAST();
    blockitem -> type = BlockItemAST::DECL;
    blockitem -> decl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = blockitem;

  }
#line 1576 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 10: /* BlockItem: Stmt  */
#line 151 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
         {
    auto blockitem = new BlockItemAST();
    blockitem -> type = BlockItemAST::STMT;
    blockitem -> stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = blockitem;
  }
#line 1587 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 11: /* Decl: ConstDecl  */
#line 163 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
              {
    auto decl = new DeclAST(DeclAST::CONST);
    decl -> constdecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = decl;
  }
#line 1597 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 12: /* Decl: VarDecl  */
#line 168 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
            {
    auto decl = new DeclAST(DeclAST::VAR);
    decl -> vardecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = decl;
  }
#line 1607 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 13: /* VarDecl: BType VarDefs ';'  */
#line 178 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                     {
    auto vardecl = new VarDeclAST();
    vardecl -> vardefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = vardecl;
  }
#line 1617 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 14: /* VarDefs: VarDef  */
#line 188 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto vardefs = new VarDefsAST();
    vardefs -> vardef.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = vardefs;
  }
#line 1627 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 15: /* VarDefs: VarDefs ',' VarDef  */
#line 193 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                       {
    auto vardefs = dynamic_cast<VarDefsAST*>((yyvsp[-2].ast_val));
    vardefs -> vardef.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = vardefs;
     
  }
#line 1638 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 16: /* VarDef: IDENT  */
#line 204 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
          {
    auto vardef = new VarDefAST(VarDefAST::IDENT);
    vardef -> ident = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = vardef;
  }
#line 1648 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 17: /* VarDef: IDENT '=' InitVal  */
#line 209 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                      {
    auto vardef = new VarDefAST(VarDefAST::IDENTDEF);
    vardef -> ident = *unique_ptr<string>((yyvsp[-2].str_val));
    vardef -> initval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = vardef;
  }
#line 1659 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 18: /* InitVal: Exp  */
#line 218 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
        {
    auto initval = new InitValAST();
    initval -> exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = initval;
  }
#line 1669 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 19: /* ConstDecl: CONST BType ConstDefs ';'  */
#line 231 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                              {
    auto constdecl = new ConstDeclAST();
    constdecl -> btype = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    constdecl -> constdefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = constdecl;
  }
#line 1680 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 20: /* BType: INT  */
#line 241 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
        {
    auto btype = new BTypeAST();
    btype -> val = "int";
    (yyval.ast_val) = btype;
  }
#line 1690 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 21: /* ConstDefs: ConstDef  */
#line 251 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
             {
    auto constdefs = new ConstDefsAST();
    constdefs -> constdef.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = constdefs;

  }
#line 1701 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 22: /* ConstDefs: ConstDefs ',' ConstDef  */
#line 257 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                           {
    auto constdefs = dynamic_cast<ConstDefsAST*>((yyvsp[-2].ast_val));
    constdefs -> constdef.push_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = constdefs;

  }
#line 1712 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 23: /* ConstDef: IDENT '=' ConstInitVal  */
#line 269 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                           {
  auto constdef = new ConstDefAST();
  constdef -> ident = *unique_ptr<string>((yyvsp[-2].str_val));
  constdef -> constinitval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
  (yyval.ast_val) = constdef;    

  }
#line 1724 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 24: /* ConstInitVal: ConstExp  */
#line 281 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
             {
    auto constinitval = new ConstInitValAST();
    constinitval -> constexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = constinitval ;
  }
#line 1734 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 25: /* ConstExp: Exp  */
#line 291 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
        {
    auto constexp = new ConstExpAST();
    constexp -> exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = constexp;
  }
#line 1744 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 26: /* Stmt: RETURN Exp ';'  */
#line 302 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                   {
    auto stmt = new StmtAST(StmtAST::RETURNEXP);
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1754 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 27: /* Stmt: RETURN ';'  */
#line 307 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
               {
    auto stmt = new StmtAST(StmtAST::RETURNNULL);
    (yyval.ast_val) = stmt;
  }
#line 1763 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 28: /* Stmt: Exp ';'  */
#line 311 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto stmt = new StmtAST(StmtAST::EXP);
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1773 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 29: /* Stmt: ';'  */
#line 316 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
        {
    auto stmt = new StmtAST(StmtAST::NUL);
    (yyval.ast_val) = stmt;
  }
#line 1782 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 30: /* Stmt: Block  */
#line 320 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
          {
    auto stmt = new StmtAST(StmtAST::BLOCK);
    stmt -> block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1792 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 31: /* Stmt: Lval '=' Exp ';'  */
#line 325 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                    {
    auto stmt = new StmtAST(StmtAST::LVALEXP);
    stmt -> lval = unique_ptr<BaseAST>((yyvsp[-3].ast_val));
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1803 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 32: /* Stmt: IF '(' Exp ')' Stmt  */
#line 331 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                        {
    auto stmt = new StmtAST(StmtAST::IF);
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    stmt -> if_stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1814 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 33: /* Stmt: IF '(' Exp ')' Stmt ELSE Stmt  */
#line 337 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                                 {
    auto stmt = new StmtAST(StmtAST::IFELSE);
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    stmt -> if_stmt = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    stmt -> else_stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1826 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 34: /* Stmt: WHILE '(' Exp ')' Stmt  */
#line 344 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                           {
    auto stmt = new StmtAST(StmtAST::WHILE);
    stmt -> exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    stmt -> while_stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = stmt;
  }
#line 1837 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 35: /* Stmt: BREAK ';'  */
#line 350 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
              {
    auto stmt = new StmtAST(StmtAST::BREAK);
    (yyval.ast_val) = stmt;
  }
#line 1846 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 36: /* Stmt: CONTINUE ';'  */
#line 354 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                 {
    auto stmt = new StmtAST(StmtAST::CONTINUE);
    (yyval.ast_val) = stmt;
  }
#line 1855 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 37: /* Exp: LOrExp  */
#line 364 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto exp = new ExpAST();
    exp -> lorexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = exp;
  }
#line 1865 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 38: /* LOrExp: LAndExp  */
#line 375 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
            {
    auto lor = new LOrExpAST(LOrExpAST::LANDEXP);
    lor -> landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = lor;
  }
#line 1875 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 39: /* LOrExp: LOrExp LOR LAndExp  */
#line 380 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                       {
    auto lor = new LOrExpAST(LOrExpAST::LORLAND);
    lor -> lorexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    lor -> landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = lor;
  }
#line 1886 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 40: /* LAndExp: EqExp  */
#line 392 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
          {
    auto land = new LAndExpAST(LAndExpAST::EQEXP);
    land -> eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = land;
  }
#line 1896 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 41: /* LAndExp: LAndExp LAND EqExp  */
#line 397 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                       {
    auto land = new LAndExpAST(LAndExpAST::LANDEQ);
    land -> landexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    land -> eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = land;
  }
#line 1907 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 42: /* EqExp: RelExp  */
#line 409 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto eq = new EqExpAST(EqExpAST::RELEXP);
    eq -> relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = eq;
  }
#line 1917 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 43: /* EqExp: EqExp EQ RelExp  */
#line 414 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                   {
    auto eq = new EqExpAST(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    eq -> operator_type = "==";
    eq -> relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = eq;
  }
#line 1929 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 44: /* EqExp: EqExp NE RelExp  */
#line 421 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                   {
    auto eq = new EqExpAST(EqExpAST::EQREL);
    eq -> eqexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    eq -> operator_type = "!=";
    eq -> relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = eq;
  }
#line 1941 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 45: /* RelExp: AddExp  */
#line 434 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto rel = new RelExpAST(RelExpAST::ADDEXP);
    rel -> addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = rel;
  }
#line 1951 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 46: /* RelExp: RelExp LT AddExp  */
#line 439 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                     {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    rel -> operator_type = "<";
    rel -> addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = rel;
  }
#line 1963 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 47: /* RelExp: RelExp GT AddExp  */
#line 446 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                     {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    rel -> operator_type = ">";
    rel -> addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = rel;
  }
#line 1975 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 48: /* RelExp: RelExp LE AddExp  */
#line 453 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                     {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    rel -> operator_type = "<=";
    rel -> addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = rel;
  }
#line 1987 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 49: /* RelExp: RelExp GE AddExp  */
#line 460 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                     {
    auto rel = new RelExpAST(RelExpAST::RELADD);
    rel -> relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    rel -> operator_type = ">=";
    rel -> addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = rel;
  }
#line 1999 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 50: /* AddExp: MulExp  */
#line 473 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto addexp = new AddExpAST(AddExpAST::MULONLY);
    addexp -> mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = addexp;
  }
#line 2009 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 51: /* AddExp: AddExp '+' MulExp  */
#line 478 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                      {
    auto addexp = new AddExpAST(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    addexp -> mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    addexp -> operator_type = "+";
    (yyval.ast_val) = addexp;
  }
#line 2021 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 52: /* AddExp: AddExp '-' MulExp  */
#line 485 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                      {
    auto addexp = new AddExpAST(AddExpAST::ADDOPMUL);
    addexp -> addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    addexp -> mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    addexp -> operator_type = "-";
    (yyval.ast_val) = addexp;
  }
#line 2033 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 53: /* MulExp: UnaryExp  */
#line 498 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
             {
    auto mulexp = new MulExpAST(MulExpAST::UNARYEXP);
    mulexp -> unrayexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = mulexp;
  }
#line 2043 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 54: /* MulExp: MulExp '*' UnaryExp  */
#line 503 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                        {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    mulexp -> unrayexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    mulexp -> operator_type = "*";
    (yyval.ast_val) = mulexp;
  }
#line 2055 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 55: /* MulExp: MulExp '/' UnaryExp  */
#line 510 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                        {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    mulexp -> unrayexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    mulexp -> operator_type = "/";
    (yyval.ast_val) = mulexp;
  }
#line 2067 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 56: /* MulExp: MulExp '%' UnaryExp  */
#line 517 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                        {
    auto mulexp = new MulExpAST(MulExpAST::MULOPUNRAY);
    mulexp -> mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    mulexp -> unrayexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    mulexp -> operator_type = "%";
    (yyval.ast_val) = mulexp;
  }
#line 2079 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 57: /* PrimaryExp: '(' Exp ')'  */
#line 530 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
               {
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::EXP);
    primaryexp -> exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = primaryexp;
  }
#line 2089 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 58: /* PrimaryExp: Number  */
#line 535 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
           {
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::NUMBER);
    primaryexp -> number = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = primaryexp;
  }
#line 2099 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 59: /* PrimaryExp: Lval  */
#line 540 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
         {
    auto primaryexp = new PrimaryExpAST(PrimaryExpAST::LVAL);
    primaryexp -> lval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = primaryexp;
  }
#line 2109 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 60: /* UnaryExp: PrimaryExp  */
#line 551 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
               {
    auto unaryexp = new UnaryExpAST(UnaryExpAST::PRIMARYEXP);
    unaryexp -> primaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = unaryexp;
  }
#line 2119 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 61: /* UnaryExp: UnaryOp UnaryExp  */
#line 556 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
                    {
    auto unaryexp = new UnaryExpAST(UnaryExpAST::UNARYEXP);
    unaryexp -> unaryop = *unique_ptr<std::string>((yyvsp[-1].str_val));
    unaryexp -> unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = unaryexp;
  }
#line 2130 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 62: /* UnaryOp: '+'  */
#line 568 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
       {
    (yyval.str_val) = new std::string("+");
  }
#line 2138 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 63: /* UnaryOp: '-'  */
#line 571 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
       {
    (yyval.str_val) = new std::string("-");
  }
#line 2146 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 64: /* UnaryOp: '!'  */
#line 574 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
       {
    (yyval.str_val) = new std::string("!");
  }
#line 2154 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 65: /* Number: INT_CONST  */
#line 583 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
              {
    auto int_con = new NumberAST();
    int_con -> int_const = ((yyvsp[0].int_val));
    (yyval.ast_val) = int_con;
  }
#line 2164 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;

  case 66: /* Lval: IDENT  */
#line 592 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"
          {
    auto lval = new LValAST();
    lval -> ident = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = lval;

  }
#line 2175 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"
    break;


#line 2179 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/_codeql_build_dir/sysy.tab.cpp"

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (ast, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
                      yytoken, &yylval, ast);
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
  ++yynerrs;

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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, ast);
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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (ast, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 605 "/home/runner/work/LEARNING_Compilers/LEARNING_Compilers/src/front/sysy.y"


// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
    // ✅ 输出详细错误信息
    std::cerr << "Error at line " << yylineno << ": " << s << std::endl;
    std::cerr << "Near token: '" << yytext << "'" << std::endl;
}
