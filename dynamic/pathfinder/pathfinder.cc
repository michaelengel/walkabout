/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// File: pathfinder.cc
// Desc: This file contains the supporting code for instrumention
//	     It instruments the guest program using Dynamo's NET (next executing 
//		 tail) method.
//		 Monitors back branches and invokes the trace builder to build hot 
//		 traces 
//		 Monitor values of memory references of the hot trace

// Updates:
// 07 Jun 01 - David: initial version
// 12 Jun 01 - David: Optimizer added
// 16 Jun 01 - David: added monitor_eaddr() to profiles the memory location of 
// 					  loads and stores in the trace.
// 23 Jun 01 - David: %fp and %sp no longer adjasted when entering or leaving 
//					  fragment, all memory references are relocated.
// 14 Jul 01 - David: fix that damn bug that need spilling of registers at 
//					  SAVEs!! > 2weeks of debuging.
//					  Note: SAVE spill sequence is in wrong order!
// 18 Jul 01 - David: change structuring of out_portal.
// 					  entry_points changed from map<unsigned, Fragment> to 
//					  map<pair<unsigned,unsigned>, Fragment>
// 18 Jul 01 - David: checking for loop in a trace is no longer done, since 
//					  incomplete traces are now allowed.  
//					  err..  New condition, loop or link fragment
// 19 Jul 01 - David: added host_context.cur_fragment for holding the current 
//					  executing fragment.  All exits now uses its host fragment 
//					  address as the source.
// 01 Aug 01 - David: VM is removed, no more relocation
// 13 Aug 01 - David: nasty bug in li benchmark.  fallthrough of the backbranch 
//					  may already have an outportal from a previous exit.  
//					  Totally redo outportals.
// 15 Aug 01 - David: try to remove the check for back branch targets.
//					  add entry point lookup in relocate_target, this will 
//					  speed up fragment linking.

#include "pathfinder.h"
//#define DEBUG_PATHFINDER
// *****************************************************************************
// Optimize Function
// *****************************************************************************

#define MAX_CACHE 1024 * 1024
// 1M in size

// area of memory to hold the code cache
unsigned code_cache[MAX_CACHE / 4];			
unsigned *cache_ptr = code_cache;		// pointer into the current free section

unsigned back_branch;	// or a call if call is the start of trace
// the target of the back branch that started the trace
unsigned back_branch_target;

// current map of guest addresses to host addresses
map<unsigned, RAddr> guest_host;	 
								// guest addr -> host code in current fragment

list<unsigned>::iterator frag_inst;		// trace iterator

// current out portal list
//list< pair<unsigned, unsigned> > out_portal;	// last in first out order
list<outportal_struct> out_portal;				// last in first out order

host_context_type host_context;		// use to hold context switch info and tmps


// ***********************
// use for encoding 
#define CLOSURE_DIM 2048				// twice the amount of instructions.
struct RCLOSURE_STRUCT {
  int size;
  RClosure cls[CLOSURE_DIM];
};

struct RCLOSURE_STRUCT closures;
RBlock new_sect(int, char*); 
static void myfail(char *fmt, ...);
// ***********************

// forward declaration of generated emulator function by genemu
int encode(sint32_t);
void run( );
extern unsigned progtop;

//unsigned low_range, high_range;
// Used to collect profiled value of instructions during trace selection mode
// Uses: (a) collects target of Jumpl, ret, retl instructions
// 	     (b) collects memory dereferencing of loads and stores
// (b) was used originally by the relocator to determine whether relocation 
// need to be done for loads and store, at that time only load and store that
// access outside of the stack are relocated.  This is now depricated, 
// as the relocator will relocate every loads and stores using the mirrored
// stack technique to aviod register spill traps.
// Currently it is really used for predicted values (a).
// (b) can be used for data prefetching, but I never had the time to completely 
// produce the prefetching optimization.
void monitor_eaddr(unsigned inst, unsigned value) {

	// Used by relocator before stack mirroring
	//if ((value >= low_range) && (value <= high_range)) {	
	//if ((value <= progtop) || (value >= (DEFAULT_VM_SIZE))) {	
	//	reference_map[inst] = value;
	//} else {
	//	reference_map[inst] = 0;
	//}	
	
	// reference_map[inst] = value;
	// problem with traces having the same set of repeating instructions
	// hence overwriting the same predicted values.
	// eg. the repeated returns behaviour of the SPEC2000 mcf benchmark
	// at the beginning of a trace.
	// 		0x122a8: ret 
	//		0x122ac: restore 
	// 		0x122a8: ret 
	//		0x122ac: restore 
	// 		0x122a8: ret 
	//		0x122ac: restore 
	// 		...
	
	// now use trace iterator as index
	if (inst != trace.back()) 
		abort();
	unsigned ind = *((unsigned*)(&(--(trace.end()))));
	//list<unsigned>::iterator ind = --(trace.end());
	//reference_map.insert(pair<list<unsigned>::iterator, unsigned>(ind, value));
	reference_map[ind] = value;	
}

