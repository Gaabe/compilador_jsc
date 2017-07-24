/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_NEW_PARSER_TAB_H_INCLUDED
# define YY_YY_NEW_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_OPENPAR = 258,
    T_CLOSEPAR = 259,
    T_OPENBRAC = 260,
    T_CLOSEBRAC = 261,
    T_OPENCURL = 262,
    T_CLOSECURL = 263,
    T_COMMA = 264,
    T_SEMICOL = 265,
    T_ASSIGN = 266,
    T_PLUS = 267,
    T_MINUS = 268,
    T_MUL = 269,
    T_DIV = 270,
    T_LT = 271,
    T_GT = 272,
    T_LTE = 273,
    T_GTE = 274,
    T_EQUAL = 275,
    T_NOTEQUAL = 276,
    T_AND = 277,
    T_OR = 278,
    T_NOT = 279,
    T_LET = 280,
    T_DEF = 281,
    T_IF = 282,
    T_ELSE = 283,
    T_WHILE = 284,
    T_BREAK = 285,
    T_RETURN = 286,
    T_CONTINUE = 287,
    T_ID = 288,
    T_NUMBER = 289,
    T_ERROR = 290
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_NEW_PARSER_TAB_H_INCLUDED  */
