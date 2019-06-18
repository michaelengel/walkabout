/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */


#include <stdlib.h>
#include "instrm_parser.h"

int main(int argc, char* argv[])
{
	extern char *optarg;
    char *inst_file = NULL;

    while(1) {
        int c = getopt( argc, argv, "i:" );
        if( c == -1 ) break;
        switch(c) {
 	    	case 'i': inst_file = optarg;
              		  cerr << "instrument file = " << inst_file << endl;
    		          break;
            case '?': fprintf( stderr, "Unrecognized option\n" );
        }
    }
	cout << "This is a program used for testing the input instrumentation!\n";
    if( argc < 2) {
        cout << "Usage: instrm_test -i <filename>" << endl;
		return 1;
    }
	
	InstrmParser instrm_parser(inst_file, 1);
	instrm_parser.yyparse();

	// print the table contents of the instrumented code 		
	map<string, table_code*>::iterator i;
	for (i=instrm_parser.inst2icode.begin(); i!=instrm_parser.inst2icode.end();
		i++) {
		cerr << "Instruction: " << (*i).first << " is part of table " << 
			(*i).second << endl;
	}
	cerr << endl;
	for (i=instrm_parser.table2icode.begin(); 
			i!=instrm_parser.table2icode.end(); i++) {
		cerr << "*Table: " << (*i).first << endl;
		list< pair<int, void*> >::iterator li;
		for (li=(*i).second->instrm_code.begin(); 
				li!=(*i).second->instrm_code.end(); li++) {
			switch ((*li).first) {
			case T_CODE:
				cerr << "TCODE: " << (char*)((*li).second) << endl;
				break;
			case T_SSL:
				cerr << "TSSL: " << (char*)((*li).second) << endl;
				break;
			case T_SEMANTICS:
				cerr << "SEMANTICS" << endl;
				break;
			case T_PARAM:
				cerr << "TPARAM: " << (int)((*li).second) << endl;
				break;
			default:
				abort();
			}
		}
	}
	
}