// forward declarations
RAddr in_TLB(unsigned from, unsigned to);
void pathfinder(unsigned);

void specialised_emu_params(char *args) {
//	Used to define additional options for the instrumented emulator
//  ie. eg. add command options to change trigger value, turn of specilisation
//      etc.
}

// keep track of the call/return pairs
int call_made;

void initialise_states() {
	// depricated, used by relocator before mirrored stack.
	// This need only be done once at the very beginning
	// determine the range of values statically allocated data
		
    //SectionInfo *si = bf->GetSectionInfoByName( ".rodata" );
	//low_range = si->uNativeAddr;
    //si = bf->GetSectionInfoByName( ".bss" );
	//if( si == NULL || si->uSectionSize == 0 ) {
    //	si = bf->GetSectionInfoByName( ".data" );
	//}	
	//high_range = si->uNativeAddr + si->uSectionSize;	
	
	// !!!!!!!!!!!!!!!!!!
	// kill all closures
 	for (int i=0; i<closures.size; i++) {
   		delete closures.cls[i];
  	}
	closures.size = 0;
		
	// kill all label, RAddr!!!!
	map<unsigned, RAddr>::iterator mi;
	for (mi = guest_host.begin(); mi != guest_host.end(); mi++) {
		delete (*mi).second->label;
		delete (*mi).second;		
	}
	guest_host.clear();
	
	// kill list of out portals 
	out_portal.clear();
	
	// fail = myfail;	// assign new fail routine, well ATM myfail is empty
	call_made = 0;
}

void finalise_states() {
	reference_map.clear();	
}

// profile mode use for extending next executing tail scheme by profiling
// say the next 10 iterations and then determining the best of them.
// also use for data prefetching optimisation
int profile_mode = 0;
void relocator(unsigned mode) {
	//if (profile_mode) {
		// end profile collecting and call pathfinder
	pathfinder(mode);
}

// used for debugging 
int test_in_out_code = 0;
int state = 0;

