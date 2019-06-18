/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// File: NET.cc
// Desc: Interface file for used in instrumentation profiles
//    	 This is the main file for implementing the Next executing tail scheme
//
// Updates:
// 04 Aug 01 - David: initial version

#include "pathfinder.h"

// ***************************************************************************
// variable definitions
// ***************************************************************************
// Uses are explained in pathfinder.h

unsigned trigger = 50;
//unsigned trigger = 0xffffffff;
unsigned trigger_increment = 50;

map< pair<unsigned, unsigned>, int > edge_cnt;
int trace_mode = 0;
list<unsigned> trace;	
pair<unsigned,unsigned> trace_edge;
unsigned max_trace_size = 1024;		// around 4K in size
//map<unsigned, Fragment> entry_points;
map<pair<unsigned,unsigned>, Fragment> entry_points;
map<unsigned, unsigned> reference_map;
//map<list<unsigned>::iterator, unsigned> reference_map;
unsigned trace_type;
int call_stack;

// ***************************************************************************
// Function definitions
// ***************************************************************************

// Signal end of trace.
// If current instruction is the same back branch that started trace building, 
// a valid trace if found, invoke optimizer
void end_build_trace(unsigned c_inst, unsigned b_target) {
	trace_mode = 0;
/*	// check to see if it is a valid trace
	if (trace.front() == trace.back()) {
		relocator();
	}
*/
	// checking for loop is no longer done, since incomplete traces are now 
	// allowed not quite, new condition now, loop or link fragment
//	map<pair<unsigned,unsigned>, Fragment>::iterator mi = 
//			entry_points.find(pair<unsigned,unsigned>(c_inst, b_target));
//	if ((trace.front() == trace.back()) || (mi != entry_points.end())) {
		// push back delay slot of the back branch
		trace.push_back(c_inst + 4);
		relocator(trace_type);
//	}
	reference_map.clear();
}
		
// Add instruction curr_inst to the trace list
// Check also that the trace is not getting too big
void add_to_trace(unsigned curr_inst) {
	trace.push_back(curr_inst);
	if (trace.size() >= max_trace_size) {
		// discard trace
		trace_mode = 0;
		reference_map.clear();
	}
}

// start building a hot trace from back branch edge
void start_trace(pair<unsigned, unsigned> edge) {
	#ifdef DEBUG_PATHFINDER
	cerr << "Starting trace finding!" << endl;	
	#endif
	trace_mode = 1;
	// increase the trigger count
	//trigger += trigger_increment;  	
	// or iterate through the entire map and reset all counters or just empty 
	// the map
	edge_cnt.clear();
	trace.clear();
	// add edge to the current trace
	trace.push_back(edge.first);
	trace_edge = edge;
	call_stack = 0;
}

// increments the branch count for edge (addr1, addr2)
// if count reaches trigger, call start_trace to begin trace building.
// mode is as follows:
// 0 - normal start of trace in NET, ie back branch
// 1 - exit from fragment
// 2 - start of trace is a call
void increment_counter(int addr1, int addr2, unsigned mode) {
	// construct the edge.
	pair<unsigned, unsigned> edge = pair<unsigned, unsigned>(addr1, addr2);
	map< pair<unsigned, unsigned>, int >::iterator i;
	if ((i = edge_cnt.find(edge)) == edge_cnt.end()) {
		// not found in map, add it and set count to 1
		edge_cnt[edge] = 1;
	} else {
		(*i).second ++;					// increment counter by 1
		if ((*i).second >= trigger) {
			start_trace(edge);
			trace_type = mode;
		}
	}
}

