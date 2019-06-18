/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       codegen_c.cc
 * OVERVIEW:   C language specific methods (Implementation of CodeGenC)
 *===========================================================================*/

/* $Revision: 1.2 $
 * 23 Apr 01 - Nathan: Initial version
 * 11 May 01 - Nathan: Optimized GetBitslice
 */


#include "codegen_c.h"


string CodeGenC::sprintVariableDecl (const string &type, const string &name)
{
    return string("extern ") + type + " " + name + ";\n";
}
   
string CodeGenC::sprintVariableDefn (const string &type, const string &name)
{
    return type + " " + name + ";\n";
}

string CodeGenC::sprintVariableDefn (const string &type, const string &name,
                                     const string &init)
{
    return type + " " + name + " = " + init + ";\n";
}

string CodeGenC::sprintArrayVarDefn (const string &type, const string &name,
                                     unsigned elems)
{
    return type + " " + name + "[" + elems + "];\n";
}

string CodeGenC::sprintArrayVarDefn (const string &type, const string &name,
                                     unsigned elems, const string &init)
{
    return type + " " + name + "[" + elems + "] = " + init + ";\n";
}

string CodeGenC::sprintParamDeclList (cparams_t &params)
{
    string s = "";
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        if( it != params.begin() )
            s += ", ";
        s += it->type + " " + it->name;
    }
    return s;
}

string CodeGenC::sprintParamCallList (cparams_t &params)
{
    string s = "";
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        if( it != params.begin() )
            s += ", ";
        s += it->name;
    }
    return s;
}

string CodeGenC::sprintFunctionDecl (const string &type, const string &name,
                                     cparams_t &params )
{
    return type + " " + name + "( " + sprintParamDeclList( params ) + " );";
}

string CodeGenC::sprintFunctionDefn (const string &type, const string &name,
                                     cparams_t &params, const string &body)
{
    return type + " " + name + "( " + sprintParamDeclList( params ) + " )\n{\n" +
        body + (body[body.length()-1]=='\n'?"":"\n") + "}\n";
}

string CodeGenC::sprintFunctionCall (const string &name, cparams_t &params)
{
    return name + "( " + sprintParamCallList( params ) + " )";
}

string CodeGenC::sprintClosureDefn (const string &type, const string &name,
                                    cparams_t &params, cparams_t &funcParams,
                                    const string &body)
{
    string structName = "__s_" + name;
    string structTypeName = "__s_" + name + "_t";
    string funcName = "__f_" + name;
    string funcTypeName = "__f_" + name + "_t";
    
    string structure = sprintVariableDefn(
        sprintStructDefn( structTypeName, params ),
        structName );
    string wrapperBody, makeBody;
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        wrapperBody += sprintVariableDefn( it->type, it->name,
                                           structName + "." + it->name );
        makeBody += structName + "." + it->name + " = " + it->name + ";\n";
    }
    string func = sprintFunctionDefn( type, funcName, funcParams,
                                      wrapperBody + body );
    string funcType = string("(") + type + " *)(";
    for( cparams_it it = funcParams.begin(); it != funcParams.end(); it++ ) {
        if( it != funcParams.begin() ) funcType += ",";
        funcType += it->type;
    }
    funcType += ")";
    string funcDef = "typedef " + funcType + " " + funcTypeName + ";\n";
    makeBody += "return " + funcName + ";\n";
    string makeFunc = sprintFunctionDefn( funcTypeName, "__make_" + name, params, makeBody );
    return "/*** Pseudo-closure: " + name + " ***/\n" +
        funcDef + structure + func + makeFunc;
}

string CodeGenC::sprintClosureCreate(const string &name, cparams_t &params)
{
    return sprintFunctionCall( "__make_" + name, params );
}

string CodeGenC::sprintClosureCall(const string &varname, cparams_t &params)
{
    return sprintFunctionCall( "(*" + varname + ")", params );
}

string CodeGenC::sprintClosureType (const string &type, const string &name,
                                    cparams_t &params)
{
    return "__f_" + name + "_t"; /* Use the typedef, luke */
}

string CodeGenC::sprintMacroDefn (const string &name, cparams_t &params,
                                  const string &body)
{
    /* This is probably slow - s/\n/\\\n/g */
    string tmp;
    for( string::const_iterator it = body.begin(); it != body.end(); it++ ) {
        if( *it == '\n' )
            tmp.append("\\\n");
        else
            tmp.append(1,*it);
    }
    if( params.size() == 0 )
        return "#define " + name + " " + tmp + "\n";
    else
        return "#define " + name + "(" + sprintParamCallList(params) + ") " +
            tmp + "\n";
}