void pathfinder(unsigned mode) {
//  turing off traps..  previledge instructions
//	register unsigned l0 __asm__ ("l0");
//	__asm__ ("sir 0");
//	__asm__ ("rdpr %pstate, %l0");
//	l0 = l0 | 0x00000004;
//	__asm__ ("wrpr %l0, %g0, %pstate");

	#ifdef DEBUG_PATHFINDER
	// state is used to limit the levels of fragment generation
	state ++;	
	if (state > 1000) {
		finalise_states();
		trigger = 0xffffffff;
		cerr << "!trace limit exceeded" << endl;
		return;
	}
	#endif
	
	initialise_states();
	
	Fragment frag;	
	frag.host_address = cache_ptr;
	frag.reloc_block = new_sect(0, "fragment");

	//first instruction
	//	emitm(2 << 30 | 0x2b << 19, 4); // flushw

	back_branch_target = *(++(++(trace.begin())));
	back_branch = *(--(--(trace.end())));
	
	// take away the first inst which is the back branch or exit
	trace.erase(trace.begin());	
	// first inst is now the delay slot instruction or target of exit
	// or a call
	
	if (mode != 1) {
		// as long as the trace head is not from an out portal,
		// kill this delay slot instruction!!
		trace.erase(trace.begin());	
	}
	
	#ifdef DEBUG_PATHFINDER
	if ((test_in_out_code) || (state >= 1000)) {	
		list<unsigned>::iterator li = trace.begin();
		SAVE(14, imode(-128), 14);
		// set virtual PC to next inst
		int o0 = 8;					// o0 is register number 8
		decode_sethi((*li), o0);
		ADD(o0, imode((*li) & 0x000003ff), o0); 
		// rd %ccr, %o1 		
		//emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4); 
		// set virtual nPC
		li++;
		decode_sethi((*li), 10);
		ADD(10, imode((*li) & 0x000003ff), 10); 
	
		// set icc to 0xffffffff
		mov(imode(-1), 9); 
	
		int o3 = 11;
		decode_sethi((unsigned)jump_out_fragment, o3);	
		JMPL(dispA(o3, ((unsigned)jump_out_fragment & 0x000003ff)), 0);  
		// result to %g0, never returns.	
		fNOP();	
		goto Code_Gen;
	}
	#endif
	
	// create the map of instructions of the fragment
	// this step is necessary for the use of relocate_target() to check whether 
	// an out portal is needed.
	// really, this is for obtaining an RAddr for CTI.
	// err..  yes, memory leak!  same source instructions overwrite the same 
	// location
	for (list<unsigned>::iterator li = trace.begin(); li != trace.end(); li++) {
		char guest_addr[12];
		sprintf(guest_addr, "%x", *li);
		guest_host[*li] = addr_new(label_new(guest_addr), 0);
		#ifdef DEBUG_PATHFINDER
		cerr << guest_addr << endl;
		#endif
	}
           			
	// ****************** generate code in fragment cache *********************
	
	frag_inst = trace.begin();		
	// 1st inst is the entry point
	frag.raddr = addr_new(label_new(guest_host[trace.front()]->label->name), 0);
	label_define(frag.raddr->label, 0);
	// add entry point to the map of fragments
	entry_points[trace_edge] = frag;

	while (frag_inst != trace.end()) {	
		#ifdef DEBUG_PATHFINDER
		//cerr << "Doing instruction " << hex << *frag_inst << endl;
		#endif
		//label_define(guest_host[*frag_inst]->label, 0);
		// label define aborts on multiple definitions, but multiple definition 
		// of the same instruction is allowed in a fragment.  Each same 
		// instruction have it's own label, but the map (guest_host) only 
		// keeps the last one of them.
		// yes, all previous labels are not used but have memory on the heap.
		// and yes, memory leak!  since those are not released with the map.
		// possible solution, have a multimap or check label existence before 
		// creation.
		RLabel lbl = guest_host[*frag_inst]->label;
    	lbl->block = currb;
   	 	lbl->offset = lc();
		
		encode(*frag_inst);
		frag_inst++;
	}
	
	// add out portals of the fragment.  1st is the fallthrough of the back 
	// branch	
	/*if (guest_host.find(back_branch+8) != guest_host.end()) {
		;
	} else {
		outportal_struct o;
		o.edge = pair<unsigned, unsigned>(back_branch, back_branch+8);
		o.exit_type = 0;
		o.offset = cur_pc();
		out_portal.push_front(o);		
		char guest_addr[12];
		sprintf(guest_addr, "%X", back_branch+8);	// could be incorrect here
		guest_host[back_branch+8] = addr_new(label_new(guest_addr), 0);			
	}*/
	//  bahhh...  always create one for the fallthrough portal
		// save %sp, -128, %sp
		// call fallthrough
		// nop
		// rd %ccr, %o1
		// sethi %hi(to), %o0
		// add %o0, %lo(to), %o0
		// sethi %hi(from), %o3
		// add %o3, %lo(from), %o3
		// call jump_out_fragment
		SAVE(14, imode(-128), 14);									
      	emitm(1 << 30 | 3 & 0x3fffffff, 4);   						
		fNOP();
		emitm(0, 4);
		mov(imode(3), 10); 
		emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4);	
		decode_sethi(back_branch+8, 8);								
		ADD(8, imode((back_branch+8) & 0x000003ff), 8); 			
		decode_sethi(back_branch, 11);								
		ADD(11, imode(back_branch & 0x000003ff), 11); 				
		decode_sethi((unsigned)jump_out_fragment, 6);	
		JMPL(dispA(6, ((unsigned)jump_out_fragment & 0x000003ff)), 0);  
		fNOP();	

	// all other portals
	for (list <outportal_struct>::iterator op = out_portal.begin(); 
			op != out_portal.end(); op++) {	
		#ifdef DEBUG_PATHFINDER
		cerr << "Doing out portal " << hex << (*op).edge.first << " -> " 
			<< (*op).edge.second;
		#endif
		label_define((*op).raddr->label, 0);
		// rd %ccr, %g6  such that there's no chance of changes to the ccr
		//emitm(2 << 30 | 40 << 19 | 2 << 14 | (6 & 0x1f) << 25, 4); 
		// generate a SAVE to get scratch registers
		// SAVE(14, imode(-72), 14);  err..  72 is too small if callee decides 
		// to write to the parent frame.
		SAVE(14, imode(-128), 14);
		//mov(rmode(6), 9);
		
      	emitm(1 << 30 | 3 & 0x3fffffff, 4);   // call fallthrough+1
		fNOP();

		// write the relative position to the instruction that exited.
		emitm(cur_pc() - (*op).offset - 12, 4);
		
		// setup the call to jump back to the emulator
		// setup parameter addr of jump_out_fragment
		int o0 = 8;					// o0 is register number 8
		int o2 = 10;				// o2 is register number 10
		switch ((*op).exit_type) {
		case 0:
			// branch exit
			mov(imode(0), o2); 
			// rd %ccr, %o1
			emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4);
			decode_sethi((*op).edge.second, o0);
			ADD(o0, imode(((*op).edge.second) & 0x000003ff), o0); 
			break;
		case 1:
			// indirect exit
			mov(imode(1), o2); 
			// condition code
			// mov %g7, %o1			
			// mov(rmode(7), 9);						
			// rd %ccr, %o1
			emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4);
			// register value in %g5
			mov(rmode(5), o0);
			fNOP();
			break;
		case 2:
			// call exit
			mov(imode(2), o2); 
			// rd %ccr, %o1
			emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4);
			decode_sethi((*op).edge.second, o0);
			ADD(o0, imode(((*op).edge.second) & 0x000003ff), o0); 
			break;
		default:
			cerr << "Unknown exit type!" << endl;
			abort();
		}
		//emitm(2 << 30 | 0x2b << 19, 4); 		// flushw
		// %o3 is the from address
		decode_sethi((*op).edge.first, 11);
		ADD(11, imode(((*op).edge.first) & 0x000003ff), 11); 
		
		decode_sethi((unsigned)jump_out_fragment, 6);	
		JMPL(dispA(6, ((unsigned)jump_out_fragment & 0x000003ff)), 0);  
			// result to %g0, never returns.	
		fNOP();	
		#ifdef DEBUG_PATHFINDER
		cerr << ".. Success " << endl;
		#endif
	}


