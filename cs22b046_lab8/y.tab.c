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
#line 1 "prob.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
typedef struct node Node;
typedef struct list List; 
int yylex();
void yyerror(const char *s);
extern FILE *yyin;
int error_flag = 0;
int stopping = 1;
int temp_var_count = 0; // Counter for temporary variables
int tac_count = 0;      // Counter for TAC lines
char tac[10000][10000];     // Assuming we won't exceed 100 TAC lines, and each TAC line is at most 100 chars

// Function to generate a new temporary variable
char* new_temp() {
    static char temp[10];
    sprintf(temp, "t%d", temp_var_count++);
    return strdup(temp); // Return the temporary variable name
}

void add_tac(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(tac[tac_count++], format, args);
    va_end(args);
}


struct node {
 char value[100]; // typecasting
 List* trueList;
 bool notassign;
 List* falseList;
 List* nextList;
};

struct list {
 int addr;
 List* next;
};

char* gen_label() {
 char* label = (char*)malloc(sizeof(char) * 100);
 if (label == NULL) {
 perror("Memory allocation failed");
 exit(EXIT_FAILURE);
 }
 sprintf(label, "t%d", temp_var_count);
 temp_var_count++;
 return label;
}

Node* createNode(char* value, List* trueList, List* falseList, List* nextList){
 Node* n = (Node*)malloc(sizeof(Node));
 strcpy(n->value, value);
 n->nextList = nextList;
 n->trueList = trueList;
 n->falseList = falseList;
 return n;
}

List* makelist(int addr){
 List* list = (List*)malloc(sizeof(List));
 list->addr = addr;
 list->next = NULL;
}

List* merge(List* l1, List* l2) {
 if (l1 == NULL) return l2;
 if (l2 == NULL) return l1;

 List* curr = l1;
 while (curr->next != NULL) {
 curr = curr->next;
 }
 curr->next = l2;
 return l1;
}

void backpatch(List* list, int addr){
 List* curr = list;
 while(curr != NULL){
 sprintf(tac[curr->addr] + strlen(tac[curr->addr]),"%d\n",addr);
 curr = curr->next;
 }
}

void printList(List* list, const char* label) {
 printf("%s: ", label);
 List* curr = list;
 while (curr != NULL) {
 printf("%d -> ", curr->addr);
 curr = curr->next;
 }
 printf("NULL\n");
}






