/* A Bison parser, made by GNU Bison 3.7.5.  */

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "./parser/sysy.yy"

    #include <cmath>
    #include <cstring>
    #include <FlexLexer.h>
    #include "./AST/ast.hpp"
    #include "./Type/Type.hpp"
    extern char *yytext;
    extern yyFlexLexer *yyflexlexer;
    #define yylineno yyflexlexer->lineno()
    #define yylex() yyflexlexer->yylex()
    void yyerror(const char* fmt, ...);
    using namespace std;
    extern ASTBuilder ASTbuilder;

#line 86 "sysy.tab.cc"

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

#include "sysy.tab.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTCONST = 3,                   /* INTCONST  */
  YYSYMBOL_IDENT = 4,                      /* IDENT  */
  YYSYMBOL_FLOATCONST = 5,                 /* FLOATCONST  */
  YYSYMBOL_ADD = 6,                        /* ADD  */
  YYSYMBOL_SUB = 7,                        /* SUB  */
  YYSYMBOL_MUL = 8,                        /* MUL  */
  YYSYMBOL_DIV = 9,                        /* DIV  */
  YYSYMBOL_MOD = 10,                       /* MOD  */
  YYSYMBOL_LT = 11,                        /* LT  */
  YYSYMBOL_GT = 12,                        /* GT  */
  YYSYMBOL_LE = 13,                        /* LE  */
  YYSYMBOL_GE = 14,                        /* GE  */
  YYSYMBOL_EQ = 15,                        /* EQ  */
  YYSYMBOL_NE = 16,                        /* NE  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_NOT = 19,                       /* NOT  */
  YYSYMBOL_ASSIGN = 20,                    /* ASSIGN  */
  YYSYMBOL_LP = 21,                        /* LP  */
  YYSYMBOL_RP = 22,                        /* RP  */
  YYSYMBOL_LC = 23,                        /* LC  */
  YYSYMBOL_RC = 24,                        /* RC  */
  YYSYMBOL_LB = 25,                        /* LB  */
  YYSYMBOL_RB = 26,                        /* RB  */
  YYSYMBOL_COMMA = 27,                     /* COMMA  */
  YYSYMBOL_SEMI = 28,                      /* SEMI  */
  YYSYMBOL_IF = 29,                        /* IF  */
  YYSYMBOL_ELSE = 30,                      /* ELSE  */
  YYSYMBOL_WHILE = 31,                     /* WHILE  */
  YYSYMBOL_BREAK = 32,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 33,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 34,                    /* RETURN  */
  YYSYMBOL_CONST = 35,                     /* CONST  */
  YYSYMBOL_INT = 36,                       /* INT  */
  YYSYMBOL_VOID = 37,                      /* VOID  */
  YYSYMBOL_FLOAT = 38,                     /* FLOAT  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_Root = 40,                      /* Root  */
  YYSYMBOL_CompUnit = 41,                  /* CompUnit  */
  YYSYMBOL_Decl = 42,                      /* Decl  */
  YYSYMBOL_ConstDecl = 43,                 /* ConstDecl  */
  YYSYMBOL_VarDecl = 44,                   /* VarDecl  */
  YYSYMBOL_BType = 45,                     /* BType  */
  YYSYMBOL_ConstDef = 46,                  /* ConstDef  */
  YYSYMBOL_VarDef = 47,                    /* VarDef  */
  YYSYMBOL_Idents = 48,                    /* Idents  */
  YYSYMBOL_InitVals = 49,                  /* InitVals  */
  YYSYMBOL_InitVal = 50,                   /* InitVal  */
  YYSYMBOL_FuncDef = 51,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 52,               /* FuncFParams  */
  YYSYMBOL_FuncFParamArray = 53,           /* FuncFParamArray  */
  YYSYMBOL_FuncFParam = 54,                /* FuncFParam  */
  YYSYMBOL_BlockItems = 55,                /* BlockItems  */
  YYSYMBOL_Block = 56,                     /* Block  */
  YYSYMBOL_Stmt = 57,                      /* Stmt  */
  YYSYMBOL_Exp = 58,                       /* Exp  */
  YYSYMBOL_Cond = 59,                      /* Cond  */
  YYSYMBOL_LVal = 60,                      /* LVal  */
  YYSYMBOL_PrimaryExp = 61,                /* PrimaryExp  */
  YYSYMBOL_Number = 62,                    /* Number  */
  YYSYMBOL_UnaryExp = 63,                  /* UnaryExp  */
  YYSYMBOL_FuncCall = 64,                  /* FuncCall  */
  YYSYMBOL_FuncRParams = 65,               /* FuncRParams  */
  YYSYMBOL_MulExp = 66,                    /* MulExp  */
  YYSYMBOL_AddExp = 67,                    /* AddExp  */
  YYSYMBOL_RelExp = 68,                    /* RelExp  */
  YYSYMBOL_EqExp = 69,                     /* EqExp  */
  YYSYMBOL_LAndExp = 70,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 71                     /* LOrExp  */
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
typedef yytype_uint8 yy_state_t;

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  88
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  164

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


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
      35,    36,    37,    38
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    48,    48,    55,    58,    61,    64,    70,    73,    79,
      85,    94,   100,   109,   115,   125,   132,   135,   141,   146,
     167,   170,   177,   180,   183,   190,   195,   201,   206,   214,
     218,   225,   230,   243,   248,   258,   261,   264,   267,   273,
     276,   283,   286,   289,   292,   295,   298,   301,   304,   307,
     310,   313,   319,   325,   331,   335,   342,   345,   348,   354,
     360,   369,   372,   376,   380,   384,   390,   394,   401,   404,
     410,   413,   417,   421,   428,   431,   435,   442,   445,   449,
     453,   457,   463,   466,   470,   477,   480,   487,   490
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
  "\"end of file\"", "error", "\"invalid token\"", "INTCONST", "IDENT",
  "FLOATCONST", "ADD", "SUB", "MUL", "DIV", "MOD", "LT", "GT", "LE", "GE",
  "EQ", "NE", "AND", "OR", "NOT", "ASSIGN", "LP", "RP", "LC", "RC", "LB",
  "RB", "COMMA", "SEMI", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE",
  "RETURN", "CONST", "INT", "VOID", "FLOAT", "$accept", "Root", "CompUnit",
  "Decl", "ConstDecl", "VarDecl", "BType", "ConstDef", "VarDef", "Idents",
  "InitVals", "InitVal", "FuncDef", "FuncFParams", "FuncFParamArray",
  "FuncFParam", "BlockItems", "Block", "Stmt", "Exp", "Cond", "LVal",
  "PrimaryExp", "Number", "UnaryExp", "FuncCall", "FuncRParams", "MulExp",
  "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
