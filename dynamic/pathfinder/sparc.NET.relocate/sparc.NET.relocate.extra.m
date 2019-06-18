/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

#include "pathfinder.cc"

reg_or_imm_Instance encode_roi( sint32_t pc )
{
match pc to
| "imode" (simm13) => return imode(simm13);
| "rmode" (rs2) => return rmode(rs2);
else abort();
endmatch
}


void process_eaddr(sint32_t pc, unsigned scratch) 
{
	match pc to
	| "absoluteA" (simm13) => 
		mov(imode(simm13), scratch);
	| "dispA" (rs1, simm13) => 
		ADD(rs1, imode(simm13), scratch);
	| "indirectA" (rs1) =>
		mov(rmode(rs1), scratch);
	| "indexA" (rs1, rs2) =>
		ADD(rs1, rmode(rs2), scratch);
	else abort();
	endmatch
}

	
address__Instance relocate_eaddr(sint32_t pc, sint32_t scratch)
{

if (delay_slot_relocated) {
	delay_slot_relocated = 0;
	scratch = 6; 
	match pc to
	| "absoluteA" (simm13) => 
		return dispA(scratch, simm13);
	| "dispA" (rs1, simm13) => 
		return dispA(scratch, simm13);
	| "indirectA" (rs1) =>
		return indexA(scratch, rs1);
	| "indexA" (rs1, rs2) =>
		return indexA(scratch, rs2);
	else abort();
	endmatch	
}

// if scratch is %g0, we are in big trouble!
//if (scratch == 0) 
	scratch = 6;   // in cases like ld %o0, %o0!!!!
// store instruction don't have a scratch register!! so they should be 0
	
match pc to
| "absoluteA" (simm13) => 
	if (do_relocate_eaddr(pc)) {
		decode_sethi((unsigned)mem, scratch);
//		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 
		return dispA(scratch, simm13);
	} else {
		abort();
	}
| "dispA" (rs1, simm13) => 
	if (do_relocate_eaddr(pc)) {
		decode_sethi((unsigned)mem, scratch);
//		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 
		ADD(scratch, rmode(rs1), scratch); 	
		return dispA(scratch, simm13);
	} else {
		return dispA(rs1, simm13);		
	} 
| "indirectA" (rs1) =>
	if (do_relocate_eaddr(pc)) {
		decode_sethi((unsigned)mem, scratch);
//		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 
		return indexA(scratch, rs1);
	} else {	
		return indirectA(rs1);
	} 
| "indexA" (rs1, rs2) =>
	if (do_relocate_eaddr(pc)) {
		decode_sethi((unsigned)mem, scratch);
//		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 
		ADD(scratch, rmode(rs1), scratch); 		
		return indexA(scratch, rs2);
	} else {		
		return indexA(rs1, rs2);
	} 
else abort();
endmatch
}



void encode_delay_load_store( sint32_t pc ) 
{
	sint32_t nextPC;
	match [nextPC] pc to
	| "LD" (eaddr, rd) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "LDD" (eaddr, rd) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "LDDF" (eaddr, fdd) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "LDF" (eaddr, fds) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "LDSB" (eaddr, rd) => relocate_eaddr(eaddr,0 ); delay_slot_relocated = 1;
	| "LDSH" (eaddr, rd) => relocate_eaddr(eaddr,0 ); delay_slot_relocated = 1;
	| "LDSTUB" (eaddr, rd) => relocate_eaddr(eaddr,0); delay_slot_relocated = 1;
	| "LDUB" (eaddr, rd) => relocate_eaddr(eaddr,0); delay_slot_relocated = 1;
	| "LDUH" (eaddr, rd) => relocate_eaddr(eaddr,0); delay_slot_relocated = 1;
	| "ST" (rd, eaddr) => relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;
	| "STB" (rd, eaddr) => relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;
	| "STD" (rd, eaddr) => relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;
	| "STDF" (fdd, eaddr) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "STF" (fds, eaddr) => relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;
	| "STH" (rd, eaddr) => relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;
	else 
		// other instructions
		;
	endmatch
}


