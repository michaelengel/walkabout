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
#include <vector>
#include <string>

extern int yylineno;
extern bool yyNewLineHack;

#define YYPARSE_PARAM dict
#define DICT (*(SledDict *)dict)
%}

%union {
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
}
%{
    FieldIdent *lt;
    int yyerror( char *str );
%}

%token LEQUAL NEQUAL ELLIPSIS RARROW GEQUAL ADDRESS ANY ASSEMBLY BIT COLUMNS
%token COMPONENT CONSTRUCTORS DISCARD ELSE EPSILON FETCH FIELDINFO
%token FIELDS FOR IS KEEP NAMES OF OPCODE OTHERWISE PATTERNS MOST SIGNIFICANT
%token PCUNITBITS PLACEHOLDER RELOCATABLE SOME SPARSE SYNTAX TO TYPE USING
%token WHEN WHICH WORDSIZE NEWLINE CHECKED UNCHECKED GUARANTEED
%token <ch> '<' '>' '=' '[' ']' '(' ')' '+' '-' '*' '/' '&' '@' '#' '$' '%' 
%token <ch> ';' ',' '|' ' ' '\t' '\v' '\r'
%token <str> IDENT STRING LABEL
%token <id> FIELDIDENT RELOCIDENT PATTERNIDENT
%token <i> INTEGER
%type <op> relop binop
%type <str> fieldname typeOpt noise
%type <fieldident> fieldspec
%type <fiv> fieldidents identbinding
%type <fielditem> fielditems fielditem
%type <binding> fieldnamebinding
%type <strv> idents fieldnamebindings fieldnames
%type <expr> pattern disjunct sequent conjunct dotsr atomic constraint genexpr
%type <expr> addressable expr equation equations patternsopt equationsopt
%type <branch> branch branches branchesOpt
%type <list> integers addressables arglist
%type <opcode> opcode
%type <opname> opname
%type <operand> operand
%type <operands> operands
%type <bitslice> bitslice
%type <ch> noisechar
%type <flag> bangOpt
%type <id> patbinding
%left '+' '-'
%left '*' '/'
%%
specs : specs spec | ;
spec : tokenspec 
	| fieldinfospec {}
	| patternspec	{}
	| constructorspec {}
	| PCUNITBITS INTEGER
        | BIT INTEGER IS MOST SIGNIFICANT {}
        | KEEP opcodes {}
        | DISCARD opcodes {}
	| placeholder
	| relocatable;

tokenspec : token fieldspecs {};
token : FIELDS OF IDENT '(' INTEGER ')' { lt = new FieldIdent( $3, $5 );
                                          DICT.fields[*$3] = lt; };
fieldspecs: fieldspecs fieldspec	{ $2->parent = lt; }
	| ;
fieldspec: IDENT INTEGER ':' INTEGER	{ $$ = new FieldIdent($1, $2, $4);
                                          DICT.fields[*$1]=$$; }
	| IDENT INTEGER			{ $$ = new FieldIdent($1, $2, $2);
                                          DICT.fields[*$1]=$$; };

fieldinfospec : FIELDINFO fieldinfos
fieldinfos: fieldinfos fieldinfo | ;
fieldinfo: identbinding IS '[' fielditems ']' 
			{ unsigned len = strlen("ERR");
			  for( vector<string *>::iterator i = $4.strv->begin();
			       i != $4.strv->end(); i++ ) {
			      if( (*i) && (*i)->length() > len ) 
			          len = (*i)->length();
			  }
			  for( vector<FieldIdent *>::iterator i=
			       $1->begin(); i != $1->end(); i++ ) {
			      (*i)->checked = $4.checked;
			      (*i)->names = $4.strv;
			      (*i)->maxDisasmLen = len;
			  }
			} ;
identbinding: FIELDIDENT		{ $$ = new vector<FieldIdent *>(1,
							(FieldIdent *)$1); }
	| '[' fieldidents ']'		{ $$ = $2; } 
	;
fieldidents: fieldidents FIELDIDENT	{ $$ = $1; $$->push_back(
							(FieldIdent *)$2); }
	|				{ $$ = new vector<FieldIdent *>(); }
	;
