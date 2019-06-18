/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// Updates:
// May 2001 - David: initial version
// 21 May 2001 - David: added pattern to match ${ .... $}
// 23 May 2001 - David: problem with pattern matching ${ .... $} with multiple 
//		tables as the lexer alway returns the match with the longest text.  
//		New start condition (exclusive) "inst_code" is called at the instance 
//		of ${
// 06 Jun 2001 - David: changed the syntax. Added s2 more start conditions 
//		definition and ssl_param (for both SSL() and PARAM())


%name InstrmScanner
 
%header{
#include "instrm_parser.h"
#define MAX_LINE 1024      
%}

%{
#include "instrm_scanner.h"
%}

%define CONSTRUCTOR_PARAM FILE* inFile, bool trace
%define CONSTRUCTOR_INIT : theLine(1), traceLines(trace)
%define CONSTRUCTOR_CODE  yyin = inFile;
%define MEMBERS \
public: \
    int theLine;        /* the current line number */\
    char lineBuf[MAX_LINE]; /* the current line */ \
    bool traceLines;    /* echo each lines as it is scanned */
	int level;

%define LEX_PARAM YY_InstrmParser_STYPE &yylval 

%x inst_code
%x support_code
%x ssl_param
%%


[,"\[\]] 		{ 	
					return yytext[0];   
				}
				
		
"{"    			{
					BEGIN(inst_code);
					level = 1;
					return yytext[0];   
				}
				
DEFINITION  	{
					yylval.str = strdup(yytext);
	        		return InstrmParser::DEFINITION;
				}
				
INSTRUMENTATION_ROUTINES  	{
					BEGIN(support_code);
					yylval.str = strdup(yytext);
        			return InstrmParser::INSTRUMENTATION_ROUTINES;
				}

FETCHEXECUTE  	{
					yylval.str = strdup(yytext);
        			return InstrmParser::FETCHEXECUTE;
				}
				
_?[A-Za-z][A-Za-z0-9_]* {
					yylval.str = strdup(yytext);
	        		return InstrmParser::NAME;
				}
				
[ \t]+  		;
#.*$    		;   /* comment */
.               ;

	 
	 				
<inst_code>"{"  {
					// ECHO;
					level++;
					yymore();
				}
				
<inst_code>"}"  {
					// ECHO;
					level--;
					if (level == 0) {
						BEGIN(INITIAL);
						yylval.str = strdup(yytext);
						return InstrmParser::INSTRM_END;   
					}
					yymore();
				}
				
<inst_code>"SSL("  {
					// ECHO;
					BEGIN(ssl_param);
					// copy all the C code upto this point and return it.
					yylval.str = strdup(yytext);
        			return InstrmParser::SSL;
				}
				
<inst_code>"PARAM("  {
					// ECHO;
					BEGIN(ssl_param);
					// copy all the C code upto this point and return it.
					yylval.str = strdup(yytext);
        			return InstrmParser::PARAM;
				}
				
<inst_code>"SSL_INST_SEMANTICS"  {
					yylval.str = strdup(yytext);
        			return InstrmParser::SSL_INST_SEMANTICS;
				}
				
<inst_code>.	{
					yymore();
					// yylval.str = strdup(yytext);
					// return InstrmParser::C_CODE;
				}
				
<inst_code>"\n"	{
					yymore();
				}
				
<ssl_param>")"  {
					BEGIN(inst_code);
					// yylval.str = strdup(yytext);
					// return InstrmParser::SSL_PARAM_END;   
					return yytext[0];   
				}
				
<ssl_param>"%"  {
					return yytext[0];   
				}
				
<ssl_param>_?[A-Za-z][A-Za-z0-9_]* {
					yylval.str = strdup(yytext);
        			return InstrmParser::NAME;
				}
				
<support_code>(.|\n)*	{
					yylval.str = strdup(yytext);
					return InstrmParser::C_CODE;   
				}
	
\n.*            {   // funky print-next-line-for-debug token
                    if (yyleng > MAX_LINE)
                    {
                        cerr << theLine + 1 << ": line too long.\n";
                        exit(1);
                    }
                    // saves the next line
                    strcpy(lineBuf, yytext+1);
                    theLine++;
                    // reparse from char 1
                    // give back all but the \n to rescan
                    yyless(1);

                    // echo the line if line tracing is turned on
                    if (traceLines)
                    cerr << lineBuf << endl;

                    // Quell a warning
                    yy_last_accepting_state = 0;
                    yy_last_accepting_cpos = 0;
                }
%%

// old rules

// "${"(.|\n)*(\n"$}")  {
//				yylval.str = strdup(yytext);
//       		return InstrmParser::INSTC_CODE;
//      		}

//"%%"(.|\n)*(\n"%%")  {
//					yylval.str = strdup(yytext);
//        			return InstrmParser::SUPP_CODE;
//     			}
