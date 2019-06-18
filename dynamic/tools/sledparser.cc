
/*  A Bison parser, made from dynamic/tools/sledparser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	LEQUAL	257
#define	NEQUAL	258
#define	ELLIPSIS	259
#define	RARROW	260
#define	GEQUAL	261
#define	ADDRESS	262
#define	ANY	263
#define	ASSEMBLY	264
#define	BIT	265
#define	COLUMNS	266
#define	COMPONENT	267
#define	CONSTRUCTORS	268
#define	DISCARD	269
#define	ELSE	270
#define	EPSILON	271
#define	FETCH	272
#define	FIELDINFO	273
#define	FIELDS	274
#define	FOR	275
#define	IS	276
#define	KEEP	277
#define	NAMES	278
#define	OF	279
#define	OPCODE	280
#define	OTHERWISE	281
#define	PATTERNS	282
#define	MOST	283
#define	SIGNIFICANT	284
#define	PCUNITBITS	285
#define	PLACEHOLDER	286
#define	RELOCATABLE	287
#define	SOME	288
#define	SPARSE	289
#define	SYNTAX	290
#define	TO	291
#define	TYPE	292
#define	USING	293
#define	WHEN	294
#define	WHICH	295
#define	WORDSIZE	296
#define	NEWLINE	297
#define	CHECKED	298
#define	UNCHECKED	299
#define	GUARANTEED	300
#define	IDENT	301
#define	STRING	302
#define	LABEL	303
#define	FIELDIDENT	304
#define	RELOCIDENT	305
#define	PATTERNIDENT	306
#define	INTEGER	307

#line 11 "dynamic/tools/sledparser.y"

#include "sledtree.h"
#include <vector>
#include <string>

extern int yylineno;
extern bool yyNewLineHack;

#define YYPARSE_PARAM dict
#define DICT (*(SledDict *)dict)

#line 23 "dynamic/tools/sledparser.y"
typedef union {
    string *str;
    vector<string *> *strv;
    FieldIdent *fieldident;
    vector<FieldIdent *> *fiv;
    Expr *expr;
    ListExpr *list;
    ExprOp op;
    BitSlice bitslice;
    char ch;

    Ident *id;
    struct { int pos; string *str; } binding;
    struct { Expr *eq, *pat; } branch;
    OpcodePart opname;
    vector<OpcodePart> *opcode;
    OperandIdent * operand;
    struct { string *asmstr; vector<OperandIdent *> *ops; } operands;
    bool flag;
    struct { FieldChecked checked; vector<string *> *strv; } fielditem;
    int i;
} YYSTYPE;
#line 45 "dynamic/tools/sledparser.y"

    FieldIdent *lt;
    int yyerror( char *str );
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		272
#define	YYFLAG		-32768
#define	YYNTBASE	82

#define YYTRANSLATE(x) ((unsigned)(x) <= 307 ? yytranslate[x] : 144)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,    67,     2,
    68,     2,    69,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,    66,    81,     2,    60,    61,    62,    58,     2,    52,
    53,    56,    54,    64,    55,     2,    57,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    77,    63,    47,
    49,    48,     2,    59,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    50,     2,    51,    80,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    78,    65,    79,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    70,    71,    72,    73,    74,    75,    76
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     4,     6,     8,    10,    12,    15,    21,    24,
    27,    29,    31,    34,    41,    44,    45,    50,    53,    56,
    59,    60,    66,    68,    72,    75,    76,    79,    80,    83,
    84,    86,    88,    90,    95,   100,   104,   106,   110,   113,
   114,   116,   118,   121,   124,   125,   129,   141,   147,   151,
   153,   157,   161,   163,   167,   169,   172,   174,   177,   179,
   181,   183,   185,   187,   190,   193,   197,   201,   206,   210,
   213,   214,   216,   218,   224,   232,   236,   238,   242,   244,
   247,   248,   250,   252,   254,   256,   258,   260,   263,   266,
   267,   275,   278,   280,   284,   286,   288,   290,   292,   296,
   298,   301,   304,   307,   309,   310,   311,   313,   315,   316,
   318,   319,   323,   327,   328,   331,   334,   337,   340,   343,
   346,   347,   349,   351,   353,   355,   357,   359,   361,   363,
   365,   367,   369,   371,   373,   375,   377,   379,   381,   383,
   385,   387,   390,   392,   395,   397,   404,   408,   412,   413,
   416,   417,   421,   423,   427,   429,   433,   437,   441,   443,
   447,   452,   459,   464,   465,   467,   469,   471,   473,   479
};

static const short yyrhs[] = {    82,
    83,     0,     0,    84,     0,    88,     0,   100,     0,   116,
     0,    31,    76,     0,    11,    76,    22,    29,    30,     0,
    23,   119,     0,    15,   119,     0,   142,     0,   143,     0,
    85,    86,     0,    20,    25,    70,    52,    76,    53,     0,
    86,    87,     0,     0,    70,    76,    77,    76,     0,    70,
    76,     0,    19,    89,     0,    89,    90,     0,     0,    91,
    22,    50,    94,    51,     0,    73,     0,    50,    92,    51,
     0,    92,    73,     0,     0,    93,    70,     0,     0,    94,
    95,     0,     0,    44,     0,    45,     0,    46,     0,    35,
    50,    96,    51,     0,    24,    50,    98,    51,     0,    96,
    64,    97,     0,    97,     0,    99,    49,    76,     0,    98,
    99,     0,     0,    71,     0,    70,     0,    28,   101,     0,
   101,   102,     0,     0,    70,    22,   103,     0,    70,    22,
     9,    25,    50,    93,    51,    64,    41,    22,   103,     0,
    50,    93,    51,    22,   103,     0,   103,    65,   104,     0,
   104,     0,   104,    63,   105,     0,   104,    77,   105,     0,
   105,     0,   105,    58,   106,     0,   106,     0,     5,   107,
     0,   107,     0,   108,     5,     0,   108,     0,   109,     0,
    70,     0,   111,     0,    17,     0,    72,   103,     0,    34,
    70,     0,    50,   110,    51,     0,    52,   103,    53,     0,
   121,    52,   113,    53,     0,   111,   115,   112,     0,   110,
   111,     0,     0,    73,     0,    75,     0,    78,    76,    37,
    76,    79,     0,    78,    76,    37,    76,    12,    76,    79,
     0,    50,   114,    51,     0,   139,     0,   113,    64,   139,
     0,   139,     0,   114,    76,     0,     0,    47,     0,    49,
     0,    48,     0,     3,     0,     7,     0,     4,     0,    14,
   117,     0,   117,   118,     0,     0,   125,   120,   122,   129,
   126,   132,   127,     0,   119,   120,     0,   120,     0,   120,
    80,   121,     0,   121,     0,    70,     0,    71,     0,    75,
     0,   122,   123,   130,     0,   130,     0,    70,   124,     0,
    73,   124,     0,    75,   124,     0,    81,     0,     0,     0,
   128,     0,    43,     0,     0,    66,     0,     0,    77,   128,
    70,     0,    77,   128,    75,     0,     0,   130,   131,     0,
   130,    71,     0,   130,    76,     0,   130,     7,     0,   130,
     3,     0,   130,    62,     0,     0,    47,     0,    48,     0,
    49,     0,    50,     0,    51,     0,    52,     0,    53,     0,
    54,     0,    55,     0,    56,     0,    57,     0,    58,     0,
    59,     0,    60,     0,    61,     0,    63,     0,    64,     0,
    65,     0,    66,     0,   133,     0,   135,   136,     0,    43,
     0,   133,   134,     0,   134,     0,    40,    78,   137,    79,
    22,   103,     0,    27,    22,   103,     0,    78,   137,    79,
     0,     0,    22,   103,     0,     0,   137,    64,   138,     0,
   138,     0,   139,   115,   139,     0,    76,     0,    70,   140,
   124,     0,   111,   140,   124,     0,   139,   141,   139,     0,
    71,     0,    52,   139,    53,     0,   121,    52,   113,    53,
     0,    59,    50,    76,    77,    76,    51,     0,    59,    50,
    76,    51,     0,     0,    54,     0,    55,     0,    56,     0,
    57,     0,    32,    21,   111,    22,   103,     0,    33,    93,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    82,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    94,    95,    97,    98,    99,   101,   104,   105,
   105,   106,   120,   122,   124,   126,   128,   129,   130,   134,
   135,   136,   137,   138,   139,   141,   147,   153,   154,   155,
   157,   157,   159,   160,   160,   161,   162,   171,   183,   192,
   193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
   203,   204,   205,   206,   214,   215,   216,   217,   218,   219,
   220,   221,   222,   223,   224,   226,   227,   229,   230,   232,
   233,   235,   236,   237,   238,   239,   240,   243,   244,   245,
   246,   252,   253,   256,   257,   260,   261,   262,   270,   283,
   284,   285,   286,   287,   288,   290,   294,   295,   295,   296,
   296,   297,   298,   299,   300,   301,   309,   310,   311,   312,
   313,   314,   314,   314,   314,   314,   314,   314,   314,   314,
   314,   314,   314,   315,   315,   315,   315,   315,   315,   315,
   317,   318,   319,   320,   321,   322,   323,   325,   326,   327,
   328,   329,   330,   331,   332,   333,   338,   342,   343,   344,
   345,   347,   348,   349,   351,   352,   353,   354,   358,   359
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","LEQUAL",
"NEQUAL","ELLIPSIS","RARROW","GEQUAL","ADDRESS","ANY","ASSEMBLY","BIT","COLUMNS",
"COMPONENT","CONSTRUCTORS","DISCARD","ELSE","EPSILON","FETCH","FIELDINFO","FIELDS",
"FOR","IS","KEEP","NAMES","OF","OPCODE","OTHERWISE","PATTERNS","MOST","SIGNIFICANT",
"PCUNITBITS","PLACEHOLDER","RELOCATABLE","SOME","SPARSE","SYNTAX","TO","TYPE",
"USING","WHEN","WHICH","WORDSIZE","NEWLINE","CHECKED","UNCHECKED","GUARANTEED",
"'<'","'>'","'='","'['","']'","'('","')'","'+'","'-'","'*'","'/'","'&'","'@'",
"'#'","'$'","'%'","';'","','","'|'","' '","'\\t'","'\\v'","'\\r'","IDENT","STRING",
"LABEL","FIELDIDENT","RELOCIDENT","PATTERNIDENT","INTEGER","':'","'{'","'}'",
"'^'","'!'","specs","spec","tokenspec","token","fieldspecs","fieldspec","fieldinfospec",
"fieldinfos","fieldinfo","identbinding","fieldidents","idents","fielditems",
"fielditem","fieldnamebindings","fieldnamebinding","fieldnames","fieldname",
"patternspec","patbindings","patbinding","pattern","disjunct","sequent","conjunct",
"dotsr","atomic","constraint","addressables","addressable","genexpr","arglist",
"integers","relop","constructorspec","constructors","constructor","opcodes",
"opcode","opname","operands","operand","bangOpt","nlhack","nlhackoff","nlOpt",
"wsOpt","typeOpt","noise","noisechar","branchesOpt","branches","branch","equationsopt",
"patternsopt","equations","equation","expr","bitslice","binop","placeholder",
"relocatable", NULL
};
#endif

static const short yyr1[] = {     0,
    82,    82,    83,    83,    83,    83,    83,    83,    83,    83,
    83,    83,    84,    85,    86,    86,    87,    87,    88,    89,
    89,    90,    91,    91,    92,    92,    93,    93,    94,    94,
    95,    95,    95,    95,    95,    96,    96,    97,    98,    98,
    99,    99,   100,   101,   101,   102,   102,   102,   103,   103,
   104,   104,   104,   105,   105,   106,   106,   107,   107,   108,
   108,   108,   108,   108,   108,   108,   108,   108,   109,   110,
   110,   111,   111,   112,   112,   112,   112,   113,   113,   114,
   114,   115,   115,   115,   115,   115,   115,   116,   117,   117,
   118,   119,   119,   120,   120,   121,   121,   121,   122,   122,
   123,   123,   123,   124,   124,   125,   126,   127,   127,   128,
   128,   129,   129,   129,   130,   130,   130,   130,   130,   130,
   130,   131,   131,   131,   131,   131,   131,   131,   131,   131,
   131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
   132,   132,   132,   133,   133,   134,   134,   135,   135,   136,
   136,   137,   137,   138,   139,   139,   139,   139,   139,   139,
   139,   140,   140,   140,   141,   141,   141,   141,   142,   143
};

static const short yyr2[] = {     0,
     2,     0,     1,     1,     1,     1,     2,     5,     2,     2,
     1,     1,     2,     6,     2,     0,     4,     2,     2,     2,
     0,     5,     1,     3,     2,     0,     2,     0,     2,     0,
     1,     1,     1,     4,     4,     3,     1,     3,     2,     0,
     1,     1,     2,     2,     0,     3,    11,     5,     3,     1,
     3,     3,     1,     3,     1,     2,     1,     2,     1,     1,
     1,     1,     1,     2,     2,     3,     3,     4,     3,     2,
     0,     1,     1,     5,     7,     3,     1,     3,     1,     2,
     0,     1,     1,     1,     1,     1,     1,     2,     2,     0,
     7,     2,     1,     3,     1,     1,     1,     1,     3,     1,
     2,     2,     2,     1,     0,     0,     1,     1,     0,     1,
     0,     3,     3,     0,     2,     2,     2,     2,     2,     2,
     0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     2,     1,     2,     1,     6,     3,     3,     0,     2,
     0,     3,     1,     3,     1,     3,     3,     3,     1,     3,
     4,     6,     4,     0,     1,     1,     1,     1,     5,     2
};

static const short yydefact[] = {     2,
     0,     0,    90,     0,    21,     0,     0,    45,     0,     0,
    28,     1,     3,    16,     4,     5,     6,    11,    12,     0,
    88,    96,    97,    98,    10,    93,    95,    19,     0,     9,
    43,     7,     0,   170,    13,     0,    89,     0,    92,     0,
    26,    23,    20,     0,     0,    28,     0,    44,    72,    73,
     0,    27,     0,    15,     0,   121,    94,     0,     0,     0,
     0,     0,     0,    18,     8,   114,   100,    24,    25,    30,
     0,     0,     0,     0,    63,     0,    71,     0,    61,     0,
    73,    46,    50,    53,    55,    57,    59,    60,    62,     0,
   169,     0,   105,   105,   105,   111,   121,   111,   119,   118,
   122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
   132,   133,   134,   135,   136,   120,   137,   138,   139,   140,
   116,   117,   115,     0,    14,     0,    56,     0,    65,     0,
     0,    64,     0,     0,     0,     0,    58,    85,    87,    86,
    82,    84,    83,     0,     0,    17,   104,   101,   102,   103,
   110,     0,    99,   149,   107,     0,     0,    31,    32,    33,
    22,    29,    48,    28,    66,    70,    67,    49,    51,    52,
    54,    81,     0,   164,   159,   155,     0,   164,    69,     0,
    77,     0,    79,   112,   113,     0,     0,   143,     0,   109,
   141,   145,   151,    40,     0,     0,     0,     0,     0,   105,
     0,   105,     0,   165,   166,   167,   168,     0,    68,     0,
     0,     0,     0,   153,     0,   108,    91,   144,     0,   142,
     0,    42,    41,     0,    37,     0,     0,    76,    80,   160,
     0,   156,     0,   157,     0,   158,    78,   147,     0,     0,
   148,     0,   150,    35,    39,    34,     0,     0,     0,     0,
     0,   161,     0,   152,   154,    36,    38,     0,   163,     0,
     0,    74,     0,     0,     0,     0,   146,    47,   162,    75,
     0,     0
};

static const short yydefgoto[] = {     1,
    12,    13,    14,    35,    54,    15,    28,    43,    44,    58,
    34,   124,   162,   224,   225,   221,   226,    16,    31,    48,
    82,    83,    84,    85,    86,    87,    88,   130,    89,   179,
   182,   197,   144,    17,    21,    37,    25,    26,    90,    66,
    97,   148,    38,   154,   217,   152,    98,    67,   123,   190,
   191,   192,   193,   220,   213,   214,   215,   200,   208,    18,
    19
};

static const short yypact[] = {-32768,
   235,   -59,-32768,    61,-32768,    20,    61,-32768,   -23,    74,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    79,
    77,-32768,-32768,-32768,    61,    46,-32768,   -26,    37,    61,
    -8,-32768,    86,    64,    69,   127,-32768,    61,    46,    61,
-32768,-32768,-32768,   158,   110,-32768,   160,-32768,-32768,-32768,
   161,-32768,   111,-32768,   154,    46,-32768,   -32,   136,   114,
   -39,   103,    93,   116,-32768,   140,    26,-32768,-32768,-32768,
   138,   175,   -12,   173,-32768,   130,-32768,    93,   153,    93,
   155,   151,   -11,   186,-32768,-32768,   207,-32768,   147,   191,
   151,   169,   150,   150,   150,   171,-32768,   171,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   179,-32768,    93,-32768,   197,-32768,   -43,
    40,   151,    93,    93,    93,    93,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -27,    48,-32768,-32768,-32768,-32768,-32768,
-32768,   -55,    26,    28,-32768,   198,   201,-32768,-32768,-32768,
-32768,-32768,   151,-32768,-32768,-32768,-32768,   -11,   186,   186,
-32768,-32768,    48,   108,   200,-32768,   177,   202,-32768,   204,
   172,    80,   172,-32768,-32768,   237,   182,-32768,    48,   214,
    27,-32768,   240,-32768,   -14,    -1,   -37,   165,   215,   150,
   227,   150,    48,-32768,-32768,-32768,-32768,    48,-32768,    48,
    93,    48,   -51,-32768,   185,-32768,-32768,-32768,    93,-32768,
    58,-32768,-32768,    52,-32768,   220,   206,-32768,-32768,-32768,
   195,-32768,   196,-32768,    82,   172,   172,   151,    32,    48,
-32768,    48,   151,-32768,-32768,-32768,   -14,   199,   232,   -40,
    15,-32768,   252,-32768,   172,-32768,-32768,   254,-32768,   203,
   205,-32768,    93,    93,   226,   208,   151,   151,-32768,-32768,
   278,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   -42,-32768,-32768,-32768,    33,-32768,    62,-32768,-32768,-32768,
   -62,   149,    36,   148,   212,-32768,-32768,-32768,   -31,-32768,
    83,-32768,    73,-32768,-32768,-32768,   282,   100,    -4,-32768,
-32768,   -85,-32768,-32768,-32768,   192,-32768,   194,-32768,-32768,
-32768,   101,-32768,-32768,    81,    54,  -138,   117,-32768,-32768,
-32768
};


#define	YYLAST		295


static const short yytable[] = {    27,
    91,    51,    27,    61,    75,   181,   183,   165,   149,   150,
   259,    72,   240,   228,   184,   131,    20,   132,    68,   185,
    27,    76,   172,    41,   173,    27,   261,   241,    99,    49,
    52,    50,   100,    27,   198,    57,   260,    77,   229,    78,
    69,    46,   174,   175,    29,    49,    42,    81,   176,   227,
   177,   134,    32,   186,   186,   222,   223,    79,    23,    80,
    49,    47,    81,   163,   183,   135,   187,   187,    52,   236,
   188,   237,   101,   102,   103,   104,   105,   106,   107,   108,
   109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
   119,   120,   167,   262,    33,   240,   121,    73,   166,   173,
    36,   122,   246,   255,   133,   189,    45,    73,   244,    75,
   253,    74,   178,   178,   232,   247,   234,   174,   175,    75,
    49,   196,    81,   176,    39,    40,    76,   222,   223,    39,
    22,    23,   209,    52,   252,    24,    76,    56,    53,   180,
   180,   178,    77,   210,    78,   210,  -106,  -106,   238,   138,
   139,  -106,    77,   140,    78,    55,   243,   178,    49,   -96,
    50,    60,    79,    23,    80,    49,   199,    81,   180,   169,
   170,   178,    79,    23,    80,    49,   178,    81,   178,    59,
   178,    62,    63,    65,   180,    70,    64,   138,   139,    71,
   125,   140,    92,   141,   142,   143,   126,   128,   180,   129,
   267,   268,   156,   180,   -96,   180,   -98,   180,   178,    93,
   178,   137,    94,   157,    95,   133,    96,   230,   204,   205,
   206,   207,   158,   159,   160,   204,   205,   206,   207,   161,
   147,   141,   142,   143,   271,   180,   151,   180,   204,   205,
   206,   207,   145,   136,   146,     2,   164,   194,     3,     4,
   195,   -97,   201,     5,     6,   203,   216,     7,   211,   212,
   199,   219,     8,   233,   231,     9,    10,    11,   248,   249,
   250,   251,   258,   263,   257,   264,   269,   272,   265,   256,
   266,   168,   245,   171,   127,   235,   270,   242,    30,   155,
   153,   218,   239,   254,   202
};

static const short yycheck[] = {     4,
    63,    33,     7,    46,    17,   144,   145,    51,    94,    95,
    51,    51,    64,    51,    70,    78,    76,    80,    51,    75,
    25,    34,    50,    50,    52,    30,    12,    79,     3,    73,
    70,    75,     7,    38,   173,    40,    77,    50,    76,    52,
    73,    50,    70,    71,    25,    73,    73,    75,    76,    51,
    78,    63,    76,    27,    27,    70,    71,    70,    71,    72,
    73,    70,    75,   126,   203,    77,    40,    40,    70,   208,
    43,   210,    47,    48,    49,    50,    51,    52,    53,    54,
    55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
    65,    66,    53,    79,    21,    64,    71,     5,   130,    52,
    22,    76,    51,   242,    65,    78,    70,     5,    51,    17,
    79,     9,   144,   145,   200,    64,   202,    70,    71,    17,
    73,   164,    75,    76,    25,    80,    34,    70,    71,    30,
    70,    71,    53,    70,    53,    75,    34,    38,    70,   144,
   145,   173,    50,    64,    52,    64,    70,    71,   211,     3,
     4,    75,    50,     7,    52,    29,   219,   189,    73,    52,
    75,    52,    70,    71,    72,    73,    59,    75,   173,   134,
   135,   203,    70,    71,    72,    73,   208,    75,   210,    22,
   212,    22,    22,    30,   189,    50,    76,     3,     4,    76,
    53,     7,    77,    47,    48,    49,    22,    25,   203,    70,
   263,   264,    24,   208,    52,   210,    52,   212,   240,    70,
   242,     5,    73,    35,    75,    65,    77,    53,    54,    55,
    56,    57,    44,    45,    46,    54,    55,    56,    57,    51,
    81,    47,    48,    49,     0,   240,    66,   242,    54,    55,
    56,    57,    52,    58,    76,    11,    50,    50,    14,    15,
    50,    52,    76,    19,    20,    52,    43,    23,    22,    78,
    59,    22,    28,    37,    50,    31,    32,    33,    49,    64,
    76,    76,    41,    22,    76,    22,    51,     0,    76,   247,
    76,   133,   221,   136,    73,   203,    79,   215,     7,    98,
    97,   191,   212,   240,   178
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
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

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
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
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

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
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
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


  switch (yyn) {

case 4:
#line 84 "dynamic/tools/sledparser.y"
{;
    break;}
case 5:
#line 85 "dynamic/tools/sledparser.y"
{;
    break;}
case 6:
#line 86 "dynamic/tools/sledparser.y"
{;
    break;}
case 8:
#line 88 "dynamic/tools/sledparser.y"
{;
    break;}
case 9:
#line 89 "dynamic/tools/sledparser.y"
{;
    break;}
case 10:
#line 90 "dynamic/tools/sledparser.y"
{;
    break;}
case 13:
#line 94 "dynamic/tools/sledparser.y"
{;
    break;}
case 14:
#line 95 "dynamic/tools/sledparser.y"
{ lt = new FieldIdent( yyvsp[-3].str, yyvsp[-1].i );
                                          DICT.fields[*yyvsp[-3].str] = lt; ;
    break;}
case 15:
#line 97 "dynamic/tools/sledparser.y"
{ yyvsp[0].fieldident->parent = lt; ;
    break;}
case 17:
#line 99 "dynamic/tools/sledparser.y"
{ yyval.fieldident = new FieldIdent(yyvsp[-3].str, yyvsp[-2].i, yyvsp[0].i);
                                          DICT.fields[*yyvsp[-3].str]=yyval.fieldident; ;
    break;}
case 18:
#line 101 "dynamic/tools/sledparser.y"
{ yyval.fieldident = new FieldIdent(yyvsp[-1].str, yyvsp[0].i, yyvsp[0].i);
                                          DICT.fields[*yyvsp[-1].str]=yyval.fieldident; ;
    break;}
case 22:
#line 107 "dynamic/tools/sledparser.y"
{ unsigned len = strlen("ERR");
			  for( vector<string *>::iterator i = yyvsp[-1].fielditem.strv->begin();
			       i != yyvsp[-1].fielditem.strv->end(); i++ ) {
			      if( (*i) && (*i)->length() > len ) 
			          len = (*i)->length();
			  }
			  for( vector<FieldIdent *>::iterator i=
			       yyvsp[-4].fiv->begin(); i != yyvsp[-4].fiv->end(); i++ ) {
			      (*i)->checked = yyvsp[-1].fielditem.checked;
			      (*i)->names = yyvsp[-1].fielditem.strv;
			      (*i)->maxDisasmLen = len;
			  }
			;
    break;}
case 23:
#line 120 "dynamic/tools/sledparser.y"
{ yyval.fiv = new vector<FieldIdent *>(1,
							(FieldIdent *)yyvsp[0].id); ;
    break;}
case 24:
#line 122 "dynamic/tools/sledparser.y"
{ yyval.fiv = yyvsp[-1].fiv; ;
    break;}
case 25:
#line 124 "dynamic/tools/sledparser.y"
{ yyval.fiv = yyvsp[-1].fiv; yyval.fiv->push_back(
							(FieldIdent *)yyvsp[0].id); ;
    break;}
case 26:
#line 126 "dynamic/tools/sledparser.y"
{ yyval.fiv = new vector<FieldIdent *>(); ;
    break;}
case 27:
#line 128 "dynamic/tools/sledparser.y"
{ yyval.strv = yyvsp[-1].strv; yyval.strv->push_back(yyvsp[0].str); ;
    break;}
case 28:
#line 129 "dynamic/tools/sledparser.y"
{ yyval.strv = new vector<string *>(); ;
    break;}
case 29:
#line 130 "dynamic/tools/sledparser.y"
{ if( yyvsp[0].fielditem.strv != NULL ) yyvsp[-1].fielditem.strv = yyvsp[0].fielditem.strv;
					  else yyvsp[-1].fielditem.checked = yyvsp[0].fielditem.checked;
					  yyval.fielditem = yyvsp[-1].fielditem;
					;
    break;}
case 30:
#line 134 "dynamic/tools/sledparser.y"
{ yyval.fielditem.checked = FC_CHECKED; yyval.fielditem.strv = NULL; ;
    break;}
case 31:
#line 135 "dynamic/tools/sledparser.y"
{ yyval.fielditem.checked = FC_CHECKED; yyval.fielditem.strv = NULL; ;
    break;}
case 32:
#line 136 "dynamic/tools/sledparser.y"
{ yyval.fielditem.checked = FC_UNCHECKED; yyval.fielditem.strv =NULL; ;
    break;}
case 33:
#line 137 "dynamic/tools/sledparser.y"
{ yyval.fielditem.checked = FC_GUARANTEED; yyval.fielditem.strv=NULL; ;
    break;}
case 34:
#line 138 "dynamic/tools/sledparser.y"
{ yyval.fielditem.strv = yyvsp[-1].strv; ;
    break;}
case 35:
#line 139 "dynamic/tools/sledparser.y"
{ yyval.fielditem.strv = yyvsp[-1].strv; ;
    break;}
case 36:
#line 142 "dynamic/tools/sledparser.y"
{ yyval.strv = yyvsp[-2].strv; 
					  if( (unsigned)yyvsp[0].binding.pos >= yyval.strv->size() )
					      yyval.strv->resize(yyvsp[0].binding.pos+1, NULL);
					  (*yyval.strv)[yyvsp[0].binding.pos] = yyvsp[0].binding.str; 
					;
    break;}
case 37:
#line 147 "dynamic/tools/sledparser.y"
{ yyval.strv = new vector<string *>();
					  if( (unsigned)yyvsp[0].binding.pos >= yyval.strv->size() ) 
					      yyval.strv->resize(yyvsp[0].binding.pos+1, NULL );
					  (*yyval.strv)[yyvsp[0].binding.pos] = yyvsp[0].binding.str;
					;
    break;}
case 38:
#line 153 "dynamic/tools/sledparser.y"
{ yyval.binding.pos = yyvsp[0].i; yyval.binding.str = yyvsp[-2].str; ;
    break;}
case 39:
#line 154 "dynamic/tools/sledparser.y"
{ yyval.strv = yyvsp[-1].strv; yyval.strv->push_back(yyvsp[0].str); ;
    break;}
case 40:
#line 155 "dynamic/tools/sledparser.y"
{ yyval.strv = new vector<string *>(); ;
    break;}
case 46:
#line 161 "dynamic/tools/sledparser.y"
{ yyval.id = new PatternIdent(yyvsp[-2].str,yyvsp[0].expr); ;
    break;}
case 47:
#line 163 "dynamic/tools/sledparser.y"
{ ListExpr *list = new ListExpr( );
		  for( unsigned i = 0; i != yyvsp[-5].strv->size(); i++){ 
		      int n = i;
		      if( (*yyvsp[-5].strv)[i]->compare("_") ) 
		      list->add( new PatternIdent((*yyvsp[-5].strv)[i], yyvsp[0].expr->expand(&n))); 
		  }
		  yyval.id = new PatternIdent( yyvsp[-10].str, list );
		;
    break;}
case 48:
#line 172 "dynamic/tools/sledparser.y"
{ for( unsigned i = 0; i != yyvsp[-3].strv->size(); i++){ 
		      int n = i;
		      if( (*yyvsp[-3].strv)[i]->compare("_") ) 
		      yyval.id = new PatternIdent((*yyvsp[-3].strv)[i], yyvsp[0].expr->expand(&n)); }
		;
    break;}
case 49:
#line 183 "dynamic/tools/sledparser.y"
{ if( yyvsp[-2].expr->type == TYPE_LIST ) {
				      yyval.expr = yyvsp[-2].expr; 
				      ((ListExpr *)yyval.expr)->addAll(yyvsp[0].expr);
				  } else {
				      yyval.expr = new ListExpr();
				      ((ListExpr *)yyval.expr)->addAll(yyvsp[-2].expr);
				      ((ListExpr *)yyval.expr)->addAll(yyvsp[0].expr);
				  }
				;
    break;}
case 51:
#line 193 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr( TYPE_UNKNOWN, PAT_SEQ, yyvsp[-2].expr, yyvsp[0].expr ); ;
    break;}
case 52:
#line 194 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr( TYPE_UNKNOWN, PAT_SEQ, yyvsp[-2].expr, yyvsp[0].expr ); ;
    break;}
case 54:
#line 196 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, yyvsp[-2].expr, yyvsp[0].expr ); ;
    break;}
case 56:
#line 198 "dynamic/tools/sledparser.y"
{ yyval.expr = new UnaryExpr( TYPE_UNKNOWN, PAT_LDOT, yyvsp[0].expr ); ;
    break;}
case 58:
#line 200 "dynamic/tools/sledparser.y"
{ yyval.expr = new UnaryExpr( TYPE_UNKNOWN, PAT_RDOT, yyvsp[-1].expr ); ;
    break;}
case 61:
#line 203 "dynamic/tools/sledparser.y"
{ yyval.expr = NULL; ;
    break;}
case 63:
#line 205 "dynamic/tools/sledparser.y"
{ yyval.expr = new NullExpr(); ;
    break;}
case 64:
#line 206 "dynamic/tools/sledparser.y"
{ PatternIdent *id = (PatternIdent *)symtab.lookup(*yyvsp[-1].str);
				  if( id ) {
				      if( id->type == TYPE_UNKNOWN && id->expr != NULL )
				          id->expr = new OffsetExpr(yyvsp[0].expr);
				  } else {
				      id = new PatternIdent( yyvsp[-1].str, new OffsetExpr(yyvsp[0].expr) );
				  }
				  yyval.expr = yyvsp[0].expr; ;
    break;}
case 65:
#line 214 "dynamic/tools/sledparser.y"
{;
    break;}
case 66:
#line 215 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[-1].list; ;
    break;}
case 67:
#line 216 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 68:
#line 217 "dynamic/tools/sledparser.y"
{ yyval.expr = new InvokeExpr(DICT.insts[*yyvsp[-3].opname.name], yyvsp[-1].list); ;
    break;}
case 69:
#line 218 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr( TYPE_UNKNOWN, yyvsp[-1].op, yyvsp[-2].expr, yyvsp[0].expr ); ;
    break;}
case 70:
#line 219 "dynamic/tools/sledparser.y"
{ yyval.list = yyvsp[-1].list; yyval.list->add(yyvsp[0].expr); ;
    break;}
case 71:
#line 220 "dynamic/tools/sledparser.y"
{ yyval.list = new ListExpr(); ;
    break;}
case 72:
#line 221 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[0].id; ;
    break;}
case 73:
#line 222 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[0].id; ;
    break;}
case 74:
#line 223 "dynamic/tools/sledparser.y"
{ yyval.expr = new RangeExpr(yyvsp[-3].i,yyvsp[-1].i); ;
    break;}
case 75:
#line 225 "dynamic/tools/sledparser.y"
{ yyval.expr = new RangeExpr(yyvsp[-5].i,yyvsp[-3].i,yyvsp[-1].i); ;
    break;}
case 76:
#line 226 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[-1].list; ;
    break;}
case 78:
#line 229 "dynamic/tools/sledparser.y"
{ yyval.list = yyvsp[-2].list; yyval.list->add(yyvsp[0].expr); ;
    break;}
case 79:
#line 230 "dynamic/tools/sledparser.y"
{ yyval.list = new ListExpr(); yyval.list->add(yyvsp[0].expr); ;
    break;}
case 80:
#line 232 "dynamic/tools/sledparser.y"
{ yyval.list = yyvsp[-1].list; yyval.list->add(new IntegerExpr(yyvsp[0].i)); ;
    break;}
case 81:
#line 233 "dynamic/tools/sledparser.y"
{ yyval.list = new GeneratorExpr(); ;
    break;}
case 82:
#line 235 "dynamic/tools/sledparser.y"
{ yyval.op = REL_LS; ;
    break;}
case 83:
#line 236 "dynamic/tools/sledparser.y"
{ yyval.op = REL_EQ; ;
    break;}
case 84:
#line 237 "dynamic/tools/sledparser.y"
{ yyval.op = REL_GS; ;
    break;}
case 85:
#line 238 "dynamic/tools/sledparser.y"
{ yyval.op = REL_LE; ;
    break;}
case 86:
#line 239 "dynamic/tools/sledparser.y"
{ yyval.op = REL_GE; ;
    break;}
case 87:
#line 240 "dynamic/tools/sledparser.y"
{ yyval.op = REL_NE; ;
    break;}
case 91:
#line 247 "dynamic/tools/sledparser.y"
{ DICT.InstantiateInstructions( yyvsp[-5].opcode, yyvsp[-4].operands.ops, yyvsp[-4].operands.asmstr, yyvsp[-3].str, yyvsp[-1].branch.eq, yyvsp[-1].branch.pat ); 
	  symtab.leaveScope();
	;
    break;}
case 93:
#line 253 "dynamic/tools/sledparser.y"
{;
    break;}
case 94:
#line 256 "dynamic/tools/sledparser.y"
{ yyval.opcode = yyvsp[-2].opcode; yyval.opcode->push_back(yyvsp[0].opname); ;
    break;}
case 95:
#line 257 "dynamic/tools/sledparser.y"
{ yyval.opcode = new vector<OpcodePart>();
                                  yyval.opcode->push_back(yyvsp[0].opname); ;
    break;}
case 96:
#line 260 "dynamic/tools/sledparser.y"
{ yyval.opname.name = yyvsp[0].str; yyval.opname.expr = yyval.opname.list = NULL; ;
    break;}
case 97:
#line 261 "dynamic/tools/sledparser.y"
{ yyval.opname.name = yyvsp[0].str; yyval.opname.expr = yyval.opname.list = NULL; ;
    break;}
case 98:
#line 263 "dynamic/tools/sledparser.y"
{ yyval.opname.name = &yyvsp[0].id->name; 
		  if( ((PatternIdent *)yyvsp[0].id)->expr && 
		      ((PatternIdent *)yyvsp[0].id)->expr->type == TYPE_LIST )
		      yyval.opname.list = ((ListExpr *)((PatternIdent *)yyvsp[0].id)->expr); 
		  yyval.opname.expr = yyvsp[0].id;
                ;
    break;}
case 99:
#line 271 "dynamic/tools/sledparser.y"
{ yyval.operands = yyvsp[-2].operands; 
		  yyval.operands.ops->push_back(yyvsp[-1].operand); 
		  if( yyvsp[-1].operand->expr == NULL || ( yyvsp[-1].operand->expr->getKind() == FIELD &&
                      ((FieldIdent *)yyvsp[-1].operand->expr)->hasNames() == false ) ) {
		      yyvsp[-1].operand->isImmed = true;
		  } else {
		      yyvsp[-1].operand->isImmed = false;
		  }
                  yyval.operands.asmstr->append("%s");
		  yyval.operands.asmstr->append(*yyvsp[0].str);
		  delete yyvsp[0].str; 
		;
    break;}
case 100:
#line 283 "dynamic/tools/sledparser.y"
{ yyval.operands.ops = new vector<OperandIdent *>(); yyval.operands.asmstr = yyvsp[0].str; ;
    break;}
case 101:
#line 284 "dynamic/tools/sledparser.y"
{ yyval.operand = new OperandIdent( yyvsp[-1].str, yyvsp[0].flag, NULL ); ;
    break;}
case 102:
#line 285 "dynamic/tools/sledparser.y"
{ yyval.operand = new OperandIdent( &yyvsp[-1].id->name, yyvsp[0].flag, yyvsp[-1].id ); ;
    break;}
case 103:
#line 286 "dynamic/tools/sledparser.y"
{ yyval.operand = new OperandIdent( &yyvsp[-1].id->name, yyvsp[0].flag, yyvsp[-1].id ); ;
    break;}
case 104:
#line 287 "dynamic/tools/sledparser.y"
{ yyval.flag = true; ;
    break;}
case 105:
#line 288 "dynamic/tools/sledparser.y"
{ yyval.flag = false; ;
    break;}
case 106:
#line 290 "dynamic/tools/sledparser.y"
{ yyNewLineHack = true; /* Not my idea... */ 
			  /* constructor creates its own scope for operands */
				  symtab.enterScope();
				;
    break;}
