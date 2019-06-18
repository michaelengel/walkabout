/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       codegen_java.cc
 * OVERVIEW:   Java language specific methods (Implementation of CodeGenJava)
 *===========================================================================*/

/* $Revision: 1.2 $
 * 23 Apr 01 - Nathan: Initial version
 */


#include "codegen_java.h"


string CodeGenJava::sprintVariableDefn (const string &type, const string &name)
{
    return type + " " + name + ";\n";
}

string CodeGenJava::sprintArrayVarDefn (const string &type, const string &name,
                                     unsigned elems)
{
    return type + " " + name + "[] = new " + type + "[" + elems + "];\n";
}

string CodeGenJava::sprintFunctionDefn (const string &type, const string &name,
                                   cparams_t &params, const string &body)
{
    return string("public ") +
                  CodeGenC::sprintFunctionDefn( type, name, params, body );
}
string CodeGenJava::sprintStructDefn (const string &name, cparams_t &params)
{
    return sprintRecordDefn( "class", name, params );
}

string CodeGenJava::sprintClosureDefn (const string &type, const string &name,
                                       cparams_t &params, cparams_t &funcParams,
                                       const string &body)
{
    /* Build an inner class, with a constructor and a single invoke
     * method */
    string baseType = sprintClosureType(type, name, params);
    string clzName = "__s_" + name + "_t";
    string clz = "class " + clzName + " extends " + baseType + " {\n";
    string consBody = "";
    string invokeBody = "";
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        consBody += "__l_" + it->name + " = " + it->name + ";\n";
        clz += sprintVariableDefn(it->type, "__l_" + it->name);
    }
    consBody += "return this;\n";
    string funcs = sprintFunctionDefn(baseType, "set", params, consBody) +
        sprintFunctionDefn(type, "invoke", funcParams, body);
    clz += funcs + "};\n";

    if( closureTypes.find(baseType) == closureTypes.end() ) {
        /* New base type */
        string clz = "class " + baseType + " {\n" + funcs + "};\n" + clz;
        closureTypes.insert(baseType);
    }
    
    return clz + clzName + " __s_" + name + ";\n";
}

string CodeGenJava::sprintClosureCreate (const string &name, cparams_t &params)
{
    return sprintFunctionCall( "__s_" + name + ".set", params );
}

string CodeGenJava::sprintClosureCall(const string &varname, cparams_t &params)
{
    return sprintFunctionCall( varname + ".invoke", params );
}

string CodeGenJava::sprintClosureType (const string &type, const string &name,
                                       cparams_t &params)
{
    string tag = "";
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        if( it->type == "boolean" ) tag += "B";
        else tag += it->type[0];
    }
    return type + "Closure" + tag;
}


string CodeGenJava::sprintType (const Type &type)
{
    int width = type.getSize();
    if( width <= 0 ) return "void";
    if( type.getType() == INTEGER ) {
        if( width == 1 ) return "int"; // avoids a lot of casts :)
        if( width <= 8 ) return "byte";
        if( width <= 16 ) return "short";
        if( width <= 32 ) return "int";
        if( width <= 64 ) return "long";
        return string("int") + width;
    } else if( type.getType() == BOOLEAN ) {
        return "boolean";
    } else { /* FLOATP */
        if( width <= 32 ) return "float";
        if( width <= 64 ) return "double";
        return string("float") + width;
    }
    return "NO_SUCH_TYPE";
}

