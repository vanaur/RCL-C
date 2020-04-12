/*
 * Copyright (c) 2019-2020 vanaur.
 *
 * This file is part of RCL
 * (see https://github.com/vanaur/The-Runtime-Core-Library).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

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

/* Substitute the variable and function names.  */
#define yyparse         RCLparse
#define yylex           RCLlex
#define yyerror         RCLerror
#define yylval          RCLlval
#define yychar          RCLchar
#define yydebug         RCLdebug
#define yynerrs         RCLnerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 2 "RCL.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <VM\Core\Syntax\Absyn.h>
#include <Tools\console-color\console-color.h>
#define initialize_lexer RCL_initialize_lexer
extern int yyparse(void);
extern int yylex(void);
int yy_mylinenumber;
extern int initialize_lexer(FILE * inp);
void yyerror(const char *str)
{
  extern char *RCLtext;
  if (!strcmp(RCLtext, ""))
    cc_fprintf(CC_FG_RED, stderr, "\n    > Parsing error: expected code after line %d.\n\n", yy_mylinenumber + 1);
  else
    cc_fprintf(CC_FG_RED, stderr, "\n    > Parsing error [%d]: %s at `%s`.\n\n", yy_mylinenumber + 1, str, RCLtext);
/*   fprintf(stderr,"error: line %d: %s at %s\n",
    yy_mylinenumber + 1, str, RCLtext); */
}

Program YY_RESULT_Program_ = 0;
Program pProgram(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}

Identifier YY_RESULT_Identifier_ = 0;
Identifier pIdentifier(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Identifier_;
  }
}

Preprocessor YY_RESULT_Preprocessor_ = 0;
Preprocessor pPreprocessor(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Preprocessor_;
  }
}

Definition YY_RESULT_Definition_ = 0;
Definition pDefinition(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Definition_;
  }
}

RCL_Structure_Field YY_RESULT_RCL_Structure_Field_ = 0;
RCL_Structure_Field pRCL_Structure_Field(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_RCL_Structure_Field_;
  }
}

FFI_Type_Signature YY_RESULT_FFI_Type_Signature_ = 0;
FFI_Type_Signature pFFI_Type_Signature(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_FFI_Type_Signature_;
  }
}

Ptr YY_RESULT_Ptr_ = 0;
Ptr pPtr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Ptr_;
  }
}

FFI_Type_SignatureT YY_RESULT_FFI_Type_SignatureT_ = 0;
FFI_Type_SignatureT pFFI_Type_SignatureT(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_FFI_Type_SignatureT_;
  }
}

Operation YY_RESULT_Operation_ = 0;
Operation pOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Operation_;
  }
}

LiteralOperation YY_RESULT_LiteralOperation_ = 0;
LiteralOperation pLiteralOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_LiteralOperation_;
  }
}

Data YY_RESULT_Data_ = 0;
Data pData(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Data_;
  }
}

ListOperation YY_RESULT_ListOperation_ = 0;
ListOperation pListOperation(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListOperation_;
  }
}

ListFFI_Type_Signature YY_RESULT_ListFFI_Type_Signature_ = 0;
ListFFI_Type_Signature pListFFI_Type_Signature(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListFFI_Type_Signature_;
  }
}

ListPtr YY_RESULT_ListPtr_ = 0;
ListPtr pListPtr(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListPtr_;
  }
}

ListDefinition YY_RESULT_ListDefinition_ = 0;
ListDefinition pListDefinition(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDefinition_;
  }
}

ListPreprocessor YY_RESULT_ListPreprocessor_ = 0;
ListPreprocessor pListPreprocessor(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListPreprocessor_;
  }
}

ListRCL_Structure_Field YY_RESULT_ListRCL_Structure_Field_ = 0;
ListRCL_Structure_Field pListRCL_Structure_Field(FILE *inp)
{
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListRCL_Structure_Field_;
  }
}