case 107:
#line 294 "dynamic/tools/sledparser.y"
{ yyNewLineHack = false; ;
    break;}
case 110:
#line 296 "dynamic/tools/sledparser.y"
{;
    break;}
case 112:
#line 297 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 113:
#line 298 "dynamic/tools/sledparser.y"
{ yyval.str = &yyvsp[0].id->name; ;
    break;}
case 114:
#line 299 "dynamic/tools/sledparser.y"
{ yyval.str = NULL; ;
    break;}
case 115:
#line 300 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; *yyval.str += yyvsp[0].ch; ;
    break;}
case 116:
#line 301 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; 
				  for( string::iterator s = yyvsp[0].str->begin(); 
				       s != yyvsp[0].str->end(); s++ ) {
				          if( *s == '%' ) *yyval.str += "%%";
				          else *yyval.str += *s;
				  }
				  delete yyvsp[0].str;
				;
    break;}
case 117:
#line 309 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; *yyval.str += yyvsp[0].i; ;
    break;}
case 118:
#line 310 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; *yyval.str += ">="; ;
    break;}
case 119:
#line 311 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; *yyval.str += "<="; ;
    break;}
case 120:
#line 312 "dynamic/tools/sledparser.y"
{ yyval.str = yyvsp[-1].str; *yyval.str += "%%"; ;
    break;}