int encode( sint32_t pc )
{
RAddr raddr;
sint32_t nextPC;
match [nextPC] pc to
| "ADD" (rs1, reg_or_imm, rd) => ADD( rs1, encode_roi( reg_or_imm ), rd );
| "ADDcc" (rs1, reg_or_imm, rd) => ADDcc( rs1, encode_roi( reg_or_imm ), rd );
| "ADDX" (rs1, reg_or_imm, rd) =>  ADDX( rs1, encode_roi( reg_or_imm ), rd );
| "ADDXcc" (rs1, reg_or_imm, rd) => ADDXcc( rs1, encode_roi( reg_or_imm ), rd );
| "AND" (rs1, reg_or_imm, rd) => AND( rs1, encode_roi( reg_or_imm ), rd );
| "ANDcc" (rs1, reg_or_imm, rd) => ANDcc( rs1, encode_roi( reg_or_imm ), rd );
| "ANDN" (rs1, reg_or_imm, rd) => ANDN( rs1, encode_roi( reg_or_imm ), rd );
| "ANDNcc" (rs1, reg_or_imm, rd) => ANDNcc( rs1, encode_roi( reg_or_imm ), rd );
| "BA" (reloc) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	// emliminate this branch, but can't remove this if it is the back branch!!
	if (reloc < pc)
		BA(relocate(reloc, pc));	
//| "BA,a" (reloc) => 
| "BCC" (reloc) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BCS(raddr);
	} else {
		BCC(raddr);		
	}
| "BCC,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BCS(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BCC_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BCS" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BCC(raddr);
	} else {
		BCS(raddr);		
	}
| "BCS,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BCC(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BCS_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BE" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BNE(raddr);
	} else {
		BE(raddr);		
	}
| "BE,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BNE(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BE_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BG" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BLE(raddr);
	} else {
		BG(raddr);		
	}
| "BG,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BLE(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BG_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BGE" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BL(raddr);
	} else {
		BGE(raddr);		
	}
| "BGE,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BL(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BGE_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BGU" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BLEU(raddr);
	} else {
		BGU(raddr);		
	}
| "BGU,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BLEU(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BGU_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BL" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BGE(raddr);
	} else {
		BL(raddr);		
	}
| "BL,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BGE(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BL_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BLE" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BG(raddr);
	} else {
		BLE(raddr);		
	}
| "BLE,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BG(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BLE_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BLEU" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BGU(raddr);
	} else {
		BLEU(raddr);		
	}
| "BLEU,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BGU(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BLEU_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
//| "BN" (reloc) => 	
//| "BN,a" (reloc) => 
| "BNE" (reloc) => 	
/*	encode_delay_load_store(nextPC);
	map<unsigned, RAddr>::iterator mi = guest_host.find(reloc);
	if (mi == guest_host.end()) {
		// jumping out of the fragment
		// add to list of out portals
		out_portal.push_front(pair<unsigned, unsigned>(reloc,0));		
		// create label
		char guest_addr[12];
		sprintf(guest_addr, "%X", reloc);	
		raddr = guest_host[reloc] = addr_new(label_new(guest_addr), 0);
		BNE(raddr);				
	} else {
		BNE((*mi).second);
		// setup fallthrough as the out portal
		if (guest_host.find(pc+8) != guest_host.end()) {
			//for (mi = guest_host.begin(); mi != guest_host.end(); mi++)
			//	cerr << hex << "*" << (*mi).first << endl;
			// Some other instruction also has out portal here.
			;
		} else {
			out_portal.push_front(pair<unsigned, unsigned>(pc+8,0));		
			char guest_addr[12];
			sprintf(guest_addr, "%X", pc+8);	// could be incorrect here
			guest_host[pc+8] = addr_new(label_new(guest_addr), 0);		
		}
		frag_inst++;
		encode(*frag_inst);
		BA(guest_host[pc+8]);
		fNOP();
	}
*/	
	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BE(raddr);
	} else {
		BNE(raddr);		
	}
| "BNE,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BE(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BNE_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BNEG" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BPOS(raddr);
	} else {
		BNEG(raddr);		
	}
| "BNEG,a" (reloc) => 	
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BPOS(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BNEG_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "BPOS" (reloc) => 	
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		BNEG(raddr);
	} else {
		BPOS(raddr);		
	}
| "BPOS,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BNEG(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		BPOS_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
		
