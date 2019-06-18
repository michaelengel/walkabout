/*
 * Copyright (C) 2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

// File: sparc.NET.direct.extra.m
// Desc: Supporting code for instrumention
// Updates:
// 01 Aug 01 - David: initial version
// 14 Aug 01 - David: changed ret and retl conditions.  skip code generation 
//             only when a call is found in the fragment cache.
// 30 Aug 01 - David: fixed bug in BA,a and BA
// 04 Sep 01 - David: change code gen for call if start of trace is a call

#include "pathfinder.cc"

reg_or_imm_Instance encode_roi( sint32_t pc )
{
	match pc to
	| "imode" (simm13) => return imode(simm13);
	| "rmode" (rs2) => return rmode(rs2);
	else abort();
	endmatch
}

address__Instance encode_eaddr( sint32_t pc )
{
	match pc to
	| "absoluteA" (simm13) => 
		return absoluteA(simm13);
	| "dispA" (rs1, simm13) => 
		return dispA(rs1, simm13);
	| "indirectA" (rs1) =>
		return indirectA(rs1);
	| "indexA" (rs1, rs2) =>
		return indexA(rs1, rs2);
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

int unconditional_annul(sint32_t pc) 
{
	match pc to
	| "BA,a" (reloc) => 
		return 1;
	| "BN,a" (reloc) => 
		return 1;
	else return 0;			
	endmatch
}

int is_nop(sint32_t pc) 
{
	match pc to
	| "NOP" () => return 1;
	else return 0;			
	endmatch
}

int is_restore(sint32_t pc) 
{
	match pc to
	| "RESTORE" (rs1, reg_or_imm, rd) =>
		if ((rs1 == 0) && (rd == 0) && (reg_or_imm == 0)) {
			return 1;
		} else {
			return 0;
		}
	else return 0;			
	endmatch
}

int check_plt(unsigned pc, int& g1) 
{
	match pc to
	| "sethi" (val, rd) => 
		if (rd == 1) {
			g1 = val;
			return 1;
		} else {
			return 0;
		}
	else return 0;			
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
	// emliminate this branch, but can't remove this if it is the back branch!!
	if ((unsigned)pc == back_branch) {
		BA(relocate(reloc, pc));	
	} else {
		if (is_nop(nextPC))
			frag_inst++;		// remove the delay slot aswell	
	}
| "BA,a" (reloc) => 
	// emliminate this branch and delay slot, but can't remove this if it is the back branch!!
	if ((unsigned)pc == back_branch) {
		BA(relocate(reloc, pc));
	} else {
		// frag_inst++;			// remove the delay slot aswell		
		// oops, delay slot is never included in the trace
	}
| "BCC" (reloc) => 
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
		BCC_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BCS" (reloc) => 	
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
		BCS_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BE" (reloc) => 	
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
		BE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BG" (reloc) => 	
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
		BG_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BGE" (reloc) => 	
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
		BGE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BGU" (reloc) => 	
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
		BGU_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BL" (reloc) => 	
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
		BL_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BLE" (reloc) => 	
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
		BLE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BLEU" (reloc) => 	
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
		BLEU_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
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
		BNE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BNEG" (reloc) => 	
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
		BNEG_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
| "BPOS" (reloc) => 	
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
		BPOS_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
		
| "BVC" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		BVS(raddr);
	} else {
		BVC(raddr);		
	}

| "BVC,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BVS(raddr);
		fNOP();
	} else {
		BVC_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}

| "BVS" (reloc) =>
	if (relocate_target(reloc, pc, raddr)) {
		BVC(raddr);
	} else {
		BVS(raddr);		
	}
| "BVS,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		BVC(raddr);
		fNOP();
	} else {
		BVS_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}

| "call_" (eaddr) => 
	int scratch = 5;	
	int o7 = 15; 
	decode_sethi((unsigned)pc, o7);
	ADD(o7, imode((unsigned)pc & 0x000003ff), o7); 
	process_eaddr(eaddr, scratch);
	indirect_jump(pc, scratch);
| "call__" (reloc) => 	 
	if (is_restore(nextPC)) {
		frag_inst++;		// remove restore instruction			
		goto the_end;
	}
	
	// now just generate: set guest pc => %o7
	int o7 = 15; 
	decode_sethi((unsigned)pc, o7);
	ADD(o7, imode((unsigned)pc & 0x000003ff), o7); 
	call_made++;
	if (!end_call(reloc, pc)) {
		if (is_nop(nextPC)) {
			frag_inst++;		// remove the delay slot aswell			
		}
	}
	/*
	if (mov_o7(nextPC) == 1) {
		if (mov_o7(PC-4) == 2) {
			// its a PLT entry call 
			frag_inst++;		// remove next instruction
			curr_p -= 3;		// move relocatable pointer back by 3 instr
		}
	}*/