case 121:
#line 313 "dynamic/tools/sledparser.y"
{ yyval.str = new string(); ;
    break;}
case 142:
#line 318 "dynamic/tools/sledparser.y"
{ yyval.branch.eq = yyvsp[-1].expr; yyval.branch.pat = yyvsp[0].expr; ;
    break;}
case 143:
#line 319 "dynamic/tools/sledparser.y"
{ yyval.branch.eq = yyval.branch.pat = NULL ;
    break;}
case 145:
#line 321 "dynamic/tools/sledparser.y"
{ yyerror( "WHEN branches not supported yet\n" ); ;
    break;}
case 146:
#line 322 "dynamic/tools/sledparser.y"
{ yyval.branch.eq = yyvsp[-3].expr; yyval.branch.pat = yyvsp[0].expr; ;
    break;}
case 147:
#line 323 "dynamic/tools/sledparser.y"
{ yyval.branch.eq = NULL; yyval.branch.pat = yyvsp[0].expr; ;
    break;}
case 148:
#line 325 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 149:
#line 326 "dynamic/tools/sledparser.y"
{ yyval.expr = NULL; ;
    break;}
case 150:
#line 327 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 151:
#line 328 "dynamic/tools/sledparser.y"
{ yyval.expr = NULL; ;
    break;}
