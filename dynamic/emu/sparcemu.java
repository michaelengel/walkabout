


 






 










import sun.misc.Unsafe;

import java.math.BigInteger;
import java.io.Writer;
import java.io.StringWriter;
import java.io.PrintWriter;









 


 







public class sparcemu 



{

       



    public long mem;
    public long icount = 0;
    public boolean debug = false;

    protected static Unsafe unsafe = Unsafe.getUnsafe();





     

    public float _tmpf;   
    public double _tmpd;
    protected int _tmpfOffset;
    protected int _tmpdOffset;

     
    public sparcemu(int base 



    ) {

        try {
            _tmpfOffset = unsafe.fieldOffset(getClass().getField("_tmpf"));
            _tmpdOffset = unsafe.fieldOffset(getClass().getField("_tmpd"));
        } catch( Exception e ) { 
            System.out.println("JVM broken: " + e.toString() ); 
            throw new RuntimeException(); 
        }

        regs = new RegisterFile();



        mem = (long)base;
        mem &= 0xffffffffL;
    }










     
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

    float doubleToFloat( double v ) {
        return intToFloat(((int)doubleToLong(v)));
    }
    double floatToDouble( float v ) {
        return longToDouble(((long)floatToInt(v)));
    }

    double sqrt(double x) { return Math.sqrt(x); }
    float sqrt(float x) { return (float)Math.sqrt( (double)x ); }


     
    void setMembyte  ( long a, byte x )  { unsafe.putByte(mem+a,x); }
    void setMembyte  ( long a, int x )   { unsafe.putByte(mem+a,(byte)x); }
    void setMemshort ( long a, short x ) { unsafe.putShort(mem+a,x); }
    void setMemshort ( long a, int x )   { unsafe.putShort(mem+a,(short)x); }
    void setMemint   ( long a, int x )   { unsafe.putInt(mem+a,x); }
    void setMemint   ( long a, float x ) { unsafe.putFloat(mem+a,x); }
    void setMemlong  ( long a, long x )  { unsafe.putLong(mem+a,x); }
    void setMemlong  ( long a, double x ){ unsafe.putDouble(mem+a,x); }
    void setMemfloat ( long a, float x ) { unsafe.putFloat(mem+a,x); }
    void setMemdouble( long a, double x ){ unsafe.putDouble(mem+a,x); }

    byte   getMembyte  ( long a ) { return unsafe.getByte(mem+a); }
    short  getMemshort ( long a ) { return unsafe.getShort(mem+a); }
    int    getMemint   ( long a ) { return unsafe.getInt(mem+a); }
    long   getMemlong  ( long a ) { return unsafe.getLong(mem+a); }
    float  getMemfloat ( long a ) { return unsafe.getFloat(mem+a); }
    double getMemdouble( long a ) { return unsafe.getDouble(mem+a); }


    byte getByte(int a) {
        return getMembyte((long) a);
    }

    int getWord(int a) {
        return getMemint((long) a);
    }

     
    void assertion( boolean cond ) {
        if( cond ) {
	    throw new RuntimeException( "Assertion failed" );
	}
    }

 
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






        return ( IMMEDTOSTRING( address));

    }
    


    final static String aNames[] = { "", ",a" };
final static String cdNames[] = { "%c0", "%c1", "%c2", "%c3", "%c4", "%c5", "%c6", "%c7", "%c8", "%c9", "%c10", "%c11", "%c12", "%c13", "%c14", "%c15", "%c16", "%c17", "%c18", "%c19", "%c20", "%c21", "%c22", "%c23", "%c24", "%c25", "%c26", "%c27", "%c28", "%c29", "%c30", "%c31" };
final static String fddNames[] = { "%f0to1", "a", "%f2to3", "b", "%f4to5", "c", "%f6to7", "d", "%f8to9", "e", "%f10to11", "f", "%f12to13", "g", "%f14to15", "h", "%f16to17", "i", "%f18to19", "j", "%f20to21", "k", "%f22to23", "l", "%f24to25", "m", "%f26to27", "n", "%f28to29", "o", "%f30to31", "p" };
final static String fdqNames[] = { "%f0to3", "q", "r", "s", "%f4to7", "t", "u", "v", "%f8to11", "w", "x", "y", "%f12to15", "z", "A", "B", "%f16to19", "C", "D", "E", "%f20to23", "F", "G", "H", "%f24to27", "I", "J", "K", "%f28to31", "L", "M", "N" };
final static String fdsNames[] = { "%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6", "%f7", "%f8", "%f9", "%f10", "%f11", "%f12", "%f13", "%f14", "%f15", "%f16", "%f17", "%f18", "%f19", "%f20", "%f21", "%f22", "%f23", "%f24", "%f25", "%f26", "%f27", "%f28", "%f29", "%f30", "%f31" };
final static String fs1dNames[] = { "%f0to1", "a", "%f2to3", "b", "%f4to5", "c", "%f6to7", "d", "%f8to9", "e", "%f10to11", "f", "%f12to13", "g", "%f14to15", "h", "%f16to17", "i", "%f18to19", "j", "%f20to21", "k", "%f22to23", "l", "%f24to25", "m", "%f26to27", "n", "%f28to29", "o", "%f30to31", "p" };
final static String fs1qNames[] = { "%f0to3", "q", "r", "s", "%f4to7", "t", "u", "v", "%f8to11", "w", "x", "y", "%f12to15", "z", "A", "B", "%f16to19", "C", "D", "E", "%f20to23", "F", "G", "H", "%f24to27", "I", "J", "K", "%f28to31", "L", "M", "N" };
final static String fs1sNames[] = { "%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6", "%f7", "%f8", "%f9", "%f10", "%f11", "%f12", "%f13", "%f14", "%f15", "%f16", "%f17", "%f18", "%f19", "%f20", "%f21", "%f22", "%f23", "%f24", "%f25", "%f26", "%f27", "%f28", "%f29", "%f30", "%f31" };
final static String fs2dNames[] = { "%f0to1", "a", "%f2to3", "b", "%f4to5", "c", "%f6to7", "d", "%f8to9", "e", "%f10to11", "f", "%f12to13", "g", "%f14to15", "h", "%f16to17", "i", "%f18to19", "j", "%f20to21", "k", "%f22to23", "l", "%f24to25", "m", "%f26to27", "n", "%f28to29", "o", "%f30to31", "p" };
final static String fs2qNames[] = { "%f0to3", "q", "r", "s", "%f4to7", "t", "u", "v", "%f8to11", "w", "x", "y", "%f12to15", "z", "A", "B", "%f16to19", "C", "D", "E", "%f20to23", "F", "G", "H", "%f24to27", "I", "J", "K", "%f28to31", "L", "M", "N" };
final static String fs2sNames[] = { "%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6", "%f7", "%f8", "%f9", "%f10", "%f11", "%f12", "%f13", "%f14", "%f15", "%f16", "%f17", "%f18", "%f19", "%f20", "%f21", "%f22", "%f23", "%f24", "%f25", "%f26", "%f27", "%f28", "%f29", "%f30", "%f31" };
final static String rdNames[] = { "%g0", "%g1", "%g2", "%g3", "%g4", "%g5", "%g6", "%g7", "%o0", "%o1", "%o2", "%o3", "%o4", "%o5", "%sp", "%o7", "%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7", "%i0", "%i1", "%i2", "%i3", "%i4", "%i5", "%fp", "%i7" };
final static String rs1Names[] = { "%g0", "%g1", "%g2", "%g3", "%g4", "%g5", "%g6", "%g7", "%o0", "%o1", "%o2", "%o3", "%o4", "%o5", "%sp", "%o7", "%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7", "%i0", "%i1", "%i2", "%i3", "%i4", "%i5", "%fp", "%i7" };
final static String rs2Names[] = { "%g0", "%g1", "%g2", "%g3", "%g4", "%g5", "%g6", "%g7", "%o0", "%o1", "%o2", "%o3", "%o4", "%o5", "%sp", "%o7", "%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7", "%i0", "%i1", "%i2", "%i3", "%i4", "%i5", "%fp", "%i7" };
public int disassembleaddress_( int pc, Writer out ) throws java.io.IOException
{
int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    if ((MATCH_w_32_0 >> 13 & 0x1)   == 1) { 
      int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
      int   simm13 = 
        (((int)(( (MATCH_w_32_0 & 0x1fff)   ) << (4*8-(  13 )))) >> (4*8-(  13 ))) ;
      nextPC = 4 + MATCH_p; 
      
      

       { out.write(rs1Names[rs1] );

      out.write( " + " );

      out.write( IMMEDTOSTRING( simm13));

       opname = "dispA"; }

      
      
      
    }   
    else { 
      int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
      int  rs2 = (MATCH_w_32_0 & 0x1f)  ;
      nextPC = 4 + MATCH_p; 
      
      

       { out.write(rs1Names[rs1] );

      out.write( " + " );

      out.write(rs2Names[rs2] );

       opname = "indexA"; }

      
      
      
    }   
    
  } 
  
    
  
}


return nextPC;
}
public int disassemblereg_or_imm( int pc, Writer out ) throws java.io.IOException
{
int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    if ((MATCH_w_32_0 >> 13 & 0x1)   == 1) { 
      int   simm13 = 
        (((int)(( (MATCH_w_32_0 & 0x1fff)   ) << (4*8-(  13 )))) >> (4*8-(  13 ))) ;
      nextPC = 4 + MATCH_p; 
      
      

       { out.write( IMMEDTOSTRING( simm13));

       opname = "imode"; }

      
      
      
    }   
    else { 
      int  rs2 = (MATCH_w_32_0 & 0x1f)  ;
      nextPC = 4 + MATCH_p; 
      
      

       { out.write(rs2Names[rs2] );

       opname = "rmode"; }

      
      
      
    }   
    
  } 
  
    
  
}


return nextPC;
}
public int disassembleregaddr( int pc, Writer out ) throws java.io.IOException
{
int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    if ((MATCH_w_32_0 >> 13 & 0x1)   == 1) { 
      nextPC = MATCH_p; 
      
      

      { opname = "#ERR#"; ; };

      
      
      
    }   
    else { 
      int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
      int  rs2 = (MATCH_w_32_0 & 0x1f)  ;
      nextPC = 4 + MATCH_p; 
      
      

       { out.write(rs1Names[rs1] );

      out.write( " + " );

      out.write(rs2Names[rs2] );

       opname = "indexR"; }

      
      
      
    }   
    
  } 
  
    
  
}


return nextPC;
}


    int disassembleInstruction( int pc, Writer out ) throws java.io.IOException {
        int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    
      switch((MATCH_w_32_0 >> 30 & 0x3)  ) {
        case 0: 
          
            switch((MATCH_w_32_0 >> 22 & 0x7)  ) {
              case 0: case 1: case 3: case 4: case 5: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };} break;
              case 2: 
                if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BN,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BE,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BLE,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BL,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BLEU,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BCS,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BNEG,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BVS,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BA,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BNE,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BG,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BGE,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BGU,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BCC,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BPOS,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BVC,a"; }

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                else 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BN"; }

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BE"; }

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BLE"; }

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BL"; }

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BLEU"; }

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BCS"; }

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BNEG"; }

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BVS"; }

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BA"; }

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BNE"; }

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BG"; }

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BGE"; }

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BGU"; }

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BCC"; }

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BPOS"; }

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "BVC"; }

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                break;
              case 6: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                    case 0: case 1: case 2: case 3: case 4: case 5: case 6: 
                    case 7: case 10: case 11: case 12: case 13: case 14: 
                    case 15: 
                      { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };} break;
                    case 8: 
                      if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                        int  reloc = 
                          4 * (((int)(( 
                                      (MATCH_w_32_0 & 0x3fffff) 
                                              ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                          ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write( IMMEDTOSTRING( reloc));

                         opname = "FBA,a"; }

                        
                        
                        
                      }   
                      else { 
                        int  reloc = 
                          4 * (((int)(( 
                                      (MATCH_w_32_0 & 0x3fffff) 
                                              ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                          ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write( IMMEDTOSTRING( reloc));

                         opname = "FBA"; }

                        
                        
                        
                      }   
                      
                      break;
                    case 9: 
                      if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                        int  reloc = 
                          4 * (((int)(( 
                                      (MATCH_w_32_0 & 0x3fffff) 
                                              ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                          ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write( IMMEDTOSTRING( reloc));

                         opname = "FBE,a"; }

                        
                        
                        
                      }   
                      else { 
                        int  reloc = 
                          4 * (((int)(( 
                                      (MATCH_w_32_0 & 0x3fffff) 
                                              ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                          ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write( IMMEDTOSTRING( reloc));

                         opname = "FBE"; }

                        
                        
                        
                      }   
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 7: 
                if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CBN,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB123,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB12,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB13,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB1,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB23,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB2,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB3,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CBA,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB0,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB03,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB02,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB023,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB01,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB013,a"; }

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB012,a"; }

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                else 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CBN"; }

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB123"; }

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB12"; }

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB13"; }

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB1"; }

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB23"; }

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB2"; }

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB3"; }

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CBA"; }

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB0"; }

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB03"; }

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB02"; }

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB023"; }

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB01"; }

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB013"; }

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           { out.write( IMMEDTOSTRING( reloc));

                           opname = "CB012"; }

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                break;
              default: assertion(false);
            }   
          break;
        case 1: case 3: 
          { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };} break;
        case 2: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ADD"; }

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "AND"; }

                  
                  
                  
                }
                
                break;
              case 2: case 3: case 4: case 6: case 7: case 9: case 10: 
              case 11: case 12: case 13: case 14: case 15: case 18: case 19: 
              case 20: case 22: case 23: case 25: case 26: case 27: case 28: 
              case 29: case 30: case 31: case 32: case 33: case 34: case 35: 
              case 36: case 37: case 38: case 39: case 40: case 41: case 42: 
              case 43: case 44: case 45: case 46: case 47: case 49: case 50: 
              case 51: case 53: case 54: case 55: case 56: case 57: case 58: 
              case 59: case 60: case 61: case 62: case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };} break;
              case 5: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ANDN"; }

                  
                  
                  
                }
                
                break;
              case 8: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ADDX"; }

                  
                  
                  
                }
                
                break;
              case 16: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ADDcc"; }

                  
                  
                  
                }
                
                break;
              case 17: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ANDcc"; }

                  
                  
                  
                }
                
                break;
              case 21: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ANDNcc"; }

                  
                  
                  
                }
                
                break;
              case 24: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ADDXcc"; }

                  
                  
                  
                }
                
                break;
              case 48: 
                if ((MATCH_w_32_0 >> 25 & 0x1f)   == 0 && 
                  (0 <= (MATCH_w_32_0 >> 13 & 0x1)   && 
                  (MATCH_w_32_0 >> 13 & 0x1)   < 2)) { 
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", %y" );

                   opname = "WRY"; }

                  
                  
                  
                }   
                else { 
                  int  rdi = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", %asr" );

                  out.write( IMMEDTOSTRING( rdi));

                   opname = "WRASR"; }

                  
                  
                  
                }   
                
                break;
              case 52: 
                if (10 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 65 || 
                  68 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 512) 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };}   
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff)  ) {
                    case 0: case 1: case 2: case 3: case 4: case 5: case 6: 
                    case 7: case 8: 
                      { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction1(pc, out); };} break;
                    case 9: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FABSs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 65: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FADDs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 66: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FADDd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 67: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FADDq"; }

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              default: assertion(false);
            }   
          break;
        default: assertion(false);
      }   
    
  } 
  
   
     
    
    
  
}


