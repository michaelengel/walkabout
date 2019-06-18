#define YY_InstrmParser_h_included

/*  A Bison++ parser, made from dynamic/tools/instrm/instrmparser.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */


#line 1 "/usr/local/lib/bison.cc"
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/* #line 73 "/usr/local/lib/bison.cc" */
#line 85 "dynamic/tools/instrm/instrm_parser.cc"
#line 20 "dynamic/tools/instrm/instrmparser.y"

// #include "global.h"

#include <iostream>
#include <strstream>
#include <string>
#include <set>
#include <list>
#include <map>

class InstrmScanner;

#define T_CODE 1
#define T_SSL 2
#define T_SEMANTICS 3
#define T_PARAM 4

class table_code {	// instrument code for all the instructions within the table
public:
	// table_code() { tableset = new set<string>; }
	set<string>					tableset;			// all instructions belowing to the table
	list< pair<int, void*> > 	instrm_code;		// the instrumented code for this table
	// string 					before, after;
	list<string> 				*param_list;
};


#line 54 "dynamic/tools/instrm/instrmparser.y"
typedef union {
    char*			str;
    int             num;
	list<string>*	lstring;
} yy_InstrmParser_stype;
#define YY_InstrmParser_STYPE yy_InstrmParser_stype
#line 60 "dynamic/tools/instrm/instrmparser.y"

#include "instrm_scanner.h"
#define YY_InstrmParser_DEBUG  1 
#define YY_InstrmParser_PARSE_PARAM  \

#define YY_InstrmParser_CONSTRUCTOR_PARAM  \
    const string& instrmFile, \
    bool trace
#define YY_InstrmParser_CONSTRUCTOR_CODE  \
    FILE* inFile = fopen(instrmFile.c_str(),"r"); \
    if (inFile == NULL) { \
        ostrstream ost; \
        ost << "can't open `" << instrmFile << "' for reading"; \
        abort(); \
    } \
    Scanner = new InstrmScanner(inFile, trace); \
    if (trace) yydebug = 1;
#define YY_InstrmParser_MEMBERS  \
public: \
        virtual ~InstrmParser(); \
		map<string, table_code*> table2icode; \
		map<string, table_code*> inst2icode;  \
		string support_code; \
protected: \
\
    /** \
     * The scanner. \
     */ \
    InstrmScanner* Scanner; \
	table_code				 *current_table;	\
	// use to hold current instructions in the table

#line 73 "/usr/local/lib/bison.cc"
/* %{ and %header{ and %union, during decl */
#define YY_InstrmParser_BISON 1
#ifndef YY_InstrmParser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_InstrmParser_COMPATIBILITY 1
#else
#define  YY_InstrmParser_COMPATIBILITY 0
#endif
#endif

#if YY_InstrmParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_InstrmParser_LTYPE
#define YY_InstrmParser_LTYPE YYLTYPE
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_InstrmParser_STYPE 
#define YY_InstrmParser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_InstrmParser_DEBUG
#define  YY_InstrmParser_DEBUG YYDEBUG
#endif
#endif
#ifdef YY_InstrmParser_STYPE
#ifndef yystype
#define yystype YY_InstrmParser_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_InstrmParser_USE_GOTO
#define YY_InstrmParser_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_InstrmParser_USE_GOTO
#define YY_InstrmParser_USE_GOTO 0
#endif

#ifndef YY_InstrmParser_PURE

/* #line 117 "/usr/local/lib/bison.cc" */
#line 201 "dynamic/tools/instrm/instrm_parser.cc"

#line 117 "/usr/local/lib/bison.cc"
/*  YY_InstrmParser_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */

/* #line 121 "/usr/local/lib/bison.cc" */
#line 210 "dynamic/tools/instrm/instrm_parser.cc"

#line 121 "/usr/local/lib/bison.cc"
/* prefix */
#ifndef YY_InstrmParser_DEBUG

/* #line 123 "/usr/local/lib/bison.cc" */
#line 217 "dynamic/tools/instrm/instrm_parser.cc"

#line 123 "/usr/local/lib/bison.cc"
/* YY_InstrmParser_DEBUG */
#endif


#ifndef YY_InstrmParser_LSP_NEEDED

/* #line 128 "/usr/local/lib/bison.cc" */
#line 227 "dynamic/tools/instrm/instrm_parser.cc"

#line 128 "/usr/local/lib/bison.cc"
 /* YY_InstrmParser_LSP_NEEDED*/
#endif



