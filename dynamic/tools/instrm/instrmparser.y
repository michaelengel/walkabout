/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// Updates:
// -- May 2001 - David: initial version
// 04 Jun 2001 - David: changed the syntax. added special tokens and states 
//					for parsing the instrumented C code.



%name InstrmParser     

// stuff to go in instrmparser.h
%header{
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

%}

//============================================================================
// Declaration of semantic value type. Must be first thing in
// sslparser.cc (or at least before including sslscanner.h which needs
// to know about this type).
//============================================================================

%union {
    char*			str;
    int             num;
	list<string>*	lstring;
}

%{
#include "instrm_scanner.h"
%}

%define DEBUG 1 

%define PARSE_PARAM \

%define CONSTRUCTOR_PARAM \
    const string& instrmFile, \
    bool trace

%define CONSTRUCTOR_CODE \
    FILE* inFile = fopen(instrmFile.c_str(),"r"); \
    if (inFile == NULL) { \
        ostrstream ost; \
        ost << "can't open `" << instrmFile << "' for reading"; \
        abort(); \
    } \
    Scanner = new InstrmScanner(inFile, trace); \
    if (trace) yydebug = 1;

%define MEMBERS \
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


/*==============================================================================
 * Declaration of token types, associativity and precedence
 *============================================================================*/
%token INSTRUMENTATION_ROUTINES FETCHEXECUTE DEFINITION 
%token <str> NAME SSL PARAM C_CODE INSTRM_END SSL_INST_SEMANTICS

%type <str> inst
%type <lstring> list_parameter

%%

specification:
        specification parts
    |   parts
    ;

parts:
        DEFINITION definition
    |   fetch_execute 
	|	support_code        
    ;

definition:
		definition definition_part
	|	definition_part
	;
	
definition_part:
		table_assign 
	| 	semantics
	;

table_assign:
        NAME '[' {
			if (table2icode.find($1) != table2icode.end()) {
				cerr << "Duplicate table name - " << $1 << endl;
				abort();
			}
			// create the table
			table2icode[$1] = current_table = new table_code;
			//tableset = new set<string>;
						
		} insts ']' {
			// create the table name and add the list of instructions
			//table2icode[$1]->tableset = tableset;
			
			for (set<string>::iterator i=current_table->tableset.begin(); 
				i!=current_table->tableset.end(); i++) {
				inst2icode[*i] = current_table;
				
			}
        }
   	;

insts:
        inst ',' insts {
			current_table->tableset.insert($1);
        }
		
	| 	inst {
			current_table->tableset.insert($1);	
		}
	;
		
inst:
		'"' NAME '"' {
        	$$ = $2;
		}
    ;


semantics:
        NAME list_parameter {
		
			// $1 with the SSL??  no, can have same name but different number 
			// of instr in the table.
			
			// check parameter? hmm possibly need to make sure each 
			// instruction here corresponds exactly as it is specified in 
			// the SSL.
			
			if (table2icode.find($1) == table2icode.end()) {
				cerr << $1 << "Not found." << endl;
				abort();
			}
			current_table = table2icode[$1];
 			current_table->param_list = $2;
 			// current_table->instrm_code = $3;

        } instrm_code
	;
		
fetch_execute:
		FETCHEXECUTE {
			if (table2icode.find("FETCHEXECUTE") != table2icode.end()) {
				cerr << "More than one fetch_execute defined." << endl;
				abort();
			}
			current_table = table2icode["FETCHEXECUTE"] = 
				inst2icode["FETCHEXECUTE"] = new table_code;;

		} instrm_code		
	;
	

list_parameter:
        list_parameter ',' NAME {
            $1->push_back($3);
            $$ = $1;
        }

    |   NAME {
    		$$ = new list<string>;
			$$->push_back($1);
        }
    ;
	

/* instrm_code:
		INSTC_CODE {
			string icode = $1;			
			// kill the enclosing brakets
			unsigned pos = icode.find("$}");
			icode.replace(pos, 2, "");
		    pos = icode.find("${");
			icode.replace(pos, 2, "");
			// cerr << "icdoe = : " << icode << endl;
			
			// search and replace for all parameter
			int off = 0;
			while (1) {
				unsigned pos = icode.find("$", off);
				if (pos == icode.npos) {
					break;
				}
											
				switch (icode[pos+1]) {
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
					// parameters				
					int param_no = icode[pos+1];
					list<string>::iterator i = current_table->param_list->begin();
					param_no--;
					while (param_no) {
						i++;
						param_no--;
					}
					icode.replace(pos, 2, *i);
					break;
				}
				default:
					break;
				}
				off += 2;
			}
			
			pos = icode.find("$$");
			// separate the bodies of code before and after instrumentation 
			// semantics
			current_table->before = icode.substr(0, pos); // kill the enclosing
														  // brackets
			current_table->after  = icode.substr(pos+2, icode.npos);
			// cerr << "Before: " << current_table->before << endl;
			// cerr << "After: " << current_table->after << endl;			
		}
		;
*/ 

instrm_code:
		'{' instrm_body INSTRM_END {
			$3[strlen($3)-2] = 0;	// remove the last '}'
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, $3));
		}
	;
		
instrm_body:
		instrm_body	instrm_body_part
	|	instrm_body_part
	;
		
instrm_body_part:
		PARAM NAME ')' {			
			$1[strlen($1)-6] = 0;	// remove the last 'PARAM('
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, $1));
			// look up the parameter list for this table and determined the 
			// parameter number
			int param_num = 0;
			list<string>::iterator i;			
			for (i=current_table->param_list->begin(); i!=current_table->param_list->end(); 
					i++, param_num++) {
				if ((*i) == $2) {
					current_table->instrm_code.push_back(
						pair<int, void*>(T_PARAM, (void*)param_num));
					break;
				}
			}
			if (i == current_table->param_list->end()) {
				cerr << "Parameter " << $2 << " undefined." << endl;
			}
		}
		
	|	SSL '%' NAME ')' {
			$1[strlen($1)-4] = 0;	// remove the last 'SSL('
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, $1));
			// cheat by prepending 'regs.r' to $3
			char* t = (char*)malloc(sizeof(char)*12);
			memcpy(t, "regs.r_", 8);
			current_table->instrm_code.push_back(pair<int, void*>(T_SSL, strcat(t, $3)));
		}
		
	|	SSL_INST_SEMANTICS {
			$1[strlen($1)-19] = 0;	// remove the last 'SSL_INST_SEMANTICS'
			current_table->instrm_code.push_back(pair<int, void*>(T_CODE, $1));
			current_table->instrm_code.push_back(pair<int, void*>(T_SEMANTICS, 0));
		}
	;	

/*support_code:
		INSTRUMENTATION_ROUTINES {
			support_code = $1;
			unsigned pos = support_code.find("%%");
			support_code.replace(pos, 2, "");
		    pos = support_code.find("%%");
			support_code.replace(pos, 2, "");
		}	
*/

support_code:
		INSTRUMENTATION_ROUTINES C_CODE {
			support_code = $2;
		}	
	;

%%

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

