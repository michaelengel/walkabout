/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// File: pathfinder.h
// Desc: Interface file for used in instrumentation profiles
//
// Updates:
// 04 Aug 01 - David: initial version

#ifndef PATHFINDER_H

#define PATHFINDER_H

#include <map>
#include <list>
#include <iostream>
extern "C" {
#include "mclib.h"
#include "sparc-enc.h"
};

// an edge is made up by a pair of addresses (branch_inst, target_inst)

// the upper value that an edge must reach to trigger the building of traces.
// this is value is incremented by trigger_increment each time start_trace is 
// called.
extern unsigned trigger;
extern unsigned trigger_increment;

// map edge to execution counts
extern map< pair<unsigned, unsigned>, int > edge_cnt;

// informs the emulator whethere it is in trace mode.  If so, it will add
// current instruction at %pc to the trace.
extern int trace_mode;

// holds the list of instructions in the current trace
extern list<unsigned> trace;	
extern pair<unsigned,unsigned> trace_edge;

// max size of number of instructions that the trace can hold before giving up
extern unsigned max_trace_size;		// around 4K in size

typedef struct {
	unsigned *host_address;		// fragment location on the host machine
	RBlock   reloc_block;
	RAddr	 raddr;
} Fragment;

// map of entry points of code fragments
extern map<pair<unsigned,unsigned>, Fragment> entry_points;

// map of predited values and profiled memory references
extern map<unsigned, unsigned> reference_map;
//extern map<list<unsigned>::iterator, unsigned> reference_map;

// signals the level of tracing and what to do with a particular type
extern unsigned trace_type;   
// 0 - normal start of trace in NET, ie back branch
// 1 - exit from fragment
// 2 - start of trace is a call

// number of call stack of the trace
extern int call_stack;

// **************************************************************************
// Function declarations
// **************************************************************************

// or the code generator.  This function should be virtual so that
// different instances using different profiles overwrites the code
// generator with the new code generator.
void relocator(unsigned mode);

// going from emulator to fragment (usually)
// param 1: host address (fragment address) where you are jumping to
// param 2: =0 mean from emulator, other number means from exits of
// fragments.  Since this function will emulated the delay slot inst
// if == 0.  Exits do not do this as the delay slot inst is part of 
// the fragment itself
void jump_to_fragment(unsigned*, unsigned=0);

// going from fragment to emulator
// param 1: to (source address)
// param 2: condition codes
// param 3: npc (using only when test_in_out_code is turn on)
// param 4: from (source address)
void jump_out_fragment(unsigned, unsigned, unsigned, unsigned);

// Signal end of trace.
// If current instruction is the same back branch that started trace building, 
// a valid trace if found, invoke optimizer
// At the moment, optimizer just prints the trace to the screen
void end_build_trace(unsigned, unsigned);

// Add instruction curr_inst to the trace list
// Check also that the trace is not getting too big
void add_to_trace(unsigned);

// start building a hot trace from back branch edge
void start_trace(pair<unsigned, unsigned>);

// increments the branch count for edge (addr1, addr2)
// if count reaches trigger, call start_trace to begin trace building.
// mode = 0 if in emulator mode, mode = 1 if it is a fragment exit
void increment_counter(int addr1, int addr2, unsigned mode);

// **************************************************************************
// Optimizing declarations
// **************************************************************************
// area of memory to hold the code cache
extern unsigned code_cache[];					
extern unsigned *cache_ptr;			  // pointer into the current free section

// the target of the back branch that started the trace
extern unsigned back_branch_target;

struct outportal_struct{
	pair<unsigned, unsigned> edge;
	unsigned exit_type;	 	// 0: branch exit
							// 1: if indirect jump;
							// 2: if call
	RAddr raddr;			// label for the portal
	unsigned offset;		// offset of the exit instruction relative 
							// to beginning of fragment.
};

// current out portal list
extern list<outportal_struct> out_portal;			// last in first out order

typedef struct {
	unsigned	ccr;			// conditon codes
	unsigned 	fsr;			// fsr register
	unsigned 	y;				// y register
	unsigned 	fsr_tick;		// old fsr register
	unsigned* 	cur_fragment;	
	unsigned 	temp[16];		// use as scratch space when running out 
								// of registers
} host_context_type; 

extern host_context_type host_context;
#endif