return nextPC;
}

    int disassembleInstruction1( int pc, Writer out ) throws java.io.IOException {
	int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    
      switch((MATCH_w_32_0 >> 30 & 0x3)  ) {
        case 0: 
          if ((MATCH_w_32_0 >> 22 & 0x7)   == 6) 
            
              switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                case 0: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBN,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBN"; }

                    
                    
                    
                  }   
                  
                  break;
                case 1: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBNE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBNE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 2: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBLG,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBLG"; }

                    
                    
                    
                  }   
                  
                  break;
                case 3: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUL,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUL"; }

                    
                    
                    
                  }   
                  
                  break;
                case 4: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBL,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBL"; }

                    
                    
                    
                  }   
                  
                  break;
                case 5: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUG,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUG"; }

                    
                    
                    
                  }   
                  
                  break;
                case 6: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBG,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBG"; }

                    
                    
                    
                  }   
                  
                  break;
                case 7: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBU,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBU"; }

                    
                    
                    
                  }   
                  
                  break;
                case 8: case 9: 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
                case 10: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 11: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBGE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBGE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 12: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUGE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBUGE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 13: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBLE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBLE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 14: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBULE,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBULE"; }

                    
                    
                    
                  }   
                  
                  break;
                case 15: 
                  if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBO,a"; }

                    
                    
                    
                  }   
                  else { 
                    int  reloc = 
                      4 * (((int)(( 
                                  (MATCH_w_32_0 & 0x3fffff) 
                                          ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                      ( MATCH_p ) ;
                    nextPC = 4 + MATCH_p; 
                    
                    

                     { out.write( IMMEDTOSTRING( reloc));

                     opname = "FBO"; }

                    
                    
                    
                  }   
                  
                  break;
                default: assertion(false);
              }    
          else 
            { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
          break;
        case 1: 
          { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
        case 2: 
          if (0 <= (MATCH_w_32_0 >> 19 & 0x3f)   && 
            (MATCH_w_32_0 >> 19 & 0x3f)   < 52) 
            { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
          else 
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 54: case 55: case 57: case 58: case 60: case 61: case 62: 
              case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
              case 52: 
                if (6 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 41 || 
                  111 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 196 || 
                  212 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 512) 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff)  ) {
                    case 0: case 2: case 3: case 4: case 44: case 45: 
                    case 46: case 47: case 48: case 49: case 50: case 51: 
                    case 52: case 53: case 54: case 55: case 56: case 57: 
                    case 58: case 59: case 60: case 61: case 62: case 63: 
                    case 64: case 65: case 66: case 67: case 68: case 72: 
                    case 76: case 80: case 81: case 82: case 83: case 84: 
                    case 85: case 86: case 87: case 88: case 89: case 90: 
                    case 91: case 92: case 93: case 94: case 95: case 96: 
                    case 97: case 98: case 99: case 100: case 101: case 102: 
                    case 103: case 104: case 106: case 107: case 108: 
                    case 109: case 197: case 202: case 207: case 208: 
                      { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
                    case 1: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FMOVs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FNEGs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 41: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FSQRTs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 42: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FSQRTd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 43: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FSQRTq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 69: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FSUBs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 70: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FSUBd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 71: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FSUBq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 73: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FMULs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 74: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FMULd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 75: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FMULq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 77: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FDIVs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 78: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FDIVd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 79: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FDIVq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 105: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FsMULd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 110: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FdMULq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 196: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FiTOs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 198: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FdTOs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 199: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FqTOs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 200: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FiTOd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 201: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FsTOd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 203: 
                      { 
                        int  fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fddNames[fdd] );

                         opname = "FqTOd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 204: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FiTOq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 205: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FsTOq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 206: 
                      { 
                        int  fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fdqNames[fdq] );

                         opname = "FdTOq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 209: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2sNames[fs2s] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FsTOi"; }

                        
                        
                        
                      }
                      
                      break;
                    case 210: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2dNames[fs2d] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FdTOi"; }

                        
                        
                        
                      }
                      
                      break;
                    case 211: 
                      { 
                        int  fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs2qNames[fs2q] );

                        out.write( ", " );

                        out.write(fdsNames[fds] );

                         opname = "FqTOi"; }

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 53: 
                if (0 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 81 || 
                  88 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 512) 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff)  ) {
                    case 84: 
                      { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
                    case 81: 
                      { 
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                         opname = "FCMPs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 82: 
                      { 
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                         opname = "FCMPd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 83: 
                      { 
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                         opname = "FCMPq"; }

                        
                        
                        
                      }
                      
                      break;
                    case 85: 
                      { 
                        int  fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2s = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1sNames[fs1s] );

                        out.write( ", " );

                        out.write(fs2sNames[fs2s] );

                         opname = "FCMPEs"; }

                        
                        
                        
                      }
                      
                      break;
                    case 86: 
                      { 
                        int  fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2d = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1dNames[fs1d] );

                        out.write( ", " );

                        out.write(fs2dNames[fs2d] );

                         opname = "FCMPEd"; }

                        
                        
                        
                      }
                      
                      break;
                    case 87: 
                      { 
                        int  fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  fs2q = (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { out.write(fs1qNames[fs1q] );

                        out.write( ", " );

                        out.write(fs2qNames[fs2q] );

                         opname = "FCMPEq"; }

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 56: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { disassembleaddress_( address_, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "JMPL"; }

                  
                  
                  
                }
                
                break;
              case 59: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { disassembleaddress_( address_, out );

                   opname = "FLUSH"; }

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        case 3: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LD"; }

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDUB"; }

                  
                  
                  
                }
                
                break;
              case 2: case 4: case 5: case 6: case 7: case 8: case 11: 
              case 12: case 14: case 15: case 18: case 20: case 21: case 22: 
              case 23: case 24: case 27: case 28: case 30: case 31: case 34: 
              case 36: case 37: case 38: case 39: case 40: case 41: case 42: 
              case 43: case 44: case 45: case 46: case 47: case 50: case 52: 
              case 53: case 54: case 55: case 56: case 57: case 58: case 59: 
              case 60: case 61: case 62: case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };} break;
              case 3: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDD"; }

                  
                  
                  
                }
                
                break;
              case 9: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDSB"; }

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDSH"; }

                  
                  
                  
                }
                
                break;
              case 13: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDSTUB"; }

                  
                  
                  
                }
                
                break;
              case 16: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 17: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDUBA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 19: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDDA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 25: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDSBA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 26: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDSHA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 29: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDSTUBA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction2(pc, out); };}   
                
                break;
              case 32: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  fds = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(fdsNames[fds] );

                   opname = "LDF"; }

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], %fsr" );

                   opname = "LDFSR"; }

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  fdd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(fddNames[fdd] );

                   opname = "LDDF"; }

                  
                  
                  
                }
                
                break;
              case 48: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  cd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(cdNames[cd] );

                   opname = "LDC"; }

                  
                  
                  
                }
                
                break;
              case 49: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], %csr" );

                   opname = "LDCSR"; }

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  cd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(cdNames[cd] );

                   opname = "LDDC"; }

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        default: assertion(false);
      }   
    
  } 
  
   
     
    
    
  
}


