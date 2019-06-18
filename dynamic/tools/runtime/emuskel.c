/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "${INTERFACE_FILE}"

${USER_CODE_TOP}

@EMULATOR

/* Globals */
long int ${PREFIX}icount;
int ${PREFIX}debug;

/* General purpose macros */
#define BITSLICE(x,lo,hi) (((x) & ((1LL<<(hi+1))-1))>>lo)
#define BITSLICEASSN(x,lo,hi,exp) (x=((x)&(~((1LL<<((hi)+1))-1)^((1LL<<(lo))-1)))|\
                                 (((exp)&((1LL<<((hi)-(lo)+1))-1))<<(lo)));
#define ROTATELEFT(x,n,w) (((x)<<(n)) | ((x)>>((w)-(n)) ))
#define ROTATERIGHT(x,n,w) (((x)>>(n)) | ((x)<<((w)-(n)) ))

/* Memory access macros */
@BYTESWAP
#include <byteswap.h>
#define getMemsint8_t(a)   *((sint8_t *)&${PREFIX}mem[a])
#define getMemsint16_t(a)  bswap_16(*((sint16_t *)&${PREFIX}mem[a]))
#define getMemsint32_t(a)  bswap_32(*((sint32_t *)&${PREFIX}mem[a]))
#define getMemsint64_t(a)  bswap_64(*((sint64_t *)&${PREFIX}mem[a]))
#define getMemuint8_t(a)   *((uint8_t *)&${PREFIX}mem[a])
#define getMemuint16_t(a)  bswap_16(*((uint16_t *)&${PREFIX}mem[a]))
#define getMemuint32_t(a)  bswap_32(*((uint32_t *)&${PREFIX}mem[a]))
#define getMemuint64_t(a)  bswap_64(*((uint64_t *)&${PREFIX}mem[a]))
#define getMemfloat32_t(a) sint32ToFloat32(getMemsint32_t(a))
#define getMemfloat64_t(a) sint64ToFloat64(getMemsint64_t(a))
#define getMemfloat128_t(a) *((float128_t *)&${PREFIX}mem[a])
#define setMemsint8_t(a,x)   getMemsint8_t(a) = x
#define setMemsint16_t(a,x)  *((sint16_t *)&${PREFIX}mem[a]) = bswap_16(x)
#define setMemsint32_t(a,x)  *((sint32_t *)&${PREFIX}mem[a]) = bswap_32(x)
#define setMemsint64_t(a,x)  *((sint64_t *)&${PREFIX}mem[a]) = bswap_64(x)
#define setMemuint8_t(a,x)   getMemuint8_t(a) = x
#define setMemuint16_t(a,x)  *((uint16_t *)&${PREFIX}mem[a]) = bswap_16(x)
#define setMemuint32_t(a,x)  *((uint32_t *)&${PREFIX}mem[a]) = bswap_32(x)
#define setMemuint64_t(a,x)  *((uint64_t *)&${PREFIX}mem[a]) = bswap_64(x)
#define setMemfloat32_t(a,x) setMemsint32_t(a, float32ToSint32(x))
#define setMemfloat64_t(a,x) setMemsint64_t(a, float64ToSint64(x))
#define setMemfloat128_t(a,x) getMemfloat128_t(a) = x
@BYTESWAP
@!BYTESWAP
#define getMemsint8_t(a)   *((sint8_t *)&${PREFIX}mem[a])
#define getMemsint16_t(a)  *((sint16_t *)&${PREFIX}mem[a])
#define getMemsint32_t(a)  *((sint32_t *)&${PREFIX}mem[a])
#define getMemsint64_t(a)  *((sint64_t *)&${PREFIX}mem[a])
#define getMemuint8_t(a)   *((uint8_t *)&${PREFIX}mem[a])
#define getMemuint16_t(a)  *((uint16_t *)&${PREFIX}mem[a])
#define getMemuint32_t(a)  *((uint32_t *)&${PREFIX}mem[a])
#define getMemuint64_t(a)  *((uint64_t *)&${PREFIX}mem[a])
#define getMemfloat32_t(a) *((float32_t *)&${PREFIX}mem[a])
#define getMemfloat64_t(a) *((float64_t *)&${PREFIX}mem[a])
#define getMemfloat128_t(a) *((float128_t *)&${PREFIX}mem[a])
#define setMemsint8_t(a,x)   getMemsint8_t(a) = x
#define setMemsint16_t(a,x)  getMemsint16_t(a) = x
#define setMemsint32_t(a,x)  getMemsint32_t(a) = x
#define setMemsint64_t(a,x)  getMemsint64_t(a) = x
#define setMemuint8_t(a,x)   getMemuint8_t(a) = x
#define setMemuint16_t(a,x)  getMemuint16_t(a) = x
#define setMemuint32_t(a,x)  getMemuint32_t(a) = x
#define setMemuint64_t(a,x)  getMemuint64_t(a) = x
#define setMemfloat32_t(a,x) getMemfloat32_t(a) = x
#define setMemfloat64_t(a,x) getMemfloat64_t(a) = x
#define setMemfloat128_t(a,x) getMemfloat128_t(a) = x
@!BYTESWAP