#endif

#define YYPACT_NINF (-87)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      96,    99,   -87,    24,   -87,    13,    96,   -87,    31,   143,
      39,   -87,    42,    19,   -87,   -87,   -87,    42,   -87,    42,
     -87,    41,   -87,   -10,   -87,   -87,     9,    11,   -87,   -87,
      61,   191,   123,   191,    55,    85,     8,   -87,    55,    28,
     -87,    44,   -87,   123,   123,   123,   123,   185,   -87,   -87,
       7,   -87,   -87,   -87,   -87,    98,   193,    67,   -87,    81,
     -87,    73,    55,    99,   -87,    55,   158,   -87,   -87,   -87,
      79,   -87,   -15,   -87,   123,   123,   123,   123,   123,   123,
     -87,   -87,   -87,    90,    97,   111,   115,    20,   -87,    42,
     117,   -87,   -87,   119,    36,   128,   100,   -87,   -87,   -87,
     -87,   -87,    52,   -87,   -87,   191,   147,   -87,   -87,   -87,
      98,    98,   123,   123,   -87,   -87,   -87,   150,   -87,   -87,
     -87,   -87,   123,   -87,   123,   -87,   123,   -87,   -87,   161,
     193,   155,   186,   176,   157,   181,   -87,   177,   187,   -87,
     153,   123,   123,   123,   123,   123,   123,   123,   123,   153,
     -87,   -87,   189,   193,   193,   193,   193,   155,   155,   186,
     176,   -87,   153,   -87
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    13,     0,    14,     0,     2,     3,     0,     0,
       0,     4,     0,     0,     1,     5,     6,     0,     7,     0,
       8,    18,    11,    16,    18,     9,     0,     0,    10,    12,
       0,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      59,    54,    60,     0,     0,     0,     0,     0,    17,    22,
      57,    61,    58,    70,    65,    74,    52,     0,    15,     0,
      25,    33,     0,     0,    27,     0,     0,    62,    63,    64,
       0,    24,     0,    20,     0,     0,     0,     0,     0,     0,
      19,    40,    42,     0,     0,     0,     0,     0,    35,     0,
       0,    44,    36,     0,    57,     0,    34,    28,    30,    26,
      66,    68,     0,    56,    23,     0,     0,    71,    72,    73,
      75,    76,     0,     0,    48,    49,    50,     0,    39,    37,
      38,    43,     0,    31,     0,    67,     0,    21,    55,     0,
      77,    82,    85,    87,    53,     0,    51,     0,     0,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,    32,    45,    78,    79,    80,    81,    83,    84,    86,
      88,    47,     0,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -87,   -87,   -87,     1,   -87,   -87,     5,   190,   192,    65,
     -87,   -25,   214,   194,   -87,   159,   -87,   -20,   -86,   -30,
     108,   -59,   -87,   -87,   -24,   -87,   -87,   137,   -75,    72,
      76,    77,   -87
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,     9,    35,    25,    22,    23,
      72,    48,    11,    36,    96,    37,    90,    91,    92,    93,
     129,    50,    51,    52,    53,    54,   102,    55,    56,   131,
     132,   133,   134
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      94,    49,    57,    49,   120,    10,    12,    15,    58,   104,
      31,    10,   105,    14,    60,    32,    70,    49,    64,    67,
      68,    69,    73,    40,    41,    42,    43,    44,    13,    33,
      62,    94,    74,    34,    32,    63,   101,   130,   130,    45,
      27,    46,    97,    21,   106,    99,    24,     2,   116,     4,
      65,   107,   108,   109,   152,    63,   122,   117,    17,    18,
      88,    74,    30,   161,    89,    66,   153,   154,   155,   156,
     130,   130,   130,   130,   125,    49,   163,    26,    59,   126,
     127,    94,    26,    38,    40,    41,    42,    43,    44,    61,
      94,   119,   137,    80,   138,    89,   139,     2,    95,     4,
      45,   103,    46,    94,    59,    81,    75,    76,    77,    82,
      83,   112,    84,    85,    86,    87,     1,     2,   113,     4,
      40,    41,    42,    43,    44,   124,    40,    41,    42,    43,
      44,     1,     2,     3,     4,     2,    45,     4,    46,   114,
      59,   118,    45,   115,    46,    82,    83,   121,    84,    85,
      86,    87,     1,     2,   123,     4,    40,    41,    42,    43,
      44,    40,    41,    42,    43,    44,   141,   142,   143,   144,
      19,    20,    45,   128,    46,   148,    59,    45,   136,    46,
     100,    82,    83,   140,    84,    85,    86,    87,    40,    41,
      42,    43,    44,   147,    40,    41,    42,    43,    44,    78,
      79,   145,   146,   149,    45,   150,    46,    28,    47,    71,
      45,    29,    46,   151,    47,   110,   111,   157,   158,   162,
      16,   135,    98,   159,    39,   160
};

