/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// File: context_switch.cc
// Desc: defines 2 functions
//		 - Jump to fragment and Jump out fragment
//
// Updates:
// 04 Aug 01 - David: initial version

int debug_frag = 0;


// ***************************************************************************
// Jump Out Fragment
// ***************************************************************************

void jump_out_fragment(unsigned to, unsigned icc, unsigned npc, unsigned from) {
	// context switch ***** pre - previous window *****  yes, that right!
	// write contents of real registers back to virtual registers
	// hmmmmm.  edges obtains by (from,to) may not be exactly what they are, to
	// is always correct, but from could actually be different as each out 
	// portal is only unique on the target, so multiple branches with the same 
	// target will use the same out portal.  Fix to get better performance!!

	extern RegisterFile regs;
	register unsigned g6 __asm__ ("g6");	
	register int* g5 __asm__ ("g5");	
	host_context.temp[0] = to;
	host_context.temp[1] = host_context.temp[0] + 4;	

	// test in out code when icc is equal to -1
	// see outportal of the code generator how this is done
	if (icc == -1) {	
		host_context.temp[1] = npc;	
		g5 = regs.rd;
		// %g registers
		__asm__ ("st %%g1, %0" : : "m" (regs.rd[1])); 
		__asm__ ("st %%g2, %0" : : "m" (regs.rd[2])); 
		__asm__ ("st %%g3, %0" : : "m" (regs.rd[3])); 
		__asm__ ("st %%g4, %0" : : "m" (regs.rd[4])); 

		unsigned fsr;
		__asm__ ("st %%fsr, %0" : : "m" (fsr)); 
		switch (unsigned char fcc = (fsr & 0x00000c00) >> 10) {
		case 0:
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FZF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FLF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FGF)); 
			break;
		case 1:
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FZF)); 
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FLF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FGF)); 
			break;
		case 2:
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FZF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FLF)); 
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FGF)); 
			break;
		default:
			abort();
		}

		__asm__ ("restore"); 
		__asm__ ("restore"); 
		__asm__ ("flushw"); 

		__asm__ ("st %o0, [%g5 + 32]"); 
		__asm__ ("st %o1, [%g5 + 36]"); 
		__asm__ ("st %o2, [%g5 + 40]"); 
		__asm__ ("st %o3, [%g5 + 44]"); 
		__asm__ ("st %o4, [%g5 + 48]"); 
		__asm__ ("st %o5, [%g5 + 52]"); 
		__asm__ ("swap [%g5+56], %o6"); 
		__asm__ ("swap [%g5+60], %o7"); 
		
		__asm__ ("st %l0, [%g5 + 64]"); 
		__asm__ ("st %l1, [%g5 + 68]"); 
		__asm__ ("st %l2, [%g5 + 72]"); 
		__asm__ ("st %l3, [%g5 + 76]"); 
		__asm__ ("st %l4, [%g5 + 80]"); 
		__asm__ ("st %l5, [%g5 + 84]"); 
		__asm__ ("st %l6, [%g5 + 88]"); 
		__asm__ ("st %l7, [%g5 + 92]"); 
	
		__asm__ ("st %i0, [%g5 + 96]"); 
		__asm__ ("st %i1, [%g5 + 100]"); 
		__asm__ ("st %i2, [%g5 + 104]"); 
		__asm__ ("st %i3, [%g5 + 108]"); 
		__asm__ ("st %i4, [%g5 + 112]"); 
		__asm__ ("st %i5, [%g5 + 116]");
		__asm__ ("swap [%g5+120], %i6"); 
		__asm__ ("swap [%g5+124], %i7"); 
		
		__asm__ ("st %%f0, %0" : : "m" (*regs_fds[0])); 
		__asm__ ("st %%f1, %0" : : "m" (*regs_fds[1])); 
		__asm__ ("st %%f2, %0" : : "m" (*regs_fds[2])); 
		__asm__ ("st %%f3, %0" : : "m" (*regs_fds[3])); 
		__asm__ ("st %%f4, %0" : : "m" (*regs_fds[4])); 
		__asm__ ("st %%f5, %0" : : "m" (*regs_fds[5])); 
		__asm__ ("st %%f6, %0" : : "m" (*regs_fds[6])); 
		__asm__ ("st %%f7, %0" : : "m" (*regs_fds[7])); 
		__asm__ ("st %%f8, %0" : : "m" (*regs_fds[8])); 
		__asm__ ("st %%f9, %0" : : "m" (*regs_fds[9])); 
		__asm__ ("st %%f10, %0" : : "m" (*regs_fds[10])); 
		__asm__ ("st %%f11, %0" : : "m" (*regs_fds[11])); 
		__asm__ ("st %%f12, %0" : : "m" (*regs_fds[12])); 
		__asm__ ("st %%f13, %0" : : "m" (*regs_fds[13])); 
		__asm__ ("st %%f14, %0" : : "m" (*regs_fds[14])); 
		__asm__ ("st %%f15, %0" : : "m" (*regs_fds[15])); 
		__asm__ ("st %%f16, %0" : : "m" (*regs_fds[16])); 
		__asm__ ("st %%f17, %0" : : "m" (*regs_fds[17])); 
		__asm__ ("st %%f18, %0" : : "m" (*regs_fds[18])); 
		__asm__ ("st %%f19, %0" : : "m" (*regs_fds[19])); 
		__asm__ ("st %%f20, %0" : : "m" (*regs_fds[20])); 
		__asm__ ("st %%f21, %0" : : "m" (*regs_fds[21])); 
		__asm__ ("st %%f22, %0" : : "m" (*regs_fds[22])); 
		__asm__ ("st %%f23, %0" : : "m" (*regs_fds[23])); 
		__asm__ ("st %%f24, %0" : : "m" (*regs_fds[24])); 
		__asm__ ("st %%f25, %0" : : "m" (*regs_fds[25])); 
		__asm__ ("st %%f26, %0" : : "m" (*regs_fds[26])); 
		__asm__ ("st %%f27, %0" : : "m" (*regs_fds[27])); 
		__asm__ ("st %%f28, %0" : : "m" (*regs_fds[28])); 
		__asm__ ("st %%f29, %0" : : "m" (*regs_fds[29])); 
		__asm__ ("st %%f30, %0" : : "m" (*regs_fds[30])); 
		__asm__ ("st %%f31, %0" : : "m" (*regs_fds[31])); 
		
		goto SET_PC;
	}
				
	// %g registers
	// restoring of %g registers must be done first before the checks for 
	// fragment linking.  libraries (eg map) can change those damn %g registers
	__asm__ ("st %%g1, %0" : : "m" (regs.rd[1])); 
	__asm__ ("st %%g2, %0" : : "m" (regs.rd[2])); 
	__asm__ ("st %%g3, %0" : : "m" (regs.rd[3])); 
	__asm__ ("st %%g4, %0" : : "m" (regs.rd[4])); 