idents: idents IDENT			{ $$ = $1; $$->push_back($2); }
	|				{ $$ = new vector<string *>(); } ;
fielditems : fielditems fielditem	{ if( $2.strv != NULL ) $1.strv = $2.strv;
					  else $1.checked = $2.checked;
					  $$ = $1;
					}
	|				{ $$.checked = FC_CHECKED; $$.strv = NULL; }
fielditem: CHECKED			{ $$.checked = FC_CHECKED; $$.strv = NULL; }
	| UNCHECKED			{ $$.checked = FC_UNCHECKED; $$.strv =NULL; }
	| GUARANTEED			{ $$.checked = FC_GUARANTEED; $$.strv=NULL; }
	| SPARSE '[' fieldnamebindings ']' { $$.strv = $3; }
	| NAMES '[' fieldnames ']'	{ $$.strv = $3; } 
	;
fieldnamebindings: fieldnamebindings ',' fieldnamebinding
					{ $$ = $1; 
					  if( (unsigned)$3.pos >= $$->size() )
					      $$->resize($3.pos+1, NULL);
					  (*$$)[$3.pos] = $3.str; 
					}
	| fieldnamebinding		{ $$ = new vector<string *>();
					  if( (unsigned)$1.pos >= $$->size() ) 
					      $$->resize($1.pos+1, NULL );
					  (*$$)[$1.pos] = $1.str;
					}
	;
fieldnamebinding: fieldname '=' INTEGER { $$.pos = $3; $$.str = $1; };
fieldnames: fieldnames fieldname	{ $$ = $1; $$->push_back($2); }
	|				{ $$ = new vector<string *>(); }
	;
fieldname: STRING | IDENT ;

patternspec: PATTERNS patbindings ;
patbindings: patbindings patbinding | ;
patbinding: IDENT IS pattern		{ $$ = new PatternIdent($1,$3); }
	| IDENT IS ANY OF '[' idents ']' ',' WHICH IS pattern 
		{ ListExpr *list = new ListExpr( );
		  for( unsigned i = 0; i != $6->size(); i++){ 
		      int n = i;
		      if( (*$6)[i]->compare("_") ) 
		      list->add( new PatternIdent((*$6)[i], $11->expand(&n))); 
		  }
		  $$ = new PatternIdent( $1, list );
		}
	| '[' idents ']' IS pattern	
		{ for( unsigned i = 0; i != $2->size(); i++){ 
		      int n = i;
		      if( (*$2)[i]->compare("_") ) 
		      $$ = new PatternIdent((*$2)[i], $5->expand(&n)); }
		};

/* pattern: pattern '|' disjunct	{ $$ = new BinaryExpr( TYPE_PATTERN, PAT_OR, $1, $3 ); }
	| disjunct ; */
/*** Note ***
 * This works for the set of specs we have, but is not generally correct.
 */
pattern: pattern '|' disjunct	{ if( $1->type == TYPE_LIST ) {
				      $$ = $1; 
				      ((ListExpr *)$$)->addAll($3);
				  } else {
				      $$ = new ListExpr();
				      ((ListExpr *)$$)->addAll($1);
				      ((ListExpr *)$$)->addAll($3);
				  }
				}
	| disjunct ;
disjunct: disjunct ';' sequent	{ $$ = new BinaryExpr( TYPE_UNKNOWN, PAT_SEQ, $1, $3 ); }
	| disjunct ':' sequent	{ $$ = new BinaryExpr( TYPE_UNKNOWN, PAT_SEQ, $1, $3 ); }
	| sequent ;
sequent: sequent '&' conjunct	{ $$ = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, $1, $3 ); }
	| conjunct ;
conjunct: ELLIPSIS dotsr	{ $$ = new UnaryExpr( TYPE_UNKNOWN, PAT_LDOT, $2 ); }
	| dotsr ;
dotsr: atomic ELLIPSIS		{ $$ = new UnaryExpr( TYPE_UNKNOWN, PAT_RDOT, $1 ); }
	| atomic ;