case 152:
#line 329 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr(TYPE_UNKNOWN,PAT_AND,yyvsp[-2].expr,yyvsp[0].expr); ;
    break;}
case 154:
#line 331 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr(TYPE_UNKNOWN,yyvsp[-1].op,yyvsp[-2].expr,yyvsp[0].expr); ;
    break;}
case 155:
#line 332 "dynamic/tools/sledparser.y"
{ yyval.expr = new IntegerExpr(yyvsp[0].i); ;
    break;}
case 156:
#line 333 "dynamic/tools/sledparser.y"
{ Expr *e = new PatternIdent(yyvsp[-2].str,NULL); 
					  if( yyvsp[-1].bitslice.lo <= yyvsp[-1].bitslice.hi ) 
					      yyval.expr = new BitSliceExpr(e,yyvsp[-1].bitslice);
					  else yyval.expr = e; 
					;
    break;}
case 157:
#line 338 "dynamic/tools/sledparser.y"
{ if( yyvsp[-1].bitslice.lo <= yyvsp[-1].bitslice.hi )
					      yyval.expr = new BitSliceExpr(yyvsp[-2].expr,yyvsp[-1].bitslice); 
					  else yyval.expr = yyvsp[-2].expr;
					;
    break;}
case 158:
#line 342 "dynamic/tools/sledparser.y"
{ yyval.expr = new BinaryExpr(TYPE_UNKNOWN,yyvsp[-1].op,yyvsp[-2].expr,yyvsp[0].expr); ;
    break;}