#line 178 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NUMBER = 259,                  /* NUMBER  */
    EQ = 260,                      /* EQ  */
    PEQ = 261,                     /* PEQ  */
    MEQ = 262,                     /* MEQ  */
    MLEQ = 263,                    /* MLEQ  */
    DEQ = 264,                     /* DEQ  */
    MM = 265,                      /* MM  */
    PP = 266,                      /* PP  */
    LB = 267,                      /* LB  */
    RB = 268,                      /* RB  */
    GT = 269,                      /* GT  */
    LT = 270,                      /* LT  */
    GTE = 271,                     /* GTE  */
    LTE = 272,                     /* LTE  */
    NE = 273,                      /* NE  */
    EE = 274,                      /* EE  */
    AND = 275,                     /* AND  */
    OR = 276,                      /* OR  */
    IF = 277,                      /* IF  */
    ELSE = 278,                    /* ELSE  */
    T = 279,                       /* T  */
    F = 280,                       /* F  */
    WHILE = 281,                   /* WHILE  */
    NN = 282                       /* NN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define NUMBER 259
#define EQ 260
#define PEQ 261
#define MEQ 262
#define MLEQ 263
#define DEQ 264
#define MM 265
#define PP 266
#define LB 267
#define RB 268
#define GT 269
#define LT 270
#define GTE 271
#define LTE 272
#define NE 273
#define EE 274
#define AND 275
#define OR 276
#define IF 277
#define ELSE 278
#define T 279
#define F 280
#define WHILE 281
#define NN 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 108 "prob.y"

    char* value;   // For storing string values like identifiers and numbers
     struct node* node;
     int instr;
     struct list* list;

#line 292 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_EQ = 5,                         /* EQ  */
  YYSYMBOL_PEQ = 6,                        /* PEQ  */
  YYSYMBOL_MEQ = 7,                        /* MEQ  */
  YYSYMBOL_MLEQ = 8,                       /* MLEQ  */
  YYSYMBOL_DEQ = 9,                        /* DEQ  */
  YYSYMBOL_MM = 10,                        /* MM  */
  YYSYMBOL_PP = 11,                        /* PP  */
  YYSYMBOL_LB = 12,                        /* LB  */
  YYSYMBOL_RB = 13,                        /* RB  */
  YYSYMBOL_GT = 14,                        /* GT  */
  YYSYMBOL_LT = 15,                        /* LT  */
  YYSYMBOL_GTE = 16,                       /* GTE  */
  YYSYMBOL_LTE = 17,                       /* LTE  */
  YYSYMBOL_NE = 18,                        /* NE  */
  YYSYMBOL_EE = 19,                        /* EE  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_IF = 22,                        /* IF  */
  YYSYMBOL_ELSE = 23,                      /* ELSE  */
  YYSYMBOL_T = 24,                         /* T  */
  YYSYMBOL_F = 25,                         /* F  */
  YYSYMBOL_WHILE = 26,                     /* WHILE  */
  YYSYMBOL_NN = 27,                        /* NN  */
  YYSYMBOL_28_ = 28,                       /* '+'  */
  YYSYMBOL_29_ = 29,                       /* '-'  */
  YYSYMBOL_30_ = 30,                       /* '*'  */
  YYSYMBOL_31_ = 31,                       /* '/'  */
  YYSYMBOL_32_ = 32,                       /* '%'  */
  YYSYMBOL_33_ = 33,                       /* '('  */
  YYSYMBOL_34_ = 34,                       /* ')'  */
  YYSYMBOL_35_ = 35,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 36,                  /* $accept  */
  YYSYMBOL_program = 37,                   /* program  */
  YYSYMBOL_stmt_list = 38,                 /* stmt_list  */
  YYSYMBOL_stmt = 39,                      /* stmt  */
  YYSYMBOL_ASSIGN = 40,                    /* ASSIGN  */
  YYSYMBOL_expr = 41,                      /* expr  */
  YYSYMBOL_N = 42,                         /* N  */
  YYSYMBOL_M = 43,                         /* M  */
  YYSYMBOL_term = 44,                      /* term  */
  YYSYMBOL_UM = 45,                        /* UM  */
  YYSYMBOL_DOUBLE = 46                     /* DOUBLE  */
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
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  100

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


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
       2,     2,     2,     2,     2,     2,     2,    32,     2,     2,
      33,    34,    30,    28,     2,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
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
      25,    26,    27
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   131,   131,   146,   156,   165,   216,   223,   234,   240,
     246,   256,   260,   261,   264,   265,   266,   267,   268,   272,
     280,   287,   294,   301,   308,   313,   321,   329,   335,   342,
     350,   356,   362,   368,   374,   376,   377,   381,   388,   390,
     392,   410,   457,   508,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   537,   540,   545,   548
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "NUMBER",
  "EQ", "PEQ", "MEQ", "MLEQ", "DEQ", "MM", "PP", "LB", "RB", "GT", "LT",
  "GTE", "LTE", "NE", "EE", "AND", "OR", "IF", "ELSE", "T", "F", "WHILE",
  "NN", "'+'", "'-'", "'*'", "'/'", "'%'", "'('", "')'", "';'", "$accept",
  "program", "stmt_list", "stmt", "ASSIGN", "expr", "N", "M", "term", "UM",
  "DOUBLE", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-50)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-58)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     123,   -50,    96,   -31,   -50,   -50,   -50,    -7,   -50,    -7,
       3,     4,   -50,   152,   -50,    44,   -50,    10,   -18,    -9,
      73,    64,   -50,   123,   -50,   -50,   -50,   -50,   -50,    -7,
      -7,    -7,    -7,    -7,    -7,   -50,   -50,    -7,    -7,    -7,
      -7,    -7,   -50,    -7,    17,     2,    21,    32,    34,   -50,
     -50,   196,    -7,   -50,   -50,   -50,    87,    87,    87,    87,
     160,   160,    -7,    -7,   238,   238,   122,   122,   122,   174,
     -50,   -50,   -50,    47,   -50,   -50,   -50,   -50,   -50,    56,
     -50,   217,   182,    97,   -50,   -50,   -50,   -50,   -50,   -50,
     -50,   123,   -50,    22,   123,   -50,   -50,   -50,   123,   -50
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,     0,     0,    36,    37,    39,    57,    56,    57,
       0,    39,     4,     0,    35,     0,     9,    39,    57,     0,
      33,     0,     1,     0,    14,    15,    16,    17,    18,    57,
      57,    57,    57,    57,    57,    39,    38,    57,    57,    57,
      57,    57,    12,    57,    40,    43,    59,    58,     0,     8,
      11,     0,    57,    34,    24,     3,    25,    26,    27,    28,
      29,    30,    57,    57,    19,    20,    21,    22,    23,     0,
      54,    59,    58,    42,    53,    48,    47,    46,    45,    41,
      39,     0,    31,    32,     5,    50,    52,    55,    49,    51,
      44,     0,    39,     6,     0,    38,    10,    39,     0,     7
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -50,   -50,    50,   -22,   -50,     1,   -49,    -6,   -50,   -50,
     -30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    10,    11,    12,    43,    13,    63,    23,    14,    15,
      48
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      19,    55,    18,    22,    -2,    74,     4,     5,    20,     7,
      21,     8,    75,    76,    73,     9,    50,     4,     5,    51,
       7,    70,     8,    49,    52,    77,     9,    71,    72,    62,
      56,    57,    58,    59,    60,    61,    78,    79,    64,    65,
      66,    67,    68,    87,    69,    95,    97,    44,    45,    90,
      85,    86,    17,    81,    46,    47,     0,    71,    72,    88,
      89,     0,     0,    82,    83,    53,    71,    72,     0,    93,
       0,     0,    96,     0,    91,     0,    99,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    94,    29,    30,    31,
      32,    98,    37,    38,    39,    40,    41,     1,    54,   -57,
     -57,   -58,   -58,   -58,   -58,     0,   -57,   -57,     2,    16,
       0,    29,    30,    31,    32,    33,    34,    35,     3,     0,
       4,     5,     6,     7,     1,     8,   -57,   -57,     0,     9,
       0,     0,     0,   -57,   -57,     2,    29,    30,    31,    32,
      33,    34,    35,    36,     0,     3,     0,     4,     5,     6,
       7,     0,     8,     0,     0,     0,     9,    24,    25,    26,
      27,    28,     0,     0,     0,     0,    29,    30,    31,    32,
      33,    34,    35,    36,    29,    30,    31,    32,   -58,   -58,
      37,    38,    39,    40,    41,     0,     0,    42,    29,    30,
      31,    32,    33,    34,    35,    36,    29,    30,    31,    32,
      33,    34,    37,    38,    39,    40,    41,     0,     0,    84,
      29,    30,    31,    32,    33,    34,    35,    36,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,     0,
      80,    29,    30,    31,    32,    33,    34,    35,    36,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
       0,    92,    29,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,     0,    39,    40,
      41
};

