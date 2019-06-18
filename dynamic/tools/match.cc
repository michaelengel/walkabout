/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       match.cc
 * OVERVIEW:   routines to generate matching statements suitable for processing
 *             by the New Jersey toolkit
 *===========================================================================*/

/* $Revision: 1.2 $
 * 18 May 01 - Nathan: Initial version, split from gendecode.cc
 */

#include "codegen.h"
#include "ss.h"
#include "sledtree.h"

string mangleMatchParam(const string &);

string sprintMatchLine( const string &name, const list<int> &params,
                        const string &body )
{
    string s = "| \"" + name + "\" (";
    for( list<int>::const_iterator it = params.begin(); it != params.end();
         it++ ) {
        if( it != params.begin() )
            s += ", ";
        s += mangleMatchParam(theSemTable[*it].sName);
    }
    return s + ") => " + body;
}


string sprintMatchLine( const string &name,
                        const vector<OperandIdent *> &params,
                        const string &body )
{
    string s = "| \"" + name + "\" (";
    for( vector<OperandIdent *>::const_iterator it = params.begin();
         it != params.end(); it++ ) {
        if( it != params.begin() )
            s += ", ";
        s += (*it)->name;
    }
    return s + ") => " + body;
}
    