//| "BVC" (reloc) => BVC(relocate_target(reloc));
//| "BVC,a" (reloc) => BVCA(relocate_target(reloc));
//| "BVS" (reloc) => BVS(relocate_target(reloc));
//| "BVS,a" (reloc) => BVSA(relocate_target(reloc));
| "call__" (reloc) => 	 
	//if (relocate_delay_slot(nextPC))
	//	encode_delay_load_store(nextPC);
	//call(relocate(reloc));
	
	// now just generate: set guest pc => %o7
	int o7 = 15; 
	decode_sethi((unsigned)pc, o7);
	ADD(o7, imode((unsigned)pc & 0x000003ff), o7); 
		
| "FABSs" (fs2s, fds) => FABSs( fs2s, fds );
| "FADDd" (fs1d, fs2d, fdd) => FADDd( fs1d, fs2d, fdd );
| "FADDq" (fs1q, fs2q, fdq) => FADDq( fs1q, fs2q, fdq );
| "FADDs" (fs1s, fs2s, fds) => FADDs( fs1s, fs2s, fds );
//| "FBA" (reloc) => 
//| "FBA,a" (reloc) => 
//| "FBE" (reloc) => 
//| "FBE,a" (reloc) => 
//| "FBG" (reloc) => 
//| "FBG,a" (reloc) => 
//| "FBGE" (reloc) => 
//| "FBGE,a" (reloc) => 
//| "FBL" (reloc) => 
//| "FBL,a" (reloc) => 
//| "FBLE" (reloc) => 
//| "FBLE,a" (reloc) => 
//| "FBLG" (reloc) => 
//| "FBLG,a" (reloc) => 
//| "FBN" (reloc) => 
//| "FBN,a" (reloc) => 
//| "FBNE" (reloc) => 
//| "FBNE,a" (reloc) => 
//| "FBO" (reloc) => 
//| "FBO,a" (reloc) => 
//| "FBU" (reloc) => 
//| "FBU,a" (reloc) => 
//| "FBUE" (reloc) => 
//| "FBUE,a" (reloc) => 
| "FBUG" (reloc) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		FBULE(raddr);
	} else {
		FBUG(raddr);		
	}
//| "FBUG,a" (reloc) => FBUGA(relocate_target(reloc));
| "FBUGE" (reloc) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		FBUL(raddr);
	} else {
		FBUGE(raddr);		
	}
| "FBUGE,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBUL(raddr);
		fNOP();
	} else {
		encode_delay_load_store(nextPC);		
		FBUGE_a(raddr);		
		if (pc != back_branch) {
			// do delay slot since its not in the trace
			// err!!  relocation for loads and stores....
			encode(nextPC);
		}
	}
| "FBUL" (reloc) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		FBUGE(raddr);		
	} else {
		FBUL(raddr);
	}
//| "FBUL,a" (reloc) => FBULA(relocate_target(reloc));
| "FBULE" (reloc) =>
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
	if (relocate_target(reloc, pc, raddr)) {
		FBUG(raddr);		
	} else {
		FBULE(raddr);
	}