//	__asm__ ("st %%g5, %0" : : "m" (regs.rd[5])); 
//	__asm__ ("st %%g6, %0" : : "m" (regs.rd[6])); 
//	__asm__ ("st %%g7, %0" : : "m" (regs.rd[7])); 

	//__asm__ ("rd %%y, %0" : : "r" (regs.r_Y)); 
	__asm__ ("rd %%y, %0" : "=r" (regs.r_Y) : ); 
	unsigned fsr;
	__asm__ ("st %%fsr, %0" : : "m" (fsr)); 
	
	// check to see if target is in the fragment cache
	{
		pair<unsigned,unsigned> edge = pair<unsigned,unsigned>(from, to);
		map<pair<unsigned,unsigned>, Fragment>::iterator mi = 
			entry_points.find(edge);
	 	if (mi == entry_points.end()) {	
			#ifdef CONSIDER_EXITS
			if (npc == 2)
				// special treatment if exit is a call
				increment_counter(from, to, 2);
			else 
				increment_counter(from, to, 1);
			#endif
		} else {
			host_context.temp[3] = (unsigned)((*mi).second.host_address);
			// if it is an indirect jump, don't even think about fragment 
			// linking.
			switch (npc) {
			case 0:
			case 2:
			case 3:
				// Fragment Linking
				#ifdef FRAG_LINK
				if (!debug_frag) {							
					// Half linking - useful if we want to instrument portals
					/*
					// damn, already got its fragment, so just go there
					// but skip the delay slot instruction 
					g6 = host_context.temp[3];
					__asm__ ("wr %0, 0, %%ccr" : : "r" (icc)); 	
					__asm__ ("restore"); 
					__asm__ ("restore"); 
					__asm__ ("jmpl %g6, %g0"); 
					__asm__ ("nop"); 
					*/				
				
					register unsigned i7 __asm__ ("i7");	
					unsigned* start = (unsigned*)(i7 - 4);
					int scratch = 6;
					
					#ifndef CONSV_LINK
					unsigned exit_inst_off = *((unsigned*)(i7 + 8));
					// check to see if a branch can get there, if not
					// have to use a jump and link instruction.
					unsigned exit_inst_p = (unsigned)start - exit_inst_off;
					unsigned exit_inst = *((unsigned*)exit_inst_p);
					
					if (npc == 0) {
						// overwrite field disp19, v9 allows only 19 bits for
						// address with prediction
						#ifdef V9_CODEGEN
    		 			*((unsigned*)exit_inst_p) = (exit_inst & 0xfff80000 |
							(int)((unsigned)host_context.temp[3] - exit_inst_p) 
							>> 2 & 0x7ffff); 
						#else
     		 			*((unsigned*)exit_inst_p) = (exit_inst & 0xffc00000 |
							(int)((unsigned)host_context.temp[3] - exit_inst_p) 
							>> 2 & 0x3fffff); 
						#endif
               		__asm__ ("flush %0" : : "r" ((unsigned)(exit_inst_p)));
					}
					
					#endif
					
					// Full linking
					// check to see if a branch can get there, if not
					// have to use a jump and link instruction.
	 				/*
					// sethi
    				*(start) = 
						(0 << 30 | 4 << 22 | (scratch & 0x1f) << 25 | 
						(unsigned)(host_context.temp[3]) >> 10 & 0x3fffff); 
					// jmpl
    	 			*(start + 1) = 
        				(2 << 30 | 56 << 19 | 1 << 13 | scratch << 14 | 
          				(unsigned)(host_context.temp[3]) & 0x3ff | (scratch & 0x1f) << 25); 
					// nop
    				*(start + 2) = 
        	        	(0 << 30 | 4 << 22 | 0 << 25);
					*/
					// BA	
    	 			*(start) = 
     					(0 << 30 | 2 << 22 | 8 << 25 | 0 << 29 |
						(int)((unsigned)host_context.temp[3] - (unsigned)start) 
						>> 2 & 0x3fffff); 
	 				// nop
    				*(start + 1) = 
        	        	(0 << 30 | 4 << 22 | 0 << 25);

            	    __asm__ ("flush %0" : : "r" ((unsigned)(start - 2)));
                	__asm__ ("flush %0" : : "r" ((unsigned)(start)));
	                __asm__ ("flush %0" : : "r" ((unsigned)(start + 2)));
					
					__asm__ ("wr %0, 0, %%ccr" : : "r" (icc)); 	
					g6 = host_context.temp[3];
					__asm__ ("restore"); 
					__asm__ ("jmpl %g6, %g0"); 
					__asm__ ("restore");
				} 						
				#else
					// do the restoring
					;
				#endif
				break;
			case 1:
					// do half link, or better, a switch table search, 
					// but no space though!!!
					// for now, do half link
				
					// Half linking - useful if we want to instrument portals
					// already got its fragment, so just go there
					// but skip the delay slot instruction 
					__asm__ ("wr %0, 0, %%ccr" : : "r" (icc)); 	
					g6 = host_context.temp[3];
					__asm__ ("restore"); 
					__asm__ ("jmpl %g6, %g0"); 
					__asm__ ("restore"); 
					break;
			default:
				cerr << "Unknown exit type.." << endl;
				abort();
			}
		}
	}
	
	g5 = regs.rd;
	
	// restore integer condition codes
	__asm__ ("stb %0, %1" : : "g" ((icc & 0x00000001)), "m" (regs.r_CF)); 
	__asm__ ("stb %0, %1" : : "g" ((icc & 0x00000002)>>1), "m" (regs.r_OF)); 
	__asm__ ("stb %0, %1" : : "g" ((icc & 0x00000004)>>2), "m" (regs.r_ZF)); 
	__asm__ ("stb %0, %1" : : "g" ((icc & 0x00000008)>>3), "m" (regs.r_NF)); 

	//	cerr << "Out: combined = " <<
	//		((regs.r_CF | (regs.r_OF<<1) | (regs.r_ZF<<2) | (regs.r_NF<<3)) |
	//		(regs.r_CF<<4) | (regs.r_OF<<5) | (regs.r_ZF<<6) | (regs.r_NF<<7) & 0x000000FF)
	//		<< endl;
	
	// restore floating point condition codes
	//	cerr << "Out: %fsr = " << hex << fsr << endl;
	//if (host_context.fsr_tick == fsr) {
		// nothing is changed for the fsr
	//	;
	//} else {
			
		switch (unsigned char fcc = (fsr & 0x00000c00) >> 10) {
		case 0:
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FZF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FLF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FGF)); 
			break;
		case 1:
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FZF)); 
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FLF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FGF)); 
			break;
		case 2:
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FZF)); 
			__asm__ ("stb %%g0, %0" : : "m" (regs.r_FLF)); 
			__asm__ ("stb %0, %1" : : "r" (1), "m" (regs.r_FGF)); 
			break;
		default:
			abort();
		}

	regs.r_FSR = fsr;	
	//	__asm__ ("wr %0, 0, %%y" : : "r" (host_context.y)); 	
	//	__asm__ ("ld %0, %%fsr" : : "m" (host_context.fsr)); 

	// go back to the stub frame of the fallthrough
	__asm__ ("restore"); 

	// go back to the frame of the fragment
	__asm__ ("restore"); 
	
	__asm__ ("flushw"); 

	// write out the %o, %l and %i registers 
	__asm__ ("st %o0, [%g5 + 32]"); 
	__asm__ ("st %o1, [%g5 + 36]"); 
	__asm__ ("st %o2, [%g5 + 40]"); 
	__asm__ ("st %o3, [%g5 + 44]"); 
	__asm__ ("st %o4, [%g5 + 48]"); 
	__asm__ ("st %o5, [%g5 + 52]"); 

	// make %o6 back to guest address mode
	//	__asm__ ("sub %o6, &mem, %g3");
	//	__asm__ ("st %g3, [%g5 + 56]"); 
	// restore emulator stack
