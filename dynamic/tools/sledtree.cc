/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*=============================================================================
 * FILE:       sledtree.cc
 * OVERVIEW:   All the functions needed to create and solve the abstract syntax
 *             tree which results from parsing in a sled specification.
 *===========================================================================*/

/* $Revision: 1.2 $
 * 07 Feb 01 - Nathan: Initial version
 */


#include "sledtree.h"

#ifdef DEBUG_SLED
#define dprintf printf
#else
#define dprintf(arg...)
#endif

Symtab symtab("SLED Symbol table");

void FieldIdent::printOn( FILE *f ) const
{
    fprintf( f, "FieldIdent: %s [%d:%d] %s", name.c_str(),
             lo, hi, (checked==FC_CHECKED?"":
                      checked==FC_UNCHECKED?"unchecked ":"guaranteed "));
    if( parent )
        fprintf( f, "in %s", parent->name.c_str());
#if 0
    if( names ) for( int i=0; i<(1<<(hi-lo+1)); i++ )
        fprintf(f, "  %d => %s\n", i,
                (i<names->size()&&(*names)[i]!=NULL) ? (*names)[i]->c_str():"");
#endif
}

int FieldIdent::getSize( void )
{
    if( parent )
        return parent->getSize();
    else
        return hi-lo+1;
}

bool FieldIdent::findName( const string &name, int *result ) const
{
    if( names ) {
        for( unsigned i=0; i < names->size(); i++ ) {
            if( (*names)[i] && *((*names)[i]) == name ) {
                if( result ) *result = i;
                return true;
            }
        }
    }
    return false;
}

BitStringExpr *FieldIdent::toBitStringExpr()
{
    BitStringExpr *expr = new BitStringExpr();
    expr->strs.add( getSize(), OP_EQUAL, -1, -1, 0 );
    return expr;
}
    
bool FieldIdent::isEquivalent( const FieldIdent &a ) const
{
    if( hi-lo != a.hi-a.lo ) /* must be the same size */
        return false;
    if( names == a.names ) /* if they share the vector (possibly both null) */
        return true;
    if( names == NULL || a.names == NULL ) /* only one is null = not equiv. */
        return false;
    if( names->size() != a.names->size() ) /* vector must be the same size */
        return false;
    
    /* Both have a name vector, so compare names */
    for( unsigned i=0; i<names->size(); i++ ) {
        string *n1 = (*names)[i];
        string *n2 = (*a.names)[i];
        if( n1 == n2 ) continue;
        if( n1 == NULL || n2 == NULL || *n1 != *n2 ) return false;
    }
    return true;
}

bool FieldIdent::overlaps( const FieldIdent &a ) const
{
    if( names == NULL || a.names == NULL ) return false;
    for( unsigned i=0; i<names->size(); i++ ) {
        if( (*names)[i] != NULL && a.findName( *(*names)[i], NULL ) )
            return true;
    }
    return false;
}

void RelocIdent::printOn( FILE *f) const
{
    fprintf( f, "RelocIdent: %s", name.c_str() );
}
void PatternIdent::printOn( FILE *f ) const
{
    fprintf( f, "PatternIdent: %s => ", name.c_str() );
    if( expr ) expr->printOn( f );
}
void PatternIdent::add( Expr *e, ExprOp op )
{
    if( expr == NULL )
        expr = e;
    else
        expr = new BinaryExpr( expr->type, op, expr, e );
}

void OperandIdent::printOn( FILE *f ) const
{
    fprintf( f, "OperandIdent: %s ", name.c_str() );
}
void InstIdent::printOn( FILE *f ) const
{
    fprintf( f, "Instruction: %s %s => ", name.c_str(), disasm.c_str() );
    if( expr ) expr->printOn( f );
    for( vector<OperandIdent *>::const_iterator i = params.begin(); i != params.end(); i++ ){
        fprintf( f, "\n    %s => ", (*i)->name.c_str() );
        if((*i)->expr) (*i)->expr->printOn(f);
    }
    fprintf( f, "\n" );
}
Expr *InstIdent::solve()
{
    if( expr ) return expr->solve();
    return expr;
}

/* Expressions */
static char *exprOpNames[] = { "<=","<","=","!=",">",">=",
                               "|","&",";","...","...",
                               "+","-","*","/" };
                               