case 159:
#line 343 "dynamic/tools/sledparser.y"
{ yyval.expr = new StringExpr(yyvsp[0].str); ;
    break;}
case 160:
#line 344 "dynamic/tools/sledparser.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 161:
#line 345 "dynamic/tools/sledparser.y"
{ yyval.expr = new InvokeExpr(DICT.insts[*yyvsp[-3].opname.name], yyvsp[-1].list); ;
    break;}
case 162:
#line 347 "dynamic/tools/sledparser.y"
{ yyval.bitslice.lo = yyvsp[-3].i; yyval.bitslice.hi = yyvsp[-1].i; ;
    break;}
case 163:
#line 348 "dynamic/tools/sledparser.y"
{ yyval.bitslice.lo = yyval.bitslice.hi = yyvsp[-1].i; ;
    break;}
case 164:
#line 349 "dynamic/tools/sledparser.y"
{ yyval.bitslice.lo = 0; yyval.bitslice.hi = -1; ;
    break;}
case 165:
#line 351 "dynamic/tools/sledparser.y"
{ yyval.op = EXP_ADD; ;
    break;}
case 166:
#line 352 "dynamic/tools/sledparser.y"
{ yyval.op = EXP_SUB; ;
    break;}
case 167:
#line 353 "dynamic/tools/sledparser.y"
{ yyval.op = EXP_MUL; ;
    break;}
case 168:
#line 354 "dynamic/tools/sledparser.y"
{ yyval.op = EXP_DIV; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
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

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
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
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 360 "dynamic/tools/sledparser.y"

int yyerror(char *s)
{
//    symtab.printOn(stdout);
    fprintf( stderr, "Stopped at %s:%d: %s\n", yyfilename,yylineno, s );
    abort();
}