/* DEFAULT LTYPE*/
#ifdef YY_InstrmParser_LSP_NEEDED
#ifndef YY_InstrmParser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_InstrmParser_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
      /* We used to use `unsigned long' as YY_InstrmParser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

#ifndef YY_InstrmParser_STYPE
#define YY_InstrmParser_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_InstrmParser_PARSE
#define YY_InstrmParser_PARSE yyparse
#endif
#ifndef YY_InstrmParser_LEX
#define YY_InstrmParser_LEX yylex
#endif
#ifndef YY_InstrmParser_LVAL
#define YY_InstrmParser_LVAL yylval
#endif
#ifndef YY_InstrmParser_LLOC
#define YY_InstrmParser_LLOC yylloc
#endif
#ifndef YY_InstrmParser_CHAR
#define YY_InstrmParser_CHAR yychar
#endif
#ifndef YY_InstrmParser_NERRS
#define YY_InstrmParser_NERRS yynerrs
#endif
#ifndef YY_InstrmParser_DEBUG_FLAG
#define YY_InstrmParser_DEBUG_FLAG yydebug
#endif
#ifndef YY_InstrmParser_ERROR
#define YY_InstrmParser_ERROR yyerror
#endif
#ifndef YY_InstrmParser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_InstrmParser_PARSE_PARAM
#ifndef YY_InstrmParser_PARSE_PARAM_DEF
#define YY_InstrmParser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_InstrmParser_PARSE_PARAM
#define YY_InstrmParser_PARSE_PARAM void
#endif
#endif
#if YY_InstrmParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_InstrmParser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_InstrmParser_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_InstrmParser_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_InstrmParser_PURE
#ifndef YYPURE
#define YYPURE YY_InstrmParser_PURE
#endif
#endif
#ifdef YY_InstrmParser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_InstrmParser_DEBUG 
#endif
#endif
#ifndef YY_InstrmParser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_InstrmParser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_InstrmParser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_InstrmParser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */

/* #line 236 "/usr/local/lib/bison.cc" */
#line 340 "dynamic/tools/instrm/instrm_parser.cc"
#define	INSTRUMENTATION_ROUTINES	258
#define	FETCHEXECUTE	259
#define	DEFINITION	260
#define	NAME	261
#define	SSL	262
#define	PARAM	263
#define	C_CODE	264
#define	INSTRM_END	265
#define	SSL_INST_SEMANTICS	266


#line 236 "/usr/local/lib/bison.cc"
 /* #defines tokens */
#else
/* CLASS */
#ifndef YY_InstrmParser_CLASS
#define YY_InstrmParser_CLASS InstrmParser
#endif
#ifndef YY_InstrmParser_INHERIT
#define YY_InstrmParser_INHERIT
#endif
#ifndef YY_InstrmParser_MEMBERS
#define YY_InstrmParser_MEMBERS 
#endif
#ifndef YY_InstrmParser_LEX_BODY
#define YY_InstrmParser_LEX_BODY  
#endif
#ifndef YY_InstrmParser_ERROR_BODY
#define YY_InstrmParser_ERROR_BODY  
#endif
#ifndef YY_InstrmParser_CONSTRUCTOR_PARAM
#define YY_InstrmParser_CONSTRUCTOR_PARAM
#endif
#ifndef YY_InstrmParser_CONSTRUCTOR_CODE
#define YY_InstrmParser_CONSTRUCTOR_CODE
#endif
#ifndef YY_InstrmParser_CONSTRUCTOR_INIT
#define YY_InstrmParser_CONSTRUCTOR_INIT
#endif
/* choose between enum and const */
#ifndef YY_InstrmParser_USE_CONST_TOKEN
#define YY_InstrmParser_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_InstrmParser_USE_CONST_TOKEN != 0
#ifndef YY_InstrmParser_ENUM_TOKEN
#define YY_InstrmParser_ENUM_TOKEN yy_InstrmParser_enum_token
#endif
#endif

