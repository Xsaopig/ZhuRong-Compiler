/* A Bison parser, made by GNU Bison 3.7.5.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SYSY_TAB_HH_INCLUDED
# define YY_YY_SYSY_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    INTCONST = 258,                /* INTCONST  */
    IDENT = 259,                   /* IDENT  */
    FLOATCONST = 260,              /* FLOATCONST  */
    ADD = 261,                     /* ADD  */
    SUB = 262,                     /* SUB  */
    MUL = 263,                     /* MUL  */
    DIV = 264,                     /* DIV  */
    MOD = 265,                     /* MOD  */
    LT = 266,                      /* LT  */
    GT = 267,                      /* GT  */
    LE = 268,                      /* LE  */
    GE = 269,                      /* GE  */
    EQ = 270,                      /* EQ  */
    NE = 271,                      /* NE  */
    AND = 272,                     /* AND  */
    OR = 273,                      /* OR  */
    NOT = 274,                     /* NOT  */
    ASSIGN = 275,                  /* ASSIGN  */
    LP = 276,                      /* LP  */
    RP = 277,                      /* RP  */
    LC = 278,                      /* LC  */
    RC = 279,                      /* RC  */
    LB = 280,                      /* LB  */
    RB = 281,                      /* RB  */
    COMMA = 282,                   /* COMMA  */
    SEMI = 283,                    /* SEMI  */
    IF = 284,                      /* IF  */
    ELSE = 285,                    /* ELSE  */
    WHILE = 286,                   /* WHILE  */
    BREAK = 287,                   /* BREAK  */
    CONTINUE = 288,                /* CONTINUE  */
    RETURN = 289,                  /* RETURN  */
    CONST = 290,                   /* CONST  */
    INT = 291,                     /* INT  */
    VOID = 292,                    /* VOID  */
    FLOAT = 293                    /* FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "sysy.yy"

	int    type_int;
	float  type_float;
	char   type_id[32];
	struct node *ptr;

#line 109 "sysy.tab.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SYSY_TAB_HH_INCLUDED  */