#define ADDROF(x) ( (uint32_t)(((char *)&x) - ${PREFIX}mem) )

static inline sint32_t float32ToSint32( float32_t f ) {
    return *((sint32_t *)&f);
}
static inline sint64_t float64ToSint64( float64_t f ) {
    return *((sint64_t *)&f);
}
static inline float32_t sint32ToFloat32( sint32_t i ) {
    return *((float32_t *)&i);
}
static inline float64_t sint64ToFloat64( sint64_t i ) {
    return *((float64_t *)&i);
}

/* "Default" values */
#define defaultsint8_t 0
#define defaultsint16_t 0
#define defaultsint32_t 0
#define defaultsint64_t 0
#define defaultuint8_t 0
#define defaultuint16_t 0
#define defaultuint32_t 0
#define defaultuint64_t 0
#define defaultfloat32_t 0
#define defaultfloat64_t 0
#define defaultfloat128_t 0

/* Function names */
#define loge(x) log(x)
#define log2(x) log(x)/log(2)
#define arctan(x) atan(x)
#define arcsin(x) asin(x)
#define arccos(x) acos(x)
#define fround(a,b,x) rint(x)

@EMULATOR

/* Macros and types for NJMC */
#define getDword(pc) getMemuint32_t(pc)
#define getWord(pc) getMemuint16_t(pc)
#define getByte(pc) getMemuint8_t(pc)

#define addressToPC(pc) (pc)
#define doFlagTRAP(c,t) if(c) doTrap(t)
typedef uint32_t dword;

@DISASM
#ifndef HEXCUTOFF
#define HEXCUTOFF 1024
#endif
#define IMMEDTOSTRING( v, x, l ) \
    snprintf( v,l, ((x)>HEXCUTOFF || ((signed)(x))<-HEXCUTOFF)?"0x%x":"%d", x )
#define BADOPCODE(x) *opname = "#ERR#";

/**************** Disassembler core ******************/
${DISASM_PARAMS}

int ${PREFIX}disassembleInstruction( int pc, char *buf, size_t buflen, char **opname )
{
    ${DISASM_FUNC}
}

disasm_fmt_t defDisasmFmt = { "%08X: ", "%02x ", 1, 6, 10, 32, true, false };

int ${PREFIX}sprintDisassembleInst( int pc, char *buf, int buflen,
                           disasm_fmt_t &fmt = defDisasmFmt )        
{
    char operands[80];
    char *opname;
    int pos = 0;
    int npc = ${PREFIX}disassembleInstruction( pc, operands,
                                      sizeof(operands), &opname );
    pos += snprintf( buf+pos, buflen-pos, fmt.addrfmt, pc );
    if( fmt.showHex ) {
        for( int i=0; i < fmt.hexlen; i+=fmt.hexwidth, pc+=fmt.hexwidth ) {
            if( pc < npc )                                                 
                pos += snprintf( buf+pos, buflen-pos, fmt.hexfmt,
                                 (int)mem[pc]&((1<<(8*fmt.hexwidth))-1) );
            else                                                           
                pos += snprintf( buf+pos, buflen-pos, "%*c",
            fmt.hexwidth*2+1, ' ' );   
        }
    }    
    pos += snprintf( buf+pos, buflen-pos, "%-*s", fmt.opnamelen, opname );
    pos += snprintf( buf+pos, buflen-pos, " " );
    pos += snprintf( buf+pos, buflen-pos, "%-*s", fmt.operandlen, operands );
    return npc;
}
@DISASM

@EMULATOR

void ${PREFIX}unimplemented( int pc ) {
    char buf[80];
    ${PREFIX}sprintDisassembleInst( pc, buf, sizeof(buf) );
    fprintf(stderr, "Unimplmented instruction: %s\n", buf );
    exit(1);
}

/************** Register definitions **************/
${REGISTER_STRUCT_DEFN}
${REGISTER_ACCESSORS}

/**************** Register Dumping ****************/
${REGISTER_DUMP_FUNCS}

/*********** Parameter decoding support ***********/
${PARAM_FUNCS}

/**************** Flag call macros ****************/
${FLAG_CALL_FUNCS}

/******************* Operations *******************/
${OPERATION_FUNCS}

${EXECUTE_FUNC}

/**************** Global Functions ****************/
void ${PREFIX}executeOneInstruction()
{
    ${EXECUTE_ONE_FUNC}
}

void ${PREFIX}run( )
{
    while(1) {
#ifndef NDEBUG
        ${PREFIX}icount++;
@DISASM
        if( ${PREFIX}debug ) {
            char buf[80];
            ${PREFIX}sprintDisassembleInst( ${PC}, buf, sizeof(buf) );
            dumpMainRegisters( stderr );
            fprintf( stderr, "%5ld|%s\n", ${PREFIX}icount, buf );
        }
@DISASM
#endif
        executeOneInstruction();
    }
}

@EMULATOR

${USER_CODE_BOTTOM}