class YY_InstrmParser_CLASS YY_InstrmParser_INHERIT
{
public: 
#if YY_InstrmParser_USE_CONST_TOKEN != 0
/* static const int token ... */

/* #line 280 "/usr/local/lib/bison.cc" */
#line 399 "dynamic/tools/instrm/instrm_parser.cc"
static const int INSTRUMENTATION_ROUTINES;
static const int FETCHEXECUTE;
static const int DEFINITION;
static const int NAME;
static const int SSL;
static const int PARAM;
static const int C_CODE;
static const int INSTRM_END;
static const int SSL_INST_SEMANTICS;


#line 280 "/usr/local/lib/bison.cc"
 /* decl const */
#else
enum YY_InstrmParser_ENUM_TOKEN { YY_InstrmParser_NULL_TOKEN=0

/* #line 283 "/usr/local/lib/bison.cc" */
#line 417 "dynamic/tools/instrm/instrm_parser.cc"
	,INSTRUMENTATION_ROUTINES=258
	,FETCHEXECUTE=259
	,DEFINITION=260
	,NAME=261
	,SSL=262
	,PARAM=263
	,C_CODE=264
	,INSTRM_END=265
	,SSL_INST_SEMANTICS=266


#line 283 "/usr/local/lib/bison.cc"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_InstrmParser_PARSE (YY_InstrmParser_PARSE_PARAM);
 virtual void YY_InstrmParser_ERROR(char *msg) YY_InstrmParser_ERROR_BODY;
#ifdef YY_InstrmParser_PURE
#ifdef YY_InstrmParser_LSP_NEEDED
 virtual int  YY_InstrmParser_LEX (YY_InstrmParser_STYPE *YY_InstrmParser_LVAL,YY_InstrmParser_LTYPE *YY_InstrmParser_LLOC) YY_InstrmParser_LEX_BODY;
#else
 virtual int  YY_InstrmParser_LEX (YY_InstrmParser_STYPE *YY_InstrmParser_LVAL) YY_InstrmParser_LEX_BODY;
#endif
#else
 virtual int YY_InstrmParser_LEX() YY_InstrmParser_LEX_BODY;
 YY_InstrmParser_STYPE YY_InstrmParser_LVAL;
#ifdef YY_InstrmParser_LSP_NEEDED
 YY_InstrmParser_LTYPE YY_InstrmParser_LLOC;
#endif
 int   YY_InstrmParser_NERRS;
 int    YY_InstrmParser_CHAR;
#endif
#if YY_InstrmParser_DEBUG != 0
 int YY_InstrmParser_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_InstrmParser_CLASS(YY_InstrmParser_CONSTRUCTOR_PARAM);
public:
 YY_InstrmParser_MEMBERS 
};
/* other declare folow */
#if YY_InstrmParser_USE_CONST_TOKEN != 0

/* #line 314 "/usr/local/lib/bison.cc" */
#line 463 "dynamic/tools/instrm/instrm_parser.cc"
const int YY_InstrmParser_CLASS::INSTRUMENTATION_ROUTINES=258;
const int YY_InstrmParser_CLASS::FETCHEXECUTE=259;
const int YY_InstrmParser_CLASS::DEFINITION=260;
const int YY_InstrmParser_CLASS::NAME=261;
const int YY_InstrmParser_CLASS::SSL=262;
const int YY_InstrmParser_CLASS::PARAM=263;
const int YY_InstrmParser_CLASS::C_CODE=264;
const int YY_InstrmParser_CLASS::INSTRM_END=265;
const int YY_InstrmParser_CLASS::SSL_INST_SEMANTICS=266;


#line 314 "/usr/local/lib/bison.cc"
 /* const YY_InstrmParser_CLASS::token */
#endif
/*apres const  */
YY_InstrmParser_CLASS::YY_InstrmParser_CLASS(YY_InstrmParser_CONSTRUCTOR_PARAM) YY_InstrmParser_CONSTRUCTOR_INIT
{
#if YY_InstrmParser_DEBUG != 0
YY_InstrmParser_DEBUG_FLAG=0;
#endif
YY_InstrmParser_CONSTRUCTOR_CODE;
};
#endif

/* #line 325 "/usr/local/lib/bison.cc" */
#line 489 "dynamic/tools/instrm/instrm_parser.cc"


#define	YYFINAL		48
#define	YYFLAG		-32768
#define	YYNTBASE	19

#define YYTRANSLATE(x) ((unsigned)(x) <= 266 ? yytranslate[x] : 36)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    15,     2,     2,    18,     2,     2,     2,
    17,     2,     2,    14,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    12,     2,    13,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    16,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11
};

#if YY_InstrmParser_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     5,     8,    10,    12,    15,    17,    19,    21,
    22,    28,    32,    34,    38,    39,    44,    45,    49,    53,
    55,    59,    62,    64,    68,    73,    75
};

