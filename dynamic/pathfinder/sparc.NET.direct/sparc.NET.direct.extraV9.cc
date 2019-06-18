#define sign_extend(N,SIZE) (((int)((N) << (sizeof(unsigned)*8-(SIZE)))) >> (sizeof(unsigned)*8-(SIZE)))
#include <assert.h>

#line 2 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
// File: sparc.NET.direct.extraV9.m
// Desc: Supporting code for instrumention
// Updates:
// 01 Aug 01 - David: initial version
// 14 Aug 01 - David: changed ret and retl conditions.  
//			   skip code generation only when a call is found in the fragment 
//			   cache.
// 30 Aug 01 - David: fixed bug in BA,a and BA
// 04 Sep 01 - David: change code gen for call if start of trace is a call

#include "pathfinder.cc"

reg_or_imm_Instance encode_roi( sint32_t pc )
{


#line 15 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 15 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) { 
      int /* [~4096..4095] */ simm13 = 
        sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
      
      #line 16 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       return imode(simm13);

      
      
      
    } /*opt-block*//*opt-block+*/
    else { 
      unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
      
      #line 17 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       return rmode(rs2);

      
      
      
    } /*opt-block*//*opt-block+*/
    
  }goto MATCH_finished_f; 
  
  MATCH_finished_f: (void)0; /*placeholder for label*/
  
}

#line 21 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
}

address__Instance encode_eaddr( sint32_t pc )
{


#line 24 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 24 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 25 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
         

        		return absoluteA(simm13);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 27 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
         

        		return dispA(rs1, simm13);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 29 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
        

        		return indirectA(rs1);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 31 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
        

        		return indexA(rs1, rs2);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_e; 
  
  MATCH_finished_e: (void)0; /*placeholder for label*/
  
}

#line 36 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
}