bool CodeGenJava::hasExactType (const Type &type)
{
    int width = type.getSize();
    if( type.getType() == INTEGER ) {
        return type.getSigned() == true &&
            (width == 8 || width == 16 || width == 32 || width == 64);
    } else if( type.getType() == FLOATP ) {
        return (width == 32 || width == 64 );
    } else if (type.getType() == BOOLEAN ) {
        return width == 1;
    }
    return false;
}
Type CodeGenJava::getNearestType( const Type &type )
{
    int width = type.getSize();
    Type result = type;
    if( type.getType() == INTEGER ) {
        if( width <= 8 ) result.setSize(8);
        else if( width <= 16 ) result.setSize(16);
        else if( width <= 32 ) result.setSize(32);
        else result.setSize(64);
    } else if( type.getType() == FLOATP ) {
        if( width <= 32 ) result.setSize(32);
        else result.setSize(64);
    } else if (type.getType() != BOOLEAN ) {
        result = VOID_TYPE;
    }
    return result;
}

string CodeGenJava::sprintTypeCast (const Type &fromType, const Type &toType,
                                      const string &expr)
{
    if( toType.getType() == BOOLEAN && fromType.getType() == INTEGER ||
        fromType.getType() == UNKNOWN ) {
        return "((" + expr + ") != 0)";
    } else if( fromType.getType() == BOOLEAN && toType.getType() == INTEGER ) {
        return "((" + expr + ") ? 1 : 0)";
    } else if( (!isTypeSupported(toType) || !isTypeSupported(fromType)) &&
               !(toType == fromType) ) {
        return "cast" + sprintType(fromType) + "To" + sprintType(toType) +
            "(" + expr + ")";
    } else {
        return CodeGenC::sprintTypeCast( fromType, toType, expr );
    }
}

string CodeGenJava::sprintTypeChange (const Type &fromType, const Type &toType,
                                      const string &expr)
{
    /* This is somewhat less trivial in java, actually, and we depend on
     * a few functions in the skeleton to do the basic conversions for us.
     */
    if( fromType.getType() == toType.getType() ){
        /* Types are either the same or differ only in size/signedness */
        if( fromType.getType() == FLOATP ) {
            if( fromType.getSize() == 32 && toType.getSize() == 64 )
                return "floatToDouble(" + expr + ")";
            else if( fromType.getSize() == 64 && toType.getSize() == 32 )
                return "doubleToFloat(" + expr + ")";
        }
        return sprintTypeCast( fromType, toType, expr );
    } else {
        string s;
        if( fromType.getType() == INTEGER && toType.getType() == FLOATP ) {
            /* Int to float */
            if( fromType.getSize() == 32 ) s = "intToFloat(" + expr + ")";
            else if( fromType.getSize() == 64 ) s = "longToDouble(" + expr + ")";
            else s = "UNSUPPORTED_CONVERSION(" + expr + ")";
        } else if( toType.getType() == INTEGER && fromType.getType() == FLOATP ) {
            /* float to int */
            if( fromType.getSize() == 32 ) s = "floatToInt(" + expr + ")";
            else if( fromType.getSize() == 64 ) s = "doubleToLong(" + expr + ")";
            else s = "UNSUPPORTED_CONVERSION(" + expr + ")";
        } else {
            return sprintTypeCast( fromType, toType, expr );
        }
        Type tmpType = fromType;
        tmpType.setType( toType.getType() );
        /*
         * Similarly, we do an additional cast here in the case that the sizes
         * differ (really though, try not to do that :)
         */
        return sprintTypeCast( tmpType, toType, s );
    }
        
}

string CodeGenJava::sprintGetBitslice( const Type &baseType, const string &expr,
                                       int lo, int hi )
{
    /* Mostly duplicated from CodeGenC - to change >> to >>>. Argh. */
    string s;
    Type unsType = baseType;
    
    if( baseType.getType() == FLOATP ) {
        unsType.setType(INTEGER);
        s = sprintTypeChange( baseType, unsType, expr );
    } else {
        s = "(" + expr + ")";
    }
    
    if( lo > 0 )
        s = string("(") + s + ">>>" + lo + ")";
    if( hi + 1 != baseType.getSize() || baseType.getType() == UNKNOWN ) {
        char buf[20];
        long long mask = (1LL<<(hi-lo+1))-1;
        sprintf( buf, "0x%llxL", mask );
        s = string("(") + s + "&" + buf + ")";
    }
    if( !(unsType == baseType) )
        return sprintTypeChange(unsType, baseType, s);
    return s;
}                                 