static const short yyrhs[] = {    19,
    20,     0,    20,     0,     5,    21,     0,    29,     0,    35,
     0,    21,    22,     0,    22,     0,    23,     0,    27,     0,
     0,     6,    12,    24,    25,    13,     0,    26,    14,    25,
     0,    26,     0,    15,     6,    15,     0,     0,     6,    31,
    28,    32,     0,     0,     4,    30,    32,     0,    31,    14,
     6,     0,     6,     0,    16,    33,    10,     0,    33,    34,
     0,    34,     0,     8,     6,    17,     0,     7,    18,     6,
    17,     0,    11,     0,     3,     9,     0
};

#endif

#if YY_InstrmParser_DEBUG != 0
static const short yyrline[] = { 0,
   109,   111,   114,   116,   117,   120,   122,   125,   127,   130,
   140,   152,   157,   162,   169,   188,   190,   200,   203,   209,
   273,   280,   282,   285,   306,   315,   332
};

static const char * const yytname[] = {   "$","error","$illegal.","INSTRUMENTATION_ROUTINES",
"FETCHEXECUTE","DEFINITION","NAME","SSL","PARAM","C_CODE","INSTRM_END","SSL_INST_SEMANTICS",
"'['","']'","','","'\"'","'{'","')'","'%'","specification","parts","definition",
"definition_part","table_assign","@1","insts","inst","semantics","@2","fetch_execute",
"@3","list_parameter","instrm_code","instrm_body","instrm_body_part","support_code",
""
};
#endif

static const short yyr1[] = {     0,
    19,    19,    20,    20,    20,    21,    21,    22,    22,    24,
    23,    25,    25,    26,    28,    27,    30,    29,    31,    31,
    32,    33,    33,    34,    34,    34,    35
};

static const short yyr2[] = {     0,
     2,     1,     2,     1,     1,     2,     1,     1,     1,     0,
     5,     3,     1,     3,     0,     4,     0,     3,     3,     1,
     3,     2,     1,     3,     4,     1,     2
};

static const short yydefact[] = {     0,
     0,    17,     0,     0,     2,     4,     5,    27,     0,     0,
     3,     7,     8,     9,     1,     0,    18,    20,    10,    15,
     6,     0,     0,    26,     0,    23,     0,     0,     0,     0,
     0,    21,    22,     0,     0,    13,    19,    16,     0,    24,
     0,    11,     0,    25,    14,    12,     0,     0
};

static const short yydefgoto[] = {     4,
     5,    11,    12,    13,    27,    35,    36,    14,    29,     6,
     9,    20,    17,    25,    26,     7
};

static const short yypact[] = {    -2,
    -4,-32768,     6,     4,-32768,-32768,-32768,-32768,     1,    -6,
     6,-32768,-32768,-32768,-32768,     8,-32768,-32768,-32768,     7,
-32768,     0,    14,-32768,     3,-32768,     9,    16,     1,    17,
    10,-32768,-32768,    19,    13,    15,-32768,-32768,    11,-32768,
    18,-32768,     9,-32768,-32768,-32768,    30,-32768
};

static const short yypgoto[] = {-32768,
    27,-32768,    21,-32768,-32768,    -9,-32768,-32768,-32768,-32768,
-32768,-32768,    12,-32768,    20,-32768
};


#define	YYLAST		45


static const short yytable[] = {    18,
     1,     2,     3,    47,     8,    19,     1,     2,     3,    22,
    23,    10,    32,    24,    22,    23,    16,    30,    24,    31,
    28,    37,    39,    34,    41,    42,    40,    44,    43,    48,
    15,    21,    45,    46,     0,     0,     0,     0,     0,     0,
    38,     0,     0,     0,    33
};

static const short yycheck[] = {     6,
     3,     4,     5,     0,     9,    12,     3,     4,     5,     7,
     8,     6,    10,    11,     7,     8,    16,    18,    11,     6,
    14,     6,     6,    15,     6,    13,    17,    17,    14,     0,
     4,    11,    15,    43,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    -1,    -1,    -1,    25
};