string CodeGenC::sprintMacroCall (const string &name, cparams_t &params)
{
    if( params.size() == 0 )
        return name;
    else
        return name + "(" + sprintParamCallList(params) + ") ";
}

string CodeGenC::sprintRecordDefn (const string &form, const string &name, cparams_t &params)
{
    string s = form + " " + name + " {\n";
    for( cparams_it it = params.begin(); it != params.end(); it++ ) {
        if( it->count == 0 )
            s += sprintVariableDefn( it->type, it->name );
        else
            s += sprintArrayVarDefn( it->type, it->name, it->count );
    }
    s += "}";
    return s;
}
string CodeGenC::sprintStructDefn (const string &name, cparams_t &params)
{
    return sprintRecordDefn( "struct", name, params );
}

string CodeGenC::sprintUnionDefn (const string &name, cparams_t &params)
{
    return sprintRecordDefn( "union", name, params );
}

string CodeGenC::sprintComment (const string &comment)
{
    /* FIXME: escaping */
    if( comment.find_first_of('\n') ) { /* Multiline comment */
        return string("/*\n * ") + searchAndReplace(comment, "\n", "\n * ") + "\n */\n";
    }
    return string("/*") + comment + "*/";
}

string CodeGenC::sprintAssign (const string &lhs, const string &rhs)
{
    return lhs + " = " + rhs + ";\n";
}

string CodeGenC::sprintSwitch (const string &on, const string &opts,
                               const string &def)
{
    return string("switch (") + on + ") {\n" + opts +
        (def != string() ? "default: " + def : "") + "}\n";
}

string CodeGenC::sprintSwitchBranch (const string &value, const string &stmt,
                                     bool needBreak)
{
    return string("case ") + value + ": " + stmt + (needBreak?"break;\n": "\n");
}

string CodeGenC::sprintReturn (const string &expr)
{
    return string("return ") + expr + ";\n";
}

string CodeGenC::sprintAddressOf (const string &expr)
{
    return "&(" + expr + ")";
}

string CodeGenC::sprintDereference (const string &expr)
{
    return "*(" + expr + ")";
}

string CodeGenC::sprintType (const Type &type)
{
    int width = type.getSize();
    if( width <= 0 || type.getType() == TVOID ) 
        return "void";
    if( type.getType() == INTEGER ) {
        if( width > 64 ) return "NO_SUCH_TYPE";
        if( type.getSigned() ) {
            if( width <= 8 ) return "sint8_t";
            if( width <= 16 ) return "sint16_t";
            if( width <= 32 ) return "sint32_t";
            if( width <= 64 ) return "sint64_t";
        } else {
            if( width <= 8 ) return "uint8_t";
            if( width <= 16 ) return "uint16_t";
            if( width <= 32 ) return "uint32_t";
            if( width <= 64 ) return "uint64_t";
        }
    } else if( type.getType() == BOOLEAN ) {
        return "uint8_t";
    } else { /* FLOATP */
        if( width == 32 ) return "float32_t";
        if( width == 64 ) return "float64_t";
        if( width > 64 ) return "float128_t";
    }
    return "NO_SUCH_TYPE";
}

bool CodeGenC::hasExactType( const Type &type )
{
    int width = type.getSize();
    if( type.getType() == INTEGER ) {
        return (width == 8 || width == 16 || width == 32 || width == 64);
    } else if( type.getType() == FLOATP ) {
        return (width == 32 || width == 64 || width == 128 );
    } else if (type.getType() == BOOLEAN ) {
        return width == 1;
    }
    return false;
}
Type CodeGenC::getNearestType( const Type &type )
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
        else if( width <= 64 ) result.setSize(64);
        else result.setSize(128);
    } else if (type.getType() != BOOLEAN ) {
        result = VOID_TYPE;
    }
    return result;
}
string CodeGenC::sprintTypeCast (const Type &fromType, const Type &toType,
                                 const string &expr)
{
    if( fromType == toType || toType.getSize() == 0 ||
        (fromType.getType() == INTEGER && toType.getType() == BOOLEAN) ||
        (fromType.getType() == BOOLEAN && toType.getType() == INTEGER) ||
        sprintType(toType) == sprintType(fromType) ) {
        return expr;
    }
    
    return "((" + sprintType(toType) + ")" + expr + ")";
}