static const yytype_int8 yycheck[] =
{
       6,    23,    33,     0,     0,     3,    24,    25,     7,    27,
       9,    29,    10,    11,    44,    33,    34,    24,    25,    18,
      27,     4,    29,    13,    33,     4,    33,    10,    11,    35,
      29,    30,    31,    32,    33,    34,     4,     3,    37,    38,
      39,    40,    41,    73,    43,    23,    95,     3,     4,    79,
       3,     4,     2,    52,    10,    11,    -1,    10,    11,     3,
       4,    -1,    -1,    62,    63,     1,    10,    11,    -1,    91,
      -1,    -1,    94,    -1,    80,    -1,    98,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    92,    14,    15,    16,
      17,    97,    28,    29,    30,    31,    32,     1,    34,     3,
       4,    14,    15,    16,    17,    -1,    10,    11,    12,    13,
      -1,    14,    15,    16,    17,    18,    19,    20,    22,    -1,
      24,    25,    26,    27,     1,    29,     3,     4,    -1,    33,
      -1,    -1,    -1,    10,    11,    12,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    22,    -1,    24,    25,    26,
      27,    -1,    29,    -1,    -1,    -1,    33,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    19,    20,    21,    14,    15,    16,    17,    18,    19,
      28,    29,    30,    31,    32,    -1,    -1,    35,    14,    15,
      16,    17,    18,    19,    20,    21,    14,    15,    16,    17,
      18,    19,    28,    29,    30,    31,    32,    -1,    -1,    35,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,    -1,
      34,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    31,    32,
      -1,    34,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    12,    22,    24,    25,    26,    27,    29,    33,
      37,    38,    39,    41,    44,    45,    13,    38,    33,    43,
      41,    41,     0,    43,     5,     6,     7,     8,     9,    14,
      15,    16,    17,    18,    19,    20,    21,    28,    29,    30,
      31,    32,    35,    40,     3,     4,    10,    11,    46,    13,
      34,    41,    33,     1,    34,    39,    41,    41,    41,    41,
      41,    41,    43,    42,    41,    41,    41,    41,    41,    41,
       4,    10,    11,    46,     3,    10,    11,     4,     4,     3,
      34,    41,    41,    41,    35,     3,     4,    46,     3,     4,
      46,    43,    34,    39,    43,    23,    39,    42,    43,    39
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    38,    38,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    40,    40,    40,    40,    40,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    42,    43,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    45,    45,    46,    46
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     1,     4,     6,    10,     3,     2,
       7,     3,     2,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     2,     3,     1,     1,     1,     0,     0,
       2,     3,     3,     2,     4,     3,     3,     3,     3,     4,
       4,     4,     4,     3,     3,     4,     1,     0,     1,     1
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
        yyerror (YY_("syntax error: cannot back up")); \
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
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
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
  case 2: /* program: stmt_list  */
#line 131 "prob.y"
                  {
        if(!error_flag)
     {  backpatch((yyvsp[0].node)->nextList,tac_count);
        printf("----------Intermediate Code----------\n");
        for (int i = 0; i < tac_count; i++) {
            printf("%d: %s", i, tac[i]);
        }
        temp_var_count = 0;
        tac_count = 0; // Reset intermediate code index
    }
   
    }
#line 1419 "y.tab.c"
    break;

  case 3: /* stmt_list: stmt_list M stmt  */
#line 146 "prob.y"
                          {
            if(!error_flag)
            {
            // printList($1->nextList, "$1->nextList before backpatch");
            backpatch((yyvsp[-2].node)->nextList,(yyvsp[-1].instr));
            Node* temp = createNode("",NULL,NULL,(yyvsp[0].node)->nextList);
            (yyval.node) = temp;
            }
        
        }
#line 1434 "y.tab.c"
    break;

  case 4: /* stmt_list: stmt  */
#line 156 "prob.y"
                 { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,(yyvsp[0].node)->nextList);
            (yyval.node) = temp;
        } 
    }