Code_Gen:	
	// correct the relocations frag
	set_address(frag.reloc_block, (unsigned)cache_ptr);
	
	#ifdef DEBUG_PATHFINDER
	cerr << "closure total is " << closures.size << endl;
	#endif
	/* apply closures */
 	for (int i=0; i<closures.size; i++) {
		#ifdef DEBUG_PATHFINDER
		cerr << "applying closure " << i << endl;  
		#endif
   		apply_closure(closures.cls[i], cl_emitm, (FailCont)fail);
  	}
	
	if ((unsigned)cache_ptr + block_size(frag.reloc_block) >= 
		(unsigned)code_cache + MAX_CACHE) {
		cerr << "Fragmet cache exceeded, need flushing." << endl;
		abort();
	}
	// copy the code into the code cache
	block_copy((unsigned char*)cache_ptr, frag.reloc_block, 0, 
		block_size(frag.reloc_block));
	//	block_copy((unsigned char*)cache_ptr, frag.reloc_block, 4, 
	//		block_size(frag.reloc_block));
		
	// flush the caches!!!
	unsigned* end = (unsigned*)((unsigned)cache_ptr + 
		block_size(frag.reloc_block));
	unsigned* ptr = (unsigned*)((unsigned)cache_ptr);
	__asm__ ("flush %0" : : "r" ((unsigned)(ptr-1)));
	while (ptr < end) {
		__asm__ ("flush %0" : : "r" (ptr));
		ptr += 2;
	}
	//	unsigned end = (unsigned)cache_ptr + block_size(frag.reloc_block);
	//	register unsigned l0 __asm__ ("l0");	
	//	l0 = (unsigned)cache_ptr;
	//	while (l0 < end) {
	//		__asm__ ("flush %l0");
	//		__asm__ ("add %l0, 8, %l0");
	//	}

	// increment the free code cache pointer after the fragment
	cache_ptr = end;

	// make it align to 8
	cache_ptr = (unsigned*)(((unsigned)cache_ptr & 0xfffffff8) + 0x8);	

	// want to kill fragment's relocatable block!  NJMC function not there?	
	// can be free by hand, but what if other fragments wants to link to
	// this one??  
	// should be safe, since you don't want other fragments to link to
	// the middle of this one anyway.  (portals maybe, but it used another
	// scheme)
	// only problem is that mclib don't have the function to destroy this
	
	#ifdef DEBUG_PATHFINDER
	//dump_reference_map();
	#endif
	finalise_states();
}