//| "FBULE,a" (reloc) => FBULEA(relocate_target(reloc));
| "FCMPd" (fs1d, fs2d) => FCMPd( fs1d, fs2d );
| "FCMPEd" (fs1d, fs2d) => FCMPEd( fs1d, fs2d );
| "FCMPEq" (fs1q, fs2q) => FCMPEq( fs1q, fs2q );
| "FCMPEs" (fs1s, fs2s) => FCMPEs( fs1s, fs2s );
| "FCMPq" (fs1q, fs2q) => FCMPq( fs1q, fs2q );
| "FCMPs" (fs1s, fs2s) => FCMPs( fs1s, fs2s );
| "FDIVd" (fs1d, fs2d, fdd) => FDIVd( fs1d, fs2d, fdd );
| "FDIVq" (fs1q, fs2q, fdq) => FDIVq( fs1q, fs2q, fdq );
| "FDIVs" (fs1s, fs2s, fds) => FDIVs( fs1s, fs2s, fds );
| "FdMULq" (fs1q, fs2q, fdq) => FdMULq( fs1q, fs2q, fdq );
| "FdTOi" (fs2d, fds) => FdTOi( fs2d, fds );
| "FdTOq" (fs2d, fdq) => FdTOq( fs2d, fdq );
| "FdTOs" (fs2d, fds) => FdTOs( fs2d, fds );
| "FiTOd" (fs2s, fdd) => FiTOd( fs2s, fdd );
| "FiTOq" (fs2s, fdq) => FiTOq( fs2s, fdq );
| "FiTOs" (fs2s, fds) => FiTOs( fs2s, fds );
//| "FLUSH" (eaddr) => FLUSH( relocate_eaddr( eaddr ) ); 
| "FMOVs" (fs2s, fds) => FMOVs( fs2s, fds );
| "FMULd" (fs1d, fs2d, fdd) => FMULd( fs1d, fs2d, fdd );
| "FMULq" (fs1q, fs2q, fdq) => FMULq( fs1q, fs2q, fdq );
| "FMULs" (fs1s, fs2s, fds) => FMULs( fs1s, fs2s, fds );
| "FNEGs" (fs2s, fds) => FNEGs( fs2s, fds );
| "FqTOd" (fs2q, fdd) => FqTOd( fs2q, fdd );
| "FqTOi" (fs2q, fds) => FqTOi( fs2q, fds );
| "FqTOs" (fs2q, fds) => FqTOs( fs2q, fds );
| "FsMULd" (fs1d, fs2d, fdd) => FsMULd( fs1d, fs2d, fdd );
| "FSQRTd" (fs2d, fdd) => FSQRTd( fs2d, fdd );
| "FSQRTq" (fs2q, fdq) => FSQRTq( fs2q, fdq );
| "FSQRTs" (fs2s, fds) => FSQRTs( fs2s, fds );
| "FsTOd" (fs2s, fdd) => FsTOd( fs2s, fdd );
| "FsTOi" (fs2s, fds) => FsTOi( fs2s, fds );
| "FsTOq" (fs2s, fdq) => FsTOq( fs2s, fdq );
| "FSUBd" (fs1d, fs2d, fdd) => FSUBd( fs1d, fs2d, fdd );
| "FSUBq" (fs1q, fs2q, fdq) => FSUBq( fs1q, fs2q, fdq );
| "FSUBs" (fs1s, fs2s, fds) => FSUBs( fs1s, fs2s, fds );
| "ret" ()  => ;
| "retl" ()  => ;
| "JMPL" (eaddr, rd) => 
	if (relocate_delay_slot(nextPC))
		encode_delay_load_store(nextPC);
		//		abort();  // err load / store could change eaddr
	int scratch = 5;
	//LD(relocate_eaddr(eaddr, scratch), scratch);
	process_eaddr(eaddr, scratch);
	indirect_jump(pc, scratch);