#line 325 "/usr/local/lib/bison.cc"
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_InstrmParser_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_InstrmParser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_InstrmParser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_InstrmParser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_InstrmParser_CHAR = (token), YY_InstrmParser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_InstrmParser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_InstrmParser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_InstrmParser_PURE
/* UNPURE */
#define YYLEX           YY_InstrmParser_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_InstrmParser_CHAR;                      /*  the lookahead symbol        */
YY_InstrmParser_STYPE      YY_InstrmParser_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_InstrmParser_NERRS;                 /*  number of parse errors so far */
#ifdef YY_InstrmParser_LSP_NEEDED
YY_InstrmParser_LTYPE YY_InstrmParser_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_InstrmParser_LSP_NEEDED
#define YYLEX           YY_InstrmParser_LEX(&YY_InstrmParser_LVAL, &YY_InstrmParser_LLOC)
#else
#define YYLEX           YY_InstrmParser_LEX(&YY_InstrmParser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_InstrmParser_DEBUG != 0
int YY_InstrmParser_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_InstrmParser_CLASS::
#endif
     YY_InstrmParser_PARSE(YY_InstrmParser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_InstrmParser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_InstrmParser_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_InstrmParser_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_InstrmParser_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_InstrmParser_LSP_NEEDED
  YY_InstrmParser_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_InstrmParser_LTYPE *yyls = yylsa;
  YY_InstrmParser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_InstrmParser_PURE
  int YY_InstrmParser_CHAR;
  YY_InstrmParser_STYPE YY_InstrmParser_LVAL;
  int YY_InstrmParser_NERRS;
#ifdef YY_InstrmParser_LSP_NEEDED
  YY_InstrmParser_LTYPE YY_InstrmParser_LLOC;
#endif
#endif

  YY_InstrmParser_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_InstrmParser_NERRS = 0;
  YY_InstrmParser_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_InstrmParser_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_InstrmParser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_InstrmParser_LSP_NEEDED
      YY_InstrmParser_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_InstrmParser_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_InstrmParser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_InstrmParser_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_InstrmParser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_InstrmParser_LSP_NEEDED
      yyls = (YY_InstrmParser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_InstrmParser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_InstrmParser_DEBUG != 0
      if (YY_InstrmParser_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_InstrmParser_CHAR == YYEMPTY)
    {
#if YY_InstrmParser_DEBUG != 0
      if (YY_InstrmParser_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_InstrmParser_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_InstrmParser_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_InstrmParser_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_InstrmParser_DEBUG != 0
      if (YY_InstrmParser_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_InstrmParser_CHAR);

#if YY_InstrmParser_DEBUG != 0
      if (YY_InstrmParser_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_InstrmParser_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_InstrmParser_CHAR, YY_InstrmParser_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_InstrmParser_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_InstrmParser_CHAR != YYEOF)
    YY_InstrmParser_CHAR = YYEMPTY;

  *++yyvsp = YY_InstrmParser_LVAL;
#ifdef YY_InstrmParser_LSP_NEEDED
  *++yylsp = YY_InstrmParser_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


/* #line 811 "/usr/local/lib/bison.cc" */
#line 1111 "dynamic/tools/instrm/instrm_parser.cc"

  switch (yyn) {

case 10:
#line 131 "dynamic/tools/instrm/instrmparser.y"
{
			if (table2icode.find(yyvsp[-1].str) != table2icode.end()) {
				cerr << "Duplicate table name - " << yyvsp[-1].str << endl;
				abort();
			}
			// create the table
			table2icode[yyvsp[-1].str] = current_table = new table_code;
			//tableset = new set<string>;
						
		;
    break;}
case 11:
#line 140 "dynamic/tools/instrm/instrmparser.y"
{
			// create the table name and add the list of instructions
			//table2icode[$1]->tableset = tableset;
			
			for (set<string>::iterator i=current_table->tableset.begin(); 
				i!=current_table->tableset.end(); i++) {
				inst2icode[*i] = current_table;
				
			}
        ;
    break;}
case 12:
#line 153 "dynamic/tools/instrm/instrmparser.y"
{
			current_table->tableset.insert(yyvsp[-2].str);
        ;
    break;}
case 13:
#line 157 "dynamic/tools/instrm/instrmparser.y"
{
			current_table->tableset.insert(yyvsp[0].str);	
		;
    break;}
case 14:
#line 163 "dynamic/tools/instrm/instrmparser.y"
{
        	yyval.str = yyvsp[-1].str;
		;
    break;}
case 15:
#line 170 "dynamic/tools/instrm/instrmparser.y"
{
		
			// $1 with the SSL??  no, can have same name but different number 
			// of instr in the table.
			
			// check parameter? hmm possibly need to make sure each 
			// instruction here corresponds exactly as it is specified in 
			// the SSL.
			
			if (table2icode.find(yyvsp[-1].str) == table2icode.end()) {
				cerr << yyvsp[-1].str << "Not found." << endl;
				abort();
			}
			current_table = table2icode[yyvsp[-1].str];
 			current_table->param_list = yyvsp[0].lstring;
 			// current_table->instrm_code = $3;

        ;
    break;}
case 17:
#line 191 "dynamic/tools/instrm/instrmparser.y"
{
			if (table2icode.find("FETCHEXECUTE") != table2icode.end()) {
				cerr << "More than one fetch_execute defined." << endl;
				abort();
			}
			current_table = table2icode["FETCHEXECUTE"] = 
				inst2icode["FETCHEXECUTE"] = new table_code;;

		;
    break;}
case 19:
#line 204 "dynamic/tools/instrm/instrmparser.y"
{
            yyvsp[-2].lstring->push_back(yyvsp[0].str);
            yyval.lstring = yyvsp[-2].lstring;
        ;
    break;}
case 20:
#line 209 "dynamic/tools/instrm/instrmparser.y"
{
    		yyval.lstring = new list<string>;
			yyval.lstring->push_back(yyvsp[0].str);
        ;
    break;}
case 21:
#line 274 "dynamic/tools/instrm/instrmparser.y"
{
			yyvsp[0].str[strlen(yyvsp[0].str)-2] = 0;	// remove the last '}'
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, yyvsp[0].str));
		;
    break;}
case 24:
#line 286 "dynamic/tools/instrm/instrmparser.y"
{			
			yyvsp[-2].str[strlen(yyvsp[-2].str)-6] = 0;	// remove the last 'PARAM('
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, yyvsp[-2].str));
			// look up the parameter list for this table and determined the 
			// parameter number
			int param_num = 0;
			list<string>::iterator i;			
			for (i=current_table->param_list->begin(); i!=current_table->param_list->end(); 
					i++, param_num++) {
				if ((*i) == yyvsp[-1].str) {
					current_table->instrm_code.push_back(
						pair<int, void*>(T_PARAM, (void*)param_num));
					break;
				}
			}
			if (i == current_table->param_list->end()) {
				cerr << "Parameter " << yyvsp[-1].str << " undefined." << endl;
			}
		;
    break;}
case 25:
#line 306 "dynamic/tools/instrm/instrmparser.y"
{
			yyvsp[-3].str[strlen(yyvsp[-3].str)-4] = 0;	// remove the last 'SSL('
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, yyvsp[-3].str));
			// cheat by prepending 'regs.r' to $3
			char* t = (char*)malloc(sizeof(char)*12);
			memcpy(t, "regs.r_", 8);
			current_table->instrm_code.push_back(pair<int, void*>(T_SSL, strcat(t, yyvsp[-1].str)));
		;
    break;}