#line 1445 "y.tab.c"
    break;

  case 5: /* stmt: expr ASSIGN expr ';'  */
#line 165 "prob.y"
                         { if(!error_flag)
    				{
                                      if((yyvsp[-3].node)->notassign)
                                      {
                                        printf("Rejected-cannot assign to a postfix expression\n");
                                        error_flag = 1;
                                      }    
                                      else
                                      {
                                      if(strcmp((yyvsp[-2].node)->value, "=") == 0)
                                 {
                                    add_tac("%s = %s\n", (yyvsp[-3].node), (yyvsp[-1].node)->value);
                                 }
                                 else if(strcmp((yyvsp[-2].node)->value, "+=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[-1].node)->value);
                                    char *new1 = new_temp();
                                    add_tac("%s = %s + %s\n", new1, (yyvsp[-3].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-3].node), new1);
                                 }
                                 else if(strcmp((yyvsp[-2].node)->value, "-=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[-1].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s - %s\n", new1, (yyvsp[-3].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-3].node), new1);
                                 }
                                 else if(strcmp((yyvsp[-2].node)->value, "*=") == 0){
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[-1].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s * %s\n", new1, (yyvsp[-3].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-3].node), new1);
                                 }
                                 else{
                                    char *new = new_temp();
                                    add_tac("%s = %s\n", new, (yyvsp[-1].node)->value);
                                     char *new1 = new_temp();
                                    add_tac("%s = %s / %s\n", new1, (yyvsp[-3].node),new);
                                    add_tac("%s = %s\n",(yyvsp[-3].node), new1);
                                 }
                                  Node* _temp = createNode("",NULL,NULL,NULL);
                                  (yyval.node) = _temp;
                                
                                 
                                      }
    
                                 } 
                                 
                          }
#line 1500 "y.tab.c"
    break;

  case 6: /* stmt: IF '(' expr ')' M stmt  */
#line 216 "prob.y"
                             {
                                  if(!error_flag)
                               {        backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                                  Node* temp = createNode("",NULL,NULL,merge((yyvsp[-3].node)->falseList,(yyvsp[0].node)->nextList));
                                  (yyval.node) = temp;
                                  }
                               }
#line 1512 "y.tab.c"
    break;

  case 7: /* stmt: IF '(' expr ')' M stmt ELSE N M stmt  */