void BinaryExpr::printOn( FILE *f ) const
{
    fprintf( f, "( " );
    if( left ) left->printOn(f);
    else fprintf( f, "null" );
    fprintf( f, " %s ", exprOpNames[op] );
    if( right ) right->printOn(f);
    else fprintf( f, "null" );
    fprintf( f, " )" );
}

Expr *BinaryExpr::expand( int *n )
{
    BinaryExpr *expr = new BinaryExpr(*this);
    expr->right = (right ? right->expand(n) : NULL);
    expr->left = (left ? left->expand(n) : NULL);
    return expr;
}

Expr *BinaryExpr::substitute( subst_func_t *sub )
{
    if( (*sub)[this] )
        return (*sub)[this]->clone();
    BinaryExpr *expr = new BinaryExpr(*this);
    expr->left = (left ? left->substitute(sub) : NULL );
    expr->right = (right ? right->substitute(sub) : NULL );
    return expr;
}

int BinaryExpr::computePosn( int in )
{
    posn = in;
    if( op == PAT_SEQ )
        return right->computePosn(left->computePosn(in));
    else {
        int l =0,r = 0;
        if( left )
            l = left->computePosn(in);
        if( right )
            r = right->computePosn(in);
        if( l != r && l != in && r != in )
            printf("Warning: sizes don't match - %d,%d\n",l,r);
        return (l>r?l:r);
    }
}

Expr *BinaryExpr::solve( void )
{
    Expr *l,*r;

    l = left->solve();
    r = right->solve();

    if( l->type == TYPE_INT && r->type == TYPE_INT ) { /* simple expr */
        if( IS_ARITH_OP(op) ) {
            int a = ((IntegerExpr *)l)->value;
            int b = ((IntegerExpr *)r)->value;
            switch(op) {
                case EXP_ADD: return new IntegerExpr( a+b );
                case EXP_SUB: return new IntegerExpr( a-b );
                case EXP_MUL: return new IntegerExpr( a*b );
                case EXP_DIV: return new IntegerExpr( a/b );
                default:
                    assert(0);
            }
        }
    } else if( IS_REL_OP(op) ) {
        Expr *res;
        if( l->getKind() == FIELD && r->type == TYPE_INT && r->getKind() == ATOMIC ) {
            res = new BitStringExpr((FieldIdent *)l, op,
                                    ((IntegerExpr *)r)->value );
            destroy(r); return res;
        } else if( r->getKind() == FIELD && l->type == TYPE_INT && l->getKind() == ATOMIC ) {
            res = new BitStringExpr((FieldIdent *)r, op,
                                    ((IntegerExpr *)l)->value );
            destroy(l); return res;
        } else if( l->getKind() == FIELD && r->type == TYPE_STRING ) {
            int n;
            if( ((FieldIdent *)l)->findName( ((StringExpr *)r)->value, &n ) ) {
                res = new BitStringExpr( (FieldIdent *)l, op, n );
                destroy(r); return res;
            }
        } else if( r->getKind() == FIELD && l->type == TYPE_STRING ) {
            int n;
            if( ((FieldIdent *)r)->findName( ((StringExpr *)l)->value, &n ) ) {
                res = new BitStringExpr( (FieldIdent *)r, op, n );
                destroy(l); return res;
            }
        } else if( op == REL_EQ && r == l ) {
            destroy(r);
            return l;
        } else if( r->getKind() == FIELD && l->getKind() == FIELD ) {
            res = new BitStringExpr( (FieldIdent *)l, (FieldIdent *)r, op );
            destroy(l); destroy(r); return res;
        }
        
        if( l->getKind() == OPERAND && op == REL_EQ ) {
            ((OperandIdent *)l)->add( r, REL_EQ );
//            return ((OperandIdent *)l)->expr;
            return l;
        } else if( r->getKind() == OPERAND && op == REL_EQ ) {
            ((OperandIdent *)r)->add( l, REL_EQ );
//            return ((OperandIdent *)r)->expr;
            return r;
        }
    } else if( IS_LOG_OP(op) ) {
        if( l->type == TYPE_BITS && r->type == TYPE_BITS ) {
            if( l->getKind() == FIELD )
                l = ((FieldIdent *)l)->toBitStringExpr();
            if( r->getKind() == FIELD )
                r = ((FieldIdent *)r)->toBitStringExpr();
            BitStringExpr *a = ((BitStringExpr *)l);
            BitStringExpr *b = ((BitStringExpr *)r);
            switch(op) {
                case PAT_AND: a->strs = a->strs & b->strs; break;
                case PAT_SEQ: a->strs = a->strs + b->strs; break;
                case PAT_OR:  a->strs |= b->strs; break;
                default:
                    assert(0);
            }
            destroy(b);
            return a;
        }
        if( l->getKind() == NULLEXPR || l->getKind() == OPERAND ) {
            destroy(l); return r;
        }
        if( r->getKind() == NULLEXPR || r->getKind() == OPERAND ) {
            destroy(r); return l;
        }
    }
    return new BinaryExpr(type,op,l,r);
}