#include "context_switch.cc"

#define NOTHING 1
#define RELOCATE 2
#define CTI 3

// ---------------------------------------------------------------------------
// bunch of function for NJMC and function doing closures
static void myfail(char *fmt, char* st, ...) {
  printf(fmt, st); 
} // ***** my_fail ***************************************************

void *mc_alloc(int size, Mc_alloc_pool pool) {
  return (void *)malloc(size);
}  // **** mc_alloc *******************************************************

void add_closure(RClosure cl) {
  closures.cls[closures.size] = cl;
  closures.size++;
} // ***** add_closure ***************************************************

// function which creates a closure at the current relocation block 
RClosure mc_alloc_closure(unsigned size, RBlock dest_block, unsigned dest_lc) {
  RClosure cl;
  cl = (RClosure)malloc(size);
  add_closure(cl);
  return cl;
}  // ***** mc_alloc_closure **********************************************

RBlock new_sect(int size, char* name) {  
// creates a new relocatable block for each label ???

  RBlock ablock;
  ablock = block_new(size);
  ablock->label->name = name;
  set_block(ablock);
  // align(8); 
  return ablock;
} // ***** new_sect ********************************************
// ---------------------------------------------------------------------------

// adjasts the displacement of branches
RAddr relocate(unsigned addr, unsigned pc) {
	map<unsigned, RAddr>::iterator mi = guest_host.find(addr);
	if (mi == guest_host.end()) {
		outportal_struct o;
		o.edge = pair<unsigned, unsigned>(pc, addr);
		o.exit_type = 0;
		o.offset = cur_pc();
		char guest_addr[12];
		sprintf(guest_addr, "%X", addr);
		o.raddr = addr_new(label_new(guest_addr), 0);	
		out_portal.push_front(o);		
		return o.raddr;
	} else
		return (*mi).second;
}