return nextPC;
}

    int disassembleInstruction2( int pc, Writer out ) throws java.io.IOException {
	int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    
      switch((MATCH_w_32_0 >> 30 & 0x3)  ) {
        case 0: 
          
            switch((MATCH_w_32_0 >> 22 & 0x7)  ) {
              case 0: 
                { 
                  int  imm22 = (MATCH_w_32_0 & 0x3fffff)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( IMMEDTOSTRING( imm22));

                   opname = "UNIMP"; }

                  
                  
                  
                }
                
                break;
              case 1: case 2: case 3: case 5: case 6: case 7: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };} break;
              case 4: 
                if ((MATCH_w_32_0 & 0x3fffff)   == 0 && 
                  (MATCH_w_32_0 >> 25 & 0x1f)   == 0) { 
                  nextPC = 4 + MATCH_p; 
                  
                  

                   {  opname = "NOP"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              default: assertion(false);
            }   
          break;
        case 1: 
          { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };} break;
        case 2: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: case 1: case 3: case 5: case 7: case 8: case 9: 
              case 13: case 16: case 17: case 19: case 21: case 23: case 24: 
              case 25: case 29: case 44: case 45: case 46: case 47: case 48: 
              case 52: case 53: case 54: case 55: case 56: case 59: case 62: 
              case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };} break;
              case 2: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "OR"; }

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SUB"; }

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ORN"; }

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "UMUL"; }

                  
                  
                  
                }
                
                break;
              case 11: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SMUL"; }

                  
                  
                  
                }
                
                break;
              case 12: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SUBX"; }

                  
                  
                  
                }
                
                break;
              case 14: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "UDIV"; }

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SDIV"; }

                  
                  
                  
                }
                
                break;
              case 18: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ORcc"; }

                  
                  
                  
                }
                
                break;
              case 20: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SUBcc"; }

                  
                  
                  
                }
                
                break;
              case 22: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "ORNcc"; }

                  
                  
                  
                }
                
                break;
              case 26: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "UMULcc"; }

                  
                  
                  
                }
                
                break;
              case 27: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SMULcc"; }

                  
                  
                  
                }
                
                break;
              case 28: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SUBXcc"; }

                  
                  
                  
                }
                
                break;
              case 30: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "UDIVcc"; }

                  
                  
                  
                }
                
                break;
              case 31: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SDIVcc"; }

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "TADDcc"; }

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "TSUBcc"; }

                  
                  
                  
                }
                
                break;
              case 34: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "TADDccTV"; }

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "TSUBccTV"; }

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "MULScc"; }

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SLL"; }

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SRL"; }

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SRA"; }

                  
                  
                  
                }
                
                break;
              case 40: 
                if ((MATCH_w_32_0 >> 14 & 0x1f)   == 0) { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%y,   " );

                  out.write(rdNames[rd] );

                   opname = "RDY"; }

                  
                  
                  
                }   
                else { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  rs1i = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%asr" );

                  out.write( IMMEDTOSTRING( rs1i));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "RDASR"; }

                  
                  
                  
                }   
                
                break;
              case 41: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%psr, " );

                  out.write(rdNames[rd] );

                   opname = "RDPSR"; }

                  
                  
                  
                }
                
                break;
              case 42: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%wim, " );

                  out.write(rdNames[rd] );

                   opname = "RDWIM"; }

                  
                  
                  
                }
                
                break;
              case 43: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%tbr, " );

                  out.write(rdNames[rd] );

                   opname = "RDTBR"; }

                  
                  
                  
                }
                
                break;
              case 49: 
                { 
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", %psr" );

                   opname = "WRPSR"; }

                  
                  
                  
                }
                
                break;
              case 50: 
                { 
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", %wim" );

                   opname = "WRWIM"; }

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", %tbr" );

                   opname = "WRTBR"; }

                  
                  
                  
                }
                
                break;
              case 57: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { disassembleaddress_( address_, out );

                   opname = "RETT"; }

                  
                  
                  
                }
                
                break;
              case 58: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                    case 0: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TN"; }

                        
                        
                        
                      }
                      
                      break;
                    case 1: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TE"; }

                        
                        
                        
                      }
                      
                      break;
                    case 2: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TLE"; }

                        
                        
                        
                      }
                      
                      break;
                    case 3: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TL"; }

                        
                        
                        
                      }
                      
                      break;
                    case 4: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TLEU"; }

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TCS"; }

                        
                        
                        
                      }
                      
                      break;
                    case 6: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TNEG"; }

                        
                        
                        
                      }
                      
                      break;
                    case 7: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TVS"; }

                        
                        
                        
                      }
                      
                      break;
                    case 8: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TA"; }

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TNE"; }

                        
                        
                        
                      }
                      
                      break;
                    case 10: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TG"; }

                        
                        
                        
                      }
                      
                      break;
                    case 11: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TGE"; }

                        
                        
                        
                      }
                      
                      break;
                    case 12: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TGU"; }

                        
                        
                        
                      }
                      
                      break;
                    case 13: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TCC"; }

                        
                        
                        
                      }
                      
                      break;
                    case 14: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TPOS"; }

                        
                        
                        
                      }
                      
                      break;
                    case 15: 
                      { 
                        int  address_ = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         { disassembleaddress_( address_, out );

                         opname = "TVC"; }

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 60: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SAVE"; }

                  
                  
                  
                }
                
                break;
              case 61: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "RESTORE"; }

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        case 3: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: case 1: case 3: case 8: case 9: case 10: case 11: 
              case 12: case 13: case 14: case 16: case 17: case 19: case 24: 
              case 25: case 26: case 27: case 28: case 29: case 30: case 32: 
              case 33: case 34: case 35: case 40: case 41: case 42: case 43: 
              case 44: case 45: case 46: case 47: case 48: case 49: case 50: 
              case 51: case 56: case 57: case 58: case 59: case 60: case 61: 
              case 62: case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };} break;
              case 2: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "LDUH"; }

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "ST"; }

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STB"; }

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STH"; }

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STD"; }

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleaddress_( address_, out );

                  out.write( "], " );

                  out.write(rdNames[rd] );

                   opname = "SWAP."; }

                  
                  
                  
                }
                
                break;
              case 18: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "LDUHA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 20: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                   opname = "STA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 21: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                   opname = "STBA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 22: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                   opname = "STHA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 23: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rdNames[rd] );

                  out.write( ", [" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                   opname = "STDA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 31: 
                if ((MATCH_w_32_0 >> 13 & 0x1)   == 0) { 
                  int  asi = (MATCH_w_32_0 >> 5 & 0xff)  ;
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  regaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "[" );

                  disassembleregaddr( regaddr, out );

                  out.write( "]" );

                  out.write( IMMEDTOSTRING( asi));

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "SWAPA"; }

                  
                  
                  
                }   
                else 
                  { 
      nextPC = MATCH_p; 
      
      

      {nextPC = disassembleInstruction3(pc, out); };}   
                
                break;
              case 36: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  fds = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(fdsNames[fds] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STF"; }

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%fsr, [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STFSR"; }

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%fq,  [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STDFQ"; }

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  fdd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(fddNames[fdd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STDF"; }

                  
                  
                  
                }
                
                break;
              case 52: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  cd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(cdNames[cd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STC"; }

                  
                  
                  
                }
                
                break;
              case 53: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%csr, [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STCSR"; }

                  
                  
                  
                }
                
                break;
              case 54: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write( "%cq,  [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STDCQ"; }

                  
                  
                  
                }
                
                break;
              case 55: 
                { 
                  int  address_ = ( MATCH_p ) ;
                  int  cd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(cdNames[cd] );

                  out.write( ", [" );

                  disassembleaddress_( address_, out );

                  out.write( "]" );

                   opname = "STDC"; }

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        default: assertion(false);
      }   
    
  } 
  
   
     
    
    
  
}


return nextPC;
}

    int disassembleInstruction3( int pc, Writer out ) throws java.io.IOException {
	int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    
      switch((MATCH_w_32_0 >> 30 & 0x3)  ) {
        case 0: 
          if ((MATCH_w_32_0 >> 22 & 0x7)   == 4) { 
            int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
            int  val = (MATCH_w_32_0 & 0x3fffff)   << 10;
            nextPC = 4 + MATCH_p; 
            
            

             { out.write( "%hi(" );

            out.write( IMMEDTOSTRING( val));

            out.write( "), " );

            out.write(rdNames[rd] );

             opname = "sethi"; }

            
            
            
          }   
          else 
            { 
      nextPC = MATCH_p; 
      
      

      { opname = "#ERR#"; ; };}   
          
          break;
        case 1: 
          { 
            int  reloc = 
              4 * (((int)(( (MATCH_w_32_0 & 0x3fffffff)   ) << (4*8-(  
                          30 )))) >> (4*8-(                             30 )))  + ( MATCH_p ) ;
            nextPC = 4 + MATCH_p; 
            
            

             { out.write( callName( reloc));

             opname = "call__"; }

            
            
            
          }
          
          break;
        case 2: 
          if (24 <= (MATCH_w_32_0 >> 19 & 0x3f)   && 
            (MATCH_w_32_0 >> 19 & 0x3f)   < 64) 
            { 
      nextPC = MATCH_p; 
      
      

      { opname = "#ERR#"; ; };}   
          else 
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: case 1: case 2: case 4: case 5: case 6: case 8: case 9: 
              case 10: case 11: case 12: case 13: case 14: case 15: case 16: 
              case 17: case 18: case 20: case 21: case 22: 
                { 
      nextPC = MATCH_p; 
      
      

      { opname = "#ERR#"; ; };} break;
              case 3: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "XOR"; }

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "XNOR"; }

                  
                  
                  
                }
                
                break;
              case 19: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "XORcc"; }

                  
                  
                  
                }
                
                break;
              case 23: 
                { 
                  int  rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  reg_or_imm = ( MATCH_p ) ;
                  int  rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   { out.write(rs1Names[rs1] );

                  out.write( ", " );

                  disassemblereg_or_imm( reg_or_imm, out );

                  out.write( ", " );

                  out.write(rdNames[rd] );

                   opname = "XNORcc"; }

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        case 3: 
          { 
      nextPC = MATCH_p; 
      
      

      { opname = "#ERR#"; ; };} break;
        default: assertion(false);
      }   
    
  } 
  
   
     
    
    
  
}


return nextPC;
}

    int disassembleInstruction4( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int disassembleInstruction5( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int disassembleInstruction6( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int disassembleInstruction7( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int disassembleInstruction8( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int disassembleInstruction9( int pc, Writer out ) throws java.io.IOException {
        return 0;
    }

    int sprintDisassembleInst( int pc, Writer out ) throws java.io.IOException {
        StringBuffer s = new StringBuffer();
        StringWriter sw = new StringWriter();

	out.flush();        
        int npc = disassembleInstruction( pc, sw );
        
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

    void disassembleRegion( int pc, int to, Writer out ) throws java.io.IOException {
        while( pc < to ) {
            pc = sprintDisassembleInst( pc, out );
        }
    }

    public void disassembleRegion( int pc, int to ) throws java.io.IOException {
        disassembleRegion( pc, to, new PrintWriter(System.out) );
    }

    
    void unimplemented(int pc) {
        throw new RuntimeException("Unimplemented Instruction");
    }

    public native int doTrap( int trapno );

    void doFlagTRAP( boolean cond, int trapno ) {
        if( cond ) {
            doTrap( trapno );
        }
    }

 
    double [] float128Add( double []a, double []b ) {
        return a;  
    }
    double [] float128Sub( double []a, double []b ) {
        return a;  
    }
    double [] float128Mul( double []a, double []b ) {
        return a;  
    }
    double [] float128Div( double []a, double []b ) {
        return a;  
    }
    double [] sqrt( double []a ) {
	return a;  
    } 

    boolean float128Equals( double []a, double []b ) {
        return a[0] == b[0] && a[1] == b[1];
    }
    boolean float128Less( double []a, double []b ) {
	return false;  
    }
    double castfloat128Todouble( double []a ) {
        return a[0];  
    }
    float castfloat128Tofloat( double []a ) {
        return doubleToFloat(a[0]);  
    }
    double [] castdoubleTofloat128( double a ) {
	double []r = { a, 0 };  
	return r;
    }
    double [] castfloatTofloat128( float a ) {
	double []r = { floatToDouble(a), 0 };  
	return r;
    }
    int castfloat128Toint( double [] a ) {
        return (int)(castfloat128Tofloat(a));
    }
    double [] castintTofloat128( int a ) {
        return castfloatTofloat128( (float)a );
    }

    final static double [] defaultfloat128 = { 0,0 };

 
    class RegisterFile {
int rd[] = new int[32];
double r_f0to3[] = new double[2];
double r_f4to7[] = new double[2];
double r_f8to11[] = new double[2];
double r_f12to15[] = new double[2];
double r_f16to19[] = new double[2];
double r_f20to23[] = new double[2];
double r_f24to27[] = new double[2];
double r_f28to31[] = new double[2];
int r_AF;
int r_CF;
int r_CTI;
int r_CWP;
int r_FGF;
int r_FLF;
int r_FSR;
int r_FZF;
int r_NEXT;
int r_NF;
int r_OF;
int r_PSR;
int r_TBR;
int r_WIM;
int r_Y;
int r_ZF;
int r_npc;
int r_pc;
};

    RegisterFile regs;

    public double getregs_fdd( int num )
{
switch (num) {
case 64: return longToDouble(doubleToLong(regs.r_f0to3[0]));

case 65: return longToDouble(doubleToLong(regs.r_f0to3[1]));

case 66: return longToDouble(doubleToLong(regs.r_f4to7[0]));

case 67: return longToDouble(doubleToLong(regs.r_f4to7[1]));

case 68: return longToDouble(doubleToLong(regs.r_f8to11[0]));

case 69: return longToDouble(doubleToLong(regs.r_f8to11[1]));

case 70: return longToDouble(doubleToLong(regs.r_f12to15[0]));

case 71: return longToDouble(doubleToLong(regs.r_f12to15[1]));

case 72: return longToDouble(doubleToLong(regs.r_f16to19[0]));

case 73: return longToDouble(doubleToLong(regs.r_f16to19[1]));

case 74: return longToDouble(doubleToLong(regs.r_f20to23[0]));

case 75: return longToDouble(doubleToLong(regs.r_f20to23[1]));

case 76: return longToDouble(doubleToLong(regs.r_f24to27[0]));

case 77: return longToDouble(doubleToLong(regs.r_f24to27[1]));

case 78: return longToDouble(doubleToLong(regs.r_f28to31[0]));

case 79: return longToDouble(doubleToLong(regs.r_f28to31[1]));

default: return 0 ;
}
}
public void setregs_fdd( int num, double val )
{
switch (num) {
case 64: regs.r_f0to3[0] = val;
break;
case 65: regs.r_f0to3[1] = val;
break;
case 66: regs.r_f4to7[0] = val;
break;
case 67: regs.r_f4to7[1] = val;
break;
case 68: regs.r_f8to11[0] = val;
break;
case 69: regs.r_f8to11[1] = val;
break;
case 70: regs.r_f12to15[0] = val;
break;
case 71: regs.r_f12to15[1] = val;
break;
case 72: regs.r_f16to19[0] = val;
break;
case 73: regs.r_f16to19[1] = val;
break;
case 74: regs.r_f20to23[0] = val;
break;
case 75: regs.r_f20to23[1] = val;
break;
case 76: regs.r_f24to27[0] = val;
break;
case 77: regs.r_f24to27[1] = val;
break;
case 78: regs.r_f28to31[0] = val;
break;
case 79: regs.r_f28to31[1] = val;
break;
}
}
public double[]  getregs_fdq( int num )
{
switch (num) {
case 80: return regs.r_f0to3;

case 81: return regs.r_f4to7;

case 82: return regs.r_f8to11;

case 83: return regs.r_f12to15;

case 84: return regs.r_f16to19;

case 85: return regs.r_f20to23;

case 86: return regs.r_f24to27;

case 87: return regs.r_f28to31;

default: return defaultfloat128;
}
}
public void setregs_fdq( int num, double[]  val )
{
switch (num) {
case 80: regs.r_f0to3 = val;
break;
case 81: regs.r_f4to7 = val;
break;
case 82: regs.r_f8to11 = val;
break;
case 83: regs.r_f12to15 = val;
break;
case 84: regs.r_f16to19 = val;
break;
case 85: regs.r_f20to23 = val;
break;
case 86: regs.r_f24to27 = val;
break;
case 87: regs.r_f28to31 = val;
break;
}
}
public float getregs_fds( int num )
{
switch (num) {
case 32: return doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])&0xffffffffL)));

case 33: return doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])>>>32)));

case 34: return doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])&0xffffffffL)));

case 35: return doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])>>>32)));

case 36: return doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])&0xffffffffL)));

case 37: return doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])>>>32)));

case 38: return doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])&0xffffffffL)));

case 39: return doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])>>>32)));

case 40: return doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])&0xffffffffL)));

case 41: return doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])>>>32)));

case 42: return doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])&0xffffffffL)));

case 43: return doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])>>>32)));

case 44: return doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])&0xffffffffL)));

case 45: return doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])>>>32)));

case 46: return doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])&0xffffffffL)));

case 47: return doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])>>>32)));

case 48: return doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])&0xffffffffL)));

case 49: return doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])>>>32)));