static const yytype_uint8 yycheck[] =
{
      59,    31,    32,    33,    90,     0,     1,     6,    33,    24,
      20,     6,    27,     0,    34,    25,    46,    47,    38,    43,
      44,    45,    47,     3,     4,     5,     6,     7,     4,    20,
      22,    90,    25,    22,    25,    27,    66,   112,   113,    19,
      21,    21,    62,     4,    74,    65,     4,    36,    28,    38,
      22,    75,    76,    77,   140,    27,    20,    87,    27,    28,
      59,    25,    21,   149,    59,    21,   141,   142,   143,   144,
     145,   146,   147,   148,    22,   105,   162,    12,    23,    27,
     105,   140,    17,    22,     3,     4,     5,     6,     7,     4,
     149,    90,   122,    26,   124,    90,   126,    36,    25,    38,
      19,    22,    21,   162,    23,    24,     8,     9,    10,    28,
      29,    21,    31,    32,    33,    34,    35,    36,    21,    38,
       3,     4,     5,     6,     7,    25,     3,     4,     5,     6,
       7,    35,    36,    37,    38,    36,    19,    38,    21,    28,
      23,    24,    19,    28,    21,    28,    29,    28,    31,    32,
      33,    34,    35,    36,    26,    38,     3,     4,     5,     6,
       7,     3,     4,     5,     6,     7,    11,    12,    13,    14,
      27,    28,    19,    26,    21,    18,    23,    19,    28,    21,
      22,    28,    29,    22,    31,    32,    33,    34,     3,     4,
       5,     6,     7,    17,     3,     4,     5,     6,     7,     6,
       7,    15,    16,    22,    19,    28,    21,    17,    23,    24,
      19,    19,    21,    26,    23,    78,    79,   145,   146,    30,
       6,   113,    63,   147,    30,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    35,    36,    37,    38,    40,    41,    42,    43,    44,
      45,    51,    45,     4,     0,    42,    51,    27,    28,    27,
      28,     4,    47,    48,     4,    46,    48,    21,    46,    47,
      21,    20,    25,    20,    22,    45,    52,    54,    22,    52,
       3,     4,     5,     6,     7,    19,    21,    23,    50,    58,
      60,    61,    62,    63,    64,    66,    67,    58,    50,    23,
      56,     4,    22,    27,    56,    22,    21,    63,    63,    63,
      58,    24,    49,    50,    25,     8,     9,    10,     6,     7,
      26,    24,    28,    29,    31,    32,    33,    34,    42,    45,
      55,    56,    57,    58,    60,    25,    53,    56,    54,    56,
      22,    58,    65,    22,    24,    27,    58,    63,    63,    63,
      66,    66,    21,    21,    28,    28,    28,    58,    24,    42,
      57,    28,    20,    26,    25,    22,    27,    50,    26,    59,
      67,    68,    69,    70,    71,    59,    28,    58,    58,    58,
      22,    11,    12,    13,    14,    15,    16,    17,    18,    22,
      28,    26,    57,    67,    67,    67,    67,    68,    68,    69,
      70,    57,    30,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    41,    41,    41,    42,    42,    43,
      43,    44,    44,    45,    45,    46,    47,    47,    48,    48,
      49,    49,    50,    50,    50,    51,    51,    51,    51,    52,
      52,    53,    53,    54,    54,    55,    55,    55,    55,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    58,    59,    60,    60,    61,    61,    61,    62,
      62,    63,    63,    63,    63,    63,    64,    64,    65,    65,
      66,    66,    66,    66,    67,    67,    67,    68,    68,    68,
      68,    68,    69,    69,    69,    70,    70,    71,    71
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     2,     3,     1,     1,     3,     1,     3,     1,     4,
       1,     3,     1,     3,     2,     5,     6,     5,     6,     1,
       3,     2,     4,     2,     3,     1,     1,     2,     2,     3,
       2,     4,     1,     2,     1,     5,     7,     5,     2,     2,
       2,     3,     1,     1,     1,     4,     3,     1,     1,     1,
       1,     1,     2,     2,     2,     1,     3,     4,     1,     3,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     1,     3
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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
  YYLTYPE *yylloc;
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Root: CompUnit  */
#line 48 "./parser/sysy.yy"
               {
    (yyval.ptr)=mknode(Root,(yyvsp[0].ptr),NULL,NULL,yylineno);
    ASTbuilder.setroot((yyval.ptr));
}
#line 1646 "sysy.tab.cc"
    break;

  case 3: /* CompUnit: Decl  */
#line 55 "./parser/sysy.yy"
               {
    (yyval.ptr)=mknode(CompUnit,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 1654 "sysy.tab.cc"
    break;

  case 4: /* CompUnit: FuncDef  */
#line 58 "./parser/sysy.yy"
              {
        (yyval.ptr)=mknode(CompUnit,(yyvsp[0].ptr),NULL,NULL,yylineno);
    }
#line 1662 "sysy.tab.cc"
    break;

  case 5: /* CompUnit: CompUnit Decl  */
#line 61 "./parser/sysy.yy"
                    {
        (yyval.ptr)=mknode(CompUnit,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1670 "sysy.tab.cc"
    break;

  case 6: /* CompUnit: CompUnit FuncDef  */
#line 64 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(CompUnit,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1678 "sysy.tab.cc"
    break;

  case 7: /* Decl: ConstDecl SEMI  */
#line 70 "./parser/sysy.yy"
                     {
    (yyval.ptr)=mknode(Decl,(yyvsp[-1].ptr),NULL,NULL,yylineno);
}
#line 1686 "sysy.tab.cc"
    break;

  case 8: /* Decl: VarDecl SEMI  */
#line 73 "./parser/sysy.yy"
                   {
        (yyval.ptr)=mknode(Decl,(yyvsp[-1].ptr),NULL,NULL,yylineno);
    }
#line 1694 "sysy.tab.cc"
    break;

  case 9: /* ConstDecl: CONST BType ConstDef  */
#line 79 "./parser/sysy.yy"
                                {
    (yyval.ptr)=mknode(ConstDecl,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    (yyval.ptr)->type=(yyvsp[-1].ptr)->type;
    (yyval.ptr)->pretype=(yyvsp[-1].ptr)->pretype;
    (yyvsp[0].ptr)->pretype=(yyval.ptr)->pretype;
}
#line 1705 "sysy.tab.cc"
    break;

  case 10: /* ConstDecl: ConstDecl COMMA ConstDef  */
#line 85 "./parser/sysy.yy"
                               {
        (yyval.ptr)=mknode(ConstDecl,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        (yyval.ptr)->type=(yyvsp[-2].ptr)->type;
        (yyval.ptr)->pretype=(yyvsp[-2].ptr)->pretype;
        (yyvsp[0].ptr)->pretype=(yyval.ptr)->pretype;
    }
#line 1716 "sysy.tab.cc"
    break;

  case 11: /* VarDecl: BType VarDef  */
#line 94 "./parser/sysy.yy"
                      {
    (yyval.ptr)=mknode(VarDecl,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    (yyval.ptr)->type=(yyvsp[-1].ptr)->type;
    (yyval.ptr)->pretype=(yyvsp[-1].ptr)->pretype;
    (yyvsp[0].ptr)->pretype=(yyval.ptr)->pretype;
}
#line 1727 "sysy.tab.cc"
    break;

  case 12: /* VarDecl: VarDecl COMMA VarDef  */
#line 100 "./parser/sysy.yy"
                           {
        (yyval.ptr)=mknode(VarDecl,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        (yyval.ptr)->type=(yyvsp[-2].ptr)->type;
        (yyval.ptr)->pretype=(yyvsp[-2].ptr)->pretype;
        (yyvsp[0].ptr)->pretype=(yyval.ptr)->pretype;
    }
#line 1738 "sysy.tab.cc"
    break;

  case 13: /* BType: INT  */
#line 109 "./parser/sysy.yy"
           {
    (yyval.ptr)=mknode(BType,NULL,NULL,NULL,yylineno);
    (yyval.ptr)->type=INT;
    (yyval.ptr)->pretype=new BasicType("int");
    (yyval.ptr)->width=4;
    }
#line 1749 "sysy.tab.cc"
    break;

  case 14: /* BType: FLOAT  */
#line 115 "./parser/sysy.yy"
            {
        (yyval.ptr)=mknode(BType,NULL,NULL,NULL,yylineno);
        (yyval.ptr)->type=FLOAT;
        (yyval.ptr)->pretype=new BasicType("float");
        (yyval.ptr)->width=4;
    }
#line 1760 "sysy.tab.cc"
    break;

  case 15: /* ConstDef: Idents ASSIGN InitVal  */
#line 125 "./parser/sysy.yy"
                                {
    (yyval.ptr)=mknode(ConstDef,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    
}
#line 1769 "sysy.tab.cc"
    break;

  case 16: /* VarDef: Idents  */
#line 132 "./parser/sysy.yy"
               {
    (yyval.ptr)=mknode(VarDef,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 1777 "sysy.tab.cc"
    break;

  case 17: /* VarDef: Idents ASSIGN InitVal  */
#line 135 "./parser/sysy.yy"
                            {
        (yyval.ptr)=mknode(VarDef,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1785 "sysy.tab.cc"
    break;

  case 18: /* Idents: IDENT  */
#line 141 "./parser/sysy.yy"
              {
    (yyval.ptr)=mknode(Idents,NULL,NULL,NULL,yylineno);
    strcpy((yyval.ptr)->type_id,(yyvsp[0].type_id));
    (yyval.ptr)->pretype=new BasicType("unknown");
}
#line 1795 "sysy.tab.cc"
    break;

  case 19: /* Idents: Idents LB Exp RB  */
#line 146 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(Idents,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-3].ptr)->type_id);
        if((yyvsp[-1].ptr)->kind==Number && (yyvsp[-1].ptr)->type==INT){
            (yyval.ptr)->pretype=((yyvsp[-3].ptr)->pretype->is_Array_Type())
                ?new Array_Type((yyvsp[-1].ptr)->type_int,*(static_cast<Array_Type*>((yyvsp[-3].ptr)->pretype)))
                :new Array_Type((yyvsp[-1].ptr)->type_int,*(static_cast<BasicType*>((yyvsp[-3].ptr)->pretype)));
        }
        else if((yyvsp[-1].ptr)->kind==Number && (yyvsp[-1].ptr)->type==FLOAT){//数组形参中有浮点数
            yyerror("size of array has non-integral type");
            exit(100);
        }
        else{
            (yyval.ptr)->pretype=((yyvsp[-3].ptr)->pretype->is_Array_Type())
                ?new Array_Type(-1,*(static_cast<Array_Type*>((yyvsp[-3].ptr)->pretype)))
                :new Array_Type(-1,*(static_cast<BasicType*>((yyvsp[-3].ptr)->pretype)));
        }
    }
#line 1818 "sysy.tab.cc"
    break;

  case 20: /* InitVals: InitVal  */
#line 167 "./parser/sysy.yy"
                  {
    (yyval.ptr)=mknode(InitVals,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 1826 "sysy.tab.cc"
    break;

  case 21: /* InitVals: InitVals COMMA InitVal  */
#line 170 "./parser/sysy.yy"
                             {
        (yyval.ptr)=mknode(InitVals,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1834 "sysy.tab.cc"
    break;

  case 22: /* InitVal: Exp  */
#line 177 "./parser/sysy.yy"
             {
    (yyval.ptr)=mknode(InitVal,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 1842 "sysy.tab.cc"
    break;

  case 23: /* InitVal: LC InitVals RC  */
#line 180 "./parser/sysy.yy"
                     {
        (yyval.ptr)=mknode(InitVal,(yyvsp[-1].ptr),NULL,NULL,yylineno);
    }
#line 1850 "sysy.tab.cc"
    break;

  case 24: /* InitVal: LC RC  */
#line 183 "./parser/sysy.yy"
            {
        (yyval.ptr)=mknode(InitVal,NULL,NULL,NULL,yylineno);
        (yyval.ptr)->pretype=new BasicType("void");
    }
#line 1859 "sysy.tab.cc"
    break;

  case 25: /* FuncDef: VOID IDENT LP RP Block  */
#line 190 "./parser/sysy.yy"
                                {
    (yyval.ptr)=mknode(FuncDef,NULL,NULL,(yyvsp[0].ptr),yylineno);
    strcpy((yyval.ptr)->type_id,(yyvsp[-3].type_id));
    (yyval.ptr)->pretype=new BasicType("void");
}
#line 1869 "sysy.tab.cc"
    break;

  case 26: /* FuncDef: BType IDENT LP FuncFParams RP Block  */
#line 195 "./parser/sysy.yy"
                                          {
        (yyval.ptr)=mknode(FuncDef,(yyvsp[-5].ptr),(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-4].type_id));
        (yyval.ptr)->pretype=(yyvsp[-5].ptr)->pretype;
        
    }
#line 1880 "sysy.tab.cc"
    break;

  case 27: /* FuncDef: BType IDENT LP RP Block  */
#line 201 "./parser/sysy.yy"
                              {
        (yyval.ptr)=mknode(FuncDef,(yyvsp[-4].ptr),NULL,(yyvsp[0].ptr),yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-3].type_id));
        (yyval.ptr)->pretype=(yyvsp[-4].ptr)->pretype;
    }
#line 1890 "sysy.tab.cc"
    break;

  case 28: /* FuncDef: VOID IDENT LP FuncFParams RP Block  */
#line 206 "./parser/sysy.yy"
                                         {
        (yyval.ptr)=mknode(FuncDef,NULL,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-4].type_id));
        (yyval.ptr)->pretype=new BasicType("void");
    }
#line 1900 "sysy.tab.cc"
    break;

  case 29: /* FuncFParams: FuncFParam  */
#line 214 "./parser/sysy.yy"
                        {
    (yyval.ptr)=mknode(FuncFParams,(yyvsp[0].ptr),NULL,NULL,yylineno);
    (yyval.ptr)->pretype=(yyvsp[0].ptr)->pretype;
}
#line 1909 "sysy.tab.cc"
    break;

  case 30: /* FuncFParams: FuncFParams COMMA FuncFParam  */
#line 218 "./parser/sysy.yy"
                                   {
        (yyval.ptr)=mknode(FuncFParams,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        (yyval.ptr)->pretype=new Product_Type((yyvsp[-2].ptr)->pretype,(yyvsp[0].ptr)->pretype);
    }
#line 1918 "sysy.tab.cc"
    break;

  case 31: /* FuncFParamArray: LB RB  */
#line 225 "./parser/sysy.yy"
                       {
    (yyval.ptr)=mknode(FuncFParamArray,NULL,NULL,NULL,yylineno);
    (yyval.ptr)->pretype=new Array_Type(0,*(new BasicType("unknown")));

}
#line 1928 "sysy.tab.cc"
    break;

  case 32: /* FuncFParamArray: FuncFParamArray LB Exp RB  */
#line 230 "./parser/sysy.yy"
                                {
        (yyval.ptr)=mknode(FuncFParamArray,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);
        if((yyvsp[-1].ptr)->kind==Number && (yyvsp[-1].ptr)->type==INT)
            (yyval.ptr)->pretype=new Array_Type((yyvsp[-1].ptr)->type_int,*(static_cast<Array_Type*>((yyvsp[-3].ptr)->pretype)));
        else if((yyvsp[-1].ptr)->kind==Number && (yyvsp[-1].ptr)->type==FLOAT){
            yyerror("size of array has non-integral type");//数组形参中有浮点数
            exit(100);
        }
        else
            (yyval.ptr)->pretype=new Array_Type(-1,*(static_cast<Array_Type*>((yyvsp[-3].ptr)->pretype)));
    }
#line 1944 "sysy.tab.cc"
    break;

  case 33: /* FuncFParam: BType IDENT  */
#line 243 "./parser/sysy.yy"
                        {
    (yyval.ptr)=mknode(FuncFParam,(yyvsp[-1].ptr),NULL,NULL,yylineno);
    strcpy((yyval.ptr)->type_id,(yyvsp[0].type_id));
    (yyval.ptr)->pretype=(yyvsp[-1].ptr)->pretype;
}
#line 1954 "sysy.tab.cc"
    break;

  case 34: /* FuncFParam: BType IDENT FuncFParamArray  */
#line 248 "./parser/sysy.yy"
                                  {
        (yyval.ptr)=mknode(FuncFParam,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));
        // static_cast<Array_Type*>($3->pretype)->basictype=*(static_cast<BasicType*>($1->pretype));
        static_cast<Array_Type*>((yyvsp[0].ptr)->pretype)->setBasicType(*(static_cast<BasicType*>((yyvsp[-2].ptr)->pretype)));
        (yyval.ptr)->pretype=(yyvsp[0].ptr)->pretype;
    }
#line 1966 "sysy.tab.cc"
    break;

  case 35: /* BlockItems: Decl  */
#line 258 "./parser/sysy.yy"
                 {
    (yyval.ptr)=mknode(BlockItems,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 1974 "sysy.tab.cc"
    break;

  case 36: /* BlockItems: Stmt  */
#line 261 "./parser/sysy.yy"
           {
        (yyval.ptr)=mknode(BlockItems,(yyvsp[0].ptr),NULL,NULL,yylineno);
    }
#line 1982 "sysy.tab.cc"
    break;

  case 37: /* BlockItems: BlockItems Decl  */
#line 264 "./parser/sysy.yy"
                      {
        (yyval.ptr)=mknode(BlockItems,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1990 "sysy.tab.cc"
    break;

  case 38: /* BlockItems: BlockItems Stmt  */
#line 267 "./parser/sysy.yy"
                      {
        (yyval.ptr)=mknode(BlockItems,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 1998 "sysy.tab.cc"
    break;

  case 39: /* Block: LC BlockItems RC  */
#line 273 "./parser/sysy.yy"
                        {
    (yyval.ptr)=mknode(Block,(yyvsp[-1].ptr),NULL,NULL,yylineno);
}
#line 2006 "sysy.tab.cc"
    break;

  case 40: /* Block: LC RC  */
#line 276 "./parser/sysy.yy"
            {
        (yyval.ptr)=mknode(Block,NULL,NULL,NULL,yylineno);
        (yyval.ptr)->pretype=new BasicType("void");
    }
#line 2015 "sysy.tab.cc"
    break;

  case 41: /* Stmt: LVal ASSIGN Exp SEMI  */
#line 283 "./parser/sysy.yy"
                           {
    (yyval.ptr)=mknode(ASSIGN,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);
}
#line 2023 "sysy.tab.cc"
    break;

  case 42: /* Stmt: SEMI  */
#line 286 "./parser/sysy.yy"
           {
        (yyval.ptr)=NULL;
    }
#line 2031 "sysy.tab.cc"
    break;

  case 43: /* Stmt: Exp SEMI  */
#line 289 "./parser/sysy.yy"
               {
        (yyval.ptr)=(yyvsp[-1].ptr);
    }
#line 2039 "sysy.tab.cc"
    break;

  case 44: /* Stmt: Block  */
#line 292 "./parser/sysy.yy"
            {
        (yyval.ptr)=(yyvsp[0].ptr);
    }
#line 2047 "sysy.tab.cc"
    break;

  case 45: /* Stmt: IF LP Cond RP Stmt  */
#line 295 "./parser/sysy.yy"
                         {
        (yyval.ptr)=mknode(IF,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 2055 "sysy.tab.cc"
    break;

  case 46: /* Stmt: IF LP Cond RP Stmt ELSE Stmt  */
#line 298 "./parser/sysy.yy"
                                   {
        (yyval.ptr)=mknode(IF,(yyvsp[-4].ptr),(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);
    }
#line 2063 "sysy.tab.cc"
    break;

  case 47: /* Stmt: WHILE LP Cond RP Stmt  */
#line 301 "./parser/sysy.yy"
                            {
        (yyval.ptr)=mknode(WHILE,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 2071 "sysy.tab.cc"
    break;

  case 48: /* Stmt: BREAK SEMI  */
#line 304 "./parser/sysy.yy"
                 {
        (yyval.ptr)=mknode(BREAK,NULL,NULL,NULL,yylineno);
    }
#line 2079 "sysy.tab.cc"
    break;

  case 49: /* Stmt: CONTINUE SEMI  */
#line 307 "./parser/sysy.yy"
                    {
        (yyval.ptr)=mknode(CONTINUE,NULL,NULL,NULL,yylineno);
    }
#line 2087 "sysy.tab.cc"
    break;

  case 50: /* Stmt: RETURN SEMI  */
#line 310 "./parser/sysy.yy"
                  {
        (yyval.ptr)=mknode(RETURN,NULL,NULL,NULL,yylineno);
    }
#line 2095 "sysy.tab.cc"
    break;

  case 51: /* Stmt: RETURN Exp SEMI  */
#line 313 "./parser/sysy.yy"
                      {
        (yyval.ptr)=mknode(RETURN,(yyvsp[-1].ptr),NULL,NULL,yylineno);
    }
#line 2103 "sysy.tab.cc"
    break;

  case 52: /* Exp: AddExp  */
#line 319 "./parser/sysy.yy"
            {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2111 "sysy.tab.cc"
    break;

  case 53: /* Cond: LOrExp  */
#line 325 "./parser/sysy.yy"
             {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2119 "sysy.tab.cc"
    break;

  case 54: /* LVal: IDENT  */
#line 331 "./parser/sysy.yy"
            {
    (yyval.ptr)=mknode(LVal,NULL,NULL,NULL,yylineno);
    strcpy((yyval.ptr)->type_id,(yyvsp[0].type_id));
}
#line 2128 "sysy.tab.cc"
    break;

  case 55: /* LVal: LVal LB Exp RB  */
#line 335 "./parser/sysy.yy"
                     {
        (yyval.ptr)=mknode(LVal,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-3].ptr)->type_id);
    }
#line 2137 "sysy.tab.cc"
    break;

  case 56: /* PrimaryExp: LP Exp RP  */
#line 342 "./parser/sysy.yy"
                      {
    (yyval.ptr)=(yyvsp[-1].ptr);
}
#line 2145 "sysy.tab.cc"
    break;

  case 57: /* PrimaryExp: LVal  */
#line 345 "./parser/sysy.yy"
           {
        (yyval.ptr)=(yyvsp[0].ptr);
    }
#line 2153 "sysy.tab.cc"
    break;

  case 58: /* PrimaryExp: Number  */
#line 348 "./parser/sysy.yy"
             {
        (yyval.ptr)=(yyvsp[0].ptr);
    }
#line 2161 "sysy.tab.cc"
    break;

  case 59: /* Number: INTCONST  */
#line 354 "./parser/sysy.yy"
                 {
    (yyval.ptr)=mknode(Number,NULL,NULL,NULL,yylineno);
    (yyval.ptr)->type_int=(yyvsp[0].type_int);
    (yyval.ptr)->type=INT;
    (yyval.ptr)->pretype=new BasicType("int");
}
#line 2172 "sysy.tab.cc"
    break;

  case 60: /* Number: FLOATCONST  */
#line 360 "./parser/sysy.yy"
                 {
        (yyval.ptr)=mknode(Number,NULL,NULL,NULL,yylineno);
        (yyval.ptr)->type_float=(yyvsp[0].type_float);
        (yyval.ptr)->type=FLOAT;
        (yyval.ptr)->pretype=new BasicType("float");
    }
#line 2183 "sysy.tab.cc"
    break;

  case 61: /* UnaryExp: PrimaryExp  */
#line 369 "./parser/sysy.yy"
                     {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2191 "sysy.tab.cc"
    break;

  case 62: /* UnaryExp: ADD UnaryExp  */
#line 372 "./parser/sysy.yy"
                   {
        (yyval.ptr)=mknode(UnaryExp,(yyvsp[0].ptr),NULL,NULL,yylineno);
        strcpy((yyval.ptr)->op,"+");
    }
#line 2200 "sysy.tab.cc"
    break;

  case 63: /* UnaryExp: SUB UnaryExp  */
#line 376 "./parser/sysy.yy"
                   {
        (yyval.ptr)=mknode(UnaryExp,(yyvsp[0].ptr),NULL,NULL,yylineno);
        strcpy((yyval.ptr)->op,"-");
    }
#line 2209 "sysy.tab.cc"
    break;

  case 64: /* UnaryExp: NOT UnaryExp  */
#line 380 "./parser/sysy.yy"
                   {
        (yyval.ptr)=mknode(UnaryExp,(yyvsp[0].ptr),NULL,NULL,yylineno);
        strcpy((yyval.ptr)->op,"!");
    }
#line 2218 "sysy.tab.cc"
    break;

  case 65: /* UnaryExp: FuncCall  */
#line 384 "./parser/sysy.yy"
               {
        (yyval.ptr)=(yyvsp[0].ptr);
    }
#line 2226 "sysy.tab.cc"
    break;

  case 66: /* FuncCall: IDENT LP RP  */
#line 390 "./parser/sysy.yy"
                      {
    (yyval.ptr)=mknode(FuncCall,NULL,NULL,NULL,yylineno);
    strcpy((yyval.ptr)->type_id,(yyvsp[-2].type_id));
}
#line 2235 "sysy.tab.cc"
    break;

  case 67: /* FuncCall: IDENT LP FuncRParams RP  */
#line 394 "./parser/sysy.yy"
                              {
        (yyval.ptr)=mknode(FuncCall,(yyvsp[-1].ptr),NULL,NULL,yylineno);
        strcpy((yyval.ptr)->type_id,(yyvsp[-3].type_id));
    }
#line 2244 "sysy.tab.cc"
    break;

  case 68: /* FuncRParams: Exp  */
#line 401 "./parser/sysy.yy"
                 {
    (yyval.ptr)=mknode(FuncRParams,(yyvsp[0].ptr),NULL,NULL,yylineno);
}
#line 2252 "sysy.tab.cc"
    break;

  case 69: /* FuncRParams: FuncRParams COMMA Exp  */
#line 404 "./parser/sysy.yy"
                            {
        (yyval.ptr)=mknode(FuncRParams,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
    }
#line 2260 "sysy.tab.cc"
    break;

  case 70: /* MulExp: UnaryExp  */
#line 410 "./parser/sysy.yy"
                 {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2268 "sysy.tab.cc"
    break;

  case 71: /* MulExp: MulExp MUL UnaryExp  */
#line 413 "./parser/sysy.yy"
                          {
        (yyval.ptr)=mknode(MulExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"*");
    }
#line 2277 "sysy.tab.cc"
    break;

  case 72: /* MulExp: MulExp DIV UnaryExp  */
#line 417 "./parser/sysy.yy"
                          {
        (yyval.ptr)=mknode(MulExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"/");
    }
#line 2286 "sysy.tab.cc"
    break;

  case 73: /* MulExp: MulExp MOD UnaryExp  */
#line 421 "./parser/sysy.yy"
                          {
        (yyval.ptr)=mknode(MulExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"%");
    }
#line 2295 "sysy.tab.cc"
    break;

  case 74: /* AddExp: MulExp  */
#line 428 "./parser/sysy.yy"
               {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2303 "sysy.tab.cc"
    break;

  case 75: /* AddExp: AddExp ADD MulExp  */
#line 431 "./parser/sysy.yy"
                        {
        (yyval.ptr)=mknode(AddExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"+");
    }
#line 2312 "sysy.tab.cc"
    break;

  case 76: /* AddExp: AddExp SUB MulExp  */
#line 435 "./parser/sysy.yy"
                        {
        (yyval.ptr)=mknode(AddExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"-");
    }
#line 2321 "sysy.tab.cc"
    break;

  case 77: /* RelExp: AddExp  */
#line 442 "./parser/sysy.yy"
               {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2329 "sysy.tab.cc"
    break;

  case 78: /* RelExp: RelExp LT AddExp  */
#line 445 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(RelExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"<");
    }
#line 2338 "sysy.tab.cc"
    break;

  case 79: /* RelExp: RelExp GT AddExp  */
#line 449 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(RelExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,">");
    }
#line 2347 "sysy.tab.cc"
    break;

  case 80: /* RelExp: RelExp LE AddExp  */
#line 453 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(RelExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"<=");
    }
#line 2356 "sysy.tab.cc"
    break;

  case 81: /* RelExp: RelExp GE AddExp  */
#line 457 "./parser/sysy.yy"
                       {
        (yyval.ptr)=mknode(RelExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,">=");
    }
#line 2365 "sysy.tab.cc"
    break;

  case 82: /* EqExp: RelExp  */
#line 463 "./parser/sysy.yy"
              {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2373 "sysy.tab.cc"
    break;

  case 83: /* EqExp: EqExp EQ RelExp  */
#line 466 "./parser/sysy.yy"
                      {
        (yyval.ptr)=mknode(EqExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"==");
    }
#line 2382 "sysy.tab.cc"
    break;

  case 84: /* EqExp: EqExp NE RelExp  */
#line 470 "./parser/sysy.yy"
                      {
        (yyval.ptr)=mknode(EqExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"!=");
    }
#line 2391 "sysy.tab.cc"
    break;

  case 85: /* LAndExp: EqExp  */
#line 477 "./parser/sysy.yy"
               {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2399 "sysy.tab.cc"
    break;

  case 86: /* LAndExp: LAndExp AND EqExp  */
#line 480 "./parser/sysy.yy"
                        {
        (yyval.ptr)=mknode(LAndExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"&&");
    }
#line 2408 "sysy.tab.cc"
    break;

  case 87: /* LOrExp: LAndExp  */
#line 487 "./parser/sysy.yy"
                {
    (yyval.ptr)=(yyvsp[0].ptr);
}
#line 2416 "sysy.tab.cc"
    break;

  case 88: /* LOrExp: LOrExp OR LAndExp  */
#line 490 "./parser/sysy.yy"
                        {
        (yyval.ptr)=mknode(LOrExp,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);
        strcpy((yyval.ptr)->op,"||");
    }
#line 2425 "sysy.tab.cc"
    break;


#line 2429 "sysy.tab.cc"

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
  *++yylsp = yyloc;

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
          = {yyssp, yytoken, &yylloc};
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
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
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

#line 497 "./parser/sysy.yy"