string CodeGenC::sprintTypeChange (const Type &fromType, const Type &toType,
                                   const string &expr)
{
    if( fromType == toType || toType.getSize() == 0 ||
        (fromType.getType() == INTEGER && toType.getType() == BOOLEAN) ||
        (fromType.getType() == BOOLEAN && toType.getType() == INTEGER) ) {
        return expr;
    }
    
    if( fromType.getType() == toType.getType() &&
        (fromType.getSize() == toType.getSize() ||
         fromType.getType() == INTEGER ) ) {
        /* differ only in signedness, so just do a regular cast
         * = easier to read :)
         */
        return sprintTypeCast( fromType, toType, expr );
    }
    if( fromType.getSize() == toType.getSize() ) {
        if( fromType == Type(FLOATP,32,true) )
            return sprintTypeCast( SIGNED(32), toType,
                                   "float32ToSint32(" + expr + ")" );
        if( fromType == Type(FLOATP,64,true) )
            return sprintTypeCast( SIGNED(64), toType,
                                   "float64ToSint64(" + expr + ")" );
        if( toType == Type(FLOATP,32,true) ) 
            return "sint32ToFloat32(" +
                sprintTypeCast( fromType, SIGNED(32), expr ) + ")";
        if( toType == Type(FLOATP,64,true) ) 
            return "sint64ToFloat64(" +
                sprintTypeCast( fromType, SIGNED(64), expr ) + ")";
    }
    /* Fallback - warning only works on LValues */
    return "(*((" + sprintType(toType) + " *)&(" + expr + ")))";
}

string CodeGenC::sprintLiteral(const string &value)
{
    /* FIXME: Need to do escaping here */
    return string("\"") + value + "\"";
}

string CodeGenC::sprintLiteral(int value)
{
    char buf[25];
    sprintf( buf, "%d", value );
    return string(buf);
}

string CodeGenC::sprintLiteral(long long value)
{
    char buf[25];
    sprintf( buf, "%lldL", value );
    return string(buf);
}

string CodeGenC::sprintLiteral(double value)
{
    char buf[25];
    sprintf( buf, "%g", value );
    return string(buf);
}

string CodeGenC::sprintLiteral(const list<int> &values)
{
    string s = "{ ";
    for( list<int>::const_iterator it = values.begin(); it != values.end(); it++ ) {
        if( it != values.begin() ) s += ", ";
        s += sprintLiteral(*it);
    }
    return s + " }";
}

string CodeGenC::sprintLiteral(const list<string> &values)
{
    string s = "{ ";
    for( list<string>::const_iterator it = values.begin(); it != values.end(); it++ ) {
        if( it != values.begin() ) s += ", ";
        s += sprintLiteral(*it);
    }
    return s + " }";
}

string CodeGenC::sprintNames(const list<string> &values)
{
    string s = "{ ";
    for( list<string>::const_iterator it = values.begin(); it != values.end(); it++ ) {
        if( it != values.begin() ) s += ", ";
        s += *it;
    }
    return s + " }";
}

string CodeGenC::sprintConstStringList( const string &name,
                                        const list<string> &arr )
{
    return "static const char *" + name + "[" + arr.size() + "] = " +
        sprintLiteral(arr) + ";\n";
}


string CodeGenC::sprintGetBitslice( const Type &baseType, const string &expr,
                                 int lo, int hi )
{
    string s = expr;
    Type resType = baseType;
    resType.setSize( hi-lo+1 );
    
    if( lo > 0 )
        s = string("(") + s + ">>" + lo + ")";
    if( hi + 1 != baseType.getSize() || baseType.getType() == UNKNOWN ||
        baseType.getSigned() == true ) {
        char buf[20];
        long long mask = (1LL<<(hi-lo+1))-1;
        sprintf( buf, "0x%llxL", mask );
        s = string("(") + s + "&" + buf + ")";
    }
    return sprintTypeCast(baseType, resType, s);
}

string CodeGenC::sprintSetBitslice( const Type &baseType, const string &lhs,
                                    const string &rhs, int lo, int hi )
{
    char buf[32], invbuf[32];
    long long mask = ((1LL<<(hi-lo+1))-1);
    sprintf( buf, "0x%llxL", mask );
    sprintf( invbuf, "0x%llxL", ~mask );
    
    return lhs + " = (" + lhs + " & " + invbuf + ") | ((" +
        rhs + " & " + buf + ") <<" + lo + ");\n";
}

string CodeGenC::sprintFormattedPrint( const string &fmt,
                                       const list<string> &parms,
                                       const string &tgt )
{
    string s = "fprintf( " + tgt + ", \"" + fmt + "\"";
    for( list<string>::const_iterator it = parms.begin();
         it != parms.end(); it++ ) {
        s += ", " + *it;
    }
    s += ");\n";
    return s;
}