void UnaryExpr::printOn( FILE *f ) const
{
    fprintf( f, "( " );
    if( op != PAT_RDOT ) fprintf( f, "%s ", exprOpNames[op] );
    if( left ) left->printOn(f);
    if( op == PAT_RDOT ) fprintf( f, "%s ", exprOpNames[op] );
    else fprintf( f, "null" );
    fprintf( f, " )" );
}

Expr *UnaryExpr::expand( int *n )
{
    UnaryExpr *expr = new UnaryExpr(*this);
    expr->left = (left ? left->expand(n) : NULL);
    return expr;
}

Expr *UnaryExpr::substitute( subst_func_t *sub )
{
    if( (*sub)[this] )
        return (*sub)[this]->clone();
    UnaryExpr *expr = new UnaryExpr(*this);
    expr->left = left->substitute(sub);
    return expr;
}

Expr *UnaryExpr::solve( void )
{
    if( op == PAT_LDOT || op == PAT_RDOT ) return left->solve(); /* ignore */
    else return clone();
}

void IntegerExpr::printOn( FILE *f ) const
{
    fprintf( f, "%d", value );
}

void FloatExpr::printOn( FILE *f ) const
{
    fprintf( f, "%f", value );
}

void StringExpr::printOn( FILE *f ) const
{
    fprintf( f, "%s", value.c_str() );
}

void OffsetExpr::printOn( FILE *f ) const
{
    if( !left )
        fprintf( f, "offset(null)" );
    else if( left->posn == -1 ) {
        fprintf( f, "offset( ");
        left->printOn(f);
        fprintf( f, " )" );
    } else {
        fprintf( f, "offset(%d)", left->posn );
    }
}

void RangeExpr::printOn( FILE *f ) const
{
    fprintf( f, "%d..%d", lo, hi );
    if( cols != 1 ) fprintf( f, " cols %d", cols );
}

Expr *RangeExpr::expand( int *n )
{
    int r = hi - lo + 1;
    int v = *n % r;
    *n /= r;
    if( cols != 1 ) v = cols*(v%cols)+(v/cols);
    return new IntegerExpr( lo + v );
}

void BitSliceExpr::printOn( FILE *f ) const
{
    left->printOn( f );
    if( range.lo <= range.hi )
        fprintf( f, "@[%d:%d]", range.lo, range.hi );
}

void ListExpr::printOn( FILE *f ) const
{
    fprintf( f, "[ " );
    for( vector<Expr *>::const_iterator i=exprs.begin(); i != exprs.end(); i++ ) {
        if(*i) (*i)->printOn(f);
        fprintf(f, " ");
    }
    fprintf( f, "]" );
}
Expr *ListExpr::expand( int *n )
{
    /* Please don't ask... */
    Expr *exp = NULL;
    for( vector<Expr *>::iterator i=exprs.begin(); i != exprs.end(); i++ ) {
        if(*i) {
            if( !exp ) exp = *i;
            else exp = new BinaryExpr(TYPE_UNKNOWN,PAT_OR,exp,*i);
        }
    }
    return exp;
}
Expr *ListExpr::solve()
{
    BitStringSet str;
    ListExpr *ret = new ListExpr();
    for( vector<Expr *>::iterator i=exprs.begin(); i != exprs.end(); i++ ) {
        if( *i ) {
            Expr *exp = (*i)->solve();
            if( exp->type == TYPE_BITS ) {
                str |= ((BitStringExpr *)exp)->strs;
                delete exp;
            }
            else ret->add(exp);
        }
    }
    if( ret->exprs.size() == 0 ) {
        delete ret;
        return new BitStringExpr(str);
    }
    ret->add( new BitStringExpr(str) );
    return ret;
}

