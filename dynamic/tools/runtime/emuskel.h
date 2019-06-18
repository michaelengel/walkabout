/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

#ifndef ${BASENAME}_emu_H
#define ${BASENAME}_emu_H

#include <stdlib.h>

typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed int sint32_t;
typedef signed long long sint64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;
typedef long double NO_SUCH_TYPE; 

@DISASM
#ifndef HAVE_disasm_fmt_t
#define HAVE_disasm_fmt_t

typedef struct {
    char *addrfmt;
    char *hexfmt;
    int hexwidth, hexlen;
    int opnamelen;
    int operandlen;
    bool showHex;
    bool showSymbols;
} disasm_fmt_t;

#endif /* !HAVE_disasm_fmt_t */

extern disasm_fmt_t defDisasmFmt;

/*
 * Low-level disassembly function. Given ${PREFIX}mem relative pc,
 * disassembles one instruction, returning the operands as a string in 
 * params, and setting name (if not NULL) to the operation name.
 * Returns the pc of the immediately following instruction
 */
int ${PREFIX}disassembleInstruction( int pc, char *params, size_t paramlen,
                                     char **name );
/*
 * Public disassembly function. Given ${PREFIX}mem relative pc, disassembles
 * the instruction and stores a human-readable representation in buf, with the
 * formatting determined by fmt. Returns the pc of the following instruction.
 */
int ${PREFIX}sprintDisassembleInst( int pc, char *buf, int buflen,
                                    disasm_fmt_t &fmt = defDisasmFmt );
@DISASM                    

@EMULATOR
/* Globals */

/* Zero if debugging should be turned off, non-zero otherwise */
extern int ${PREFIX}debug;

/* count of total instructions executed to date */
extern long int ${PREFIX}icount;

/* Functions */

/*
 * Low-level function that executes the instruction at the given pc, and returns
 * the pc of the immediately following instruction (this is not necessarily the 
 * next instruction to be executed). You don't usually need to use this directly
 */
uint32_t ${PREFIX}execute( uint32_t pc );

/*
 * Public single-step function. Executes the instruction at the current pc, and
 * updates the pc to point to the next instruction for execution.
 */
void ${PREFIX}executeOneInstruction( void );

/*
 * Callback hook for traps. cond is true if the trap should execute, and the
 * trap number is given in trap.
 */
void ${PREFIX}doTrap( int trap );

/*
 * Various register dumping procedures. 
 */
void ${PREFIX}dumpControlRegisters( FILE *f );
void ${PREFIX}dumpMainRegisters( FILE *f );
void ${PREFIX}dumpFloatRegisters( FILE *f );
void ${PREFIX}dumpAllRegisters( FILE *f );

/* Register structure */
${REGISTER_STRUCT_TYPE}

${REGISTER_STRUCT_DECL};
@EMULATOR

extern char *${PREFIX}mem;

${USER_CODE_IFACE}

#endif /* !${BASENAME}_emu_H */