case 50: return doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])&0xffffffffL)));

case 51: return doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])>>>32)));

case 52: return doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])&0xffffffffL)));

case 53: return doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])>>>32)));

case 54: return doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])&0xffffffffL)));

case 55: return doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])>>>32)));

case 56: return doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])&0xffffffffL)));

case 57: return doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])>>>32)));

case 58: return doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])&0xffffffffL)));

case 59: return doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])>>>32)));

case 60: return doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])&0xffffffffL)));

case 61: return doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])>>>32)));

case 62: return doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])&0xffffffffL)));

case 63: return doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])>>>32)));

default: return 0 ;
}
}
public void setregs_fds( int num, float val )
{
switch (num) {
case 32: regs.r_f0to3[0] = longToDouble((doubleToLong(regs.r_f0to3[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 33: regs.r_f0to3[0] = longToDouble((doubleToLong(regs.r_f0to3[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 34: regs.r_f0to3[1] = longToDouble((doubleToLong(regs.r_f0to3[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 35: regs.r_f0to3[1] = longToDouble((doubleToLong(regs.r_f0to3[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 36: regs.r_f4to7[0] = longToDouble((doubleToLong(regs.r_f4to7[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 37: regs.r_f4to7[0] = longToDouble((doubleToLong(regs.r_f4to7[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 38: regs.r_f4to7[1] = longToDouble((doubleToLong(regs.r_f4to7[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 39: regs.r_f4to7[1] = longToDouble((doubleToLong(regs.r_f4to7[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 40: regs.r_f8to11[0] = longToDouble((doubleToLong(regs.r_f8to11[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 41: regs.r_f8to11[0] = longToDouble((doubleToLong(regs.r_f8to11[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 42: regs.r_f8to11[1] = longToDouble((doubleToLong(regs.r_f8to11[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 43: regs.r_f8to11[1] = longToDouble((doubleToLong(regs.r_f8to11[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 44: regs.r_f12to15[0] = longToDouble((doubleToLong(regs.r_f12to15[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 45: regs.r_f12to15[0] = longToDouble((doubleToLong(regs.r_f12to15[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 46: regs.r_f12to15[1] = longToDouble((doubleToLong(regs.r_f12to15[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 47: regs.r_f12to15[1] = longToDouble((doubleToLong(regs.r_f12to15[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 48: regs.r_f16to19[0] = longToDouble((doubleToLong(regs.r_f16to19[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 49: regs.r_f16to19[0] = longToDouble((doubleToLong(regs.r_f16to19[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 50: regs.r_f16to19[1] = longToDouble((doubleToLong(regs.r_f16to19[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 51: regs.r_f16to19[1] = longToDouble((doubleToLong(regs.r_f16to19[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 52: regs.r_f20to23[0] = longToDouble((doubleToLong(regs.r_f20to23[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 53: regs.r_f20to23[0] = longToDouble((doubleToLong(regs.r_f20to23[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 54: regs.r_f20to23[1] = longToDouble((doubleToLong(regs.r_f20to23[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 55: regs.r_f20to23[1] = longToDouble((doubleToLong(regs.r_f20to23[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 56: regs.r_f24to27[0] = longToDouble((doubleToLong(regs.r_f24to27[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 57: regs.r_f24to27[0] = longToDouble((doubleToLong(regs.r_f24to27[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 58: regs.r_f24to27[1] = longToDouble((doubleToLong(regs.r_f24to27[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 59: regs.r_f24to27[1] = longToDouble((doubleToLong(regs.r_f24to27[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 60: regs.r_f28to31[0] = longToDouble((doubleToLong(regs.r_f28to31[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 61: regs.r_f28to31[0] = longToDouble((doubleToLong(regs.r_f28to31[0]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
case 62: regs.r_f28to31[1] = longToDouble((doubleToLong(regs.r_f28to31[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<0);
break;
case 63: regs.r_f28to31[1] = longToDouble((doubleToLong(regs.r_f28to31[1]) & 0xffffffff00000000L) | (doubleToLong(floatToDouble(val)) & 0xffffffffL) <<32);
break;
}
}


 
    public void dumpControlRegisters( PrintWriter out )
{
out.print( "" + "%CWP: " + (zeroPad( Integer.toHexString(regs.r_CWP), 8 )) + " %FGF: " + (((regs.r_FGF) != 0)? "true " : "false") + " %FLF: " + (((regs.r_FLF) != 0)? "true " : "false") + " %FSR: " + (zeroPad( Integer.toHexString(regs.r_FSR), 8 )) + " %FZF: " + (((regs.r_FZF) != 0)? "true " : "false") + " %PSR: " + (zeroPad( Integer.toHexString(regs.r_PSR), 8 )) + " %TBR: " + (zeroPad( Integer.toHexString(regs.r_TBR), 8 )) + " %WIM: " + (zeroPad( Integer.toHexString(regs.r_WIM), 8 )) + " \n" );
out.print( "" + "%Y: " + (zeroPad( Integer.toHexString(regs.r_Y), 8 )) + " %npc: " + (zeroPad( Integer.toHexString(regs.r_npc), 8 )) + " %pc: " + (zeroPad( Integer.toHexString(regs.r_pc), 8 )) + " Flags: " + (((regs.r_AF) != 0)? 'A' : '-') + (((regs.r_CF) != 0)? 'C' : '-') + (((regs.r_NF) != 0)? 'N' : '-') + (((regs.r_OF) != 0)? 'O' : '-') + (((regs.r_ZF) != 0)? 'Z' : '-') + " \n" );
}
public void dumpMainRegisters( PrintWriter out )
{
out.print( "" + "%g0: " + (zeroPad( Integer.toHexString(regs.rd[0]), 8 )) + " %g1: " + (zeroPad( Integer.toHexString(regs.rd[1]), 8 )) + " %g2: " + (zeroPad( Integer.toHexString(regs.rd[2]), 8 )) + " %g3: " + (zeroPad( Integer.toHexString(regs.rd[3]), 8 )) + " %g4: " + (zeroPad( Integer.toHexString(regs.rd[4]), 8 )) + " %g5: " + (zeroPad( Integer.toHexString(regs.rd[5]), 8 )) + " %g6: " + (zeroPad( Integer.toHexString(regs.rd[6]), 8 )) + " %g7: " + (zeroPad( Integer.toHexString(regs.rd[7]), 8 )) + " \n" );
out.print( "" + "%o0: " + (zeroPad( Integer.toHexString(regs.rd[8]), 8 )) + " %o1: " + (zeroPad( Integer.toHexString(regs.rd[9]), 8 )) + " %o2: " + (zeroPad( Integer.toHexString(regs.rd[10]), 8 )) + " %o3: " + (zeroPad( Integer.toHexString(regs.rd[11]), 8 )) + " %o4: " + (zeroPad( Integer.toHexString(regs.rd[12]), 8 )) + " %o5: " + (zeroPad( Integer.toHexString(regs.rd[13]), 8 )) + " %sp: " + (zeroPad( Integer.toHexString(regs.rd[14]), 8 )) + " %o7: " + (zeroPad( Integer.toHexString(regs.rd[15]), 8 )) + " \n" );
out.print( "" + "%l0: " + (zeroPad( Integer.toHexString(regs.rd[16]), 8 )) + " %l1: " + (zeroPad( Integer.toHexString(regs.rd[17]), 8 )) + " %l2: " + (zeroPad( Integer.toHexString(regs.rd[18]), 8 )) + " %l3: " + (zeroPad( Integer.toHexString(regs.rd[19]), 8 )) + " %l4: " + (zeroPad( Integer.toHexString(regs.rd[20]), 8 )) + " %l5: " + (zeroPad( Integer.toHexString(regs.rd[21]), 8 )) + " %l6: " + (zeroPad( Integer.toHexString(regs.rd[22]), 8 )) + " %l7: " + (zeroPad( Integer.toHexString(regs.rd[23]), 8 )) + " \n" );
out.print( "" + "%i0: " + (zeroPad( Integer.toHexString(regs.rd[24]), 8 )) + " %i1: " + (zeroPad( Integer.toHexString(regs.rd[25]), 8 )) + " %i2: " + (zeroPad( Integer.toHexString(regs.rd[26]), 8 )) + " %i3: " + (zeroPad( Integer.toHexString(regs.rd[27]), 8 )) + " %i4: " + (zeroPad( Integer.toHexString(regs.rd[28]), 8 )) + " %i5: " + (zeroPad( Integer.toHexString(regs.rd[29]), 8 )) + " %fp: " + (zeroPad( Integer.toHexString(regs.rd[30]), 8 )) + " %i7: " + (zeroPad( Integer.toHexString(regs.rd[31]), 8 )) + " \n" );
out.print( "" + "%CWP: " + (zeroPad( Integer.toHexString(regs.r_CWP), 8 )) + " %FGF: " + (((regs.r_FGF) != 0)? "true " : "false") + " %FLF: " + (((regs.r_FLF) != 0)? "true " : "false") + " %FSR: " + (zeroPad( Integer.toHexString(regs.r_FSR), 8 )) + " %FZF: " + (((regs.r_FZF) != 0)? "true " : "false") + " %PSR: " + (zeroPad( Integer.toHexString(regs.r_PSR), 8 )) + " %TBR: " + (zeroPad( Integer.toHexString(regs.r_TBR), 8 )) + " %WIM: " + (zeroPad( Integer.toHexString(regs.r_WIM), 8 )) + " \n" );
out.print( "" + "%Y: " + (zeroPad( Integer.toHexString(regs.r_Y), 8 )) + " %npc: " + (zeroPad( Integer.toHexString(regs.r_npc), 8 )) + " %pc: " + (zeroPad( Integer.toHexString(regs.r_pc), 8 )) + " Flags: " + (((regs.r_AF) != 0)? 'A' : '-') + (((regs.r_CF) != 0)? 'C' : '-') + (((regs.r_NF) != 0)? 'N' : '-') + (((regs.r_OF) != 0)? 'O' : '-') + (((regs.r_ZF) != 0)? 'Z' : '-') + " \n" );
}
public void dumpFloatRegisters( PrintWriter out )
{
out.print( "" + "%f0: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])&0xffffffffL))))) + " %f1: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])>>>32))))) + " %f2: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])&0xffffffffL))))) + " %f3: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])>>>32))))) + " %f4: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])&0xffffffffL))))) + " %f5: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])>>>32))))) + " \n" );
out.print( "" + "%f6: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])&0xffffffffL))))) + " %f7: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])>>>32))))) + " %f8: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])&0xffffffffL))))) + " %f9: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])>>>32))))) + " %f10: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])&0xffffffffL))))) + " \n" );
out.print( "" + "%f11: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])>>>32))))) + " %f12: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])&0xffffffffL))))) + " %f13: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])>>>32))))) + " %f14: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])&0xffffffffL))))) + " %f15: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])>>>32))))) + " \n" );
out.print( "" + "%f16: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])&0xffffffffL))))) + " %f17: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])>>>32))))) + " %f18: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])&0xffffffffL))))) + " %f19: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])>>>32))))) + " %f20: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])&0xffffffffL))))) + " \n" );
out.print( "" + "%f21: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])>>>32))))) + " %f22: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])&0xffffffffL))))) + " %f23: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])>>>32))))) + " %f24: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])&0xffffffffL))))) + " %f25: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])>>>32))))) + " \n" );
out.print( "" + "%f26: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])&0xffffffffL))))) + " %f27: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])>>>32))))) + " %f28: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])&0xffffffffL))))) + " %f29: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])>>>32))))) + " %f30: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])&0xffffffffL))))) + " \n" );
out.print( "" + "%f31: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])>>>32))))) + " \n" );
}
public void dumpAllRegisters( PrintWriter out )
{
out.print( "" + "%g0: " + (zeroPad( Integer.toHexString(regs.rd[0]), 8 )) + " %g1: " + (zeroPad( Integer.toHexString(regs.rd[1]), 8 )) + " %g2: " + (zeroPad( Integer.toHexString(regs.rd[2]), 8 )) + " %g3: " + (zeroPad( Integer.toHexString(regs.rd[3]), 8 )) + " %g4: " + (zeroPad( Integer.toHexString(regs.rd[4]), 8 )) + " %g5: " + (zeroPad( Integer.toHexString(regs.rd[5]), 8 )) + " %g6: " + (zeroPad( Integer.toHexString(regs.rd[6]), 8 )) + " %g7: " + (zeroPad( Integer.toHexString(regs.rd[7]), 8 )) + " \n" );
out.print( "" + "%o0: " + (zeroPad( Integer.toHexString(regs.rd[8]), 8 )) + " %o1: " + (zeroPad( Integer.toHexString(regs.rd[9]), 8 )) + " %o2: " + (zeroPad( Integer.toHexString(regs.rd[10]), 8 )) + " %o3: " + (zeroPad( Integer.toHexString(regs.rd[11]), 8 )) + " %o4: " + (zeroPad( Integer.toHexString(regs.rd[12]), 8 )) + " %o5: " + (zeroPad( Integer.toHexString(regs.rd[13]), 8 )) + " %sp: " + (zeroPad( Integer.toHexString(regs.rd[14]), 8 )) + " %o7: " + (zeroPad( Integer.toHexString(regs.rd[15]), 8 )) + " \n" );
out.print( "" + "%l0: " + (zeroPad( Integer.toHexString(regs.rd[16]), 8 )) + " %l1: " + (zeroPad( Integer.toHexString(regs.rd[17]), 8 )) + " %l2: " + (zeroPad( Integer.toHexString(regs.rd[18]), 8 )) + " %l3: " + (zeroPad( Integer.toHexString(regs.rd[19]), 8 )) + " %l4: " + (zeroPad( Integer.toHexString(regs.rd[20]), 8 )) + " %l5: " + (zeroPad( Integer.toHexString(regs.rd[21]), 8 )) + " %l6: " + (zeroPad( Integer.toHexString(regs.rd[22]), 8 )) + " %l7: " + (zeroPad( Integer.toHexString(regs.rd[23]), 8 )) + " \n" );
out.print( "" + "%i0: " + (zeroPad( Integer.toHexString(regs.rd[24]), 8 )) + " %i1: " + (zeroPad( Integer.toHexString(regs.rd[25]), 8 )) + " %i2: " + (zeroPad( Integer.toHexString(regs.rd[26]), 8 )) + " %i3: " + (zeroPad( Integer.toHexString(regs.rd[27]), 8 )) + " %i4: " + (zeroPad( Integer.toHexString(regs.rd[28]), 8 )) + " %i5: " + (zeroPad( Integer.toHexString(regs.rd[29]), 8 )) + " %fp: " + (zeroPad( Integer.toHexString(regs.rd[30]), 8 )) + " %i7: " + (zeroPad( Integer.toHexString(regs.rd[31]), 8 )) + " \n" );
out.print( "" + "%CWP: " + (zeroPad( Integer.toHexString(regs.r_CWP), 8 )) + " %FGF: " + (((regs.r_FGF) != 0)? "true " : "false") + " %FLF: " + (((regs.r_FLF) != 0)? "true " : "false") + " %FSR: " + (zeroPad( Integer.toHexString(regs.r_FSR), 8 )) + " %FZF: " + (((regs.r_FZF) != 0)? "true " : "false") + " %PSR: " + (zeroPad( Integer.toHexString(regs.r_PSR), 8 )) + " %TBR: " + (zeroPad( Integer.toHexString(regs.r_TBR), 8 )) + " %WIM: " + (zeroPad( Integer.toHexString(regs.r_WIM), 8 )) + " \n" );
out.print( "" + "%Y: " + (zeroPad( Integer.toHexString(regs.r_Y), 8 )) + " %npc: " + (zeroPad( Integer.toHexString(regs.r_npc), 8 )) + " %pc: " + (zeroPad( Integer.toHexString(regs.r_pc), 8 )) + " Flags: " + (((regs.r_AF) != 0)? 'A' : '-') + (((regs.r_CF) != 0)? 'C' : '-') + (((regs.r_NF) != 0)? 'N' : '-') + (((regs.r_OF) != 0)? 'O' : '-') + (((regs.r_ZF) != 0)? 'Z' : '-') + " \n" );
out.print( "" + "%f0: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])&0xffffffffL))))) + " %f1: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[0])>>>32))))) + " %f2: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])&0xffffffffL))))) + " %f3: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f0to3[1])>>>32))))) + " %f4: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])&0xffffffffL))))) + " %f5: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[0])>>>32))))) + " \n" );
out.print( "" + "%f6: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])&0xffffffffL))))) + " %f7: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f4to7[1])>>>32))))) + " %f8: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])&0xffffffffL))))) + " %f9: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[0])>>>32))))) + " %f10: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])&0xffffffffL))))) + " \n" );
out.print( "" + "%f11: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f8to11[1])>>>32))))) + " %f12: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])&0xffffffffL))))) + " %f13: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[0])>>>32))))) + " %f14: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])&0xffffffffL))))) + " %f15: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f12to15[1])>>>32))))) + " \n" );
out.print( "" + "%f16: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])&0xffffffffL))))) + " %f17: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[0])>>>32))))) + " %f18: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])&0xffffffffL))))) + " %f19: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f16to19[1])>>>32))))) + " %f20: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])&0xffffffffL))))) + " \n" );
out.print( "" + "%f21: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[0])>>>32))))) + " %f22: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])&0xffffffffL))))) + " %f23: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f20to23[1])>>>32))))) + " %f24: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])&0xffffffffL))))) + " %f25: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[0])>>>32))))) + " \n" );
out.print( "" + "%f26: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])&0xffffffffL))))) + " %f27: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f24to27[1])>>>32))))) + " %f28: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])&0xffffffffL))))) + " %f29: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[0])>>>32))))) + " %f30: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])&0xffffffffL))))) + " \n" );
out.print( "" + "%f31: " + (Float.toString(doubleToFloat(longToDouble((doubleToLong(regs.r_f28to31[1])>>>32))))) + " \n" );
}

    
 
    public int decodeeaddr( int pc )
{



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    if ((MATCH_w_32_0 >> 13 & 0x1)   == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f)   == 0) { 
        int   op_simm13 = 
          (((int)(( (MATCH_w_32_0 & 0x1fff)   ) << (4*8-(  13 )))) >> (4*8-(  13 ))) ;
        
        

         return ((int)( ~((((op_simm13)>>>12)&0x1L) - 1) << 13) | op_simm13);

        
        
        
      }   
      else { 
        int  op_rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
        int   op_simm13 = 
          (((int)(( (MATCH_w_32_0 & 0x1fff)   ) << (4*8-(  13 )))) >> (4*8-(  13 ))) ;
        
        

         return (regs.rd[op_rs1] + ((int)( ~((((op_simm13)>>>12)&0x1L) - 1) << 13) | op_simm13));

        
        
        
      }     
    else 
      if ((MATCH_w_32_0 & 0x1f)   == 0) { 
        int  op_rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
        
        

         return regs.rd[op_rs1];

        
        
        
      }   
      else { 
        int  op_rs1 = (MATCH_w_32_0 >> 14 & 0x1f)  ;
        int  op_rs2 = (MATCH_w_32_0 & 0x1f)  ;
        
        

         return (regs.rd[op_rs1] + regs.rd[op_rs2]);

        
        
        
      }     
    
  } 
  
    
  
}


}
public int decodereg_or_imm( int pc )
{



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    if ((MATCH_w_32_0 >> 13 & 0x1)   == 1) { 
      int   op_simm13 = 
        (((int)(( (MATCH_w_32_0 & 0x1fff)   ) << (4*8-(  13 )))) >> (4*8-(  13 ))) ;
      
      

       return ((int)( ~((((op_simm13)>>>12)&0x1L) - 1) << 13) | op_simm13);

      
      
      
    }   
    else { 
      int  op_rs2 = (MATCH_w_32_0 & 0x1f)  ;
      
      

       return regs.rd[op_rs2];

      
      
      
    }   
    
  } 
  
    
  
}


}


 
    

 
    public void executeADD( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] + op_regorimm);
}