string CodeGenJava::sprintSetBitslice( const Type &baseType, const string &lhs,
                                    const string &rhs, int lo, int hi )
{
    /* Special case again */
    if( lo == 0 && hi >= baseType.getSize()-1 )
        return lhs + " = " + rhs + ";\n";
    
    /* Same as above, except it takes a little more work */
    if( baseType.getType() == FLOATP ) {
        Type intType = baseType;
        intType.setType(INTEGER);

        char buf[32], invbuf[32];
        long long mask = ((1LL<<(hi-lo+1))-1);
        sprintf( buf, "0x%llxL", mask );
        sprintf( invbuf, "0x%llxL", ~mask );
        
        return lhs + " = " +
            sprintTypeChange(intType, baseType, "(" + 
                             sprintTypeChange(baseType, intType, lhs) +
                             " & " + invbuf + ") | (" +
                             sprintTypeChange(baseType, intType, rhs) +
                             " & " + buf + ") <<" + lo ) + ";\n";

    } else return CodeGenC::sprintSetBitslice( baseType, lhs, rhs, lo, hi );    
}

string CodeGenJava::sprintConstStringList( const string &name,
                                           const list<string> &arr )
{
    return "final static String " + name + "[] = " +
        sprintLiteral(arr) + ";\n";
}

string CodeGenJava::sprintFormattedPrint( const string &fmt,
                                          const list<string> &params,
                                          const string &tgt )
{
    /* This is fun - Java text support bites big time. So basically we
     * end up converting a simple printf fmt into a great big pile
     * of string concatenations
     */
    string s = tgt + ".print( \"\"";

    string line;
    list<string>::const_iterator arg = params.begin();
    for( unsigned int i = 0; i < fmt.length(); i++ ) {
        if( fmt[i] == '%' ) {
            i++;
            if( fmt[i] == '%' ) {
                line += fmt[i];
                continue;
            }
            if( line.length() > 0 ) {
                s += " + \"" + line + "\"";
                line = "";
            }
            string base;
            bool zeropad = false;
            bool forcesign = false;
            bool rightpad = false;
            string intType = "Integer";
            int width = 0;
            int prec = 0;
            if( fmt[i] == '+' ){ forcesign = true; i++; }
            if( fmt[i] == '-' ){ rightpad = true; i++; }
            if( fmt[i]  == '0' ){ zeropad = true; i++; }
            while( isdigit(fmt[i]) ) {
                width = width * 10 + (fmt[i] - '0');
                i++;
            }
            if( fmt[i] == '.' ){
                i++;
                while( isdigit(fmt[i]) ) {
                    prec = prec * 10 + (fmt[i] - '0');
                    i++;
                }
            }
            if( fmt[i] == 'h' ){ i++; intType = "Short"; }
            else if( fmt[i] == 'l' ) { i++; intType = "Long"; }
            switch(fmt[i]) {
                case 's':
                case 'c':
                    base = *arg++;
                    break;
                case 'd':
                    base = intType + ".toString(" + (*arg++) + ")";
                    break;
                case 'x':
                    base = intType + ".toHexString(" + (*arg++) + ")";
                    break;
                case 'e':
                    /* Need to do better here */
                    base = "Float.toString(" + (*arg++) + ")";
                    break;
            }
            if( zeropad && width) {
                base = "zeroPad( " + base + ", " + width + " )";
            } else if( rightpad && width ) {
                base = "rightPad( " + base + ", " + width + " )";
            } else if( width ) {
                base = "leftPad( " + base + ", " + width + " )";
            }
            s += " + (" + base + ")";
        } else {
            line += fmt[i];
        }
    }
    if( line.length() > 0 ) {
        s += " + \"" + line + "\"";
    }
    
    return s + " );\n";
}