//	__asm__ ("st %o6, [%g5 + 56]"); 
//	__asm__ ("ld [%g2], %o6"); 
//	__asm__ ("swap [%g5+56], %o6"); 
		
	// commenting these out allows GDB to give you a call stack
	// program possibly won't work if it writes to %i7 or %o7
//	__asm__ ("st %o7, [%g5 + 60]"); 
//	__asm__ ("ld [%g2+4], %o7"); 
	__asm__ ("swap [%g5+60], %o7"); 
		
	__asm__ ("st %l0, [%g5 + 64]"); 
	__asm__ ("st %l1, [%g5 + 68]"); 
	__asm__ ("st %l2, [%g5 + 72]"); 
	__asm__ ("st %l3, [%g5 + 76]"); 
	__asm__ ("st %l4, [%g5 + 80]"); 
	__asm__ ("st %l5, [%g5 + 84]"); 
	__asm__ ("st %l6, [%g5 + 88]"); 
	__asm__ ("st %l7, [%g5 + 92]"); 
	
	__asm__ ("st %i0, [%g5 + 96]"); 
	__asm__ ("st %i1, [%g5 + 100]"); 
	__asm__ ("st %i2, [%g5 + 104]"); 
	__asm__ ("st %i3, [%g5 + 108]"); 
	__asm__ ("st %i4, [%g5 + 112]"); 
	__asm__ ("st %i5, [%g5 + 116]");

	//	__asm__ ("sub %i6, &mem, %g3");
	//	__asm__ ("st %g3, [%g5 + 120]"); 
	// restore emulator stack