atomic: constraint
	| IDENT			{ $$ = NULL; }
	| addressable
	| EPSILON		{ $$ = new NullExpr(); }
	| LABEL pattern		{ PatternIdent *id = (PatternIdent *)symtab.lookup(*$1);
				  if( id ) {
				      if( id->type == TYPE_UNKNOWN && id->expr != NULL )
				          id->expr = new OffsetExpr($2);
				  } else {
				      id = new PatternIdent( $1, new OffsetExpr($2) );
				  }
				  $$ = $2; }
	| SOME IDENT		{}
	| '[' addressables ']'	{ $$ = $2; }
	| '(' pattern ')'	{ $$ = $2; }
	| opname '(' arglist ')'  { $$ = new InvokeExpr(DICT.insts[*$1.name], $3); }
constraint: addressable relop genexpr { $$ = new BinaryExpr( TYPE_UNKNOWN, $2, $1, $3 ); };
addressables: addressables addressable  { $$ = $1; $$->add($2); }
	|				{ $$ = new ListExpr(); }
addressable: FIELDIDENT			{ $$ = $1; } 
	| PATTERNIDENT			{ $$ = $1; } ; 
genexpr: '{' INTEGER TO INTEGER '}'	{ $$ = new RangeExpr($2,$4); }
	| '{' INTEGER TO INTEGER COLUMNS INTEGER '}' 
					{ $$ = new RangeExpr($2,$4,$6); }
	| '[' integers ']'		{ $$ = $2; }
	| expr
	;
arglist: arglist ',' expr		{ $$ = $1; $$->add($3); }
	| expr 				{ $$ = new ListExpr(); $$->add($1); }
	;
integers: integers INTEGER		{ $$ = $1; $$->add(new IntegerExpr($2)); }
	|				{ $$ = new GeneratorExpr(); }
	;
relop: '<'		{ $$ = REL_LS; }
	| '='		{ $$ = REL_EQ; }
	| '>'		{ $$ = REL_GS; }
	| LEQUAL	{ $$ = REL_LE; }
	| GEQUAL	{ $$ = REL_GE; }
	| NEQUAL	{ $$ = REL_NE; }
	;

constructorspec: CONSTRUCTORS constructors
constructors: constructors constructor
	| ;
constructor: nlhack opcode operands typeOpt nlhackoff branchesOpt nlOpt
	{ DICT.InstantiateInstructions( $2, $3.ops, $3.asmstr, $4, $6.eq, $6.pat ); 
	  symtab.leaveScope();
	}
	;

opcodes: opcodes opcode
	| opcode		{}
	;

opcode: opcode '^' opname	{ $$ = $1; $$->push_back($3); }
	| opname 		{ $$ = new vector<OpcodePart>();
                                  $$->push_back($1); }
	;
opname: IDENT			{ $$.name = $1; $$.expr = $$.list = NULL; }
	| STRING		{ $$.name = $1; $$.expr = $$.list = NULL; }
	| PATTERNIDENT		
		{ $$.name = &$1->name; 
		  if( ((PatternIdent *)$1)->expr && 
		      ((PatternIdent *)$1)->expr->type == TYPE_LIST )
		      $$.list = ((ListExpr *)((PatternIdent *)$1)->expr); 
		  $$.expr = $1;
                } 
	;
operands: operands operand noise
		{ $$ = $1; 
		  $$.ops->push_back($2); 
		  if( $2->expr == NULL || ( $2->expr->getKind() == FIELD &&
                      ((FieldIdent *)$2->expr)->hasNames() == false ) ) {
		      $2->isImmed = true;
		  } else {
		      $2->isImmed = false;
		  }
                  $$.asmstr->append("%s");
		  $$.asmstr->append(*$3);
		  delete $3; 
		}
	| noise			{ $$.ops = new vector<OperandIdent *>(); $$.asmstr = $1; }
operand: IDENT bangOpt 		{ $$ = new OperandIdent( $1, $2, NULL ); }
	| FIELDIDENT bangOpt 	{ $$ = new OperandIdent( &$1->name, $2, $1 ); }
	| PATTERNIDENT bangOpt 	{ $$ = new OperandIdent( &$1->name, $2, $1 ); }
bangOpt: '!' 			{ $$ = true; }
	| 			{ $$ = false; }
	;
