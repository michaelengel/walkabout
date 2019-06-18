/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */


%{
#include "sledtree.h"
#include "sledparser.h"
#define yywrap() 1

int yylineno = 1;
const char *yyfilename = NULL;
bool yyNewLineHack = false;
%}
%%

\!	|
\$	|
\%	|
\&	|
\'	|
\(	|
\)	|
\*	|
\+	|
\,	|
\-	|
\/	|
\:	|
\;	|
\<	|
\>	|
\=	|
\{	|
\|	|
\}	|
\@	|
\[	|
\"	|
\]	|
\^	{ yylval.ch = yytext[0]; return yytext[0]; }
\<\=	{ return LEQUAL; }
\!\=	{ return NEQUAL; }
\.\.\.	{ return ELLIPSIS; }
\=\>	{ return RARROW; }
\>\=	{ return GEQUAL; }
address	{ return ADDRESS; }
any	{ return ANY; }
assembly { return ASSEMBLY; }
bit	{ return BIT; }
checked	{ return CHECKED; }
columns	{ return COLUMNS; }
component	{ return COMPONENT; }
constructors	{ return CONSTRUCTORS; }
discard	{ return DISCARD; }
else	{ return ELSE; }
epsilon	{ return EPSILON; }
fetch	{ return FETCH; }
field	{ return FIELD; }
fieldinfo	{ return FIELDINFO; }
fields	{ return FIELDS; }
for	{ return FOR; }
guaranteed	{ return GUARANTEED; }
is	{ return IS; }
keep	{ return KEEP; }
most    { return MOST; }
names	{ return NAMES; }
of	{ return OF; }
opcode	{ return OPCODE; }
operand	{ return OPERAND; }
otherwise	{ return OTHERWISE; }
patterns	{ return PATTERNS; }
pc_unit_bits	{ return PCUNITBITS; }
placeholder	{ return PLACEHOLDER; }
relocatable	{ return RELOCATABLE; }
significant     { return SIGNIFICANT; }
some		{ return SOME; }
sparse		{ return SPARSE; }
syntax		{ return SYNTAX; }
to		{ return TO; }
type		{ return TYPE; }
unchecked	{ return UNCHECKED; }
using		{ return USING; }
when		{ return WHEN; }
which		{ return WHICH; }
wordsize	{ return WORDSIZE; }

#.*$
[a-zA-Z_.][a-zA-Z0-9_.]* { 
		  string *str = new string(yytext);
		  yylval.id = symtab.lookup(*str);
		  if( yylval.id == NULL ) {
		      yylval.str = str;
		      return IDENT;
		  }
		  switch( yylval.id->getKind() ) {
		      case FIELD: return FIELDIDENT;
		      case RELOC: return RELOCIDENT;
		      case PATTERN: return PATTERNIDENT;
		      case OPERAND: return PATTERNIDENT;
                      default: assert(0);
		  }		  
		  return IDENT; 
		}
[0-9]+		|
0x[0-9a-fA-F]+	{ yylval.i = strtol( yytext, NULL, 0 ); return INTEGER; }

\"([^\"]|\"\")*\"	{ yylval.str = new string(yytext+1,strlen(yytext)-2); return STRING; }
[a-zA-Z_.][a-zA-Z0-9_.]*\: {
                  yylval.str = new string(yytext, strlen(yytext)-1);
		  return LABEL;
		}
[\r\ \t\v]	{ if( yyNewLineHack ){ yylval.ch = ' '; return ' '; } }
\n		{ yylineno++; if( yyNewLineHack ) return NEWLINE; }

.
%%