case 26:
#line 315 "dynamic/tools/instrm/instrmparser.y"
{
			yyvsp[0].str[strlen(yyvsp[0].str)-19] = 0;	// remove the last 'SSL_INST_SEMANTICS'
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, yyvsp[0].str));
			current_table->instrm_code.push_back(pair<int, void*>(T_SEMANTICS, 0));
		;
    break;}
case 27:
#line 333 "dynamic/tools/instrm/instrmparser.y"
{
			support_code = yyvsp[0].str;
		;
    break;}
}

#line 811 "/usr/local/lib/bison.cc"
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_InstrmParser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_InstrmParser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_InstrmParser_LLOC.first_line;
      yylsp->first_column = YY_InstrmParser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_InstrmParser_NERRS;

#ifdef YY_InstrmParser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_InstrmParser_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_InstrmParser_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_InstrmParser_ERROR_VERBOSE */
	YY_InstrmParser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_InstrmParser_CHAR == YYEOF)
	YYABORT;

#if YY_InstrmParser_DEBUG != 0
      if (YY_InstrmParser_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_InstrmParser_CHAR, yytname[yychar1]);
#endif

      YY_InstrmParser_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_InstrmParser_LSP_NEEDED
  yylsp--;
#endif

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_InstrmParser_DEBUG != 0
  if (YY_InstrmParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_InstrmParser_LVAL;
#ifdef YY_InstrmParser_LSP_NEEDED
  *++yylsp = YY_InstrmParser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */

/* #line 1010 "/usr/local/lib/bison.cc" */
#line 1464 "dynamic/tools/instrm/instrm_parser.cc"
#line 338 "dynamic/tools/instrm/instrmparser.y"


InstrmParser::~InstrmParser()
{
    if (Scanner != NULL)
        delete Scanner;
}

void InstrmParser::yyerror(char* msg)
{
    cerr << "InstrmFile : " << Scanner->theLine << ": " << msg << endl;
	cerr << "Next token is: " << yychar << endl;;
    exit(1);
}

int InstrmParser::yylex()
{
    int token = Scanner->yylex(yylval);
    return token;
}

