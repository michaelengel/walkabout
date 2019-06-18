/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

#undef sun // Otherwise this may be defined when we run the preprocessor...
#ifdef UNSAFE
import sun.misc.Unsafe;
#endif
import java.math.BigInteger;
import java.io.Writer;
import java.io.StringWriter;
import java.io.PrintWriter;

#define unsigned int
#define dword int
#define addressToPC(x) (x)
#define getDword(pc) getMemint(pc)

#define BITSLICE(x,lo,hi) (((x) & ((1L<<(hi+1))-1))>>lo)
#define BITSLICEASSN(x,lo,hi,exp) (x=((x)&(~((1L<<((hi)+1))-1)^((1L<<(lo))-1)))|\

/* FIXME */
#define float128 double[]

/* Default values, should never really be used */
#define defaultbyte 0
#define defaultshort 0
#define defaultint 0
#define defaultlong 0
#define defaultfloat 0
#define defaultdouble 0

public class ${BASENAME} 
#ifdef JAVAELFPARSER 
implements genericDis{
#else
{
#endif
    /* public variables */  
#ifdef JAVAELFPARSER
    private byte binInMem[];
#endif  
    public long ${PREFIX}mem;
    public long ${PREFIX}icount = 0;
    public boolean ${PREFIX}debug = false;
#ifdef UNSAFE
    protected static Unsafe unsafe = Unsafe.getUnsafe();
#endif
#ifdef EMUDEBUG
    private String returnStringValue;
#endif

@EMULATOR
    /* internal variables */
#ifdef UNSAFE
    public float _tmpf;  // have to be public or getField won't work... *sigh*
    public double _tmpd;
    protected int _tmpfOffset;
    protected int _tmpdOffset;
#endif
@EMULATOR
    // public constructor
    public ${BASENAME}(int base 
#ifdef JAVAELFPARSER
    , byte[] iBinInMem    
#endif
    ) {
@EMULATOR
#ifdef UNSAFE
        try {
            _tmpfOffset = unsafe.fieldOffset(getClass().getField("_tmpf"));
            _tmpdOffset = unsafe.fieldOffset(getClass().getField("_tmpd"));
        } catch( Exception e ) { 
            System.out.println("JVM broken: " + e.toString() ); 
            throw new RuntimeException(); 
        }
#endif
        regs = new RegisterFile();
@EMULATOR
#ifdef JAVAELFPARSER
        binInMem = iBinInMem;
#endif
        ${PREFIX}mem = (long)base;
        ${PREFIX}mem &= 0xffffffffL;
    }

#ifndef UNSAFE
    // Various native type conversions that preserve the bit-patterns
    native float intToFloat( int v);
    native double longToDouble( long v);
    native int floatToInt( float v );
    native long doubleToLong ( double v);
#endif

@EMULATOR
#ifdef UNSAFE
    // Various type conversions that preserve the bit-patterns
    float intToFloat( int v ) {
        unsafe.putInt(this, _tmpfOffset, v);
        return _tmpf;
    }
    double longToDouble( long v ) {
        unsafe.putDouble(this, _tmpdOffset, v);
        return _tmpd;
    }
    int floatToInt( float v ) {
        _tmpf = v;
        return unsafe.getInt(this, _tmpfOffset);
    }
    long doubleToLong( double v ) {
        _tmpd = v;
        return unsafe.getLong(this, _tmpdOffset);
    }
#endif
    float doubleToFloat( double v ) {
        return intToFloat(((int)doubleToLong(v)));
    }
    double floatToDouble( float v ) {
        return longToDouble(((long)floatToInt(v)));
    }

    double sqrt(double x) { return Math.sqrt(x); }
    float sqrt(float x) { return (float)Math.sqrt( (double)x ); }
@EMULATOR

#ifdef UNSAFE
    // Memory access functions
    void setMembyte  ( long a, byte x )  { unsafe.putByte(${PREFIX}mem+a,x); }
    void setMembyte  ( long a, int x )   { unsafe.putByte(${PREFIX}mem+a,(byte)x); }
    void setMemshort ( long a, short x ) { unsafe.putShort(${PREFIX}mem+a,x); }
    void setMemshort ( long a, int x )   { unsafe.putShort(${PREFIX}mem+a,(short)x); }
    void setMemint   ( long a, int x )   { unsafe.putInt(${PREFIX}mem+a,x); }
    void setMemint   ( long a, float x ) { unsafe.putFloat(${PREFIX}mem+a,x); }
    void setMemlong  ( long a, long x )  { unsafe.putLong(${PREFIX}mem+a,x); }
    void setMemlong  ( long a, double x ){ unsafe.putDouble(${PREFIX}mem+a,x); }
    void setMemfloat ( long a, float x ) { unsafe.putFloat(${PREFIX}mem+a,x); }
    void setMemdouble( long a, double x ){ unsafe.putDouble(${PREFIX}mem+a,x); }

    byte   getMembyte  ( long a ) { return unsafe.getByte(${PREFIX}mem+a); }
    short  getMemshort ( long a ) { return unsafe.getShort(${PREFIX}mem+a); }
    int    getMemint   ( long a ) { return unsafe.getInt(${PREFIX}mem+a); }
    long   getMemlong  ( long a ) { return unsafe.getLong(${PREFIX}mem+a); }
    float  getMemfloat ( long a ) { return unsafe.getFloat(${PREFIX}mem+a); }
    double getMemdouble( long a ) { return unsafe.getDouble(${PREFIX}mem+a); }
#elif defined JAVAELFPARSER
    byte getMembyte( long a ) { return binInMem[(int)a - (int)mem]; }

    int getMemint( long a ) {

        char tempByte;
        byte arrayByte[] = new byte[4];

        for (int i = 0; i < 4; i++) {
            arrayByte[i] = binInMem[(int)a - (int)mem + i];
        }
        BigInteger test = new BigInteger(arrayByte);
        return test.intValue();
    }

    short getMemshort( long a ) {

        char tempByte;
        byte arrayByte[] = new byte[2];

        for (int i = 0; i < 2; i++) {
            arrayByte[i] = binInMem[(int)a - (int)mem + i];
        }
        BigInteger test = new BigInteger(arrayByte);
        return (short)test.intValue();
    }
#else
    // Native methods
    native void setMembyte  ( long a, byte x );
    native void setMembyte  ( long a, int x );
    native void setMemshort ( long a, short x );
    native void setMemshort ( long a, int x );
    native void setMemint   ( long a, int x );
    native void setMemint   ( long a, float x );
    native void setMemlong  ( long a, long x );
    native void setMemlong  ( long a, double x );
    native void setMemfloat ( long a, float x );
    native void setMemdouble( long a, double x );

    native byte   getMembyte  ( long a );
    native short  getMemshort ( long a );
    native int    getMemint   ( long a );
    native long   getMemlong  ( long a );
    native float  getMemfloat ( long a );
    native double getMemdouble( long a );
#endif

    byte getByte(int a) {
        return getMembyte((long) a);
    }

    int getWord(int a) {
        return getMemint((long) a);
    }

    // assert (used by njmc)
    void assert( boolean cond ) {
        if( cond ) {
	    throw new RuntimeException( "Assertion failed" );
	}
    }

@DISASM
/**************** Disassembler core ******************/
    protected final static int HEXCUTOFF = 1024;
    protected String opname;

    protected String IMMEDTOSTRING( int val ) {
        if( val > HEXCUTOFF || val < -HEXCUTOFF ) {
            return "0x" + Integer.toHexString(val);
        } else {
            return Integer.toString(val);
        }
    }
    protected String formatHex( int val, int digits ) {
        StringBuffer buf = new StringBuffer(Integer.toHexString(val));
        while( buf.length() < digits ) buf.insert(0,'0');
        return buf.toString();
    }
    protected String rightPad( String val, int length ) {
        StringBuffer buf = new StringBuffer(val);
        while( buf.length() < length )
            buf.append(' ');
        return buf.toString();
    }
    protected String leftPad( String val, int length ) {
        StringBuffer buf = new StringBuffer(val);
        while( buf.length() < length )
            buf.insert(0,' ');
        return buf.toString();
    }
    protected String zeroPad( String val, int length ) {
        StringBuffer buf = new StringBuffer(val);
        while( buf.length() < length )
            buf.insert(0,'0');
        return buf.toString();
    }
    protected String callName( int address) {
#ifdef JAVAELFPARSER        
        String funcName = (String) DisasmElf.symbolAddressMap.get(new Integer(address));
        if (funcName == null)
            return ( IMMEDTOSTRING( address));
        return funcName;
#else
        return ( IMMEDTOSTRING( address));
#endif	
    }
    
#define BADOPCODE(x) opname = "#ERR#";

    ${DISASM_PARAMS}

    int ${PREFIX}disassembleInstruction( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction1( int pc, Writer out ) throws java.io.IOException {
	${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction2( int pc, Writer out ) throws java.io.IOException {
	${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction3( int pc, Writer out ) throws java.io.IOException {
	${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction4( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction5( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction6( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction7( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction8( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}disassembleInstruction9( int pc, Writer out ) throws java.io.IOException {
        ${DISASM_FUNC}
    }

    int ${PREFIX}sprintDisassembleInst( int pc, Writer out ) throws java.io.IOException {
        StringBuffer s = new StringBuffer();
        StringWriter sw = new StringWriter();

	out.flush();        
        int npc = ${PREFIX}disassembleInstruction( pc, sw );
        
        s.append( formatHex( pc, 8 ) );
        s.append( ": " );
        for( int i = pc; i < npc; i++ ) {
            s.append( formatHex( ((int)getMembyte(i))&0xFF, 2 ) );
            s.append( " " );
        }
        for( int i = npc-pc; i<6; i++ )
            s.append("   ");
        s.append( (rightPad(opname,10)).toLowerCase() );
        s.append( sw.toString() );
        s.append( '\n' );
        out.write(s.toString());
        return npc;
    }
#ifndef EMUDEBUG
    void ${PREFIX}disassembleRegion( int pc, int to, Writer out ) throws java.io.IOException {
        while( pc < to ) {
            pc = ${PREFIX}sprintDisassembleInst( pc, out );
        }
    }

    public void ${PREFIX}disassembleRegion( int pc, int to ) throws java.io.IOException {
        ${PREFIX}disassembleRegion( pc, to, new PrintWriter(System.out) );
    }
#else
    void ${PREFIX}disassembleRegion( int pc, int to, Writer out ) throws java.io.IOException {
        while( pc < to ) {
            pc = ${PREFIX}sprintDisassembleInst( pc, out );
        }
        returnStringValue = out.toString();
    }

    public void ${PREFIX}disassembleRegion( int pc, int to ) throws java.io.IOException {
        ${PREFIX}disassembleRegion( pc, to, new StringWriter() );
    }

    String returnDisasm() {
        return returnStringValue.toLowerCase();
    }
#endif
@DISASM
    
@EMULATOR
    void unimplemented(int pc) {
        throw new RuntimeException("Unimplemented Instruction");
    }

    public native int doTrap( int trapno );

    void doFlagTRAP( boolean cond, int trapno ) {
        if( cond ) {
            doTrap( trapno );
        }
    }

/************** 128 bit float support *************/
    double [] float128Add( double []a, double []b ) {
        return a; /* Implement me! :) */
    }
    double [] float128Sub( double []a, double []b ) {
        return a; /* Implement me! :) */
    }
    double [] float128Mul( double []a, double []b ) {
        return a; /* Implement me! :) */
    }
    double [] float128Div( double []a, double []b ) {
        return a; /* Implement me! :) */
    }
    double [] sqrt( double []a ) {
	return a; /* Implement me! :) */
    } 

    boolean float128Equals( double []a, double []b ) {
        return a[0] == b[0] && a[1] == b[1];
    }
    boolean float128Less( double []a, double []b ) {
	return false; /* Implement me! :) */
    }
    double castfloat128Todouble( double []a ) {
        return a[0]; /* Implement me! :) */
    }
    float castfloat128Tofloat( double []a ) {
        return doubleToFloat(a[0]); /* Implement me! :) */
    }
    double [] castdoubleTofloat128( double a ) {
	double []r = { a, 0 }; /* Implement me! :) */
	return r;
    }
    double [] castfloatTofloat128( float a ) {
	double []r = { floatToDouble(a), 0 }; /* Implement me! :) */
	return r;
    }
    int castfloat128Toint( double [] a ) {
        return (int)(castfloat128Tofloat(a));
    }
    double [] castintTofloat128( int a ) {
        return castfloatTofloat128( (float)a );
    }

    final static double [] defaultfloat128 = { 0,0 };

/************** Register definitions **************/
    ${REGISTER_STRUCT_TYPE}
    ${REGISTER_STRUCT_DEFN}
    ${REGISTER_ACCESSORS}

/**************** Register dumpers ****************/
    ${REGISTER_DUMP_FUNCS}
    
/*********** Parameter decoding support ***********/
    ${PARAM_FUNCS}

/**************** Flag call macros ****************/
    ${FLAG_CALL_FUNCS}

/******************* Operations *******************/
    ${OPERATION_FUNCS}

    ${EXECUTE_FUNC}

/**************** Global Functions ****************/
    public void ${PREFIX}executeOneInstruction() {
        ${EXECUTE_ONE_FUNC}
    }

    public void ${PREFIX}run( ) {
@DISASM
	PrintWriter err = new PrintWriter(System.err);
@DISASM

        while( true ) {
#ifndef NDEBUG
            ${PREFIX}icount++;
@DISASM
            if( ${PREFIX}debug ) {
		err.print( 
                    rightPad( Long.toString( ${PREFIX}icount ), 5 ));
		err.print( "|" );
		try {
                    ${PREFIX}sprintDisassembleInst( ${PC}, err );
                } catch( java.io.IOException e ) {}
                dumpMainRegisters( err );
            }
@DISASM
#endif
            executeOneInstruction();
        }
    }

@EMULATOR
    
    ${USER_CODE_BOTTOM}
}