#line 224 "prob.y"
                                      {
                                       if(!error_flag)
                                      {
                                       backpatch((yyvsp[-7].node)->trueList,(yyvsp[-5].instr));
                                       backpatch((yyvsp[-7].node)->falseList,(yyvsp[-1].instr));
                                       Node* _temp = createNode("",NULL,NULL,merge((yyvsp[-4].node)->nextList,makelist((yyvsp[-2].instr))));
                                       Node* temp = createNode("",NULL,NULL,merge(_temp->nextList,(yyvsp[0].node)->nextList));
                                       (yyval.node) = temp;
                                       }
                                      }
#line 1527 "y.tab.c"
    break;

  case 8: /* stmt: LB stmt_list RB  */
#line 234 "prob.y"
                     {  if(!error_flag)
                        {
                        Node* temp = createNode("",NULL,NULL,(yyvsp[-1].node)->nextList);
                        (yyval.node) = temp;
                        }
                      }
#line 1538 "y.tab.c"
    break;

  case 9: /* stmt: LB RB  */
#line 240 "prob.y"
          { 
        if(!error_flag){
            Node* temp = createNode("",NULL,NULL,NULL);
            (yyval.node) = temp;
        }
    }
#line 1549 "y.tab.c"
    break;

  case 10: /* stmt: WHILE M '(' expr ')' M stmt  */
#line 246 "prob.y"
                                {
                                  if(!error_flag)
                              {
                               backpatch((yyvsp[0].node)->nextList,(yyvsp[-5].instr));
                               backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                               Node* temp = createNode("",NULL,NULL,(yyvsp[-3].node)->falseList);
                               (yyval.node) = temp;
                               add_tac("goto %d\n",(yyvsp[-5].instr));
                               }
                               }
#line 1564 "y.tab.c"
    break;

  case 11: /* stmt: IF '(' ')'  */
#line 256 "prob.y"
               {if(!error_flag){
            printf("Rejected - empty bool expr inside if\n");
            error_flag = 1;
        }}
#line 1573 "y.tab.c"
    break;

  case 12: /* stmt: expr ';'  */
#line 260 "prob.y"
             {if(!error_flag) {(yyval.node)=(yyvsp[-1].node);}}
#line 1579 "y.tab.c"
    break;

  case 13: /* stmt: error  */
#line 261 "prob.y"
          { if(!error_flag) {printf("Rejected - Invalid Expression\n");error_flag=1;yyerrok;}}
#line 1585 "y.tab.c"
    break;

  case 14: /* ASSIGN: EQ  */
#line 264 "prob.y"
           { (yyval.node) = createNode(strdup("="), NULL, NULL, NULL); }
#line 1591 "y.tab.c"
    break;

  case 15: /* ASSIGN: PEQ  */
#line 265 "prob.y"
            { (yyval.node) = createNode(strdup("+="), NULL, NULL, NULL); }
#line 1597 "y.tab.c"
    break;

  case 16: /* ASSIGN: MEQ  */
#line 266 "prob.y"
            { (yyval.node) = createNode(strdup("-="), NULL, NULL, NULL); }
#line 1603 "y.tab.c"
    break;

  case 17: /* ASSIGN: MLEQ  */
#line 267 "prob.y"
             { (yyval.node) = createNode(strdup("*="), NULL, NULL, NULL); }
#line 1609 "y.tab.c"
    break;

  case 18: /* ASSIGN: DEQ  */
#line 268 "prob.y"
             { (yyval.node) = createNode(strdup("/="), NULL, NULL, NULL); }
#line 1615 "y.tab.c"
    break;

  case 19: /* expr: expr '+' expr  */
#line 272 "prob.y"
                  { //printf("hi +\n");
                     if(!error_flag)
                      {
                    char* label = new_temp();
                    add_tac("%s = %s + %s\n", label, (yyvsp[-2].node)->value, (yyvsp[0].node)->value);
                    (yyval.node) = createNode(label, NULL, NULL, NULL);
                    }
                   }
#line 1628 "y.tab.c"
    break;

  case 20: /* expr: expr '-' expr  */
#line 280 "prob.y"
                  { //printf("hi -\n");
                      if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s - %s\n", label, (yyvsp[-2].node)->value, (yyvsp[0].node)->value);
                    (yyval.node) = createNode(label, NULL, NULL, NULL);
                    }
                   }
#line 1640 "y.tab.c"
    break;

  case 21: /* expr: expr '*' expr  */