ListOperation reverseListOperation(ListOperation l)
{
  ListOperation prev = 0;
  ListOperation tmp = 0;
  while (l)
  {
    tmp = l->listoperation_;
    l->listoperation_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListFFI_Type_Signature reverseListFFI_Type_Signature(ListFFI_Type_Signature l)
{
  ListFFI_Type_Signature prev = 0;
  ListFFI_Type_Signature tmp = 0;
  while (l)
  {
    tmp = l->listffi_type_signature_;
    l->listffi_type_signature_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListPtr reverseListPtr(ListPtr l)
{
  ListPtr prev = 0;
  ListPtr tmp = 0;
  while (l)
  {
    tmp = l->listptr_;
    l->listptr_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListDefinition reverseListDefinition(ListDefinition l)
{
  ListDefinition prev = 0;
  ListDefinition tmp = 0;
  while (l)
  {
    tmp = l->listdefinition_;
    l->listdefinition_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListPreprocessor reverseListPreprocessor(ListPreprocessor l)
{
  ListPreprocessor prev = 0;
  ListPreprocessor tmp = 0;
  while (l)
  {
    tmp = l->listpreprocessor_;
    l->listpreprocessor_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListRCL_Structure_Field reverseListRCL_Structure_Field(ListRCL_Structure_Field l)
{
  ListRCL_Structure_Field prev = 0;
  ListRCL_Structure_Field tmp = 0;
  while (l)
  {
    tmp = l->listrcl_structure_field_;
    l->listrcl_structure_field_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}



/* Line 189 of yacc.c  */
#line 419 "Parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
     _ERROR_ = 258,
     _SYMB_0 = 259,
     _SYMB_1 = 260,
     _SYMB_2 = 261,
     _SYMB_3 = 262,
     _SYMB_4 = 263,
     _SYMB_5 = 264,
     _SYMB_6 = 265,
     _SYMB_7 = 266,
     _SYMB_8 = 267,
     _SYMB_9 = 268,
     _SYMB_10 = 269,
     _SYMB_11 = 270,
     _SYMB_12 = 271,
     _SYMB_13 = 272,
     _SYMB_14 = 273,
     _SYMB_15 = 274,
     _SYMB_16 = 275,
     _SYMB_17 = 276,
     _SYMB_18 = 277,
     _SYMB_19 = 278,
     _SYMB_20 = 279,
     _SYMB_21 = 280,
     _SYMB_22 = 281,
     _SYMB_23 = 282,
     _SYMB_24 = 283,
     _SYMB_25 = 284,
     _SYMB_26 = 285,
     _SYMB_27 = 286,
     _SYMB_28 = 287,
     _SYMB_29 = 288,
     _SYMB_30 = 289,
     _SYMB_31 = 290,
     _SYMB_32 = 291,
     _SYMB_33 = 292,
     _SYMB_34 = 293,
     _SYMB_35 = 294,
     _SYMB_36 = 295,
     _SYMB_37 = 296,
     _SYMB_38 = 297,
     _SYMB_39 = 298,
     _SYMB_40 = 299,
     _SYMB_41 = 300,
     _SYMB_42 = 301,
     _SYMB_43 = 302,
     _SYMB_44 = 303,
     _SYMB_45 = 304,
     _SYMB_46 = 305,
     _SYMB_47 = 306,
     _SYMB_48 = 307,
     _SYMB_49 = 308,
     _SYMB_50 = 309,
     _SYMB_51 = 310,
     _SYMB_52 = 311,
     _SYMB_53 = 312,
     _SYMB_54 = 313,
     _SYMB_55 = 314,
     _SYMB_56 = 315,
     _SYMB_57 = 316,
     _SYMB_58 = 317,
     _SYMB_59 = 318,
     _SYMB_60 = 319,
     _SYMB_61 = 320,
     _SYMB_62 = 321,
     _SYMB_63 = 322,
     _SYMB_64 = 323,
     _SYMB_65 = 324,
     _SYMB_66 = 325,
     _SYMB_67 = 326,
     _SYMB_68 = 327,
     _SYMB_69 = 328,
     _SYMB_70 = 329,
     _STRING_ = 330,
     _CHAR_ = 331,
     _INTEGER_ = 332,
     _DOUBLE_ = 333
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 340 "RCL.y"

  int int_;
  char char_;
  double double_;
  char* string_;
  Program program_;
  Identifier identifier_;
  Preprocessor preprocessor_;
  Definition definition_;
  RCL_Structure_Field rcl_structure_field_;
  FFI_Type_Signature ffi_type_signature_;
  Ptr ptr_;
  FFI_Type_SignatureT ffi_type_signaturet_;
  Operation operation_;
  LiteralOperation literaloperation_;
  Data data_;
  ListOperation listoperation_;
  ListFFI_Type_Signature listffi_type_signature_;
  ListPtr listptr_;
  ListDefinition listdefinition_;
  ListPreprocessor listpreprocessor_;
  ListRCL_Structure_Field listrcl_structure_field_;




/* Line 214 of yacc.c  */
#line 560 "Parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 572 "Parser.c"

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
# if YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  59
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   184

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNRULES -- Number of states.  */
#define YYNSTATES  150

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   333

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    11,    16,    21,    26,    29,    32,
      34,    37,    39,    43,    46,    51,    56,    59,    64,    71,
      74,    77,    80,    83,    86,    90,    98,   103,   106,   110,
     115,   117,   121,   123,   126,   128,   130,   132,   134,   136,
     138,   140,   142,   144,   146,   148,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   169,   171,   173,   175,   179,
     183,   186,   189,   193,   197,   199,   201,   203,   205,   207,
     209,   211,   213,   215,   217,   219,   221,   223,   225,   227,
     229,   231,   233,   235,   237,   239,   240,   242,   246,   247,
     249,   253,   254,   257,   259,   262,   264,   267,   268,   272,
     274,   277,   279
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      80,     0,    -1,    -1,     4,    97,     5,    96,     6,    88,
      -1,     4,    97,     5,    96,    -1,     4,    97,     6,    88,
      -1,     5,    96,     6,    88,    -1,     5,    96,    -1,     6,
      88,    -1,    88,    -1,     4,    97,    -1,    74,    -1,    73,
       7,    81,    -1,    47,    75,    -1,    47,    75,    38,    73,
      -1,    48,    75,    38,    73,    -1,    39,    73,    -1,    46,
      73,    62,    82,    -1,    46,    73,    62,    82,    41,    82,
      -1,    42,    75,    -1,    72,    75,    -1,    49,    75,    -1,
      44,    77,    -1,    44,    73,    -1,    74,     8,    88,    -1,
      43,    81,     9,    93,    10,    11,    85,    -1,    61,    73,
       8,    98,    -1,     7,    74,    -1,     7,    74,    12,    -1,
       7,    74,    13,    81,    -1,    87,    -1,    14,    93,    15,
      -1,    53,    -1,    85,    94,    -1,    16,    -1,    71,    -1,
      68,    -1,    58,    -1,    65,    -1,    55,    -1,    66,    -1,
      56,    -1,    67,    -1,    57,    -1,    45,    -1,    51,    40,
      -1,    40,    -1,    63,    -1,    52,    -1,    70,    -1,    60,
      -1,    64,    -1,    54,    -1,    50,    -1,    69,    -1,    59,
      -1,    37,    -1,    95,    -1,    17,    77,    89,    -1,    89,
      18,    89,    -1,    74,    19,    -1,    74,    20,    -1,    14,
      88,    15,    -1,    21,    92,    22,    -1,    81,    -1,    91,
      -1,    90,    -1,    84,    -1,    23,    -1,    24,    -1,    25,
      -1,    16,    -1,    26,    -1,    27,    -1,    28,    -1,    29,
      -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,    34,
      -1,    77,    -1,    78,    -1,    76,    -1,    75,    -1,    -1,
      88,    -1,    88,    35,    92,    -1,    -1,    85,    -1,    85,
      35,    93,    -1,    -1,    94,    86,    -1,    89,    -1,    89,
      95,    -1,    83,    -1,    83,    96,    -1,    -1,    83,    36,
      96,    -1,    82,    -1,    82,    97,    -1,    84,    -1,    84,
      35,    98,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   465,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   475,   476,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   490,   491,   492,   494,   495,   496,
     498,   499,   500,   501,   503,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   528,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   555,   556,   557,   558,   560,   561,   562,   564,   565,
     566,   568,   569,   571,   572,   574,   575,   576,   577,   579,
     580,   582,   583
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ERROR_", "_SYMB_0", "_SYMB_1",
  "_SYMB_2", "_SYMB_3", "_SYMB_4", "_SYMB_5", "_SYMB_6", "_SYMB_7",
  "_SYMB_8", "_SYMB_9", "_SYMB_10", "_SYMB_11", "_SYMB_12", "_SYMB_13",
  "_SYMB_14", "_SYMB_15", "_SYMB_16", "_SYMB_17", "_SYMB_18", "_SYMB_19",
  "_SYMB_20", "_SYMB_21", "_SYMB_22", "_SYMB_23", "_SYMB_24", "_SYMB_25",
  "_SYMB_26", "_SYMB_27", "_SYMB_28", "_SYMB_29", "_SYMB_30", "_SYMB_31",
  "_SYMB_32", "_SYMB_33", "_SYMB_34", "_SYMB_35", "_SYMB_36", "_SYMB_37",
  "_SYMB_38", "_SYMB_39", "_SYMB_40", "_SYMB_41", "_SYMB_42", "_SYMB_43",
  "_SYMB_44", "_SYMB_45", "_SYMB_46", "_SYMB_47", "_SYMB_48", "_SYMB_49",
  "_SYMB_50", "_SYMB_51", "_SYMB_52", "_SYMB_53", "_SYMB_54", "_SYMB_55",
  "_SYMB_56", "_SYMB_57", "_SYMB_58", "_SYMB_59", "_SYMB_60", "_SYMB_61",
  "_SYMB_62", "_SYMB_63", "_SYMB_64", "_SYMB_65", "_SYMB_66", "_SYMB_67",
  "_SYMB_68", "_SYMB_69", "_SYMB_70", "_STRING_", "_CHAR_", "_INTEGER_",
  "_DOUBLE_", "$accept", "Program", "Identifier", "Preprocessor",
  "Definition", "RCL_Structure_Field", "FFI_Type_Signature", "Ptr",
  "FFI_Type_SignatureT", "Operation1", "Operation", "LiteralOperation",
  "Data", "ListOperation1", "ListFFI_Type_Signature", "ListPtr",
  "ListOperation", "ListDefinition", "ListPreprocessor",
  "ListRCL_Structure_Field", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    81,    81,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    83,    83,    83,    84,    84,    84,
      85,    85,    85,    85,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    88,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    91,    91,    92,    92,    92,    93,    93,
      93,    94,    94,    95,    95,    96,    96,    96,    96,    97,
      97,    98,    98
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     4,     4,     4,     2,     2,     1,
       2,     1,     3,     2,     4,     4,     2,     4,     6,     2,
       2,     2,     2,     2,     3,     7,     4,     2,     3,     4,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     3,     0,     1,
       3,     0,     2,     1,     2,     1,     2,     0,     3,     1,
       2,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,    97,     0,     0,     0,    71,     0,    85,    68,
      69,    70,    72,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    11,    84,    83,    81,    82,     0,    64,    67,
       9,    93,    66,    65,    57,     0,     0,     0,     0,     0,
       0,     0,     0,    99,    10,     0,     0,     0,    95,     7,
       8,    27,     0,     0,    86,     0,     0,    60,    61,     1,
       0,    94,    16,    19,    23,    22,     0,    13,     0,    21,
      20,   100,    97,     0,    11,     0,     0,     0,    97,    96,
       0,    28,     0,    62,    58,    85,    63,    12,    59,     0,
       0,     0,     4,     5,    88,     0,    24,    98,     6,    29,
      87,    17,    14,    15,     0,    88,    56,    46,    44,    53,
       0,    48,    32,    52,    39,    41,    43,    37,    55,    50,
      47,    51,    38,    40,    42,    36,    54,    49,    35,    89,
      30,     0,   101,    26,     0,     3,     0,    45,    88,    33,
       0,     0,    18,    31,    90,    34,    92,     0,   102,    25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,    28,    43,    48,    29,   129,   146,   130,    54,
      31,    32,    33,    55,   131,   139,    34,    49,    44,   133
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -95
static const yytype_int16 yypact[] =
{
       8,    48,    28,   101,   -66,   101,   -95,   -60,   101,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,     4,    59,   -95,   -95,   -95,   -95,    19,   -95,   -95,
     -95,    36,   -95,   -95,   -95,   -47,   -45,   -67,   -27,   -24,
     -20,    -3,    -1,    48,    95,    33,   -25,    20,   -16,    70,
     -95,   124,    73,   101,    56,    71,    33,   -95,   -95,   -95,
     101,   -95,   -95,   -95,   -95,   -95,    41,    60,    81,   -95,
     -95,   -95,    28,   101,   -95,   112,   115,   101,    28,   -95,
     101,   -95,    33,   -95,   120,   101,   -95,   -95,   120,    48,
      67,    68,   137,   -95,   102,   138,   -95,   -95,   -95,   -95,
     -95,   103,   -95,   -95,   101,   102,   -95,   -95,   -95,   -95,
     106,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -14,
     -95,   139,   113,   -95,    48,   -95,   135,   -95,   102,   147,
     140,   138,   -95,   -95,   -95,   -95,   -95,   102,   -95,   148
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -95,   -38,   -85,   -95,   -94,    34,   -95,   -95,     0,
     -44,   -95,   -95,    97,   -82,   -95,   149,    27,   141,    42
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -92
static const yytype_int16 yytable[] =
{
      30,   132,   -91,    50,   101,    52,    64,    75,    51,    84,
      65,    56,     1,     2,     3,     4,    88,    53,    87,    59,
      78,   138,     5,   136,     6,     7,    62,    45,    77,     8,
      63,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     4,    99,    46,    66,   132,    76,   142,
       5,    67,     6,     7,    60,    68,   144,     8,    47,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    45,    69,    93,    70,    79,    80,    96,    57,    58,
      98,    21,    22,    23,    24,    25,    26,    35,    83,    46,
      36,    85,    37,    86,    38,    39,    40,    41,    90,    92,
      72,    73,    47,    89,   135,    97,    21,    74,     4,    21,
      22,    23,    24,    25,    26,     5,   105,     6,     7,    91,
      42,    94,     8,    95,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    81,    82,    60,   106,
     102,   103,   107,   104,   134,     4,   137,   108,   141,   140,
     143,   147,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   145,   -91,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    21,    22,    23,    24,    25,    26,
      61,   149,   100,   148,    71
};

static const yytype_uint8 yycheck[] =
{
       0,    95,    16,     3,    89,     5,    73,    45,    74,    53,
      77,     7,     4,     5,     6,     7,    60,    77,    56,     0,
      36,    35,    14,   105,    16,    17,    73,    43,     8,    21,
      75,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,     7,    82,    61,    73,   141,    73,   134,
      14,    75,    16,    17,    18,    75,   138,    21,    74,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    43,    75,    73,    75,    48,     6,    77,    19,    20,
      80,    73,    74,    75,    76,    77,    78,    39,    15,    61,
      42,    35,    44,    22,    46,    47,    48,    49,    38,    72,
       5,     6,    74,    62,   104,    78,    73,    74,     7,    73,
      74,    75,    76,    77,    78,    14,    14,    16,    17,    38,
      72,     9,    21,     8,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    12,    13,    18,    37,
      73,    73,    40,     6,    41,     7,    40,    45,    35,    10,
      15,    11,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    16,    16,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    73,    74,    75,    76,    77,    78,
      31,   147,    85,   141,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     6,     7,    14,    16,    17,    21,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    73,    74,    75,    76,    77,    78,    80,    81,    84,
      88,    89,    90,    91,    95,    39,    42,    44,    46,    47,
      48,    49,    72,    82,    97,    43,    61,    74,    83,    96,
      88,    74,    88,    77,    88,    92,     7,    19,    20,     0,
      18,    95,    73,    75,    73,    77,    73,    75,    75,    75,
      75,    97,     5,     6,    74,    81,    73,     8,    36,    96,
       6,    12,    13,    15,    89,    35,    22,    81,    89,    62,
      38,    38,    96,    88,     9,     8,    88,    96,    88,    81,
      92,    82,    73,    73,     6,    14,    37,    40,    45,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    85,
      87,    93,    84,    98,    41,    88,    93,    40,    35,    94,
      10,    35,    82,    15,    93,    16,    86,    11,    98,    85
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
        case 2:

/* Line 1455 of yacc.c  */
#line 465 "RCL.y"
    { (yyval.program_) = make_PVoid(); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 466 "RCL.y"
    { (yyval.program_) = make_Prog1((yyvsp[(2) - (6)].listpreprocessor_), (yyvsp[(4) - (6)].listdefinition_), (yyvsp[(6) - (6)].operation_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 467 "RCL.y"
    { (yyval.program_) = make_Prog2((yyvsp[(2) - (4)].listpreprocessor_), (yyvsp[(4) - (4)].listdefinition_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 468 "RCL.y"
    { (yyval.program_) = make_Prog3((yyvsp[(2) - (4)].listpreprocessor_), (yyvsp[(4) - (4)].operation_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 469 "RCL.y"
    { (yyval.program_) = make_Prog4((yyvsp[(2) - (4)].listdefinition_), (yyvsp[(4) - (4)].operation_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 470 "RCL.y"
    { (yyval.program_) = make_Prog5((yyvsp[(2) - (2)].listdefinition_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 471 "RCL.y"
    { (yyval.program_) = make_Prog6((yyvsp[(2) - (2)].operation_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 472 "RCL.y"
    { (yyval.program_) = make_Prog7((yyvsp[(1) - (1)].operation_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 473 "RCL.y"
    { (yyval.program_) = make_Prog8((yyvsp[(2) - (2)].listpreprocessor_)); YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 475 "RCL.y"
    { (yyval.identifier_) = make_Name((yyvsp[(1) - (1)].string_));  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 476 "RCL.y"
    { (yyval.identifier_) = make_QualName((yyvsp[(1) - (3)].string_), (yyvsp[(3) - (3)].identifier_));  ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 478 "RCL.y"
    { (yyval.preprocessor_) = make_Import((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 479 "RCL.y"
    { (yyval.preprocessor_) = make_ImportAs((yyvsp[(2) - (4)].string_), (yyvsp[(4) - (4)].string_));  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 480 "RCL.y"
    { (yyval.preprocessor_) = make_Include((yyvsp[(2) - (4)].string_), (yyvsp[(4) - (4)].string_));  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 481 "RCL.y"
    { (yyval.preprocessor_) = make_Define((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 482 "RCL.y"
    { (yyval.preprocessor_) = make_IF((yyvsp[(2) - (4)].string_), (yyvsp[(4) - (4)].preprocessor_));  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 483 "RCL.y"
    { (yyval.preprocessor_) = make_ELSE((yyvsp[(2) - (6)].string_), (yyvsp[(4) - (6)].preprocessor_), (yyvsp[(6) - (6)].preprocessor_));  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 484 "RCL.y"
    { (yyval.preprocessor_) = make_Error((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 485 "RCL.y"
    { (yyval.preprocessor_) = make_Warning((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 486 "RCL.y"
    { (yyval.preprocessor_) = make_Info((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 487 "RCL.y"
    { (yyval.preprocessor_) = make_FaultInt((yyvsp[(2) - (2)].int_));  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 488 "RCL.y"
    { (yyval.preprocessor_) = make_FaultId((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 490 "RCL.y"
    { (yyval.definition_) = make_Def((yyvsp[(1) - (3)].string_), (yyvsp[(3) - (3)].operation_));  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 491 "RCL.y"
    { (yyval.definition_) = make_Extern((yyvsp[(2) - (7)].identifier_), (yyvsp[(4) - (7)].listffi_type_signature_), (yyvsp[(7) - (7)].ffi_type_signature_));  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 492 "RCL.y"
    { (yyval.definition_) = make_Structure((yyvsp[(2) - (4)].string_), (yyvsp[(4) - (4)].listrcl_structure_field_));  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 494 "RCL.y"
    { (yyval.rcl_structure_field_) = make_FreeField((yyvsp[(2) - (2)].string_));  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 495 "RCL.y"
    { (yyval.rcl_structure_field_) = make_EnumField((yyvsp[(2) - (3)].string_));  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 496 "RCL.y"
    { (yyval.rcl_structure_field_) = make_SpecField((yyvsp[(2) - (4)].string_), (yyvsp[(4) - (4)].identifier_));  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 498 "RCL.y"
    { (yyval.ffi_type_signature_) = make_TPrimitive((yyvsp[(1) - (1)].ffi_type_signaturet_));  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 499 "RCL.y"
    { (yyval.ffi_type_signature_) = make_TStruct((yyvsp[(2) - (3)].listffi_type_signature_));  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 500 "RCL.y"
    { (yyval.ffi_type_signature_) = make_TSSelf();  ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 501 "RCL.y"
    { (yyval.ffi_type_signature_) = make_TPtr((yyvsp[(1) - (2)].ffi_type_signature_), reverseListPtr((yyvsp[(2) - (2)].listptr_)));  ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 503 "RCL.y"
    { (yyval.ptr_) = make_Point();  ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 505 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TVoid();  ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 506 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUint8();  ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 507 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSint8();  ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 508 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUint16();  ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 509 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSint16();  ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 510 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUint32();  ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 511 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSint32();  ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 512 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUint64();  ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 513 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSint64();  ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 514 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TFloat();  ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 515 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TLongDouble();  ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 516 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TDouble();  ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 517 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUchar();  ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 518 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSchar();  ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 519 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUshort();  ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 520 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSshort();  ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 521 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUint();  ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 522 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSint();  ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 523 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_Tint();  ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 524 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TUlong();  ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 525 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TSlong();  ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 526 "RCL.y"
    { (yyval.ffi_type_signaturet_) = make_TAny();  ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 528 "RCL.y"
    { (yyval.operation_) = make_Concatenation((yyvsp[(1) - (1)].listoperation_));  ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 530 "RCL.y"
    { (yyval.operation_) = make_RepeatOperation((yyvsp[(2) - (3)].int_), (yyvsp[(3) - (3)].operation_));  ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 531 "RCL.y"
    { (yyval.operation_) = make_ParallelConcat((yyvsp[(1) - (3)].operation_), (yyvsp[(3) - (3)].operation_));  ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 532 "RCL.y"
    { (yyval.operation_) = make_Lambda((yyvsp[(1) - (2)].string_));  ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 533 "RCL.y"
    { (yyval.operation_) = make_EndLambdaScope((yyvsp[(1) - (2)].string_));  ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 534 "RCL.y"
    { (yyval.operation_) = make_Quote((yyvsp[(2) - (3)].operation_));  ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 535 "RCL.y"
    { (yyval.operation_) = make_List((yyvsp[(2) - (3)].listoperation_));  ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 536 "RCL.y"
    { (yyval.operation_) = make_Var((yyvsp[(1) - (1)].identifier_));  ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 537 "RCL.y"
    { (yyval.operation_) = make_Lit((yyvsp[(1) - (1)].data_));  ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 538 "RCL.y"
    { (yyval.operation_) = make_BasicLiteralOp((yyvsp[(1) - (1)].literaloperation_));  ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 539 "RCL.y"
    { (yyval.operation_) = make_Field((yyvsp[(1) - (1)].rcl_structure_field_));  ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 541 "RCL.y"
    { (yyval.literaloperation_) = make_Add();  ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 542 "RCL.y"
    { (yyval.literaloperation_) = make_Sub();  ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 543 "RCL.y"
    { (yyval.literaloperation_) = make_Pow();  ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 544 "RCL.y"
    { (yyval.literaloperation_) = make_Mul();  ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 545 "RCL.y"
    { (yyval.literaloperation_) = make_Div();  ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 546 "RCL.y"
    { (yyval.literaloperation_) = make_Mod();  ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 547 "RCL.y"
    { (yyval.literaloperation_) = make_Inc();  ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 548 "RCL.y"
    { (yyval.literaloperation_) = make_Dec();  ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 549 "RCL.y"
    { (yyval.literaloperation_) = make_BAnd();  ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 550 "RCL.y"
    { (yyval.literaloperation_) = make_BOr();  ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 551 "RCL.y"
    { (yyval.literaloperation_) = make_BXor();  ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 552 "RCL.y"
    { (yyval.literaloperation_) = make_BLeft();  ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 553 "RCL.y"
    { (yyval.literaloperation_) = make_BRight();  ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 555 "RCL.y"
    { (yyval.data_) = make_Int((yyvsp[(1) - (1)].int_));  ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 556 "RCL.y"
    { (yyval.data_) = make_Flt((yyvsp[(1) - (1)].double_));  ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 557 "RCL.y"
    { (yyval.data_) = make_Chr((yyvsp[(1) - (1)].char_));  ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 558 "RCL.y"
    { (yyval.data_) = make_Str((yyvsp[(1) - (1)].string_));  ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 560 "RCL.y"
    { (yyval.listoperation_) = 0;  ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 561 "RCL.y"
    { (yyval.listoperation_) = make_ListOperation((yyvsp[(1) - (1)].operation_), 0);  ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 562 "RCL.y"
    { (yyval.listoperation_) = make_ListOperation((yyvsp[(1) - (3)].operation_), (yyvsp[(3) - (3)].listoperation_));  ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 564 "RCL.y"
    { (yyval.listffi_type_signature_) = 0;  ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 565 "RCL.y"
    { (yyval.listffi_type_signature_) = make_ListFFI_Type_Signature((yyvsp[(1) - (1)].ffi_type_signature_), 0);  ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 566 "RCL.y"
    { (yyval.listffi_type_signature_) = make_ListFFI_Type_Signature((yyvsp[(1) - (3)].ffi_type_signature_), (yyvsp[(3) - (3)].listffi_type_signature_));  ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 568 "RCL.y"
    { (yyval.listptr_) = 0;  ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 569 "RCL.y"
    { (yyval.listptr_) = make_ListPtr((yyvsp[(2) - (2)].ptr_), (yyvsp[(1) - (2)].listptr_));  ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 571 "RCL.y"
    { (yyval.listoperation_) = make_ListOperation((yyvsp[(1) - (1)].operation_), 0);  ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 572 "RCL.y"
    { (yyval.listoperation_) = make_ListOperation((yyvsp[(1) - (2)].operation_), (yyvsp[(2) - (2)].listoperation_));  ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 574 "RCL.y"
    { (yyval.listdefinition_) = make_ListDefinition((yyvsp[(1) - (1)].definition_), 0);  ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 575 "RCL.y"
    { (yyval.listdefinition_) = make_ListDefinition((yyvsp[(1) - (2)].definition_), (yyvsp[(2) - (2)].listdefinition_));  ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 576 "RCL.y"
    { (yyval.listdefinition_) = 0;  ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 577 "RCL.y"
    { (yyval.listdefinition_) = make_ListDefinition((yyvsp[(1) - (3)].definition_), (yyvsp[(3) - (3)].listdefinition_));  ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 579 "RCL.y"
    { (yyval.listpreprocessor_) = make_ListPreprocessor((yyvsp[(1) - (1)].preprocessor_), 0);  ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 580 "RCL.y"
    { (yyval.listpreprocessor_) = make_ListPreprocessor((yyvsp[(1) - (2)].preprocessor_), (yyvsp[(2) - (2)].listpreprocessor_));  ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 582 "RCL.y"
    { (yyval.listrcl_structure_field_) = make_ListRCL_Structure_Field((yyvsp[(1) - (1)].rcl_structure_field_), 0);  ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 583 "RCL.y"
    { (yyval.listrcl_structure_field_) = make_ListRCL_Structure_Field((yyvsp[(1) - (3)].rcl_structure_field_), (yyvsp[(3) - (3)].listrcl_structure_field_));  ;}
    break;



/* Line 1455 of yacc.c  */
#line 2636 "Parser.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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
      if (yyn != YYPACT_NINF)
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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