nlhack:				{ yyNewLineHack = true; /* Not my idea... */ 
			  /* constructor creates its own scope for operands */
				  symtab.enterScope();
				} ;
nlhackoff: wsOpt		{ yyNewLineHack = false; } ;
nlOpt: NEWLINE | ;
wsOpt: ' ' {} | ;
typeOpt: ':' wsOpt IDENT	{ $$ = $3; }
        | ':' wsOpt PATTERNIDENT{ $$ = &$3->name; }
	| 			{ $$ = NULL; }
noise : noise noisechar 	{ $$ = $1; *$$ += $2; }
	| noise STRING		{ $$ = $1; 
				  for( string::iterator s = $2->begin(); 
				       s != $2->end(); s++ ) {
				          if( *s == '%' ) *$$ += "%%";
				          else *$$ += *s;
				  }
				  delete $2;
				}
	| noise INTEGER		{ $$ = $1; *$$ += $2; }
	| noise GEQUAL		{ $$ = $1; *$$ += ">="; }
	| noise LEQUAL		{ $$ = $1; *$$ += "<="; }
	| noise '%'		{ $$ = $1; *$$ += "%%"; }
	|	{ $$ = new string(); } ;
noisechar: '<'|'>'|'='|'['|']'|'('|')'|'+'|'-'|'*'|'/'|
        '&'|'@'|'#'|'$'|';'|','|'|'|' '
	;
branchesOpt: branches
        | equationsopt patternsopt 	{ $$.eq = $1; $$.pat = $2; }
	| NEWLINE			{ $$.eq = $$.pat = NULL } ;
branches: branches branch /* Not implemented - none of our specs uses this form */
	| branch 			{ yyerror( "WHEN branches not supported yet\n" ); }
branch: WHEN '{' equations '}' IS pattern { $$.eq = $3; $$.pat = $6; }
	| OTHERWISE IS pattern 		{ $$.eq = NULL; $$.pat = $3; }
	;
equationsopt: '{' equations '}' 	{ $$ = $2; }
	| 				{ $$ = NULL; }
patternsopt: IS pattern 		{ $$ = $2; }
	| 				{ $$ = NULL; }
equations: equations ',' equation	{ $$ = new BinaryExpr(TYPE_UNKNOWN,PAT_AND,$1,$3); }
	| equation;
equation: expr relop expr		{ $$ = new BinaryExpr(TYPE_UNKNOWN,$2,$1,$3); } ;
expr: INTEGER				{ $$ = new IntegerExpr($1); }
	| IDENT bitslice bangOpt	{ Expr *e = new PatternIdent($1,NULL); 
					  if( $2.lo <= $2.hi ) 
					      $$ = new BitSliceExpr(e,$2);
					  else $$ = e; 
					}
	| addressable bitslice bangOpt	{ if( $2.lo <= $2.hi )
					      $$ = new BitSliceExpr($1,$2); 
					  else $$ = $1;
					}
	| expr binop expr		{ $$ = new BinaryExpr(TYPE_UNKNOWN,$2,$1,$3); }
	| STRING			{ $$ = new StringExpr($1); }
	| '(' expr ')'			{ $$ = $2; }
	| opname '(' arglist ')'  { $$ = new InvokeExpr(DICT.insts[*$1.name], $3); }
	;
bitslice: '@' '[' INTEGER ':' INTEGER ']' { $$.lo = $3; $$.hi = $5; }
	| '@' '[' INTEGER ']' 		{ $$.lo = $$.hi = $3; }
	| 				{ $$.lo = 0; $$.hi = -1; } 
	;
binop: '+'				{ $$ = EXP_ADD; }
	| '-'				{ $$ = EXP_SUB; }
	| '*'				{ $$ = EXP_MUL; }
	| '/'				{ $$ = EXP_DIV; }
	;


placeholder: PLACEHOLDER FOR addressable IS pattern;
relocatable: RELOCATABLE idents;
%%
int yyerror(char *s)
{
//    symtab.printOn(stdout);
    fprintf( stderr, "Stopped at %s:%d: %s\n", yyfilename,yylineno, s );
    abort();
}