#line 287 "prob.y"
                  {  //printf("hi *\n");
                     if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s * %s\n", label, (yyvsp[-2].node)->value, (yyvsp[0].node)->value);
                    (yyval.node) = createNode(label, NULL, NULL, NULL);
                    }
                   }
#line 1652 "y.tab.c"
    break;

  case 22: /* expr: expr '/' expr  */
#line 294 "prob.y"
                   {//printf("hi /\n");
                    if(!error_flag)
            { char* label = new_temp();
                    add_tac("%s = %s / %s\n", label, (yyvsp[-2].node)->value, (yyvsp[0].node)->value);
                    (yyval.node) = createNode(label, NULL, NULL, NULL);
                   }
                   }
#line 1664 "y.tab.c"
    break;

  case 23: /* expr: expr '%' expr  */
#line 301 "prob.y"
                   { //printf("hi %\n");
                     if(!error_flag)
            {char* label = new_temp();
                    add_tac("%s = %s %% %s\n", label, (yyvsp[-2].node)->value, (yyvsp[0].node)->value);
                    (yyval.node) = createNode(label, NULL, NULL, NULL);
                    }
                   }
#line 1676 "y.tab.c"
    break;

  case 24: /* expr: '(' expr ')'  */
#line 308 "prob.y"
                   {// printf("hi (\n"); 
                       if(!error_flag)
                        {  Node* temp = createNode((yyvsp[-1].node)->value,(yyvsp[-1].node)->trueList,(yyvsp[-1].node)->falseList,NULL);
                        (yyval.node) = temp;}
                        }
#line 1686 "y.tab.c"
    break;

  case 25: /* expr: expr GT expr  */