Expr *GeneratorExpr::expand( int *n )
{
    int r = exprs.size();
    int v = *n % r;
    *n /= r;
    return exprs[v];
}
Expr *ListExpr::substitute( subst_func_t *sub )
{
    if( (*sub)[this] )
        return (*sub)[this]->clone();
    ListExpr *expr = new ListExpr();
    
    for( vector<Expr *>::iterator i = exprs.begin(); i != exprs.end(); i++ ) {
        expr->add( (*i)->substitute(sub) );
    }
    return expr;
}


Expr *ListExpr::addAll( Expr *exp )
{
    if( exp->getContent()->type == TYPE_LIST )
        return addAll( (ListExpr *)exp->getContent() );
    return add( exp );
}

Expr *ListExpr::addAll( ListExpr *list )
{
    for( vector<Expr *>::iterator i = list->exprs.begin();
         i != list->exprs.end(); i++ ) {
        add( *i );
    }
    return list;
}    

void InvokeExpr::printOn( FILE *f ) const
{
    fprintf( f, "%s(", inst->name.c_str() );
    for( vector<Expr *>::const_iterator i=params.begin(); i != params.end(); i++ ) {
        if( i != params.begin() ) fprintf(f, ", ");
        if(*i) (*i)->printOn(f);
    }
    fprintf( f, ")" );
}

/*
 * This is somewhat messy, but essentially what it does is iterate over each
 * instruction that we're supposed to be instantiating (ie when the LHS
 * contains a list, rather than a single opname), and substituting it's values
 * into the associated patterns. 
 */

void SledDict::InstantiateInstructions( vector<OpcodePart> *opcodes,
                              vector<OperandIdent *> *operands, string *asmstr,
                              string *type, Expr *equation, Expr *branches )
{
    /* Need to iterate through all opcodes in the set */
    Expr::subst_func_t replace;
    Expr *expr = NULL;
    vector<int> posn;
    string s;
    int i=0;

    posn = vector<int>(opcodes->size(), 0);

    /* Trim assembly string */
    int start = asmstr->find_first_not_of(' ');
    int end = asmstr->find_last_not_of(' ');
    if( start < 0 ) start = 0;
    if( end < 0 ) end = asmstr->length()-1;
    string disasm = asmstr->substr(start, end - start+1 );
    delete asmstr;

    /* Compute buffer sizes for disassembly */
    int maxDisasmLen = disasm.length();
    for( vector<OperandIdent *>::iterator it = operands->begin();
         it != operands->end(); it++ ) {
        if( (*it)->expr == NULL ) { /* Assume 32bit number? */
            (*it)->maxDisasmLen = 11;
        } else if( (*it)->expr->getKind() == FIELD ) {
            (*it)->maxDisasmLen = ((FieldIdent *)(*it)->expr)->maxDisasmLen;
        } else if( (*it)->expr->getKind() == PATTERN &&
                   ((PatternIdent *)(*it)->expr)->expr->type == TYPE_LIST ) {
            (*it)->maxDisasmLen = ((PatternIdent *)(*it)->expr)->maxDisasmLen;
        } else assert(0);
        maxDisasmLen = maxDisasmLen - 2 /* %s */ + (*it)->maxDisasmLen;
    }
    
    while( i >= 0 ) {
        /* Generate current opcode */
        s = string();
        expr = NULL;
        replace.clear();
        for( unsigned op=0; op != opcodes->size(); op++ ) {
            if( (*opcodes)[op].expr == NULL ) {
                /* String */
                s += *(*opcodes)[op].name;
            } else if( (*opcodes)[op].list == NULL ) {
                /* Simple pattern */
                s += *(*opcodes)[op].name;
                if( !branches ) {
                    if( !expr ) expr = (*opcodes)[op].expr;
                    else expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, expr,
                                                (*opcodes)[op].expr );
                }
            } else {
                /* List */
                Expr *e = (*opcodes)[op].list->exprs[posn[op]];
                if( e->getKind() != PATTERN || e->getContent() == e ) {
                    fprintf( stderr, "Error: Non-pattern member in constructor list\n" );
                    return;
                }
                s += ((Ident *)e)->name;
                e = e->getContent()->clone();
                replace[(*opcodes)[op].expr] = e;
                if( !branches ) {
                    if( !expr ) expr = e;
                    else expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, expr, e );
                }
            }
        }
        if( branches )
            expr = branches;
        
        Expr *origExpr = expr->clone();
        dprintf( "Constructing: %s\t", s.c_str() );
        if( expr ) {
            dprintf( " => " );
            expr->dprintOn( stdout );
            dprintf( "\n  substs to => " );
            Expr *expr2 = expr->substitute(&replace);
            if( !branches ) Expr::destroy(expr);
            expr2->dprintOn( stdout );
            dprintf( "\n  solves to => " );
            expr2->computePosn(0);
            expr = expr2->solve();
            Expr::destroy(expr2);
            expr->dprintOn( stdout );
            dprintf( "\n" );
        }

        InstIdent *inst = new InstIdent( &s, expr, origExpr, operands, disasm,
                                         maxDisasmLen + s.length() + 2
                                         /* " \0"*/ );
        /* It would be nice if we could have only "real" instructions here, but
         * unfortunately there's no guaranteed way to distinguish them
         */
        insts[s] = inst;
        