//int invert_branch = 0;
// adjasts the displacement of unconditional branches
int relocate_target(unsigned addr, unsigned pc, RAddr &raddr, unsigned annul=0) 
{
	
/*	if (!invert_branch) {
		map<unsigned, RAddr>::iterator mi = guest_host.find(addr);
		if (mi == guest_host.end()) {
			// jumping out of the fragment
			// add to list of out portals
			out_portal.push_front(pair<unsigned, unsigned>(addr,0));		
			// create label
			char guest_addr[12];
			sprintf(guest_addr, "%X", addr);	
			raddr = guest_host[addr] = addr_new(label_new(guest_addr), 0);
			return 0;
		} else {
			if (guest_host.find(pc+8) != guest_host.end()) {
				// Some other instruction also has out portal here.
				;
			} else {
				out_portal.push_front(pair<unsigned, unsigned>(pc+8,0));		
				char guest_addr[12];
				sprintf(guest_addr, "%X", pc+8);	// could be incorrect here
				guest_host[pc+8] = addr_new(label_new(guest_addr), 0);			
			}
			raddr = guest_host[addr];
			return 0;
		}
	}
*/
	// use of guest_host to determine the target of outportals are still 
	// in-efficient,
	// should use edges to characterise outportals, not just the target.  
	// (which may cause multiple CTIs to use the same outportal, but different 
	// origin the hickup in jump_out_fragment (from, to) pair!

	if (pc == back_branch) {
		// never ever invert
		// 2 cases, jumping back to the begining of fragment or exit 
		
		// fall through is created as the first out portal in relocator, before
		// code generation of out portal is done.
		map<unsigned, RAddr>::iterator mi = guest_host.find(addr);
		if (mi == guest_host.end()) {
			// jumping out of the fragment
			raddr = in_TLB(pc, addr);
			if (raddr)
				return 0;
			outportal_struct o;
			o.edge = pair<unsigned, unsigned>(pc, addr);
			o.exit_type = 0;
			o.offset = cur_pc();
			// create label
			char guest_addr[12];
			sprintf(guest_addr, "%X", addr);	
			raddr = o.raddr = addr_new(label_new(guest_addr), 0);
			out_portal.push_front(o);		
			return 0;
		} else {
			// back to the beginning of the trace	
			raddr = (*mi).second;
			return 0;
		}
	}
		
	// check for back branch from the code below could possibly be removed now..
	// but be careful!!!  This code is tricky..  You've been warned!
	// ok..  I am going to try this...  checkout an old version if there's a 
	// bug
		
	list<unsigned>::iterator ci = frag_inst;
	++ci;
	if (annul) {
		unsigned next_inst = *ci;
		// check to see if branch taken or not
		if (next_inst == pc+4) {
			// branch taken since delay slot is executed
			++ci;
			// invert, and
			// make fallthrough as the out portal
			raddr = in_TLB(pc, pc+8);
			if (raddr)
				return 1;
			outportal_struct o;
			o.edge = pair<unsigned, unsigned>(pc, pc+8);
			o.exit_type = 0;
			o.offset = cur_pc();
			char guest_addr[12];
			sprintf(guest_addr, "%X", pc+8);
			raddr = o.raddr = addr_new(label_new(guest_addr), 0);			
			out_portal.push_front(o);		
			return 1;
			// invert the thing!!
		} else {
			// branch not taken
				// jumping out of the fragment
				raddr = in_TLB(pc, addr);
				if (raddr)
					return 0;
				// add to list of out portals
				outportal_struct o;
				o.edge = pair<unsigned, unsigned>(pc, addr);
				o.exit_type = 0;
				o.offset = cur_pc();
				// create label
				char guest_addr[12];
				sprintf(guest_addr, "%X", addr);	
				raddr = o.raddr = addr_new(label_new(guest_addr), 0);
				out_portal.push_front(o);		
				return 0;
		}
	} else 
		++ci; 
	
	
	// ++(++ci);
	if (ci != trace.end()) {
		unsigned next_inst = *ci;
		if (next_inst != addr) {
			// keep current branch state
				// jumping out of the fragment
				raddr = in_TLB(pc, addr);
				if (raddr)
					return 0;
				// add to list of out portals
				outportal_struct o;
				o.edge = pair<unsigned, unsigned>(pc, addr);
				o.exit_type = 0;
				o.offset = cur_pc();
				// create label
				char guest_addr[12];
				sprintf(guest_addr, "%X", addr);	
				raddr = o.raddr = addr_new(label_new(guest_addr), 0);
				out_portal.push_front(o);		
				return 0;
		} else {
			;
		}
	}
		
	map<unsigned, RAddr>::iterator mi = guest_host.find(addr);
	if (mi == guest_host.end()) {
		// jumping out of the fragment
		raddr = in_TLB(pc, addr);
		if (raddr)
			return 0;
		// add to list of out portals
		outportal_struct o;
		o.edge = pair<unsigned, unsigned>(pc, addr);
		o.exit_type = 0;
		o.offset = cur_pc();
		// create label
		char guest_addr[12];
		sprintf(guest_addr, "%X", addr);	
		raddr = o.raddr = addr_new(label_new(guest_addr), 0);
		out_portal.push_front(o);		
		return 0;
	} else {
			// make fallthrough as the out portal
				raddr = in_TLB(pc, pc+8);
				if (raddr)
					return 1;
				outportal_struct o;
				o.edge = pair<unsigned, unsigned>(pc, pc+8);
				o.exit_type = 0;
				o.offset = cur_pc();
				char guest_addr[12];
				sprintf(guest_addr, "%X", pc+8);	// could be incorrect here
				raddr = o.raddr = addr_new(label_new(guest_addr), 0);			
				out_portal.push_front(o);		
			return 1;
			// invert the thing!!
	}
}

// 0 if it is not within static data range.
int do_relocate_eaddr(sint32_t pc) {
	map<unsigned, unsigned>::iterator mi = 
		//reference_map.find(pc);
		reference_map.find(*(unsigned*)(&(frag_inst)));
	if (mi == reference_map.end()) {
		//abort();  // not in the map?
		// possible if instructions is annul branch, hence the delay slot is 
		// not profiled, hence no value is recorded in the reference map.
		return 1;
	} else {
		return (*mi).second;
	}
}

void dump_reference_map() {
	map<unsigned, unsigned>::iterator mi;
	for (mi = reference_map.begin(); mi != reference_map.end(); mi++) {	
		cerr << "Instruction " << (*mi).first << " at guest addr " <<
		*(*(list<unsigned>::iterator *)((unsigned*)(*mi).first)) << 
		" , value " << (*mi).second << endl;
	}
}