void process_eaddr(sint32_t pc, unsigned scratch) 
{


#line 39 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 39 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 40 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
         

        		mov(imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 42 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
         

        		ADD(rs1, imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 44 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
        

        		mov(rmode(rs1), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 46 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
        

        		ADD(rs1, rmode(rs2), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_d; 
  
  MATCH_finished_d: (void)0; /*placeholder for label*/
  
}

#line 51 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
}

int unconditional_annul(sint32_t pc) 
{


#line 54 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 54 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 22 & 0x7) /* op2 at 0 */ == 2) 
      
        switch((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */) {
          case 0: 
            if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1 && 
              (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0) { 
              unsigned reloc = 
                4 * sign_extend((MATCH_w_32_0 & 0x3fffff) /* disp22 at 0 */, 
                            22) + addressToPC(MATCH_p);
              
              #line 57 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
               

              		return 1;

              
              
              
            } /*opt-block*//*opt-block+*/
            else 
              goto MATCH_label_c0;  /*opt-block+*/
            
            break;
          case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 9: 
          case 10: case 11: case 12: case 13: case 14: case 15: 
            goto MATCH_label_c0; break;
          case 8: 
            if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1 && 
              (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0) { 
              unsigned reloc = 
                4 * sign_extend((MATCH_w_32_0 & 0x3fffff) /* disp22 at 0 */, 
                            22) + addressToPC(MATCH_p);
              
              #line 55 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
               

              		return 1;

              
              
              
            } /*opt-block*//*opt-block+*/
            else 
              goto MATCH_label_c0;  /*opt-block+*/
            
            break;
          default: assert(0);
        } /* (MATCH_w_32_0 >> 25 & 0xf) -- cond at 0 --*/  
    else 
      goto MATCH_label_c0;  /*opt-block+*/
    
  }goto MATCH_finished_c; 
  
  MATCH_label_c0: (void)0; /*placeholder for label*/ 
    
    #line 59 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    return 0;			

    
     
    goto MATCH_finished_c; 
    
  MATCH_finished_c: (void)0; /*placeholder for label*/
  
}

#line 62 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
}

int is_nop(sint32_t pc) 
{


#line 65 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 65 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 22 & 0x7) /* op2 at 0 */ == 4 && 
      (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ == 0 && 
      (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0 && 
      (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) 
      
      #line 66 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       return 1;

      
       /*opt-block+*/
    else 
      goto MATCH_label_b0;  /*opt-block+*/
    
  }goto MATCH_finished_b; 
  
  MATCH_label_b0: (void)0; /*placeholder for label*/ 
    
    #line 67 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    return 0;			

    
     
    goto MATCH_finished_b; 
    
  MATCH_finished_b: (void)0; /*placeholder for label*/
  
}

#line 70 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
}

int encode( sint32_t pc )
{
RAddr raddr;
sint32_t nextPC;


#line 75 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
{ 
  dword MATCH_p = 
    
    #line 75 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    
      switch((MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */) {
        case 0: 
          
            switch((MATCH_w_32_0 >> 22 & 0x7) /* op2 at 0 */) {
              case 0: case 1: case 3: case 5: case 7: 
                goto MATCH_label_a0; break;
              case 2: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */) {
                    case 0: case 7: case 15: 
                      goto MATCH_label_a0; break;
                    case 1: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 158 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPNE(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPE_a(1, 0, 0, raddr);		

                        		} else

                        			BPE_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 149 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPNE(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPE(1, 0, 0, raddr);

                        		} else

                        			BPE(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 2: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 278 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPG(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPLE_a(1, 0, 0, raddr);		

                        		}else

                        		BPLE_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 269 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPG(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch){

                        		BPLE(1, 0, 0, raddr);		

                        		}else

                        		BPLE(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 3: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 254 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPGE(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPL_a(1, 0, 0, raddr);		

                        		}else

                        		BPL_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 245 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPGE(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        		BPL(1, 0, 0, raddr);		

                        		} else

                        		BPL(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 4: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 302 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPGU(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPLEU_a(1, 0, 0, raddr);		

                        		}else

                        		BPLEU_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        //| "BN" (reloc) => 	

                        //| "BN,a" (reloc) => 

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 293 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPGU(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch){

                        		BPLEU(1, 0, 0, raddr);		

                        		}else

                        		BPLEU(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 5: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 134 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPCC(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPCS_a(1, 0, 0, raddr);		

                        		} else

                        			BPCS_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 125 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPCC(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPCS(1, 0, 0, raddr);		

                        		} else

                        			BPCS(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 6: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 383 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPPOS(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPNEG_a(1, 0, 0, raddr);		

                        		}else

                        		BPNEG_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 374 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPPOS(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch){

                        		BPNEG(1, 0, 0, raddr);	

                        		}else	

                        		BPNEG(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 8: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 93 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	// emliminate this branch and delay slot, but can't remove this if it is the back branch!!

                        	if ((unsigned)pc == back_branch) {

                        		BPA(1, 0, 0, relocate(reloc, pc));

                        	} else {

                        		// frag_inst++;			// remove the delay slot aswell		

                        		// oops, delay slot is never included in the trace

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 85 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	// emliminate this branch, but can't remove this if it is the back branch!!

                        	if ((unsigned)pc == back_branch) {

                        		BPA(1, 0, 0, relocate(reloc, pc));	

                        	} else {

                        		if (is_nop(nextPC))

                        			frag_inst++;		// remove the delay slot aswell	

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 9: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 359 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPE(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPNE_a(1, 0, 0, raddr);		

                        		}else

                        		BPNE_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 319 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

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

                        		BPE(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch){

                        		BPNE(1, 0, 0, raddr);		

                        		}else

                        		BPNE(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 10: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 182 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPLE(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPG_a(1, 0, 0, raddr);		

                        		} else

                        			BPG_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 173 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPLE(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPG(1, 0, 0, raddr);		

                        		} else

                        			BPG(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 11: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 206 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPL(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPGE_a(1, 0, 0, raddr);		

                        		} else

                        			BPGE_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 197 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPL(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPGE(1, 0, 0, raddr);		

                        		} else

                        			BPGE(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 12: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 230 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                        

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPLEU(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPGU_a(1, 0, 0, raddr);	

                        		} else	

                        			BPGU_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 221 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPLEU(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPGU(1, 0, 0, raddr);		

                        		} else

                        			BPGU(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 13: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 110 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPCS(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        			BPCC_a(1, 0, 0, raddr);		

                        		} else

                        			BPCC_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 101 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPCS(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        			BPCC(1, 0, 0, raddr);		

                        		} else

                        			BPCC(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 14: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 407 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                        

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		BPNEG(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch){

                        		BPPOS_a(1, 0, 0, raddr);		

                        		}else

                        		BPPOS_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        		

                        //| "BVC" (reloc) => BVC(relocate_target(reloc));

                        //| "BVC,a" (reloc) => BVCA(relocate_target(reloc));

                        //| "BVS" (reloc) => BVS(relocate_target(reloc));

                        //| "BVS,a" (reloc) => BVSA(relocate_target(reloc));

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 398 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPNEG(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch){

                        		BPPOS(1, 0, 0, raddr);		

                        		}else

                        		BPPOS(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 25 & 0xf) -- cond at 0 --*/ 
                break;
              case 4: 
                if ((MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ == 0 && 
                  (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) { 
                  nextPC = 4 + MATCH_p; 
                  
                  #line 646 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   fNOP(  );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                else 
                  goto MATCH_label_a1;  /*opt-block+*/
                
                break;
              case 6: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */) {
                    case 0: case 1: case 2: case 4: case 6: case 7: case 8: 
                    case 9: case 10: case 11: case 13: case 15: 
                      goto MATCH_label_a0; break;
                    case 3: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 0) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 522 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FPBUGE(0, 0, 0, raddr);		

                        	} else {

                        		if (pc == back_branch) {

                        		FPBUL(1, 0, 0, raddr);

                        		} else

                        		FPBUL(0, 0, 0, raddr);

                        	}

                        //| "FBUL,a" (reloc) => FBULA(relocate_target(reloc));

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    case 5: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 483 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                        

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		FPBULE(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        		FPBUG_a(1, 0, 0, raddr);		

                        		} else

                        		FPBUG_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 474 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FPBULE(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        		FPBUG(1, 0, 0, raddr);		

                        		} else

                        		FPBUG(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 12: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 507 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                        

                        	if (relocate_target(reloc, pc, raddr, 1)) {

                        		// branch profiled to be taken and not a back branch

                        		FPBUL(0, 0, 0, raddr);

                        		fNOP();

                        	} else {

                        		if (pc == back_branch) {

                        		FPBUGE_a(1, 0, 0, raddr);		

                        		} else

                        		FPBUGE_a(0, 0, 0, raddr);		

                        		if ((unsigned)pc != back_branch) {

                        			// do delay slot since its not in the trace

                        			encode(nextPC);

                        		}

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 498 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FPBUL(0, 0, 0, raddr);

                        	} else {

                        		if (pc == back_branch) {

                        		FPBUGE(1, 0, 0, raddr);		

                        		} else

                        		FPBUGE(0, 0, 0, raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 14: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 0) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 532 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                        

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FPBUG(0, 0, 0, raddr);		

                        	} else {

                        		if (pc == back_branch) {

                        		FPBULE(1, 0, 0, raddr);

                        		} else

                        		FPBULE(0, 0, 0, raddr);

                        	}

                        //| "FBULE,a" (reloc) => FBULEA(relocate_target(reloc));

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 25 & 0xf) -- cond at 0 --*/ 
                break;
              default: assert(0);
            } /* (MATCH_w_32_0 >> 22 & 0x7) -- op2 at 0 --*/ 
          break;
        case 1: 
          { 
            unsigned reloc = 
              4 * sign_extend((MATCH_w_32_0 & 0x3fffffff) /* disp30 at 0 */, 
                          30) + addressToPC(MATCH_p);
            nextPC = 4 + MATCH_p; 
            
            #line 434 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
             	 

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

            	

            
            
            
          }
          
          break;
        case 2: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f) /* op3 at 0 */) {
              case 0: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 76 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ADD( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 80 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   AND( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 647 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   OR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 727 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   XOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 694 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SUB( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 82 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ANDN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 649 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ORN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 725 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   XNOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 8: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 78 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                    ADDX( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 9: case 13: case 25: case 29: case 34: case 35: case 44: 
              case 45: case 46: case 47: case 54: case 55: case 57: case 59: 
              case 62: case 63: 
                goto MATCH_label_a0; break;
              case 10: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 719 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   UMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 11: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 682 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 12: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 696 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SUBX( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 14: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 717 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   UDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 678 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 16: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 77 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ADDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 17: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 81 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ANDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 18: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 648 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 19: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 728 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   XORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 20: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 695 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SUBcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 21: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 83 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ANDNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 22: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 650 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ORNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 23: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 726 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   XNORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 24: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 79 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ADDXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 26: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 720 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   UMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 27: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 683 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 28: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 697 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SUBXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 30: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 718 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   UDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 31: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 679 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 701 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   TADDcc( rs1, encode_roi( reg_or_imm ), rd );

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

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 715 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   TSUBcc( rs1, encode_roi( reg_or_imm ), rd );

                  //| "TVC" (eaddr) => abort(); //executeTVC( relocate_eaddr( eaddr ) );

                  //| "TVS" (eaddr) => abort(); //executeTVS( relocate_eaddr( eaddr ) );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 645 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   MULScc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 681 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SLL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 685 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SRL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 684 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SRA( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 40: 
                
                  switch((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */) {
                    case 0: 
                      { 
                        unsigned rd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 654 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         RDY( rd );

                        
                        
                        
                      }
                      
                      break;
                    case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
                    case 8: case 9: case 10: case 11: case 12: case 13: 
                    case 14: case 16: case 17: case 18: case 19: case 20: 
                    case 21: case 22: case 23: case 24: case 25: case 26: 
                    case 27: case 28: case 29: case 30: case 31: 
                      goto MATCH_label_a0; break;
                    case 15: 
                      if ((MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) { 
                        nextPC = 4 + MATCH_p; 
                        
                        #line 688 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         STBAR(  );

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 14 & 0x1f) -- rs1 at 0 --*/ 
                break;
              case 41: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 651 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   RDPSR( rd );

                  
                  
                  
                }
                
                break;
              case 42: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 653 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   RDWIM( rd );

                  
                  
                  
                }
                
                break;
              case 43: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 652 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   RDTBR( rd );

                  
                  
                  
                }
                
                break;
              case 48: 
                if (1 <= (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ && 
                  (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ < 32) 
                  goto MATCH_label_a0;  /*opt-block+*/
                else { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 724 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   WRY( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                
                break;
              case 49: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 721 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   WRPSR( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 50: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 723 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   WRWIM( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 722 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   WRTBR( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 52: 
                if (111 <= (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ && 
                  (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ < 196 || 
                  212 <= (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ && 
                  (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ < 512) 
                  goto MATCH_label_a0;  /*opt-block+*/
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */) {
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
                      goto MATCH_label_a0; break;
                    case 1: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 558 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FMOVs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 562 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FNEGs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 445 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FABSs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 41: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 569 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSQRTs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 42: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 567 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSQRTd( fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 43: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 568 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSQRTq( fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 65: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 449 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FADDs( fs1s, fs2s, fds );

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

                        
                        
                        
                      }
                      
                      break;
                    case 66: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 446 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FADDd( fs1d, fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 67: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 447 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FADDq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 69: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 575 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSUBs( fs1s, fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 70: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 573 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSUBd( fs1d, fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 71: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 574 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FSUBq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 73: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 561 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FMULs( fs1s, fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 74: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 559 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FMULd( fs1d, fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 75: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 560 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FMULq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 77: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 549 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FDIVs( fs1s, fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 78: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 547 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FDIVd( fs1d, fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 79: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 548 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FDIVq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 105: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 566 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FsMULd( fs1d, fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 110: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 550 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FdMULq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 196: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 556 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FiTOs( fs2s, fds );

                        //| "FLUSH" (eaddr) => FLUSH(eaddr); 

                        
                        
                        
                      }
                      
                      break;
                    case 198: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 553 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FdTOs( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 199: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 565 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FqTOs( fs2q, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 200: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 554 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FiTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 201: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 570 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FsTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 203: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 563 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FqTOd( fs2q, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 204: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 555 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FiTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 205: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 572 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FsTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 206: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 552 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FdTOq( fs2d, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 209: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 571 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FsTOi( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 210: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 551 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FdTOi( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 211: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 564 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FqTOi( fs2q, fds );

                        
                        
                        
                      }
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 5 & 0x1ff) -- opf at 0 --*/ 
                break;
              case 53: 
                if (0 <= (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ && 
                  (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ < 81 || 
                  88 <= (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ && 
                  (MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */ < 512) 
                  goto MATCH_label_a0;  /*opt-block+*/
                else 
                  switch((MATCH_w_32_0 >> 5 & 0x1ff) /* opf at 0 */) {
                    case 84: 
                      goto MATCH_label_a0; break;
                    case 81: 
                      { 
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 546 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 82: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 541 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 83: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 545 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPq( fs1q, fs2q );

                        
                        
                        
                      }
                      
                      break;
                    case 85: 
                      { 
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 544 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPEs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 86: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 542 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPEd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 87: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 543 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                         FCMPEq( fs1q, fs2q );

                        
                        
                        
                      }
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 5 & 0x1ff) -- opf at 0 --*/ 
                break;
              case 56: 
                if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
                  
                    switch((MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */) {
                      case 0: 
                        
                          switch((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */) {
                            case 0: case 1: case 2: case 3: case 4: case 5: 
                            case 6: case 7: case 8: case 9: case 10: case 11: 
                            case 12: case 13: case 14: case 16: case 17: 
                            case 18: case 19: case 20: case 21: case 22: 
                            case 23: case 24: case 25: case 26: case 27: 
                            case 28: case 29: case 30: 
                              goto MATCH_label_a2; break;
                            case 15: 
                              if ((MATCH_w_32_0 & 0x1fff) 
                                      /* simm13 at 0 */ == 8) { 
                                nextPC = 4 + MATCH_p; 
                                
                                #line 600 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                                

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

                                			#ifdef CONSERVATIVE

                                			int scratch = 5;

                                			ADD(15, imode(8), scratch);		

                                			indirect_jump(pc, scratch);

                                			#else

                                			if (is_nop(nextPC))

                                				frag_inst++;		// remove the delay slot aswell	

                                			#endif

                                		}

                                	}

                                

                                
                                
                                
                              } /*opt-block*//*opt-block+*/
                              else 
                                goto MATCH_label_a2;  /*opt-block+*/
                              
                              break;
                            case 31: 
                              if ((MATCH_w_32_0 & 0x1fff) 
                                      /* simm13 at 0 */ == 8) { 
                                nextPC = 4 + MATCH_p; 
                                
                                #line 577 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                                 

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

                                			#ifdef CONSERVATIVE

                                			int scratch = 5;

                                			ADD(31, imode(8), scratch);		

                                			indirect_jump(pc, scratch);

                                			#else

                                			if (is_nop(nextPC))

                                				frag_inst++;		// remove the delay slot aswell	

                                			#endif

                                		}

                                	}

                                	

                                
                                
                                
                              } /*opt-block*//*opt-block+*/
                              else 
                                goto MATCH_label_a2;  /*opt-block+*/
                              
                              break;
                            default: assert(0);
                          } /* (MATCH_w_32_0 >> 14 & 0x1f) -- rs1 at 0 --*/ 
                        break;
                      case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
                      case 8: case 9: case 10: case 11: case 12: case 13: 
                      case 14: case 16: case 17: case 18: case 19: case 20: 
                      case 21: case 22: case 23: case 24: case 25: case 26: 
                      case 27: case 28: case 29: case 30: case 31: 
                        goto MATCH_label_a2; break;
                      case 15: 
                        goto MATCH_label_a3; break;
                      default: assert(0);
                    } /* (MATCH_w_32_0 >> 25 & 0x1f) -- rd at 0 --*/  
                else 
                  if ((MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 15) 
                    goto MATCH_label_a3;  /*opt-block+*/
                  else 
                    goto MATCH_label_a2;  /*opt-block+*/ /*opt-block+*/
                break;
              case 58: 
                if ((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */ == 8 && 
                  (0 <= (MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ && 
                  (MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ < 2)) { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  nextPC = 4 + MATCH_p; 
                  
                  #line 699 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   TA( encode_eaddr(eaddr) );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                else 
                  goto MATCH_label_a0;  /*opt-block+*/
                
                break;
              case 60: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 657 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   

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

                  
                  
                  
                }
                
                break;
              case 61: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 655 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   RESTORE( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              default: assert(0);
            } /* (MATCH_w_32_0 >> 19 & 0x3f) -- op3 at 0 --*/ 
          break;
        case 3: 
          
            switch((MATCH_w_32_0 >> 19 & 0x3f) /* op3 at 0 */) {
              case 0: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 635 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LD( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 643 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDUB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 644 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDUH( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 636 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDD( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 686 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   ST( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 687 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   STB( rd, encode_eaddr(eaddr));

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 693 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   STH( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 689 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   STD( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 8: case 11: case 12: case 14: case 16: case 17: case 18: 
              case 19: case 20: case 21: case 22: case 23: case 24: case 25: 
              case 26: case 27: case 28: case 29: case 30: case 31: case 33: 
              case 34: case 37: case 38: case 40: case 41: case 42: case 43: 
              case 44: case 45: case 46: case 47: case 48: case 49: case 50: 
              case 51: case 52: case 53: case 54: case 55: case 56: case 57: 
              case 58: case 59: case 60: case 61: case 62: case 63: 
                goto MATCH_label_a0; break;
              case 9: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 640 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDSB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 641 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDSH( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 13: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 642 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDSTUB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 698 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   SWAP_( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 638 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDF( encode_eaddr(eaddr), fds );

                  // | "LDFSR" (eaddr) => LDFSR(encode_eaddr(eaddr),);

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 637 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   LDDF( encode_eaddr(eaddr), fdd );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 691 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                    STF( fds, encode_eaddr(eaddr) );

                  //| "STFSR" (eaddr) => executeSTFSR( encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 690 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
                   STDF( fdd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              default: assert(0);
            } /* (MATCH_w_32_0 >> 19 & 0x3f) -- op3 at 0 --*/ 
          break;
        default: assert(0);
      } /* (MATCH_w_32_0 >> 30 & 0x3) -- op at 0 --*/ 
    
  }goto MATCH_finished_a; 
  
  MATCH_label_a0: (void)0; /*placeholder for label*/ 
    { 
      nextPC = MATCH_p; 
      
      #line 730 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
      

      	cerr << "Relocator can not generate this instructions." << endl;

      	unimplemented(pc);

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a1: (void)0; /*placeholder for label*/ 
    { 
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      unsigned val = (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ << 10;
      nextPC = 4 + MATCH_p; 
      
      #line 680 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       decode_sethi( val, rd );

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a2: (void)0; /*placeholder for label*/ 
    { 
      unsigned eaddr = addressToPC(MATCH_p);
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      nextPC = 4 + MATCH_p; 
      
      #line 623 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       

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

      	

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a3: (void)0; /*placeholder for label*/ 
    { 
      unsigned eaddr = addressToPC(MATCH_p);
      nextPC = 4 + MATCH_p; 
      
      #line 427 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
       

      	int scratch = 5;	

      	int o7 = 15; 

      	decode_sethi((unsigned)pc, o7);

      	ADD(o7, imode((unsigned)pc & 0x000003ff), o7); 

      	process_eaddr(eaddr, scratch);

      	indirect_jump(pc, scratch);

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_finished_a: (void)0; /*placeholder for label*/
  
}

#line 733 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extraV9.m"
return nextPC;
}