| "FABSs" (fs2s, fds) => FABSs( fs2s, fds );
| "FADDd" (fs1d, fs2d, fdd) => FADDd( fs1d, fs2d, fdd );
| "FADDq" (fs1q, fs2q, fdq) => FADDq( fs1q, fs2q, fdq );
| "FADDs" (fs1s, fs2s, fds) => FADDs( fs1s, fs2s, fds );
//| "FBA" (reloc) => 
//| "FBA,a" (reloc) => 

| "FBE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBNE(raddr);
	} else {
		FBE(raddr);		
	}
| "FBE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBNE(raddr);
		fNOP();
	} else {
		FBE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}

| "FBG" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBULE(raddr);
	} else {
		FBG(raddr);		
	}
| "FBG,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBULE(raddr);
		fNOP();
	} else {
		FBG_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBGE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBUL(raddr);
	} else {
		FBGE(raddr);		
	}
| "FBGE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBUL(raddr);
		fNOP();
	} else {
		FBGE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBL" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBUGE(raddr);
	} else {
		FBL(raddr);		
	}
| "FBL,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBUGE(raddr);
		fNOP();
	} else {
		FBL_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBLE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBUG(raddr);
	} else {
		FBLE(raddr);		
	}
	
| "FBLE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBUG(raddr);
		fNOP();
	} else {
		FBLE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBLG" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBUE(raddr);
	} else {
		FBLG(raddr);		
	}
| "FBLG,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBUE(raddr);
		fNOP();
	} else {
		FBLG_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
//| "FBN" (reloc) => 
//| "FBN,a" (reloc) => 
	
| "FBNE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBE(raddr);
	} else {
		FBNE(raddr);		
	}
| "FBNE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBE(raddr);
		fNOP();
	} else {
		FBNE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBO" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBU(raddr);
	} else {
		FBO(raddr);		
	}
| "FBO,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBU(raddr);
		fNOP();
	} else {
		FBO_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBU" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBO(raddr);
	} else {
		FBU(raddr);		
	}
| "FBU,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBO(raddr);
		fNOP();
	} else {
		FBU_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBUE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBLG(raddr);
	} else {
		FBUE(raddr);		
	}
| "FBUE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBLG(raddr);
		fNOP();
	} else {
		FBUE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBUG" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBLE(raddr);
	} else {
		FBUG(raddr);		
	}
| "FBUG,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBLE(raddr);
		fNOP();
	} else {
		FBUG_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBUGE" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBL(raddr);
	} else {
		FBUGE(raddr);		
	}
| "FBUGE,a" (reloc) =>
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBL(raddr);
		fNOP();
	} else {
		FBUGE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBUL" (reloc) => 
	if (relocate_target(reloc, pc, raddr)) {
		FBGE(raddr);		
	} else {
		FBUL(raddr);
	}
| "FBUL,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBGE(raddr);
		fNOP();
	} else {
		FBUL_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}
	
| "FBULE" (reloc) =>
	if (relocate_target(reloc, pc, raddr)) {
		FBG(raddr);		
	} else {
		FBULE(raddr);
	}
| "FBULE,a" (reloc) => 
	if (relocate_target(reloc, pc, raddr, 1)) {
		// branch profiled to be taken and not a back branch
		FBG(raddr);
		fNOP();
	} else {
		FBULE_a(raddr);		
		if ((unsigned)pc != back_branch) {
			// do delay slot since its not in the trace
			encode(nextPC);
		}
	}

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
//| "FLUSH" (eaddr) => FLUSH(eaddr); 
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
| "ret" ()  => 
	if (pc == back_branch) {
		int scratch = 5;
		ADD(31, imode(8), scratch);		
		do_indirect_portal(pc);
	} else {
		if (call_made <= 0) {
			int scratch = 5;
			ADD(31, imode(8), scratch);
			indirect_jump(pc, scratch);
		} else {
			call_made--;
			#ifdef AGGRESSIVE
			if (is_nop(nextPC))
				frag_inst++;		// remove the delay slot aswell	
			#else
			int scratch = 5;
			ADD(31, imode(8), scratch);		
			indirect_jump(pc, scratch);
			#endif
		}
	}
	