| "LD" (eaddr, rd) => LD( relocate_eaddr(eaddr, rd), rd );
| "LDD" (eaddr, rd) => LDD( relocate_eaddr(eaddr, rd), rd );
| "LDDF" (eaddr, fdd) => LDDF( relocate_eaddr(eaddr, 0), fdd );
| "LDF" (eaddr, fds) => LDF( relocate_eaddr(eaddr, 0), fds );
// | "LDFSR" (eaddr) => LDFSR( relocate_eaddr( eaddr ) );
| "LDSB" (eaddr, rd) => LDSB( relocate_eaddr( eaddr,rd ), rd );
| "LDSH" (eaddr, rd) => LDSH( relocate_eaddr( eaddr,rd ), rd );
| "LDSTUB" (eaddr, rd) => LDSTUB( relocate_eaddr( eaddr,rd ), rd );
| "LDUB" (eaddr, rd) => LDUB( relocate_eaddr( eaddr,rd ), rd );
| "LDUH" (eaddr, rd) => LDUH( relocate_eaddr( eaddr,rd ), rd );
| "MULScc" (rs1, reg_or_imm, rd) => MULScc( rs1, encode_roi( reg_or_imm ), rd );
| "NOP" () => fNOP(  );
| "OR" (rs1, reg_or_imm, rd) => OR( rs1, encode_roi( reg_or_imm ), rd );
| "ORcc" (rs1, reg_or_imm, rd) => ORcc( rs1, encode_roi( reg_or_imm ), rd );
| "ORN" (rs1, reg_or_imm, rd) => ORN( rs1, encode_roi( reg_or_imm ), rd );
| "ORNcc" (rs1, reg_or_imm, rd) => ORNcc( rs1, encode_roi( reg_or_imm ), rd );
| "RDPSR" (rd) => RDPSR( rd );
| "RDTBR" (rd) => RDTBR( rd );
| "RDWIM" (rd) => RDWIM( rd );
| "RDY" (rd) => RDY( rd );
| "RESTORE" (rs1, reg_or_imm, rd) => RESTORE( rs1, encode_roi( reg_or_imm ), rd );
| "SAVE" (rs1, reg_or_imm, rd) => 
	int scratch = 6;
	decode_sethi((unsigned)mem, scratch);
	//	ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 
	ADD(scratch, rmode(14), scratch);
	ST(16, dispA(scratch, 0));
	ST(17, dispA(scratch, 4));
	ST(18, dispA(scratch, 8));
	ST(19, dispA(scratch, 12));
	ST(20, dispA(scratch, 16));
	ST(21, dispA(scratch, 20));
	ST(22, dispA(scratch, 24));
	ST(23, dispA(scratch, 28));
	ST(24, dispA(scratch, 32));
	ST(25, dispA(scratch, 36));
	ST(26, dispA(scratch, 40));
	ST(27, dispA(scratch, 44));
	ST(28, dispA(scratch, 48));
	ST(29, dispA(scratch, 52));
	ST(30, dispA(scratch, 56));
	ST(31, dispA(scratch, 60));
/*	ST(24, dispA(scratch, 0));
	ST(25, dispA(scratch, 4));
	ST(26, dispA(scratch, 8));
	ST(27, dispA(scratch, 12));
	ST(28, dispA(scratch, 16));
	ST(29, dispA(scratch, 20));
	ST(30, dispA(scratch, 24));
	ST(31, dispA(scratch, 28));
	ST(16, dispA(scratch, 32));
	ST(17, dispA(scratch, 36));
	ST(18, dispA(scratch, 40));
	ST(19, dispA(scratch, 44));
	ST(20, dispA(scratch, 48));
	ST(21, dispA(scratch, 52));
	ST(22, dispA(scratch, 56));
	ST(23, dispA(scratch, 60));	*/
	SAVE( rs1, encode_roi( reg_or_imm ), rd );