#line 313 "prob.y"
                  { //printf("hi %\n");
                     if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, ">", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 1699 "y.tab.c"
    break;

  case 26: /* expr: expr LT expr  */
#line 321 "prob.y"
                  { //printf("hi %\n");
                     if(!error_flag)
                    { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "<", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 1712 "y.tab.c"
    break;

  case 27: /* expr: expr GTE expr  */
#line 329 "prob.y"
                   { //printf("hi %\n");
                    Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, ">=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                   }
#line 1723 "y.tab.c"
    break;

  case 28: /* expr: expr LTE expr  */
#line 335 "prob.y"
                   { //printf("hi %\n");
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "<=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;}
                   }
#line 1735 "y.tab.c"
    break;

  case 29: /* expr: expr NE expr  */
#line 342 "prob.y"
                  { 
                    if(!error_flag)
            { Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "!=", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;
                     }
                   }
#line 1748 "y.tab.c"
    break;

  case 30: /* expr: expr EE expr  */
#line 350 "prob.y"
                  {   if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),makelist(tac_count+1),NULL);
                     add_tac("if %s %s %s goto ", (yyvsp[-2].node)->value, "==", (yyvsp[0].node)->value);
                     add_tac("goto ");
                     (yyval.node) = temp;}
                   }
#line 1759 "y.tab.c"
    break;

  case 31: /* expr: expr AND M expr  */
#line 356 "prob.y"
                    { 
                          if(!error_flag)
            { Node* temp = createNode("",(yyvsp[0].node)->trueList,merge((yyvsp[-3].node)->falseList,(yyvsp[0].node)->falseList),NULL);
                          backpatch((yyvsp[-3].node)->trueList,(yyvsp[-1].instr));
                          (yyval.node) = temp;}
                     }
#line 1770 "y.tab.c"
    break;

  case 32: /* expr: expr OR N expr  */
#line 362 "prob.y"
                    { 
                       if(!error_flag)
            {Node* temp = createNode("",merge((yyvsp[-3].node)->trueList,(yyvsp[0].node)->trueList),(yyvsp[0].node)->falseList,NULL);
                      backpatch((yyvsp[-3].node)->falseList,(yyvsp[-1].instr));
                      (yyval.node) = temp;}
                    }
#line 1781 "y.tab.c"
    break;

  case 33: /* expr: NN expr  */
#line 368 "prob.y"
            {
   
          if(!error_flag)
            {Node* temp = createNode("",(yyvsp[0].node)->falseList,(yyvsp[0].node)->trueList,NULL);
                      (yyval.node) = temp;}
      }
#line 1792 "y.tab.c"
    break;

  case 34: /* expr: '(' expr error  */
#line 374 "prob.y"
                   { printf("Rejected - Right paranthesis missing\n"); error_flag = 1; yyerror("Rejected - Invalid operand for modulus operator");  yyerrok;  }
#line 1798 "y.tab.c"
    break;

  case 35: /* expr: term  */
#line 376 "prob.y"
         { (yyval.node)=(yyvsp[0].node) ;}
#line 1804 "y.tab.c"
    break;

  case 36: /* expr: T  */
#line 377 "prob.y"
      {  if(!error_flag)
            {Node* temp = createNode("",makelist(tac_count),NULL,NULL);
            sprintf(tac[tac_count++], "goto ");
            (yyval.node) = temp;}}
#line 1813 "y.tab.c"
    break;

  case 37: /* expr: F  */
#line 381 "prob.y"
     {  if(!error_flag)
            {Node* temp = createNode("",NULL,makelist(tac_count),NULL);
            sprintf(tac[tac_count++], "goto ");
            (yyval.node) = temp;}}
#line 1822 "y.tab.c"
    break;

  case 38: /* N: %empty  */
#line 388 "prob.y"
   {(yyval.instr) = tac_count; char *dummy = "goto "; add_tac("%s",dummy); }
#line 1828 "y.tab.c"
    break;

  case 39: /* M: %empty  */
#line 390 "prob.y"
   {(yyval.instr) = tac_count;}
#line 1834 "y.tab.c"
    break;

  case 40: /* term: UM IDENTIFIER  */
#line 392 "prob.y"
                     {
                       if((yyvsp[-1].node)->value != NULL && strcmp((yyvsp[-1].node)->value, "-") == 0)
                       {
                        if(!error_flag)
            {
                       char* label = new_temp();
                       add_tac("%s = -%s\n", label, (yyvsp[0].value));
                       Node* temp = createNode(label,NULL,NULL,NULL);
                       (yyval.node) = temp;
                       }
                       }
                       else
                       { if(!error_flag)
            {
                         Node* temp = createNode((yyvsp[0].value),NULL,NULL,NULL);
                         (yyval.node) = temp; }
                       }
                     }
#line 1857 "y.tab.c"
    break;

  case 41: /* term: UM DOUBLE IDENTIFIER  */
#line 411 "prob.y"
                           {    if(!error_flag)
            {
                               if((yyvsp[-2].node)->value != NULL && strcmp((yyvsp[-2].node)->value, "-") == 0)
                               {
                                 char* label = new_temp();
                                 if(strcmp((yyvsp[-1].node)->value, "--") == 0)
                                 {
                                   printf("Rejected - Expression is not assignable\n");   error_flag=1;
                                 }
                                 else
                                 {
                                 char* new = new_temp();
                                 char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n",new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", new, (yyvsp[0].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s\n",(yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                   
                                 }
                               }
                               else
                               {
                                char* label = new_temp();
                                if(strcmp((yyvsp[-1].node)->value, "--") == 0)
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", label, (yyvsp[0].value));
                                   add_tac("%s = %s\n", (yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[0].value));
                                   add_tac("%s = %s\n", label, (yyvsp[0].value));
                                   add_tac("%s = %s\n", (yyvsp[0].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                   
                                 }
                               }
                               (yyval.node)->notassign = true;
                               }
                           }
#line 1908 "y.tab.c"
    break;

  case 42: /* term: UM IDENTIFIER DOUBLE  */
#line 458 "prob.y"
                           {    if(!error_flag)
            {
                               if((yyvsp[-2].node)->value != NULL && strcmp((yyvsp[-2].node)->value, "-") == 0)
                               {
                                 char* label = new_temp();
                                 if(strcmp((yyvsp[0].node)->value, "--") == 0)
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, (yyvsp[-1].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 {
                                   char* new = new_temp();
                                   char* new1 = new_temp();
                                   add_tac("%s = %s\n", new, (yyvsp[-1].value));
                                   add_tac("%s = -%s\n",label,new);
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                 }
                               }
                               else
                               {
                                char* label = new_temp();
                                if(strcmp((yyvsp[0].node)->value, "--") == 0)
                                 {  char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, (yyvsp[-1].value));
                                   add_tac("%s = %s - 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;
                                 }
                                 else
                                 { char* new1 = new_temp();
                                   add_tac("%s = %s\n", label, (yyvsp[-1].value));
                                   add_tac("%s = %s + 1\n", new1, (yyvsp[-1].value));
                                   add_tac("%s = %s\n", (yyvsp[-1].value), new1);    
                                   Node* temp = createNode(label,NULL,NULL,NULL);
                                   (yyval.node) = temp;                             
                                 }
                               }
                                (yyval.node)->notassign = true;}
                           }
#line 1963 "y.tab.c"
    break;

  case 43: /* term: UM NUMBER  */
#line 508 "prob.y"
                {
                    if(!error_flag)
            {
                       if((yyvsp[-1].node)->value != NULL && strcmp((yyvsp[-1].node)->value, "-") == 0)
                       {
                       Node* temp = createNode(strcat((yyvsp[-1].node)->value,(yyvsp[0].value)),NULL,NULL,NULL);
                        (yyval.node) = temp;
                       }
                       else
                       {
                         Node* temp = createNode((yyvsp[0].value),NULL,NULL,NULL);
                         (yyval.node) = temp;
                       }}
                     
                 }
#line 1983 "y.tab.c"
    break;

  case 44: /* term: UM DOUBLE IDENTIFIER DOUBLE  */
#line 523 "prob.y"
                                    {printf("Rejected - Expression is not assignable\n");   error_flag=1; }
#line 1989 "y.tab.c"
    break;

  case 45: /* term: UM PP NUMBER  */
#line 524 "prob.y"
                    {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 1995 "y.tab.c"
    break;

  case 46: /* term: UM MM NUMBER  */
#line 525 "prob.y"
                    {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2001 "y.tab.c"
    break;

  case 47: /* term: UM NUMBER PP  */
#line 526 "prob.y"
                    {printf("Rejected - cannot increment a constant value\n");   error_flag=1; }
#line 2007 "y.tab.c"
    break;

  case 48: /* term: UM NUMBER MM  */
#line 527 "prob.y"
                    {printf("Rejected - cannot decrement a constant value\n");   error_flag=1; }
#line 2013 "y.tab.c"
    break;

  case 49: /* term: UM DOUBLE IDENTIFIER IDENTIFIER  */
#line 528 "prob.y"
                                       {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2019 "y.tab.c"
    break;

  case 50: /* term: UM IDENTIFIER DOUBLE IDENTIFIER  */
#line 529 "prob.y"
                                       {printf("Rejected - Missing operator\n"); error_flag=1;}
#line 2025 "y.tab.c"
    break;

  case 51: /* term: UM DOUBLE IDENTIFIER NUMBER  */
#line 530 "prob.y"
                                   {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2031 "y.tab.c"
    break;

  case 52: /* term: UM IDENTIFIER DOUBLE NUMBER  */
#line 531 "prob.y"
                                   {printf("Rejected - Missing operator\n");   error_flag=1;}
#line 2037 "y.tab.c"
    break;

  case 53: /* term: UM NUMBER IDENTIFIER  */
#line 532 "prob.y"
                            {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2043 "y.tab.c"
    break;

  case 54: /* term: UM IDENTIFIER NUMBER  */
#line 533 "prob.y"
                            {printf("Rejected - Missing operator\n");  error_flag=1;}
#line 2049 "y.tab.c"
    break;

  case 55: /* term: UM IDENTIFIER DOUBLE DOUBLE  */
#line 534 "prob.y"
                                   {printf("Rejected - Expression is not assignable\n"); error_flag=1;}
#line 2055 "y.tab.c"
    break;

  case 56: /* UM: '-'  */
#line 537 "prob.y"
        {   Node* temp = createNode(strdup("-"),NULL,NULL,NULL);
           (yyval.node) = temp;
            }
#line 2063 "y.tab.c"
    break;

  case 57: /* UM: %empty  */
#line 540 "prob.y"
                  {   Node* temp = createNode("not needed",NULL,NULL,NULL);
           (yyval.node) = temp;}
#line 2070 "y.tab.c"
    break;

  case 58: /* DOUBLE: PP  */
#line 545 "prob.y"
           {  
            Node* temp = createNode(strdup("++"),NULL,NULL,NULL);
           (yyval.node) = temp; }
#line 2078 "y.tab.c"
    break;

  case 59: /* DOUBLE: MM  */
#line 548 "prob.y"
             {  Node* temp = createNode(strdup("--"),NULL,NULL,NULL);
           (yyval.node) = temp; }
#line 2085 "y.tab.c"
    break;


#line 2089 "y.tab.c"

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
      yyerror (YY_("syntax error"));
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
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
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 553 "prob.y"


void yyerror(const char *s) {
    //printf("%s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    yyparse();
    
    fclose(yyin);
    return 0;
}