public void executeADDCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] + op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = (((((tmp)>>>31) & ((op_regorimm)>>>31)) & (((regs.rd[op_rd])>>>31) == 0 ? 1 : 0)) | (((((tmp)>>>31) == 0 ? 1 : 0) & (((op_regorimm)>>>31) == 0 ? 1 : 0)) & ((regs.rd[op_rd])>>>31)));
regs.r_CF = ((((tmp)>>>31) & ((op_regorimm)>>>31)) | ((((regs.rd[op_rd])>>>31) == 0 ? 1 : 0) & (((tmp)>>>31) | ((op_regorimm)>>>31))));
}
}

public void executeADDX( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = ((regs.rd[op_rs1] + op_regorimm) + ((int)((regs.r_CF)&0x1L)));
}

public void executeADDXCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = ((regs.rd[op_rs1] + op_regorimm) + ((int)((regs.r_CF)&0x1L)));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = (((((tmp)>>>31) & ((op_regorimm)>>>31)) & (((regs.rd[op_rd])>>>31) == 0 ? 1 : 0)) | (((((tmp)>>>31) == 0 ? 1 : 0) & (((op_regorimm)>>>31) == 0 ? 1 : 0)) & ((regs.rd[op_rd])>>>31)));
regs.r_CF = ((((tmp)>>>31) & ((op_regorimm)>>>31)) | ((((regs.rd[op_rd])>>>31) == 0 ? 1 : 0) & (((tmp)>>>31) | ((op_regorimm)>>>31))));
}
}

public void executeAND( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] & op_regorimm);
}

public void executeANDCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] & op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeANDN( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] & ((0 - op_regorimm) - 1));
}