#ifdef DEBUG_SLED
        for( vector<OperandIdent *>::iterator j = inst->params.begin();
             j != inst->params.end(); j++ ) {
            printf( "    %s %s =>", (*j)->isSigned?"signed":"unsigned",
                    (*j)->name.c_str() );
            if( (*j)->expr ) (*j)->expr->printOn(stdout);
            printf( "\n" );
        }
#endif
        if( type ) {
            /* This constructor has a declared "type", so we need to assign it
             * as a pattern, so that later constructors can reference it. Yes,
             * this is a pattern declaration, not a constructor declaration.
             * Argh.
             *
             * ... oh goody... this needs to have global scope, but the operand
             * decls in the same context (which occur before this) need to be
             * strictly local. Yay.
             */
            PatternIdent *pat = (PatternIdent *)symtab.lookup(*type);
            if( pat == NULL ) { /* good */
                ListExpr * list = new ListExpr();  /* There's typically many elements for these */
                list->add( inst );
                pat = new PatternIdent( type, list );
                symtab.add(-1, *type, pat );
                types[*type] = pat;
            } else {
                if( pat->getKind() != PATTERN || !pat->expr ||
                    pat->expr->type != TYPE_LIST ) {
                    /* Identifier already exists. And is incompatible.
                     * So throw a tantrum */
                    yyerror( " Attempt to redeclare identifier" );
                } else {
                    ((ListExpr *)pat->expr)->add(inst);
                }
            }
            inst->type = pat;
            if( maxDisasmLen > pat->maxDisasmLen )
                pat->maxDisasmLen = maxDisasmLen;
        }

        /* Next permutation */
        for( i=opcodes->size()-1; i >=0; i-- ) {
            if( (*opcodes)[i].list != NULL ) {
                posn[i]++;
                if( posn[i] < (int)(*opcodes)[i].list->exprs.size() )
                    break;
                else posn[i] = 0;
            }
        }
    }
    if( branches )
        Expr::destroy(branches);
}

/*
 * One constructor can include a "call" to another. We handle this by copying
 * the body of the called instruction, and appending expressions equating the
 * formals to the actuals
 */
Expr *SledDict::InvokeInstruction( string *name, ListExpr *params )
{
    assert( name );
    assert( params );
    
    if( insts.find(*name) == insts.end() ) {
        yyerror( "Unknown constructor name in invocation" );
        return NULL;
    }
    InstIdent *inst = insts[*name];
    if( params->exprs.size() != inst->params.size() ) {
        yyerror( "Parameter list mismatch in constructor invocation" );
        return NULL;
    }
    
    Expr *expr = inst->origExpr;
    Expr::subst_func_t replace;
    for( unsigned i = 0; i < inst->params.size(); i++ ) {
#if 1
        if( inst->params[i]->expr/* &&
                                    inst->params[i]->expr->getKind() == FIELD*/ ) {
            Expr *addend = new BinaryExpr(TYPE_UNKNOWN, REL_EQ,
                                          inst->params[i]->expr,
                                          params->exprs[i]);
            expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, expr, addend );
        } else {
            expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, expr,
                                   params->exprs[i] );
        }
