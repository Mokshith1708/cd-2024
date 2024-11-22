/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 128 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