public void executeANDNCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] & ((0 - op_regorimm) - 1));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeBA( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((1 == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBAA( int op_reloc )
{
regs.r_pc = op_reloc;
regs.r_npc = (op_reloc + 4);
regs.r_CTI = 1;
}

public void executeBCC( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_CF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBCCA( int op_reloc )
{
regs.r_pc = ((regs.r_CF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_CF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBCS( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_CF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBCSA( int op_reloc )
{
regs.r_pc = ((regs.r_CF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_CF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_ZF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBEA( int op_reloc )
{
regs.r_pc = ((regs.r_ZF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_ZF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBG( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBGA( int op_reloc )
{
regs.r_pc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBGE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_NF ^ regs.r_OF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBGEA( int op_reloc )
{
regs.r_pc = (((regs.r_NF ^ regs.r_OF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_NF ^ regs.r_OF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBGU( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_CF | regs.r_ZF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBGUA( int op_reloc )
{
regs.r_pc = (((regs.r_CF | regs.r_ZF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_CF | regs.r_ZF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBL( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_NF ^ regs.r_OF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBLA( int op_reloc )
{
regs.r_pc = (((regs.r_NF ^ regs.r_OF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_NF ^ regs.r_OF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBLE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBLEA( int op_reloc )
{
regs.r_pc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBLEU( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_CF | regs.r_ZF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBLEUA( int op_reloc )
{
regs.r_pc = (((regs.r_CF | regs.r_ZF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_CF | regs.r_ZF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBN( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((0 == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBNA( int op_reloc )
{
regs.r_pc = (regs.r_npc + 4);
regs.r_npc = (regs.r_npc + 8);
regs.r_CTI = 1;
}

public void executeBNE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_ZF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBNEA( int op_reloc )
{
regs.r_pc = ((regs.r_ZF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_ZF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBNEG( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_NF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBNEGA( int op_reloc )
{
regs.r_pc = ((regs.r_NF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_NF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBPOS( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_NF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBPOSA( int op_reloc )
{
regs.r_pc = ((regs.r_NF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_NF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBVC( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_OF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBVCA( int op_reloc )
{
regs.r_pc = ((regs.r_OF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_OF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeBVS( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_OF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeBVSA( int op_reloc )
{
regs.r_pc = ((regs.r_OF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_OF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeCALL__( int op_reloc )
{
regs.rd[15] = regs.r_pc;
regs.r_pc = regs.r_npc;
regs.r_npc = op_reloc;
regs.r_CTI = 1;
}

public void executeDLY_RST_IMM( byte op_rs1, int op_imm, byte op_rd )
{
int tmp1;
tmp1 = (regs.rd[op_rs1] + op_imm);
regs.rd[8] = regs.rd[24];
regs.rd[9] = regs.rd[25];
regs.rd[10] = regs.rd[26];
regs.rd[11] = regs.rd[27];
regs.rd[12] = regs.rd[28];
regs.rd[13] = regs.rd[29];
regs.rd[op_rd] = tmp1;
}

public void executeDLY_RST_REG( byte op_rs1, byte op_rs2, byte op_rd )
{
int tmp1;
tmp1 = (regs.rd[op_rs1] + regs.rd[op_rs2]);
regs.rd[8] = regs.rd[24];
regs.rd[9] = regs.rd[25];
regs.rd[10] = regs.rd[26];
regs.rd[11] = regs.rd[27];
regs.rd[12] = regs.rd[28];
regs.rd[13] = regs.rd[29];
regs.rd[op_rd] = tmp1;
}

public void executeFABSS( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,((getregs_fds(op_fs2s) < 0) ? (0 - getregs_fds(op_fs2s)) : getregs_fds(op_fs2s)));
}

public void executeFADDD( byte op_fs1d, byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,(getregs_fdd(op_fs1d) + getregs_fdd(op_fs2d)));
}

public void executeFADDQ( byte op_fs1q, byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,float128Add( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ));
}

public void executeFADDS( byte op_fs1s, byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,(getregs_fds(op_fs1s) + getregs_fds(op_fs2s)));
}

public void executeFBA( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((1 == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBAA( int op_reloc )
{
regs.r_pc = op_reloc;
regs.r_npc = (op_reloc + 4);
regs.r_CTI = 1;
}

public void executeFBE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FZF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBEA( int op_reloc )
{
regs.r_pc = ((regs.r_FZF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FZF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBG( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FGF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBGA( int op_reloc )
{
regs.r_pc = ((regs.r_FGF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FGF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBGE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FGF | regs.r_FZF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBGEA( int op_reloc )
{
regs.r_pc = (((regs.r_FGF | regs.r_FZF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FGF | regs.r_FZF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBL( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FLF == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBLA( int op_reloc )
{
regs.r_pc = ((regs.r_FLF == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FLF == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBLE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FZF | regs.r_FLF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBLEA( int op_reloc )
{
regs.r_pc = (((regs.r_FZF | regs.r_FLF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FZF | regs.r_FLF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBLG( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FGF | regs.r_FLF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBLGA( int op_reloc )
{
regs.r_pc = (((regs.r_FGF | regs.r_FLF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FGF | regs.r_FLF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBN( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((0 == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBNA( int op_reloc )
{
regs.r_pc = (regs.r_npc + 4);
regs.r_npc = (regs.r_npc + 8);
regs.r_CTI = 1;
}

public void executeFBNE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FZF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBNEA( int op_reloc )
{
regs.r_pc = ((regs.r_FZF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FZF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBO( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) == 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBOA( int op_reloc )
{
regs.r_pc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) == 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) == 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBU( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUA( int op_reloc )
{
regs.r_pc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((((regs.r_FZF | regs.r_FGF) | regs.r_FLF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FGF | regs.r_FLF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUEA( int op_reloc )
{
regs.r_pc = (((regs.r_FGF | regs.r_FLF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FGF | regs.r_FLF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUG( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FZF | regs.r_FLF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUGA( int op_reloc )
{
regs.r_pc = (((regs.r_FZF | regs.r_FLF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FZF | regs.r_FLF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUGE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FLF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUGEA( int op_reloc )
{
regs.r_pc = ((regs.r_FLF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FLF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBUL( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = (((regs.r_FZF | regs.r_FGF) != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBULA( int op_reloc )
{
regs.r_pc = (((regs.r_FZF | regs.r_FGF) != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = (((regs.r_FZF | regs.r_FGF) != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBULE( int op_reloc )
{
regs.r_pc = regs.r_npc;
regs.r_npc = ((regs.r_FGF != 0) ? (regs.r_npc + 4) : op_reloc);
regs.r_CTI = 1;
}

public void executeFBULEA( int op_reloc )
{
regs.r_pc = ((regs.r_FGF != 0) ? (regs.r_npc + 4) : regs.r_npc);
regs.r_npc = ((regs.r_FGF != 0) ? (regs.r_npc + 8) : op_reloc);
regs.r_CTI = 1;
}

public void executeFCMPD( byte op_fs1d, byte op_fs2d )
{
double tmpd;
tmpd = (getregs_fdd(op_fs1d) - getregs_fdd(op_fs2d));
{regs.r_FZF = ((getregs_fdd(op_fs1d) == getregs_fdd(op_fs2d)) ? 1 : 0);
regs.r_FLF = ((getregs_fdd(op_fs1d) < getregs_fdd(op_fs2d)) ? 1 : 0);
regs.r_FGF = ((getregs_fdd(op_fs1d) > getregs_fdd(op_fs2d)) ? 1 : 0);
}
}

public void executeFCMPED( byte op_fs1d, byte op_fs2d )
{
double tmpd;
tmpd = (getregs_fdd(op_fs1d) - getregs_fdd(op_fs2d));
{regs.r_FZF = ((getregs_fdd(op_fs1d) == getregs_fdd(op_fs2d)) ? 1 : 0);
regs.r_FLF = ((getregs_fdd(op_fs1d) < getregs_fdd(op_fs2d)) ? 1 : 0);
regs.r_FGF = ((getregs_fdd(op_fs1d) > getregs_fdd(op_fs2d)) ? 1 : 0);
}
}

public void executeFCMPEQ( byte op_fs1q, byte op_fs2q )
{
double[]  tmpD;
tmpD = float128Sub( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) );
{regs.r_FZF = (float128Equals( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ) ? 1 : 0);
regs.r_FLF = (float128Less( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ) ? 1 : 0);
regs.r_FGF = (float128Less( (getregs_fdq(op_fs2q)), (getregs_fdq(op_fs1q)) ) ? 1 : 0);
}
}

public void executeFCMPES( byte op_fs1s, byte op_fs2s )
{
float tmpf;
tmpf = (getregs_fds(op_fs1s) - getregs_fds(op_fs2s));
{regs.r_FZF = ((getregs_fds(op_fs1s) == getregs_fds(op_fs2s)) ? 1 : 0);
regs.r_FLF = ((getregs_fds(op_fs1s) < getregs_fds(op_fs2s)) ? 1 : 0);
regs.r_FGF = ((getregs_fds(op_fs1s) > getregs_fds(op_fs2s)) ? 1 : 0);
}
}

public void executeFCMPQ( byte op_fs1q, byte op_fs2q )
{
double[]  tmpD;
tmpD = float128Sub( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) );
{regs.r_FZF = (float128Equals( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ) ? 1 : 0);
regs.r_FLF = (float128Less( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ) ? 1 : 0);
regs.r_FGF = (float128Less( (getregs_fdq(op_fs2q)), (getregs_fdq(op_fs1q)) ) ? 1 : 0);
}
}

public void executeFCMPS( byte op_fs1s, byte op_fs2s )
{
float tmpf;
tmpf = (getregs_fds(op_fs1s) - getregs_fds(op_fs2s));
{regs.r_FZF = ((getregs_fds(op_fs1s) == getregs_fds(op_fs2s)) ? 1 : 0);
regs.r_FLF = ((getregs_fds(op_fs1s) < getregs_fds(op_fs2s)) ? 1 : 0);
regs.r_FGF = ((getregs_fds(op_fs1s) > getregs_fds(op_fs2s)) ? 1 : 0);
}
}

public void executeFDIVD( byte op_fs1d, byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,(getregs_fdd(op_fs1d) / getregs_fdd(op_fs2d)));
}

public void executeFDIVQ( byte op_fs1q, byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,float128Div( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ));
}

public void executeFDIVS( byte op_fs1s, byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,(getregs_fds(op_fs1s) / getregs_fds(op_fs2s)));
}

public void executeFDMULQ( byte op_fs1q, byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,float128Mul( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ));
}

public void executeFDTOI( byte op_fs2d, byte op_fds )
{
setregs_fds(op_fds,intToFloat(((int)getregs_fdd(op_fs2d))));
}

public void executeFDTOQ( byte op_fs2d, byte op_fdq )
{
setregs_fdq(op_fdq,castdoubleTofloat128(getregs_fdd(op_fs2d)));
}

public void executeFDTOS( byte op_fs2d, byte op_fds )
{
setregs_fds(op_fds,((float)getregs_fdd(op_fs2d)));
}

public void executeFITOD( byte op_fs2s, byte op_fdd )
{
setregs_fdd(op_fdd,((double)floatToInt(getregs_fds(op_fs2s))));
}

public void executeFITOQ( byte op_fs2s, byte op_fdq )
{
setregs_fdq(op_fdq,castintTofloat128(floatToInt(getregs_fds(op_fs2s))));
}

public void executeFITOS( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,((float)floatToInt(getregs_fds(op_fs2s))));
}

public void executeFLUSH( int op_eaddr )
{

}

public void executeFMOVS( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,getregs_fds(op_fs2s));
}

public void executeFMULD( byte op_fs1d, byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,(getregs_fdd(op_fs1d) * getregs_fdd(op_fs2d)));
}

public void executeFMULQ( byte op_fs1q, byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,float128Mul( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ));
}

public void executeFMULS( byte op_fs1s, byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,(getregs_fds(op_fs1s) * getregs_fds(op_fs2s)));
}

public void executeFNEGS( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,(0 - getregs_fds(op_fs2s)));
}

public void executeFQTOD( byte op_fs2q, byte op_fdd )
{
setregs_fdd(op_fdd,castfloat128Todouble(getregs_fdq(op_fs2q)));
}

public void executeFQTOI( byte op_fs2q, byte op_fds )
{
setregs_fds(op_fds,intToFloat(castfloat128Toint(getregs_fdq(op_fs2q))));
}

public void executeFQTOS( byte op_fs2q, byte op_fds )
{
setregs_fds(op_fds,castfloat128Tofloat(getregs_fdq(op_fs2q)));
}

public void executeFSMULD( byte op_fs1d, byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,(getregs_fdd(op_fs1d) * getregs_fdd(op_fs2d)));
}

public void executeFSQRTD( byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,sqrt(getregs_fdd(op_fs2d)));
}

public void executeFSQRTQ( byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,sqrt(getregs_fdq(op_fs2q)));
}

public void executeFSQRTS( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,sqrt(getregs_fds(op_fs2s)));
}

public void executeFSTOD( byte op_fs2s, byte op_fdd )
{
setregs_fdd(op_fdd,((double)getregs_fds(op_fs2s)));
}

public void executeFSTOI( byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,intToFloat(((int)getregs_fds(op_fs2s))));
}

public void executeFSTOQ( byte op_fs2s, byte op_fdq )
{
setregs_fdq(op_fdq,castfloatTofloat128(getregs_fds(op_fs2s)));
}

public void executeFSUBD( byte op_fs1d, byte op_fs2d, byte op_fdd )
{
setregs_fdd(op_fdd,(getregs_fdd(op_fs1d) - getregs_fdd(op_fs2d)));
}

public void executeFSUBQ( byte op_fs1q, byte op_fs2q, byte op_fdq )
{
setregs_fdq(op_fdq,float128Sub( (getregs_fdq(op_fs1q)), (getregs_fdq(op_fs2q)) ));
}

public void executeFSUBS( byte op_fs1s, byte op_fs2s, byte op_fds )
{
setregs_fds(op_fds,(getregs_fds(op_fs1s) - getregs_fds(op_fs2s)));
}

public void executeJMPL( int op_eaddr, byte op_rd )
{
int tmp;
tmp = op_eaddr;
regs.rd[op_rd] = regs.r_pc;
regs.r_pc = regs.r_npc;
regs.r_npc = tmp;
regs.r_CTI = 1;
}

public void executeLD( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = getMemint(op_eaddr) ;
}

public void executeLDD( int op_eaddr, byte op_rd )
{
regs.rd[(( op_rd + 0 ) & 30)- 0] = getMemint(op_eaddr) ;
regs.rd[((( op_rd + 0 ) & 30) + 1)- 0] = getMemint((op_eaddr + 4)) ;
}

public void executeLDDF( int op_eaddr, byte op_fdd )
{
setregs_fdd(op_fdd,getMemdouble(op_eaddr) );
}

public void executeLDF( int op_eaddr, byte op_fds )
{
setregs_fds(op_fds,getMemfloat(op_eaddr) );
}

public void executeLDFSR( int op_eaddr )
{
regs.r_FSR = getMemint(op_eaddr) ;
}

public void executeLDSB( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = ((int)getMembyte(op_eaddr) );
}

public void executeLDSH( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = ((int)getMemshort(op_eaddr) );
}

public void executeLDSTUB( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = ((int)getMembyte(op_eaddr) );
setMembyte(op_eaddr, (getMembyte(op_eaddr)  | 255)) ;
}

public void executeLDUB( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = ((int)getMembyte(op_eaddr) );
}

public void executeLDUH( int op_eaddr, byte op_rd )
{
regs.rd[op_rd] = ((int)getMemshort(op_eaddr) );
}

public void executeMULSCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
int tmp2;
tmp = ((regs.rd[op_rs1] >>> 1) | (((regs.r_NF ^ regs.r_OF) == 1) ? (1 << 31) : 0));
tmp2 = ((((regs.r_Y)&0x1L) == 1) ? op_regorimm : 0);
regs.r_Y = ((regs.r_Y >>> 1) | (regs.rd[op_rs1] << 31));
regs.rd[op_rd] = (tmp + tmp2);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = (((((tmp)>>>31) & ((tmp2)>>>31)) & (((regs.rd[op_rd])>>>31) == 0 ? 1 : 0)) | (((((tmp)>>>31) == 0 ? 1 : 0) & (((tmp2)>>>31) == 0 ? 1 : 0)) & ((regs.rd[op_rd])>>>31)));
regs.r_CF = ((((tmp)>>>31) & ((tmp2)>>>31)) | ((((regs.rd[op_rd])>>>31) == 0 ? 1 : 0) & (((tmp)>>>31) | ((tmp2)>>>31))));
}
}

public void executeNOP(  )
{

}

public void executeOR( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] | op_regorimm);
}

public void executeORCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] | op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeORN( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] | ((0 - op_regorimm) - 1));
}

public void executeORNCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] | ((0 - op_regorimm) - 1));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeRDPSR( byte op_rd )
{
regs.rd[op_rd] = regs.r_PSR;
}

public void executeRDTBR( byte op_rd )
{
regs.rd[op_rd] = regs.r_TBR;
}

public void executeRDWIM( byte op_rd )
{
regs.rd[op_rd] = regs.r_WIM;
}

public void executeRDY( byte op_rd )
{
regs.rd[op_rd] = regs.r_Y;
}

public void executeRESTORE( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = (regs.rd[op_rs1] + op_regorimm);
regs.rd[8] = regs.rd[24];
regs.rd[9] = regs.rd[25];
regs.rd[10] = regs.rd[26];
regs.rd[11] = regs.rd[27];
regs.rd[12] = regs.rd[28];
regs.rd[13] = regs.rd[29];
regs.rd[14] = regs.rd[30];
regs.rd[15] = regs.rd[31];
regs.rd[op_rd] = tmp;
regs.rd[16] = getMemint((regs.rd[14] + 0)) ;
regs.rd[17] = getMemint((regs.rd[14] + 4)) ;
regs.rd[18] = getMemint((regs.rd[14] + 8)) ;
regs.rd[19] = getMemint((regs.rd[14] + 12)) ;
regs.rd[20] = getMemint((regs.rd[14] + 16)) ;
regs.rd[21] = getMemint((regs.rd[14] + 20)) ;
regs.rd[22] = getMemint((regs.rd[14] + 24)) ;
regs.rd[23] = getMemint((regs.rd[14] + 28)) ;
regs.rd[24] = getMemint((regs.rd[14] + 32)) ;
regs.rd[25] = getMemint((regs.rd[14] + 36)) ;
regs.rd[26] = getMemint((regs.rd[14] + 40)) ;
regs.rd[27] = getMemint((regs.rd[14] + 44)) ;
regs.rd[28] = getMemint((regs.rd[14] + 48)) ;
regs.rd[29] = getMemint((regs.rd[14] + 52)) ;
regs.rd[30] = getMemint((regs.rd[14] + 56)) ;
regs.rd[31] = getMemint((regs.rd[14] + 60)) ;
regs.rd[op_rd] = tmp;
}

public void executeRET_IMM_VAL( byte op_rs1, int op_imm )
{
regs.rd[8] = (regs.rd[op_rs1] + op_imm);
}

public void executeRET_REG_VAL( byte op_rs1, byte op_rs2 )
{
regs.rd[8] = (regs.rd[op_rs1] + regs.rd[op_rs2]);
}

public void executeSAVE( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = (regs.rd[op_rs1] + op_regorimm);
setMemint((regs.rd[14] + 0), regs.rd[16]) ;
setMemint((regs.rd[14] + 4), regs.rd[17]) ;
setMemint((regs.rd[14] + 8), regs.rd[18]) ;
setMemint((regs.rd[14] + 12), regs.rd[19]) ;
setMemint((regs.rd[14] + 16), regs.rd[20]) ;
setMemint((regs.rd[14] + 20), regs.rd[21]) ;
setMemint((regs.rd[14] + 24), regs.rd[22]) ;
setMemint((regs.rd[14] + 28), regs.rd[23]) ;
setMemint((regs.rd[14] + 32), regs.rd[24]) ;
setMemint((regs.rd[14] + 36), regs.rd[25]) ;
setMemint((regs.rd[14] + 40), regs.rd[26]) ;
setMemint((regs.rd[14] + 44), regs.rd[27]) ;
setMemint((regs.rd[14] + 48), regs.rd[28]) ;
setMemint((regs.rd[14] + 52), regs.rd[29]) ;
setMemint((regs.rd[14] + 56), regs.rd[30]) ;
setMemint((regs.rd[14] + 60), regs.rd[31]) ;
regs.rd[24] = regs.rd[8];
regs.rd[25] = regs.rd[9];
regs.rd[26] = regs.rd[10];
regs.rd[27] = regs.rd[11];
regs.rd[28] = regs.rd[12];
regs.rd[29] = regs.rd[13];
regs.rd[30] = regs.rd[14];
regs.rd[31] = regs.rd[15];
regs.rd[op_rd] = tmp;
}

public void executeSDIV( byte op_rs1, int op_regorimm, byte op_rd )
{
long tmpl;
tmpl = ((((long)regs.r_Y) << 32) | ((long)regs.rd[op_rs1]));
regs.rd[op_rd] = ((int)(tmpl / ((long)op_regorimm)));
}

public void executeSDIVCC( byte op_rs1, int op_regorimm, byte op_rd )
{
long tmpl;
tmpl = ((((long)regs.r_Y) << 32) | ((long)regs.rd[op_rs1]));
regs.rd[op_rd] = ((int)(tmpl / ((long)op_regorimm)));
}

public void executeSDIVCCQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] / op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_CF = 0;
}
}

public void executeSDIVQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] / op_regorimm);
}

public void executeSETHI( int op_val, byte op_rd )
{
regs.rd[op_rd] = op_val;
}

public void executeSLL( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] << op_regorimm);
}

public void executeSMUL( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
long tmpl;
tmp = regs.rd[op_rs1];
tmpl = (((long)regs.rd[op_rs1]) * ((long)op_regorimm));
regs.rd[op_rd] = ((int)tmpl);
regs.r_Y = ((int)((tmpl)>>>32));
}

public void executeSMULCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
long tmpl;
tmp = regs.rd[op_rs1];
tmpl = (((long)regs.rd[op_rs1]) * ((long)op_regorimm));
regs.rd[op_rd] = ((int)tmpl);
regs.r_Y = ((int)((tmpl)>>>32));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeSMULCCQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] * op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeSMULQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] * op_regorimm);
}

public void executeSRA( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] >> op_regorimm);
}

public void executeSRL( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] >>> op_regorimm);
}

public void executeST( byte op_rd, int op_eaddr )
{
setMemint(op_eaddr, regs.rd[op_rd]) ;
}

public void executeSTB( byte op_rd, int op_eaddr )
{
setMembyte(op_eaddr, ((byte)regs.rd[op_rd])) ;
}

public void executeSTBAR(  )
{

}

public void executeSTD( byte op_rd, int op_eaddr )
{
setMemint(op_eaddr, regs.rd[(( op_rd + 0 ) & 30) - 0]) ;
setMemint((op_eaddr + 4), regs.rd[((( op_rd + 0 ) & 30) + 1) - 0]) ;
}

public void executeSTDF( byte op_fdd, int op_eaddr )
{
setMemdouble(op_eaddr, getregs_fdd(op_fdd)) ;
}

public void executeSTD_RET(  )
{
regs.rd[8] = regs.rd[24];
}

public void executeSTF( byte op_fds, int op_eaddr )
{
setMemfloat(op_eaddr, getregs_fds(op_fds)) ;
}

public void executeSTFSR( int op_eaddr )
{
setMemint(op_eaddr, regs.r_FSR) ;
}

public void executeSTH( byte op_rd, int op_eaddr )
{
setMemshort(op_eaddr, ((short)regs.rd[op_rd])) ;
}

public void executeSUB( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] - op_regorimm);
}

public void executeSUBCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] - op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = (((((tmp)>>>31) & (((op_regorimm)>>>31) == 0 ? 1 : 0)) & (((regs.rd[op_rd])>>>31) == 0 ? 1 : 0)) | (((((tmp)>>>31) == 0 ? 1 : 0) & ((op_regorimm)>>>31)) & ((regs.rd[op_rd])>>>31)));
regs.r_CF = (((((tmp)>>>31) == 0 ? 1 : 0) & ((op_regorimm)>>>31)) | (((regs.rd[op_rd])>>>31) & ((((tmp)>>>31) == 0 ? 1 : 0) | ((op_regorimm)>>>31))));
}
}

public void executeSUBX( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = ((regs.rd[op_rs1] - op_regorimm) - ((int)((regs.r_CF)&0x1L)));
}

public void executeSUBXCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = ((regs.rd[op_rs1] - op_regorimm) - ((int)((regs.r_CF)&0x1L)));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = (((((tmp)>>>31) & (((op_regorimm)>>>31) == 0 ? 1 : 0)) & (((regs.rd[op_rd])>>>31) == 0 ? 1 : 0)) | (((((tmp)>>>31) == 0 ? 1 : 0) & ((op_regorimm)>>>31)) & ((regs.rd[op_rd])>>>31)));
regs.r_CF = (((((tmp)>>>31) == 0 ? 1 : 0) & ((op_regorimm)>>>31)) | (((regs.rd[op_rd])>>>31) & ((((tmp)>>>31) == 0 ? 1 : 0) | ((op_regorimm)>>>31))));
}
}

public void executeSWAP( int op_eaddr, byte op_rd )
{
int tmp1;
tmp1 = regs.rd[op_rd];
regs.rd[op_rd] = getMemint(op_eaddr) ;
setMemint(op_eaddr, tmp1) ;
}

public void executeTA( int op_eaddr )
{
doFlagTRAP((1 == 1), (op_eaddr + 128)) ;
}

public void executeTADDCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] + op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_CF = ((((tmp)>>>31) & ((op_regorimm)>>>31)) | ((((regs.rd[op_rd])>>>31) == 0 ? 1 : 0) & (((tmp)>>>31) | ((op_regorimm)>>>31))));
}
}

public void executeTCC( int op_eaddr )
{
doFlagTRAP((regs.r_CF != 1), (op_eaddr + 128)) ;
}

public void executeTCS( int op_eaddr )
{
doFlagTRAP((regs.r_CF == 1), (op_eaddr + 128)) ;
}

public void executeTE( int op_eaddr )
{
doFlagTRAP((regs.r_ZF == 1), (op_eaddr + 128)) ;
}

public void executeTG( int op_eaddr )
{
doFlagTRAP(((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) != 1), (op_eaddr + 128)) ;
}

public void executeTGE( int op_eaddr )
{
doFlagTRAP(((regs.r_NF ^ regs.r_OF) != 1), (op_eaddr + 128)) ;
}

public void executeTGU( int op_eaddr )
{
doFlagTRAP(((regs.r_CF | regs.r_ZF) != 1), (op_eaddr + 128)) ;
}

public void executeTL( int op_eaddr )
{
doFlagTRAP(((regs.r_NF ^ regs.r_OF) == 1), (op_eaddr + 128)) ;
}

public void executeTLE( int op_eaddr )
{
doFlagTRAP(((regs.r_ZF | (regs.r_NF ^ regs.r_OF)) == 1), (op_eaddr + 128)) ;
}

public void executeTLEU( int op_eaddr )
{
doFlagTRAP(((regs.r_CF | regs.r_ZF) == 1), (op_eaddr + 128)) ;
}

public void executeTN( int op_eaddr )
{
doFlagTRAP((0 == 1), (op_eaddr + 128)) ;
}

public void executeTNE( int op_eaddr )
{
doFlagTRAP((regs.r_ZF != 1), (op_eaddr + 128)) ;
}

public void executeTNEG( int op_eaddr )
{
doFlagTRAP((regs.r_NF == 1), (op_eaddr + 128)) ;
}

public void executeTPOS( int op_eaddr )
{
doFlagTRAP((regs.r_NF != 1), (op_eaddr + 128)) ;
}

public void executeTSUBCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
tmp = regs.rd[op_rs1];
regs.rd[op_rd] = (regs.rd[op_rs1] - op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_CF = (((((tmp)>>>31) == 0 ? 1 : 0) & ((op_regorimm)>>>31)) | (((regs.rd[op_rd])>>>31) & ((((tmp)>>>31) == 0 ? 1 : 0) | ((op_regorimm)>>>31))));
}
}

public void executeTVC( int op_eaddr )
{
doFlagTRAP((regs.r_OF != 1), (op_eaddr + 128)) ;
}

public void executeTVS( int op_eaddr )
{
doFlagTRAP((regs.r_OF == 1), (op_eaddr + 128)) ;
}

public void executeUDIV( byte op_rs1, int op_regorimm, byte op_rd )
{
long tmpl;
tmpl = ((((long)regs.r_Y) << 32) | ((long)regs.rd[op_rs1]));
regs.rd[op_rd] = ((int)(tmpl / ((long)op_regorimm)));
}

public void executeUDIVCC( byte op_rs1, int op_regorimm, byte op_rd )
{
long tmpl;
tmpl = ((((long)regs.r_Y) << 32) | ((long)regs.rd[op_rs1]));
regs.rd[op_rd] = ((int)(tmpl / ((long)op_regorimm)));
}

public void executeUDIVCCQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] / op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_CF = 0;
}
}

public void executeUDIVQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] / op_regorimm);
}

public void executeUMUL( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
long tmpl;
tmp = regs.rd[op_rs1];
tmpl = (((long)regs.rd[op_rs1]) * ((long)op_regorimm));
regs.rd[op_rd] = ((int)tmpl);
regs.r_Y = ((int)((tmpl)>>>32));
}

public void executeUMULCC( byte op_rs1, int op_regorimm, byte op_rd )
{
int tmp;
long tmpl;
tmp = regs.rd[op_rs1];
tmpl = (((long)regs.rd[op_rs1]) * ((long)op_regorimm));
regs.rd[op_rd] = ((int)tmpl);
regs.r_Y = ((int)((tmpl)>>>32));
}

public void executeUMULCCQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] * op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeUMULQ( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] * op_regorimm);
}

public void executeUMULTCC( byte op_rs1, int op_regorimm, byte op_rd )
{
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeWRPSR( byte op_rs1, int op_regorimm )
{
regs.r_PSR = (regs.rd[op_rs1] ^ op_regorimm);
}

public void executeWRTBR( byte op_rs1, int op_regorimm )
{
regs.r_TBR = (regs.r_TBR | ((regs.rd[op_rs1] ^ op_regorimm) << 12));
}

public void executeWRWIM( byte op_rs1, int op_regorimm )
{
regs.r_WIM = (regs.rd[op_rs1] ^ op_regorimm);
}

public void executeWRY( byte op_rs1, int op_regorimm )
{
regs.r_Y = (regs.rd[op_rs1] ^ op_regorimm);
}

public void executeXNOR( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] ^ ((0 - op_regorimm) - 1));
}

public void executeXNORCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] ^ ((0 - op_regorimm) - 1));
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}

public void executeXOR( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] ^ op_regorimm);
}

public void executeXORCC( byte op_rs1, int op_regorimm, byte op_rd )
{
regs.rd[op_rd] = (regs.rd[op_rs1] ^ op_regorimm);
{regs.r_NF = ((regs.rd[op_rd])>>>31);
regs.r_ZF = ((regs.rd[op_rd] == 0) ? 1 : 0);
regs.r_OF = 0;
regs.r_CF = 0;
}
}



    public int execute( int pc )
{
int nextPC = pc;



{ 
  int  MATCH_p = 
    
    

    pc
    ;
  int  MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getMemint( MATCH_p ) ; 
    
      switch((MATCH_w_32_0 >> 30 & 0x3)  ) {
        case 0: 
          
            switch((MATCH_w_32_0 >> 22 & 0x7)  ) {
              case 0: case 1: case 3: case 5: case 7: 
                { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
              case 2: 
                if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBNA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBLEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBLA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBLEUA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBCSA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBNEGA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBVSA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBAA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBNEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBGA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBGEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBGUA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBCCA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBPOSA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBVCA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                else 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBN( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBLE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBL( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBLEU( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBCS( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBNEG( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBVS( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBNE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBG( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBGE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBGU( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBCC( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBPOS( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeBVC( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                break;
              case 4: 
                if ((MATCH_w_32_0 & 0x3fffff)   == 0 && 
                  (MATCH_w_32_0 >> 25 & 0x1f)   == 0) { 
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeNOP(  );

                  
                  
                  
                }   
                else 
                  { 
      int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
      int  op_val = (MATCH_w_32_0 & 0x3fffff)   << 10;
      nextPC = 4 + MATCH_p; 
      
      

       executeSETHI( op_val, ((byte)op_rd) );}   
                
                break;
              case 6: 
                if ((MATCH_w_32_0 >> 29 & 0x1)   == 1) 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBNA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBNEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBLGA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBULA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBLA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUGA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBGA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBAA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBGEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUGEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBLEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBULEA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBOA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                else 
                  
                    switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                      case 0: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBN( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 1: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBNE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 2: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBLG( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 3: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUL( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 4: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBL( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 5: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUG( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 6: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBG( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 7: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBU( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 8: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBA( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 9: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 10: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 11: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBGE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 12: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBUGE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 13: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBLE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 14: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBULE( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      case 15: 
                        { 
                          int  op_reloc = 
                            4 * (((int)(( 
                                        (MATCH_w_32_0 & 0x3fffff) 
                                                ) << (4*8-(  22 )))) >> (4*8-(  22 )))  + 
                            ( MATCH_p ) ;
                          nextPC = 4 + MATCH_p; 
                          
                          

                           executeFBO( op_reloc );

                          
                          
                          
                        }
                        
                        break;
                      default: assertion(false);
                    }    
                break;
              default: assertion(false);
            }   
          break;
        case 1: 
          { 
            int  op_reloc = 
              4 * (((int)(( (MATCH_w_32_0 & 0x3fffffff)   ) << (4*8-(  
                          30 )))) >> (4*8-(                             30 )))  + ( MATCH_p ) ;
            nextPC = 4 + MATCH_p; 
            
            

             executeCALL__( op_reloc );

            
            
            
          }
          
          break;
        case 2: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeADD( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeAND( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeOR( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeXOR( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSUB( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeANDN( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeORN( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeXNOR( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 8: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeADDX( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 9: case 13: case 25: case 29: case 34: case 35: case 44: 
              case 45: case 46: case 47: case 54: case 55: case 57: case 62: 
              case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
              case 10: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeUMUL( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 11: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSMUL( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 12: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSUBX( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 14: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeUDIV( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSDIV( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 16: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeADDCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 17: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeANDCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 18: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeORCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 19: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeXORCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 20: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSUBCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 21: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeANDNCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 22: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeORNCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 23: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeXNORCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 24: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeADDXCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 26: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeUMULCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 27: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSMULCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 28: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSUBXCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 30: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeUDIVCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 31: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSDIVCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeTADDCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeTSUBCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeMULSCC( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSLL( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSRL( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSRA( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 40: 
                
                  switch((MATCH_w_32_0 >> 14 & 0x1f)  ) {
                    case 0: 
                      { 
                        int  op_rd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeRDY( ((byte)op_rd) );

                        
                        
                        
                      }
                      
                      break;
                    case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
                    case 8: case 9: case 10: case 11: case 12: case 13: 
                    case 14: case 16: case 17: case 18: case 19: case 20: 
                    case 21: case 22: case 23: case 24: case 25: case 26: 
                    case 27: case 28: case 29: case 30: case 31: 
                      { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
                    case 15: 
                      if ((MATCH_w_32_0 >> 25 & 0x1f)   == 0) { 
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeSTBAR(  );

                        
                        
                        
                      }   
                      else 
                        { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);}   
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 41: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeRDPSR( ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 42: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeRDWIM( ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 43: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeRDTBR( ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 48: 
                if (1 <= (MATCH_w_32_0 >> 25 & 0x1f)   && 
                  (MATCH_w_32_0 >> 25 & 0x1f)   < 32) 
                  { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);}   
                else { 
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeWRY( ((byte)op_rs1), decodereg_or_imm( op_regorimm ) );

                  
                  
                  
                }   
                
                break;
              case 49: 
                { 
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeWRPSR( ((byte)op_rs1), decodereg_or_imm( op_regorimm ) );

                  
                  
                  
                }
                
                break;
              case 50: 
                { 
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeWRWIM( ((byte)op_rs1), decodereg_or_imm( op_regorimm ) );

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeWRTBR( ((byte)op_rs1), decodereg_or_imm( op_regorimm ) );

                  
                  
                  
                }
                
                break;
              case 52: 
                if (111 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 196 || 
                  212 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 512) 
                  { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);}   
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff)  ) {
                    case 0: case 2: case 3: case 4: case 6: case 7: case 8: 
                    case 10: case 11: case 12: case 13: case 14: case 15: 
                    case 16: case 17: case 18: case 19: case 20: case 21: 
                    case 22: case 23: case 24: case 25: case 26: case 27: 
                    case 28: case 29: case 30: case 31: case 32: case 33: 
                    case 34: case 35: case 36: case 37: case 38: case 39: 
                    case 40: case 44: case 45: case 46: case 47: case 48: 
                    case 49: case 50: case 51: case 52: case 53: case 54: 
                    case 55: case 56: case 57: case 58: case 59: case 60: 
                    case 61: case 62: case 63: case 64: case 68: case 72: 
                    case 76: case 80: case 81: case 82: case 83: case 84: 
                    case 85: case 86: case 87: case 88: case 89: case 90: 
                    case 91: case 92: case 93: case 94: case 95: case 96: 
                    case 97: case 98: case 99: case 100: case 101: case 102: 
                    case 103: case 104: case 106: case 107: case 108: 
                    case 109: case 197: case 202: case 207: case 208: 
                      { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
                    case 1: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFMOVS( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFNEGS( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFABSS( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 41: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSQRTS( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 42: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSQRTD( ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 43: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSQRTQ( ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 65: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFADDS( ((byte)op_fs1s), ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 66: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFADDD( ((byte)op_fs1d), ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 67: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFADDQ( ((byte)op_fs1q), ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 69: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSUBS( ((byte)op_fs1s), ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 70: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSUBD( ((byte)op_fs1d), ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 71: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSUBQ( ((byte)op_fs1q), ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 73: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFMULS( ((byte)op_fs1s), ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 74: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFMULD( ((byte)op_fs1d), ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 75: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFMULQ( ((byte)op_fs1q), ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 77: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDIVS( ((byte)op_fs1s), ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 78: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDIVD( ((byte)op_fs1d), ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 79: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDIVQ( ((byte)op_fs1q), ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 105: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSMULD( ((byte)op_fs1d), ((byte)op_fs2d), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 110: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDMULQ( ((byte)op_fs1q), ((byte)op_fs2q), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 196: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFITOS( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 198: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDTOS( ((byte)op_fs2d), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 199: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFQTOS( ((byte)op_fs2q), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 200: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFITOD( ((byte)op_fs2s), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 201: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSTOD( ((byte)op_fs2s), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 203: 
                      { 
                        int  op_fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFQTOD( ((byte)op_fs2q), ((byte)op_fdd) );

                        
                        
                        
                      }
                      
                      break;
                    case 204: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFITOQ( ((byte)op_fs2s), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 205: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSTOQ( ((byte)op_fs2s), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 206: 
                      { 
                        int  op_fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDTOQ( ((byte)op_fs2d), ((byte)op_fdq) );

                        
                        
                        
                      }
                      
                      break;
                    case 209: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFSTOI( ((byte)op_fs2s), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 210: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFDTOI( ((byte)op_fs2d), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    case 211: 
                      { 
                        int  op_fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFQTOI( ((byte)op_fs2q), ((byte)op_fds) );

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 53: 
                if (0 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 81 || 
                  88 <= (MATCH_w_32_0 >> 5 & 0x1ff)   && 
                  (MATCH_w_32_0 >> 5 & 0x1ff)   < 512) 
                  { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);}   
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff)  ) {
                    case 84: 
                      { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
                    case 81: 
                      { 
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPS( ((byte)op_fs1s), ((byte)op_fs2s) );

                        
                        
                        
                      }
                      
                      break;
                    case 82: 
                      { 
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPD( ((byte)op_fs1d), ((byte)op_fs2d) );

                        
                        
                        
                      }
                      
                      break;
                    case 83: 
                      { 
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPQ( ((byte)op_fs1q), ((byte)op_fs2q) );

                        
                        
                        
                      }
                      
                      break;
                    case 85: 
                      { 
                        int  op_fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2s = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPES( ((byte)op_fs1s), ((byte)op_fs2s) );

                        
                        
                        
                      }
                      
                      break;
                    case 86: 
                      { 
                        int  op_fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2d = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPED( ((byte)op_fs1d), ((byte)op_fs2d) );

                        
                        
                        
                      }
                      
                      break;
                    case 87: 
                      { 
                        int  op_fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f)  ;
                        int  op_fs2q = 
                          (MATCH_w_32_0 & 0x1f)  ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeFCMPEQ( ((byte)op_fs1q), ((byte)op_fs2q) );

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 56: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeJMPL( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 58: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf)  ) {
                    case 0: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTN( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 1: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTE( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 2: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTLE( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 3: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTL( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 4: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTLEU( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTCS( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 6: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTNEG( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 7: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTVS( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 8: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTA( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTNE( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 10: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTG( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 11: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTGE( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 12: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTGU( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 13: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTCC( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 14: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTPOS( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 15: 
                      { 
                        int  op_eaddr = ( MATCH_p ) ;
                        nextPC = 4 + MATCH_p; 
                        
                        

                         executeTVC( decodeeaddr( op_eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    default: assertion(false);
                  }   
                break;
              case 59: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeFLUSH( decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 60: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSAVE( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 61: 
                { 
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  int  op_regorimm = ( MATCH_p ) ;
                  int  op_rs1 = 
                    (MATCH_w_32_0 >> 14 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeRESTORE( ((byte)op_rs1), decodereg_or_imm( op_regorimm ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        case 3: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f)  ) {
              case 0: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLD( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDUB( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDUH( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDD( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeST( ((byte)op_rd), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTB( ((byte)op_rd), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTH( ((byte)op_rd), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTD( ((byte)op_rd), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 8: case 11: case 12: case 14: case 16: case 17: case 18: 
              case 19: case 20: case 21: case 22: case 23: case 24: case 25: 
              case 26: case 27: case 28: case 29: case 30: case 31: case 34: 
              case 38: case 40: case 41: case 42: case 43: case 44: case 45: 
              case 46: case 47: case 48: case 49: case 50: case 51: case 52: 
              case 53: case 54: case 55: case 56: case 57: case 58: case 59: 
              case 60: case 61: case 62: case 63: 
                { 
      nextPC = MATCH_p; 
      
      

      unimplemented(pc);} break;
              case 9: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDSB( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDSH( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 13: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDSTUB( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_rd = (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSWAP( decodeeaddr( op_eaddr ), ((byte)op_rd) );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_fds = 
                    (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDF( decodeeaddr( op_eaddr ), ((byte)op_fds) );

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDFSR( decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_fdd = 
                    (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeLDDF( decodeeaddr( op_eaddr ), ((byte)op_fdd) );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_fds = 
                    (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTF( ((byte)op_fds), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTFSR( decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  int  op_eaddr = ( MATCH_p ) ;
                  int  op_fdd = 
                    (MATCH_w_32_0 >> 25 & 0x1f)  ;
                  nextPC = 4 + MATCH_p; 
                  
                  

                   executeSTDF( ((byte)op_fdd), decodeeaddr( op_eaddr ) );

                  
                  
                  
                }
                
                break;
              default: assertion(false);
            }   
          break;
        default: assertion(false);
      }   
    
  }
}


return nextPC;
}


 
    public void executeOneInstruction() {
        regs.rd[0] = 0;
regs.r_CTI = 0;
execute(regs.r_pc);
regs.r_pc = ((regs.r_CTI == 0) ? regs.r_npc : regs.r_pc);
regs.r_npc = ((regs.r_CTI == 0) ? (regs.r_npc + 4) : regs.r_npc);

    }

    public void run( ) {
	PrintWriter err = new PrintWriter(System.err);

        while( true ) {

            icount++;
            if( debug ) {
		err.print( 
                    rightPad( Long.toString( icount ), 5 ));
		err.print( "|" );
		try {
                    sprintDisassembleInst( regs.r_pc, err );
                } catch( java.io.IOException e ) {}
                dumpMainRegisters( err );
            }

            executeOneInstruction();
        }
    }

    
    
}