| "retl" ()  =>
	if (pc == back_branch) {
		int scratch = 5;
		ADD(15, imode(8), scratch);		
		do_indirect_portal(pc);
	} else {
		if (call_made <= 0) {
			int scratch = 5;
			ADD(15, imode(8), scratch);
			indirect_jump(pc, scratch);
		} else {
			call_made--;
			#ifdef AGGRESSIVE
			if (is_nop(nextPC))
				frag_inst++;		// remove the delay slot aswell	
			#else
			int scratch = 5;
			ADD(15, imode(8), scratch);		
			indirect_jump(pc, scratch);
			#endif
		}
	}

| "JMPL" (eaddr, rd) => 
	int g1;
	if (check_plt((unsigned)pc - 4, g1)) {
		if (check_plt((unsigned)pc - 8, g1)) {
			// possibly a PLT entry
			address__Instance e = encode_eaddr(eaddr);
			// Tags defines in the generated routines
			// #define generalA_TAG 1
			// #define dispA_TAG 2
			// #define absoluteA_TAG 3
			// #define indexA_TAG 4
			// #define indirectA_TAG 5
			if ((e.tag == 2) && (e.u.dispA.rs1 == 1)) {
				// PLT call!
				// unwind the 2 sethi instructions
				currb_p = (unsigned char*)((unsigned)currb_p - 8);
				if (currb_p < currb->curbuf->data) {
					cerr << "NJMC relocation subtraction error." << endl;
					abort();
				}
				if (pc == back_branch) {
					cerr << "Fragment terminating at PLT???" << endl;
					abort();
				}				
				goto the_end;
			}
		}
	}

	if (pc == back_branch) {
		int scratch = 5;
		process_eaddr(eaddr, scratch);
		do_indirect_portal(pc);
	} else {	
		int scratch = 5;	// can't use rd, well at least not when rd = %g0
		//process_eaddr(eaddr, rd);
		//indirect_jump(pc, rd);
		process_eaddr(eaddr, scratch);
		indirect_jump(pc, scratch);
	}
	
| "LD" (eaddr, rd) => LD( encode_eaddr(eaddr), rd );
| "LDD" (eaddr, rd) => LDD( encode_eaddr(eaddr), rd );
| "LDDF" (eaddr, fdd) => LDDF( encode_eaddr(eaddr), fdd );
| "LDF" (eaddr, fds) => LDF( encode_eaddr(eaddr), fds );
| "LDFSR" (eaddr) => LDFSR(encode_eaddr(eaddr));
| "LDSB" (eaddr, rd) => LDSB( encode_eaddr(eaddr), rd );
| "LDSH" (eaddr, rd) => LDSH( encode_eaddr(eaddr), rd );
| "LDSTUB" (eaddr, rd) => LDSTUB( encode_eaddr(eaddr), rd );
| "LDUB" (eaddr, rd) => LDUB( encode_eaddr(eaddr), rd );
| "LDUH" (eaddr, rd) => LDUH( encode_eaddr(eaddr), rd );
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
	int sp = 14;
	//int scratch = 6;
	// decode_sethi((unsigned)mem, scratch);
	// ADD(scratch, rmode(14), scratch);
/*	ST(16, dispA(sp, 0));
	ST(17, dispA(sp, 4));
	ST(18, dispA(sp, 8));
	ST(19, dispA(sp, 12));
	ST(20, dispA(sp, 16));
	ST(21, dispA(sp, 20));
	ST(22, dispA(sp, 24));
	ST(23, dispA(sp, 28));
	ST(24, dispA(sp, 32));
	ST(25, dispA(sp, 36));
	ST(26, dispA(sp, 40));
	ST(27, dispA(sp, 44));
	ST(28, dispA(sp, 48));
	ST(29, dispA(sp, 52));
	ST(30, dispA(sp, 56));
	ST(31, dispA(sp, 60));	*/
	SAVE( rs1, encode_roi( reg_or_imm ), rd );