//	__asm__ ("st %i6, [%g5 + 120]"); 
//	__asm__ ("ld [%g2+8], %i6"); 
	__asm__ ("swap [%g5+120], %i6"); 
	
	// commenting these out allows GDB to give you a call stack
	// program possibly won't work if it writes to %i7 or %o7
//	__asm__ ("st %i7, [%g5 + 124]"); 
//	__asm__ ("ld [%g2+12], %i7"); 
	__asm__ ("swap [%g5+124], %i7"); 

	__asm__ ("swap [%g5+56], %o6"); 

	// floating point registers
	__asm__ ("st %%f0, %0" : : "m" (*regs_fds[0])); 
	__asm__ ("st %%f1, %0" : : "m" (*regs_fds[1])); 
	__asm__ ("st %%f2, %0" : : "m" (*regs_fds[2])); 
	__asm__ ("st %%f3, %0" : : "m" (*regs_fds[3])); 
	__asm__ ("st %%f4, %0" : : "m" (*regs_fds[4])); 
	__asm__ ("st %%f5, %0" : : "m" (*regs_fds[5])); 
	__asm__ ("st %%f6, %0" : : "m" (*regs_fds[6])); 
	__asm__ ("st %%f7, %0" : : "m" (*regs_fds[7])); 
	__asm__ ("st %%f8, %0" : : "m" (*regs_fds[8])); 
	__asm__ ("st %%f9, %0" : : "m" (*regs_fds[9])); 
	__asm__ ("st %%f10, %0" : : "m" (*regs_fds[10])); 
	__asm__ ("st %%f11, %0" : : "m" (*regs_fds[11])); 
	__asm__ ("st %%f12, %0" : : "m" (*regs_fds[12])); 
	__asm__ ("st %%f13, %0" : : "m" (*regs_fds[13])); 
	__asm__ ("st %%f14, %0" : : "m" (*regs_fds[14])); 
	__asm__ ("st %%f15, %0" : : "m" (*regs_fds[15])); 
	__asm__ ("st %%f16, %0" : : "m" (*regs_fds[16])); 
	__asm__ ("st %%f17, %0" : : "m" (*regs_fds[17])); 
	__asm__ ("st %%f18, %0" : : "m" (*regs_fds[18])); 
	__asm__ ("st %%f19, %0" : : "m" (*regs_fds[19])); 
	__asm__ ("st %%f20, %0" : : "m" (*regs_fds[20])); 
	__asm__ ("st %%f21, %0" : : "m" (*regs_fds[21])); 
	__asm__ ("st %%f22, %0" : : "m" (*regs_fds[22])); 
	__asm__ ("st %%f23, %0" : : "m" (*regs_fds[23])); 
	__asm__ ("st %%f24, %0" : : "m" (*regs_fds[24])); 
	__asm__ ("st %%f25, %0" : : "m" (*regs_fds[25])); 
	__asm__ ("st %%f26, %0" : : "m" (*regs_fds[26])); 
	__asm__ ("st %%f27, %0" : : "m" (*regs_fds[27])); 
	__asm__ ("st %%f28, %0" : : "m" (*regs_fds[28])); 
	__asm__ ("st %%f29, %0" : : "m" (*regs_fds[29])); 
	__asm__ ("st %%f30, %0" : : "m" (*regs_fds[30])); 
	__asm__ ("st %%f31, %0" : : "m" (*regs_fds[31])); 
	