void indirect_jump(unsigned pc, unsigned indirect_reg) {
	map<unsigned, unsigned>::iterator mi =
		//reference_map.find(pc);
		reference_map.find(*(unsigned*)(&(frag_inst)));
	if (mi == reference_map.end()) {
		dump_reference_map();
		abort();
	} else {
		// generate a compare with the profiled valued
		int scratch = 6; 
		decode_sethi((unsigned)((*mi).second), scratch);
		ADD(scratch, imode((unsigned)((*mi).second) & 0x000003ff), scratch); 

		// rd %ccr, %g7 		
		// emitm(2 << 30 | 40 << 19 | 2 << 14 | (7 & 0x1f) << 25, 4); 

		// cmp(indirect_reg, rmode(scratch));
		// ** it changes condition code..  
		SUB(indirect_reg, rmode(scratch), scratch);
				
		RAddr raddr;

			outportal_struct o;
			o.edge = pair<unsigned, unsigned>(pc, -((*mi).second));
			o.exit_type = 1;
			o.offset = cur_pc();
			// create label
			char guest_addr[12];
			sprintf(guest_addr, "%X", -((*mi).second));	
			raddr = o.raddr = 
				addr_new(label_new(guest_addr), 0);
			out_portal.push_front(o);		

		// BNE(raddr);
		// do a branch on integer register != 0
		BRNZ(0, scratch, raddr);
 	}
}
		
unsigned delay_slot_relocated = 0;

int relocate_delay_slot(unsigned delay_slot_addr) {
/*	map<unsigned, unsigned>::iterator mi = reference_map.find(delay_slot_addr);
	if (mi == reference_map.end()) {
		// not a load or store
		return 0;
	} else {
		if ((*mi).second)
			return 1;
		else 
			return 0;
	}
*/
return 1;
}

void link_portal(pair<unsigned,unsigned> edge) {

/*	#ifdef FRAG_LINK
	 	// check to see if target is in the fragment cache
		map<pair<unsigned,unsigned>, Fragment>::iterator mi = 
			entry_points.find(edge);
 		if (mi == entry_points.end()) {				
			
		} else {
					jump_to_fragment((*mi).second.host_address);
				}
*/		
}		
 				
RAddr in_TLB(unsigned from, unsigned to) {
	map<pair<unsigned,unsigned>, Fragment>::iterator mi = 
		entry_points.find(pair<unsigned,unsigned>(from, to));
 	if (mi == entry_points.end()) {				
		return 0;
	} else {
		return (*mi).second.raddr;
	}
}

int end_call(unsigned reloc, unsigned pc) {
	RAddr raddr;
	if (pc == back_branch) {
		// generate the call if the current instruction is last inst
		map<unsigned, RAddr>::iterator mi = guest_host.find(reloc);
		if (mi == guest_host.end()) {
			// jumping out of the fragment
			raddr = in_TLB(pc, reloc);
			if (!raddr) {
				outportal_struct o;
				o.edge = pair<unsigned, unsigned>(pc, reloc);
				o.exit_type = 2;
				o.offset = cur_pc();
				// create label
				char guest_addr[12];
				sprintf(guest_addr, "%X", reloc);	
				raddr = o.raddr = addr_new(label_new(guest_addr), 0);
				out_portal.push_front(o);		
			}
		} else {
			// back to the beginning of the trace	
			raddr = (*mi).second;
		}
		BA(raddr);
		return 1;
	} else
		return 0;
}

void do_indirect_portal(unsigned pc) {
	// outportal code
	SAVE(14, imode(-128), 14);		
    emitm(1 << 30 | 2 & 0x3fffffff, 4);   // call fallthrough
	fNOP();
	// setup the call to jump back to the emulator
	// setup parameter addr of jump_out_fragment
	int o0 = 8;					// o0 is register number 8
	int o2 = 10;				// o2 is register number 10
	// indirect exit
	mov(imode(1), o2); 
	// rd %ccr, %o1
	emitm(2 << 30 | 40 << 19 | 2 << 14 | (9 & 0x1f) << 25, 4);
	// register value in %g5
	mov(rmode(5), o0);
	fNOP();
	decode_sethi(pc, 11);
	ADD(11, imode((pc) & 0x000003ff), 11); 		
	decode_sethi((unsigned)jump_out_fragment, 6);	
	JMPL(dispA(6, ((unsigned)jump_out_fragment & 0x000003ff)), 0);  
	fNOP();	
}