| "SDIV" (rs1, reg_or_imm, rd) => SDIV( rs1, encode_roi( reg_or_imm ), rd );
| "SDIVcc" (rs1, reg_or_imm, rd) => SDIVcc( rs1, encode_roi( reg_or_imm ), rd );
| "sethi" (val, rd) => decode_sethi( val, rd );
| "SLL" (rs1, reg_or_imm, rd) => SLL( rs1, encode_roi( reg_or_imm ), rd );
| "SMUL" (rs1, reg_or_imm, rd) => SMUL( rs1, encode_roi( reg_or_imm ), rd );
| "SMULcc" (rs1, reg_or_imm, rd) => SMULcc( rs1, encode_roi( reg_or_imm ), rd );
| "SRA" (rs1, reg_or_imm, rd) => SRA( rs1, encode_roi( reg_or_imm ), rd );
| "SRL" (rs1, reg_or_imm, rd) => SRL( rs1, encode_roi( reg_or_imm ), rd );
| "ST" (rd, eaddr) => ST( rd, relocate_eaddr( eaddr,0 ) );
| "STB" (rd, eaddr) => STB( rd, relocate_eaddr( eaddr,0 ) );
| "STBAR" () => STBAR(  );
| "STD" (rd, eaddr) => STD( rd, relocate_eaddr( eaddr,0 ) );
| "STDF" (fdd, eaddr) => STDF( fdd, relocate_eaddr(eaddr, 0) );
| "STF" (fds, eaddr) =>  STF( fds, relocate_eaddr(eaddr, 0) );
//| "STFSR" (eaddr) => executeSTFSR( relocate_eaddr( eaddr ) );
| "STH" (rd, eaddr) => STH( rd, relocate_eaddr( eaddr,0 ) );
| "SUB" (rs1, reg_or_imm, rd) => SUB( rs1, encode_roi( reg_or_imm ), rd );
| "SUBcc" (rs1, reg_or_imm, rd) => SUBcc( rs1, encode_roi( reg_or_imm ), rd );
| "SUBX" (rs1, reg_or_imm, rd) => SUBX( rs1, encode_roi( reg_or_imm ), rd );
| "SUBXcc" (rs1, reg_or_imm, rd) => SUBXcc( rs1, encode_roi( reg_or_imm ), rd );
| "SWAP." (eaddr, rd) => SWAP_( relocate_eaddr( eaddr,rd ), rd );
| "TA" (eaddr) => TA( relocate_eaddr(eaddr, 0) );
| "TADDcc" (rs1, reg_or_imm, rd) => TADDcc( rs1, encode_roi( reg_or_imm ), rd );
| "TCC" (eaddr) => abort(); //TCC( relocate_eaddr( eaddr ) );
| "TCS" (eaddr) => abort(); //executeTCS( relocate_eaddr( eaddr ) );
| "TE" (eaddr) => abort(); //executeTE( relocate_eaddr( eaddr ) );
| "TG" (eaddr) => abort(); //executeTG( relocate_eaddr( eaddr ) );
| "TGE" (eaddr) => abort(); //executeTGE( relocate_eaddr( eaddr ) );
| "TGU" (eaddr) => abort(); //executeTGU( relocate_eaddr( eaddr ) );
| "TL" (eaddr) => abort(); //executeTL( relocate_eaddr( eaddr ) );
| "TLE" (eaddr) => abort(); //executeTLE( relocate_eaddr( eaddr ) );
| "TLEU" (eaddr) => abort(); //executeTLEU( relocate_eaddr( eaddr ) );
| "TN" (eaddr) => abort(); //executeTN( relocate_eaddr( eaddr ) );
| "TNE" (eaddr) => abort(); //executeTNE( relocate_eaddr( eaddr ) );
| "TNEG" (eaddr) => abort(); //executeTNEG( relocate_eaddr( eaddr ) );
| "TPOS" (eaddr) => abort(); //executeTPOS( relocate_eaddr( eaddr ) );
| "TSUBcc" (rs1, reg_or_imm, rd) => TSUBcc( rs1, encode_roi( reg_or_imm ), rd );
| "TVC" (eaddr) => abort(); //executeTVC( relocate_eaddr( eaddr ) );
| "TVS" (eaddr) => abort(); //executeTVS( relocate_eaddr( eaddr ) );
| "UDIV" (rs1, reg_or_imm, rd) => UDIV( rs1, encode_roi( reg_or_imm ), rd );
| "UDIVcc" (rs1, reg_or_imm, rd) => UDIVcc( rs1, encode_roi( reg_or_imm ), rd );
| "UMUL" (rs1, reg_or_imm, rd) => UMUL( rs1, encode_roi( reg_or_imm ), rd );
| "UMULcc" (rs1, reg_or_imm, rd) => UMULcc( rs1, encode_roi( reg_or_imm ), rd );
| "WRPSR" (rs1, reg_or_imm) => WRPSR( rs1, encode_roi( reg_or_imm ) );
| "WRTBR" (rs1, reg_or_imm) => WRTBR( rs1, encode_roi( reg_or_imm ) );
| "WRWIM" (rs1, reg_or_imm) => WRWIM( rs1, encode_roi( reg_or_imm ) );
| "WRY" (rs1, reg_or_imm) => WRY( rs1, encode_roi( reg_or_imm ) );
| "XNOR" (rs1, reg_or_imm, rd) => XNOR( rs1, encode_roi( reg_or_imm ), rd );
| "XNORcc" (rs1, reg_or_imm, rd) => XNORcc( rs1, encode_roi( reg_or_imm ), rd );
| "XOR" (rs1, reg_or_imm, rd) => XOR( rs1, encode_roi( reg_or_imm ), rd );
| "XORcc" (rs1, reg_or_imm, rd) => XORcc( rs1, encode_roi( reg_or_imm ), rd );
else 
	cerr << "Relocator can not generate this instructions." << endl;
	unimplemented(pc);
endmatch
return nextPC;
}