#else
        if( inst->params[i]->expr ) {
            if( inst->params[i]->expr->getKind() == FIELD ) {
                Expr *addend = new BinaryExpr(TYPE_UNKNOWN, REL_EQ,
                                              inst->params[i]->expr,
                                              params->exprs[i]);
                expr = new BinaryExpr( TYPE_UNKNOWN, PAT_AND, expr, addend );
            } else {
                replace[inst->params[i]->expr] = params->exprs[i];
            }
        }
#endif
    }
    if( replace.size() != 0 ) {
        return expr->substitute(&replace);
    } else {
        return expr->clone();
    }
}
 
extern FILE *yyin;
extern int yyparse(void *);

bool SledDict::readSLEDFile( const string &fname )
{
    yyin = fopen( fname.c_str(), "ro" );
    yylineno = 1;
    yyfilename = fname.c_str();
    return (yyparse(this) == 0);
}
void SledDict::printOn( FILE *out ) const
{
    fprintf( out, "Fields:\n" );
    for( field_iterator i = fields.begin(); i != fields.end(); i++ ) {
        i->second->printOn( out );
        fprintf( out, "\n" );
    }
    fprintf( out, "Types:\n" );
    for( type_iterator i = types.begin(); i != types.end(); i++ ) {
        i->second->printOn( out );
        fprintf( out, "\n" );
    }
    fprintf( out, "Instructions:\n" );
    for( inst_iterator i = insts.begin(); i != insts.end(); i++ ) {
        i->second->printOn( out );
        fprintf( out, "\n" );
    }
}

/*
 * Determine if the receiver is a specialization of the argument, ie
 * any instruction which matches the receiver will also match the argument.
 * Specifically does _NOT_ return true if only _some_ of the receiver's
 * possible bit patterns would match (although such could be considered a
 * bug in the input specification).
 *
 * Pre: Both instructions have been fully solved (ie expr is a BitStringExpr),
 * or the receiver's expression is exactly an invocation of the argument.
 */
bool InstIdent::specializes( const InstIdent &inst ) const
{
    if( expr->getKind() == INVOKER ) {
        return ( ((InvokeExpr *)expr)->inst->name == inst.name );
    }
            
    if( !expr || expr->type != TYPE_BITS ) {
        dprintf( "Warning: InstIdent::specializes() invoked on non-final instruction:\n" );
        dprintOn( stderr );
        return false;
    }
    if( !inst.expr || inst.expr->type != TYPE_BITS ) {
        dprintf( "Warning: InstIdent::specializes() called with non-final instruction:\n" );
        inst.dprintOn( stderr );
        return false; /* PRECONDITION */
    }
    BitStringSet &a = ((BitStringExpr *)expr)->strs;
    BitStringSet &b = ((BitStringExpr *)inst.expr)->strs;
    if( b.matches(a) ) {
#ifdef DEBUG_SLED
        fprintf( stderr, "Debug[sled]: %s specializes %s\n", name.c_str(),
                 inst.name.c_str() );
        printOn(stderr);
        inst.printOn(stderr);
#endif
        return true;
    }
    return false;
}

/*
 * Determine if an instruction is synthesized. Actually we cheat here
 * and just say that it's a synth if the toplevel expr is an Invoke -
 * which will do for now.
 */
bool InstIdent::isSynth( ) const
{
    return expr && expr->getKind() == INVOKER;
}

/*
 * Return a list of all the instructions in the dictionary in njmc
 * safe order (ie specializations of instructions come before the
 * more general forms). This is a partial ordering, and is currently
 * O(n^2) (if anyone knows a faster way to do this, let me know)
 */
list<InstIdent *> SledDict::orderInstructions( )
{
    list<InstIdent *> res;

    for(inst_iterator it = insts.begin(); it != insts.end(); it++ ) {
        list<InstIdent *>::iterator rit;
        for( rit = res.begin(); rit != res.end(); rit++ ) {
            if( it->second->specializes(**rit) )
                break; /* Insert here */
        }
        res.insert(rit, it->second);
    }
    return res;
}