| "SDIV" (rs1, reg_or_imm, rd) => SDIV( rs1, encode_roi( reg_or_imm ), rd );
| "SDIVcc" (rs1, reg_or_imm, rd) => SDIVcc( rs1, encode_roi( reg_or_imm ), rd );
| "sethi" (val, rd) => decode_sethi( val, rd );
| "SLL" (rs1, reg_or_imm, rd) => SLL( rs1, encode_roi( reg_or_imm ), rd );
| "SMUL" (rs1, reg_or_imm, rd) => SMUL( rs1, encode_roi( reg_or_imm ), rd );
| "SMULcc" (rs1, reg_or_imm, rd) => SMULcc( rs1, encode_roi( reg_or_imm ), rd );
| "SRA" (rs1, reg_or_imm, rd) => SRA( rs1, encode_roi( reg_or_imm ), rd );
| "SRL" (rs1, reg_or_imm, rd) => SRL( rs1, encode_roi( reg_or_imm ), rd );
| "ST" (rd, eaddr) => ST( rd, encode_eaddr(eaddr) );
| "STB" (rd, eaddr) => STB( rd, encode_eaddr(eaddr));
| "STBAR" () => STBAR(  );
| "STD" (rd, eaddr) => STD( rd, encode_eaddr(eaddr) );
| "STDF" (fdd, eaddr) => STDF( fdd, encode_eaddr(eaddr) );
| "STF" (fds, eaddr) =>  STF( fds, encode_eaddr(eaddr) );
| "STFSR" (eaddr) => STFSR( encode_eaddr(eaddr) );
| "STH" (rd, eaddr) => STH( rd, encode_eaddr(eaddr) );
| "SUB" (rs1, reg_or_imm, rd) => SUB( rs1, encode_roi( reg_or_imm ), rd );
| "SUBcc" (rs1, reg_or_imm, rd) => SUBcc( rs1, encode_roi( reg_or_imm ), rd );
| "SUBX" (rs1, reg_or_imm, rd) => SUBX( rs1, encode_roi( reg_or_imm ), rd );
| "SUBXcc" (rs1, reg_or_imm, rd) => SUBXcc( rs1, encode_roi( reg_or_imm ), rd );
| "SWAP." (eaddr, rd) => SWAP_( encode_eaddr(eaddr), rd );
| "TA" (eaddr) => TA( encode_eaddr(eaddr) );
| "TADDcc" (rs1, reg_or_imm, rd) => TADDcc( rs1, encode_roi( reg_or_imm ), rd );
//| "TCC" (eaddr) => abort(); //TCC( relocate_eaddr( eaddr ) );
//| "TCS" (eaddr) => abort(); //executeTCS( relocate_eaddr( eaddr ) );
//| "TE" (eaddr) => abort(); //executeTE( relocate_eaddr( eaddr ) );
//| "TG" (eaddr) => abort(); //executeTG( relocate_eaddr( eaddr ) );
//| "TGE" (eaddr) => abort(); //executeTGE( relocate_eaddr( eaddr ) );
//| "TGU" (eaddr) => abort(); //executeTGU( relocate_eaddr( eaddr ) );
//| "TL" (eaddr) => abort(); //executeTL( relocate_eaddr( eaddr ) );
//| "TLE" (eaddr) => abort(); //executeTLE( relocate_eaddr( eaddr ) );
//| "TLEU" (eaddr) => abort(); //executeTLEU( relocate_eaddr( eaddr ) );
//| "TN" (eaddr) => abort(); //executeTN( relocate_eaddr( eaddr ) );
//| "TNE" (eaddr) => abort(); //executeTNE( relocate_eaddr( eaddr ) );
//| "TNEG" (eaddr) => abort(); //executeTNEG( relocate_eaddr( eaddr ) );
//| "TPOS" (eaddr) => abort(); //executeTPOS( relocate_eaddr( eaddr ) );
| "TSUBcc" (rs1, reg_or_imm, rd) => TSUBcc( rs1, encode_roi( reg_or_imm ), rd );
//| "TVC" (eaddr) => abort(); //executeTVC( relocate_eaddr( eaddr ) );
//| "TVS" (eaddr) => abort(); //executeTVS( relocate_eaddr( eaddr ) );
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
the_end:
return nextPC;
}