SET_PC:
	// modify the virtual PCs
	regs.r_pc = host_context.temp[0];
	regs.r_npc = host_context.temp[1];
	regs.r_CTI = 1;
	__asm__ ("wr %0, 0, %%ccr" : : "r" (host_context.ccr)); 	
	
	// finally, call the main loop!!!
	// run();  no!!  can't use a call, it over writes %o7 !!!!!!
	
/*	g5 = (int*)run;
	// jump to it
	__asm__ ("jmpl %g5, %g0");	
	__asm__ ("nop");		// never return
*/
	
}

extern sint32_t execute(sint32_t pc);
int unconditional_annul(sint32_t pc);


// ***************************************************************************
// Jump Out Fragment
// ***************************************************************************

// parameter is a host address!!!
// mode = 0 if in emulator mode, mode = 1 if it is a fragment exit
void jump_to_fragment(unsigned *addr, unsigned mode = 0) {

	host_context.cur_fragment = addr;
	if (mode == 0) {
		if (!unconditional_annul(regs.r_pc)) {
			// do delay slot 
  			regs.rd[0] = 0;
			regs.r_CTI = 0;
			execute(regs.r_pc);
		}
	}

	register unsigned g6 __asm__ ("g6");
	register int* g5 __asm__ ("g5");	
	__asm__ ("rd %%ccr, %0" : : "r" (host_context.ccr)); 	

	if (test_in_out_code) {			
		g5 = regs.rd;

		if ((regs.r_FZF + regs.r_FLF + regs.r_FGF) > 1) {
			cerr << "Floating point condition code error" << endl;
			abort();
		}
		unsigned fsr;
		__asm__ ("st %%fsr, %0" : : "m" (fsr)); // use host machine's fsr keep exception
											// fields aexc and cexc
		unsigned char fcc = (fsr & 0x00000c00) >> 10;	
		if (fcc == 3) abort();  // err, don't know what to do.
		if (regs.r_FZF) {
			fsr = (fsr & 0xfffff3ff);
		}
		if (regs.r_FLF) {
			fsr = (fsr & 0xfffff3ff) | 0x00000400;
		}
		if (regs.r_FGF) {
			fsr = (fsr & 0xfffff3ff) | 0x00000800;
		}
		__asm__ ("ld %0, %%fsr" : : "m" (fsr));			
		g6 = (unsigned)
			((regs.r_CF | (regs.r_OF<<1) | (regs.r_ZF<<2) | (regs.r_NF<<3)) & 0x0000000f);
		__asm__ ("wr %0, 0, %%ccr" : : "r" (g6)); 		

		g6 = (unsigned)addr;	
		__asm__ ("restore");
		__asm__ ("flushw"); 
	
		__asm__ ("ld %0, %%f0" : : "m" (*regs_fds[0])); 
		__asm__ ("ld %0, %%f1" : : "m" (*regs_fds[1])); 
		__asm__ ("ld %0, %%f2" : : "m" (*regs_fds[2])); 
		__asm__ ("ld %0, %%f3" : : "m" (*regs_fds[3])); 
		__asm__ ("ld %0, %%f4" : : "m" (*regs_fds[4])); 
		__asm__ ("ld %0, %%f5" : : "m" (*regs_fds[5])); 
		__asm__ ("ld %0, %%f6" : : "m" (*regs_fds[6])); 
		__asm__ ("ld %0, %%f7" : : "m" (*regs_fds[7])); 
		__asm__ ("ld %0, %%f8" : : "m" (*regs_fds[8])); 
		__asm__ ("ld %0, %%f9" : : "m" (*regs_fds[9])); 
		__asm__ ("ld %0, %%f10" : : "m" (*regs_fds[10])); 
		__asm__ ("ld %0, %%f11" : : "m" (*regs_fds[11])); 
		__asm__ ("ld %0, %%f12" : : "m" (*regs_fds[12])); 
		__asm__ ("ld %0, %%f13" : : "m" (*regs_fds[13])); 
		__asm__ ("ld %0, %%f14" : : "m" (*regs_fds[14])); 
		__asm__ ("ld %0, %%f15" : : "m" (*regs_fds[15])); 
		__asm__ ("ld %0, %%f16" : : "m" (*regs_fds[16])); 
		__asm__ ("ld %0, %%f17" : : "m" (*regs_fds[17])); 
		__asm__ ("ld %0, %%f18" : : "m" (*regs_fds[18])); 
		__asm__ ("ld %0, %%f19" : : "m" (*regs_fds[19])); 
		__asm__ ("ld %0, %%f20" : : "m" (*regs_fds[20])); 
		__asm__ ("ld %0, %%f21" : : "m" (*regs_fds[21])); 
		__asm__ ("ld %0, %%f22" : : "m" (*regs_fds[22])); 
		__asm__ ("ld %0, %%f23" : : "m" (*regs_fds[23])); 
		__asm__ ("ld %0, %%f24" : : "m" (*regs_fds[24])); 
		__asm__ ("ld %0, %%f25" : : "m" (*regs_fds[25])); 
		__asm__ ("ld %0, %%f26" : : "m" (*regs_fds[26])); 
		__asm__ ("ld %0, %%f27" : : "m" (*regs_fds[27])); 
		__asm__ ("ld %0, %%f28" : : "m" (*regs_fds[28])); 
		__asm__ ("ld %0, %%f29" : : "m" (*regs_fds[29])); 
		__asm__ ("ld %0, %%f30" : : "m" (*regs_fds[30])); 
		__asm__ ("ld %0, %%f31" : : "m" (*regs_fds[31])); 

		__asm__ ("ld [%g5 +  4], %g1");
		__asm__ ("ld [%g5 +  8], %g2");
		__asm__ ("ld [%g5 +  12], %g3");
		__asm__ ("ld [%g5 +  16], %g4");
		
		__asm__ ("ld [%g5 + 64], %l0");
		__asm__ ("ld [%g5 + 68], %l1");
		__asm__ ("ld [%g5 + 72], %l2");
		__asm__ ("ld [%g5 + 76], %l3");
		__asm__ ("ld [%g5 + 80], %l4");
		__asm__ ("ld [%g5 + 84], %l5");
		__asm__ ("ld [%g5 + 88], %l6");
		__asm__ ("ld [%g5 + 92], %l7");

		__asm__ ("ld [%g5 +  96], %i0");
		__asm__ ("ld [%g5 + 100], %i1");
		__asm__ ("ld [%g5 + 104], %i2");
		__asm__ ("ld [%g5 + 108], %i3");
		__asm__ ("ld [%g5 + 112], %i4");
		__asm__ ("ld [%g5 + 116], %i5");
		//__asm__ ("swap [%g5 + 120], %i6");
		__asm__ ("swap [%g5 + 124], %i7");
		
		__asm__ ("ld [%g5 + 32], %o0");
		__asm__ ("ld [%g5 + 36], %o1");
		__asm__ ("ld [%g5 + 40], %o2");
		__asm__ ("ld [%g5 + 44], %o3");
		__asm__ ("ld [%g5 + 48], %o4");
		__asm__ ("ld [%g5 + 52], %o5");
		__asm__ ("swap [%g5 + 56], %o6");	
		__asm__ ("swap [%g5 + 60], %o7");	
		__asm__ ("swap [%g5 + 120], %i6");
		
		__asm__ ("jmpl %g6, %g0");	
		__asm__ ("nop"); 	
	}

	g5 = regs.rd;	
	
	//__asm__ ("st %%fsr, %0" : : "m" (host_context.fsr)); 
	
	// copy floating condition codes
	if ((regs.r_FZF + regs.r_FLF + regs.r_FGF) > 1) {
		cerr << "Floating point condition code error" << endl;
		abort();
	}

	//unsigned fsr = regs.r_FSR;
	unsigned fsr;
	__asm__ ("st %%fsr, %0" : : "m" (fsr)); // use host machine's fsr keep exception
											// fields aexc and cexc
//	cerr << "In: %fsr' = " << hex << fsr << endl;
	unsigned char fcc = (fsr & 0x00000c00) >> 10;	
	if (fcc == 3) abort();  // err, don't know what to do.
	if (regs.r_FZF) {
		fsr = (fsr & 0xfffff3ff);
	}
	if (regs.r_FLF) {
		fsr = (fsr & 0xfffff3ff) | 0x00000400;
	}
	if (regs.r_FGF) {
		fsr = (fsr & 0xfffff3ff) | 0x00000800;
	}
	__asm__ ("ld %0, %%fsr" : : "m" (fsr));	
		
//	host_context.fsr_tick = fsr;
//	cerr << "In: %fsr = " << hex << fsr << endl;

	//__asm__ ("rd %%y, %0" : : "r" (host_context.y)); 
	
	// copy condition codes
	g6 = (unsigned)
		((regs.r_CF | (regs.r_OF<<1) | (regs.r_ZF<<2) | (regs.r_NF<<3)) & 0x0000000f);
//		((regs.r_CF | (regs.r_OF<<1) | (regs.r_ZF<<2) | (regs.r_NF<<3)) |
//		(regs.r_CF<<4) | (regs.r_OF<<5) | (regs.r_ZF<<6) | (regs.r_NF<<7) & 0x000000ff);
	__asm__ ("wr %0, 0, %%ccr" : : "r" (g6)); 	
//	cerr << "In: %ccr = " << hex << g6 << endl;
	__asm__ ("wr %0, 0, %%y" : : "r" (regs.r_Y)); 	
	
	g6 = (unsigned)addr;	// copy %i0 register before the restores

	// restores the call stack
/*	__asm__ ("restore");		// jump_to_fragment
	__asm__ ("restore");		// executeCTI
	__asm__ ("restore");		// execute()
	__asm__ ("restore");		// executeOneInstruction()
	__asm__ ("restore");		// run()
*/
	__asm__ ("restore"); 
	// flush all windows to the emulator's context
	// crucial! as you want all emulator context spilled before entering gcode
	__asm__ ("flushw"); 
	
	// scratch registers
	//register int *o1 __asm__ ("o1");
	//register int o2 __asm__ ("o2");
	//register unsigned o4 __asm__ ("o4");

	// %o0 is the parameter	
	//o1 = regs.rd;
	//o2 = (int)&host_context;
	//o4 = g6; 			// copy again
	
	// copy context of the emulator to the real machine.
	// current context are all the registers values

	// floating point registers
	__asm__ ("ld %0, %%f0" : : "m" (*regs_fds[0])); 
	__asm__ ("ld %0, %%f1" : : "m" (*regs_fds[1])); 
	__asm__ ("ld %0, %%f2" : : "m" (*regs_fds[2])); 
	__asm__ ("ld %0, %%f3" : : "m" (*regs_fds[3])); 
	__asm__ ("ld %0, %%f4" : : "m" (*regs_fds[4])); 
	__asm__ ("ld %0, %%f5" : : "m" (*regs_fds[5])); 
	__asm__ ("ld %0, %%f6" : : "m" (*regs_fds[6])); 
	__asm__ ("ld %0, %%f7" : : "m" (*regs_fds[7])); 
	__asm__ ("ld %0, %%f8" : : "m" (*regs_fds[8])); 
	__asm__ ("ld %0, %%f9" : : "m" (*regs_fds[9])); 
	__asm__ ("ld %0, %%f10" : : "m" (*regs_fds[10])); 
	__asm__ ("ld %0, %%f11" : : "m" (*regs_fds[11])); 
	__asm__ ("ld %0, %%f12" : : "m" (*regs_fds[12])); 
	__asm__ ("ld %0, %%f13" : : "m" (*regs_fds[13])); 
	__asm__ ("ld %0, %%f14" : : "m" (*regs_fds[14])); 
	__asm__ ("ld %0, %%f15" : : "m" (*regs_fds[15])); 
	__asm__ ("ld %0, %%f16" : : "m" (*regs_fds[16])); 
	__asm__ ("ld %0, %%f17" : : "m" (*regs_fds[17])); 
	__asm__ ("ld %0, %%f18" : : "m" (*regs_fds[18])); 
	__asm__ ("ld %0, %%f19" : : "m" (*regs_fds[19])); 
	__asm__ ("ld %0, %%f20" : : "m" (*regs_fds[20])); 
	__asm__ ("ld %0, %%f21" : : "m" (*regs_fds[21])); 
	__asm__ ("ld %0, %%f22" : : "m" (*regs_fds[22])); 
	__asm__ ("ld %0, %%f23" : : "m" (*regs_fds[23])); 
	__asm__ ("ld %0, %%f24" : : "m" (*regs_fds[24])); 
	__asm__ ("ld %0, %%f25" : : "m" (*regs_fds[25])); 
	__asm__ ("ld %0, %%f26" : : "m" (*regs_fds[26])); 
	__asm__ ("ld %0, %%f27" : : "m" (*regs_fds[27])); 
	__asm__ ("ld %0, %%f28" : : "m" (*regs_fds[28])); 
	__asm__ ("ld %0, %%f29" : : "m" (*regs_fds[29])); 
	__asm__ ("ld %0, %%f30" : : "m" (*regs_fds[30])); 
	__asm__ ("ld %0, %%f31" : : "m" (*regs_fds[31])); 
	
	// %g, %i and %l interger registers
	__asm__ ("ld [%g5 +  4], %g1");
	__asm__ ("ld [%g5 +  8], %g2");
	__asm__ ("ld [%g5 +  12], %g3");
	__asm__ ("ld [%g5 +  16], %g4");
//	__asm__ ("ld [% +  20], %g5");
//	__asm__ ("ld [% +  24], %g6");
//	__asm__ ("ld [% +  28], %g7");

	__asm__ ("swap [%g5 + 56], %o6");	
	
	__asm__ ("ld [%g5 +  64], %l0");
	__asm__ ("ld [%g5 +  68], %l1");
	__asm__ ("ld [%g5 +  72], %l2");
	__asm__ ("ld [%g5 +  76], %l3");
	__asm__ ("ld [%g5 +  80], %l4");
	__asm__ ("ld [%g5 +  84], %l5");
	__asm__ ("ld [%g5 +  88], %l6");
	__asm__ ("ld [%g5 +  92], %l7");

	__asm__ ("ld [%g5 +  96], %i0");
	__asm__ ("ld [%g5 + 100], %i1");
	__asm__ ("ld [%g5 + 104], %i2");
	__asm__ ("ld [%g5 + 108], %i3");
	__asm__ ("ld [%g5 + 112], %i4");
	__asm__ ("ld [%g5 + 116], %i5");
	
	// frame pointer and return address is overwritten from this point	
	// save value of emulator stack
//	__asm__ ("st %i6, [%o2+8]");	
//	__asm__ ("ld [%o1 + 120], %i6");
	__asm__ ("swap [%g5 + 120], %i6");
	//	__asm__ ("ld [%o1 + 120], %o5");
	//	__asm__ ("add %o5, &mem, %i6");
	
	// commenting these out allows GDB to give you a call stack
	// program possibly won't work if it writes to %i7 or %o7
//	__asm__ ("st %i7, [%o2+12]");	
//	__asm__ ("ld [%o1 + 124], %i7");
	__asm__ ("swap [%g5 + 124], %i7");
				
	// store %o register, this assumes a free register is available
	// if no free register, will have to use the save trick below.

	__asm__ ("ld [%g5 + 32], %o0");
	__asm__ ("ld [%g5 + 36], %o1");
	__asm__ ("ld [%g5 + 40], %o2");
	__asm__ ("ld [%g5 + 44], %o3");
	__asm__ ("ld [%g5 + 48], %o4");
	__asm__ ("ld [%g5 + 52], %o5");
	//__asm__ ("swap [%g5 + 56], %o6");	
	__asm__ ("swap [%g5 + 60], %o7");

	// jump to the fragment
	__asm__ ("jmpl %g6, %g0");
	__asm__ ("nop");

/* 		
	// load the virtual %o registers into real %i registers of this frame
	__asm__ ("save %sp, -112, %sp");
	//__asm__ ("mov %i0, %l0");			// now holds the fragment addr
	__asm__ ("mov %i1, %l1");
	__asm__ ("mov %i2, %l2");			// holds the emulator context
	//__asm__ ("mov %i4, %l4");			// now holds the fragment addr

	__asm__ ("ld [%l1 + 32], %i0");
	__asm__ ("ld [%l1 + 36], %i1");
	__asm__ ("ld [%l1 + 40], %i2");
	__asm__ ("ld [%l1 + 44], %i3");
	__asm__ ("ld [%l1 + 48], %i4");
	__asm__ ("ld [%l1 + 52], %i5");
	
	//__asm__ ("swap [%l1 + 32], %i0");
	//__asm__ ("swap [%l1 + 36], %i1");
	//__asm__ ("swap [%l1 + 40], %i2");
	//__asm__ ("swap [%l1 + 44], %i3");
	//__asm__ ("swap [%l1 + 48], %i4");
	//__asm__ ("swap [%l1 + 52], %i5");
	
	// same for stack pointer
//	__asm__ ("st %i6, [%l2]");	
//	__asm__ ("ld [%l1 + 56], %i6");
	__asm__ ("swap [%l1 + 56], %i6");
	//	__asm__ ("ld [%l1 + 56], %l5");
	//	__asm__ ("add %l5, &mem, %i6");
	
	// commenting these out allows GDB to give you a call stack
	// program possibly won't work if it writes to %i7 or %o7
//	__asm__ ("st %i7, [%l2+4]");	
//	__asm__ ("ld [%l1 + 60], %i7");
	__asm__ ("swap [%l1 + 60], %i7");

	__asm__ ("flushw"); 				
	// jump to the fragment
	__asm__ ("jmpl %g6, %g0");
	
	// make %i back to %o in the parent frame
	__asm__ ("restore");		// delay slot
*/

}
