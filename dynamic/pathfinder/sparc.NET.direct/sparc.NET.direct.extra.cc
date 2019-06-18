#define sign_extend(N,SIZE) (((int)((N) << (sizeof(unsigned)*8-(SIZE)))) >> (sizeof(unsigned)*8-(SIZE)))
#include <assert.h>

#line 2 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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


#line 18 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 18 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) { 
      int /* [~4096..4095] */ simm13 = 
        sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
      
      #line 19 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       return imode(simm13);

      
      
      
    } /*opt-block*//*opt-block+*/
    else { 
      unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
      
      #line 20 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       return rmode(rs2);

      
      
      
    } /*opt-block*//*opt-block+*/
    
  }goto MATCH_finished_h; 
  
  MATCH_finished_h: (void)0; /*placeholder for label*/
  
}

#line 24 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

address__Instance encode_eaddr( sint32_t pc )
{


#line 27 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 27 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 28 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
         

        		return absoluteA(simm13);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 30 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
         

        		return dispA(rs1, simm13);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 32 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
        

        		return indirectA(rs1);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 34 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
        

        		return indexA(rs1, rs2);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_g; 
  
  MATCH_finished_g: (void)0; /*placeholder for label*/
  
}

#line 39 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

void process_eaddr(sint32_t pc, unsigned scratch) 
{


#line 42 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 42 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 43 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
         

        		mov(imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 45 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
         

        		ADD(rs1, imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 47 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
        

        		mov(rmode(rs1), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 49 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
        

        		ADD(rs1, rmode(rs2), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_f; 
  
  MATCH_finished_f: (void)0; /*placeholder for label*/
  
}

#line 54 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

int unconditional_annul(sint32_t pc) 
{


#line 57 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 57 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
              
              #line 60 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
               

              		return 1;

              
              
              
            } /*opt-block*//*opt-block+*/
            else 
              goto MATCH_label_e0;  /*opt-block+*/
            
            break;
          case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 9: 
          case 10: case 11: case 12: case 13: case 14: case 15: 
            goto MATCH_label_e0; break;
          case 8: 
            if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1 && 
              (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0) { 
              unsigned reloc = 
                4 * sign_extend((MATCH_w_32_0 & 0x3fffff) /* disp22 at 0 */, 
                            22) + addressToPC(MATCH_p);
              
              #line 58 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
               

              		return 1;

              
              
              
            } /*opt-block*//*opt-block+*/
            else 
              goto MATCH_label_e0;  /*opt-block+*/
            
            break;
          default: assert(0);
        } /* (MATCH_w_32_0 >> 25 & 0xf) -- cond at 0 --*/  
    else 
      goto MATCH_label_e0;  /*opt-block+*/
    
  }goto MATCH_finished_e; 
  
  MATCH_label_e0: (void)0; /*placeholder for label*/ 
    
    #line 62 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    return 0;			

    
     
    goto MATCH_finished_e; 
    
  MATCH_finished_e: (void)0; /*placeholder for label*/
  
}

#line 65 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

int is_nop(sint32_t pc) 
{


#line 68 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 68 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 22 & 0x7) /* op2 at 0 */ == 4 && 
      (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ == 0 && 
      (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0 && 
      (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) 
      
      #line 69 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       return 1;

      
       /*opt-block+*/
    else 
      goto MATCH_label_d0;  /*opt-block+*/
    
  }goto MATCH_finished_d; 
  
  MATCH_label_d0: (void)0; /*placeholder for label*/ 
    
    #line 70 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    return 0;			

    
     
    goto MATCH_finished_d; 
    
  MATCH_finished_d: (void)0; /*placeholder for label*/
  
}

#line 73 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

int is_restore(sint32_t pc) 
{


#line 76 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 76 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 2 && 
      (MATCH_w_32_0 >> 19 & 0x3f) /* op3 at 0 */ == 61 && 
      (0 <= (MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ && 
      (MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ < 2)) { 
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      unsigned reg_or_imm = addressToPC(MATCH_p);
      unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
      
      #line 78 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
      

      		if ((rs1 == 0) && (rd == 0) && (reg_or_imm == 0)) {

      			return 1;

      		} else {

      			return 0;

      		}

      
      
      
    } /*opt-block*//*opt-block+*/
    else 
      goto MATCH_label_c0;  /*opt-block+*/
    
  }goto MATCH_finished_c; 
  
  MATCH_label_c0: (void)0; /*placeholder for label*/ 
    
    #line 83 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    return 0;			

    
     
    goto MATCH_finished_c; 
    
  MATCH_finished_c: (void)0; /*placeholder for label*/
  
}

#line 86 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}

int check_plt(unsigned pc, int& g1) 
{


#line 89 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 89 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 22 & 0x7) /* op2 at 0 */ == 4 && 
      (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ == 0) { 
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      unsigned val = (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ << 10;
      
      #line 91 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       

      		if (rd == 1) {

      			g1 = val;

      			return 1;

      		} else {

      			return 0;

      		}

      
      
      
    } /*opt-block*//*opt-block+*/
    else 
      goto MATCH_label_b0;  /*opt-block+*/
    
  }goto MATCH_finished_b; 
  
  MATCH_label_b0: (void)0; /*placeholder for label*/ 
    
    #line 97 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
    return 0;			

    
     
    goto MATCH_finished_b; 
    
  MATCH_finished_b: (void)0; /*placeholder for label*/
  
}

#line 100 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
}




int encode( sint32_t pc )
{
RAddr raddr;
sint32_t nextPC;


#line 108 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
{ 
  dword MATCH_p = 
    
    #line 108 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                    case 0: 
                      goto MATCH_label_a0; break;
                    case 1: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 176 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 170 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BNE(raddr);

                        	} else {

                        		BE(raddr);		

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
                        
                        #line 266 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 260 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BG(raddr);

                        	} else {

                        		BLE(raddr);		

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
                        
                        #line 248 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 242 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BGE(raddr);

                        	} else {

                        		BL(raddr);		

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
                        
                        #line 284 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 278 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BGU(raddr);

                        	} else {

                        		BLEU(raddr);		

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
                        
                        #line 158 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 152 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BCC(raddr);

                        	} else {

                        		BCS(raddr);		

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
                        
                        #line 353 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 347 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPOS(raddr);

                        	} else {

                        		BNEG(raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 7: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 410 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

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

                        

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 404 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BVC(raddr);

                        	} else {

                        		BVS(raddr);		

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
                        
                        #line 126 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	// emliminate this branch and delay slot, but can't remove this if it is the back branch!!

                        	if ((unsigned)pc == back_branch) {

                        		BA(relocate(reloc, pc));

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
                        
                        #line 118 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	// emliminate this branch, but can't remove this if it is the back branch!!

                        	if ((unsigned)pc == back_branch) {

                        		BA(relocate(reloc, pc));	

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
                        
                        #line 335 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 298 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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
                        
                        #line 194 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 188 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BLE(raddr);

                        	} else {

                        		BG(raddr);		

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
                        
                        #line 212 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 206 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BL(raddr);

                        	} else {

                        		BGE(raddr);		

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
                        
                        #line 230 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 224 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BLEU(raddr);

                        	} else {

                        		BGU(raddr);		

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
                        
                        #line 140 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 134 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BCS(raddr);

                        	} else {

                        		BCC(raddr);		

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
                        
                        #line 371 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

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

                        		

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 365 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         	

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BNEG(raddr);

                        	} else {

                        		BPOS(raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 15: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 391 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 384 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BVS(raddr);

                        	} else {

                        		BVC(raddr);		

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
                  
                  #line 864 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   fNOP(  );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                else 
                  goto MATCH_label_a1;  /*opt-block+*/
                
                break;
              case 6: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */) {
                    case 0: case 8: 
                      goto MATCH_label_a0; break;
                    case 1: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 586 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 580 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBE(raddr);

                        	} else {

                        		FBNE(raddr);		

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
                        
                        #line 564 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 558 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUE(raddr);

                        	} else {

                        		FBLG(raddr);		

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
                        
                        #line 700 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 694 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBGE(raddr);		

                        	} else {

                        		FBUL(raddr);

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
                        
                        #line 525 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 519 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUGE(raddr);

                        	} else {

                        		FBL(raddr);		

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
                        
                        #line 662 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 656 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBLE(raddr);

                        	} else {

                        		FBUG(raddr);		

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
                        
                        #line 487 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 481 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBULE(raddr);

                        	} else {

                        		FBG(raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 7: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 624 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 618 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBO(raddr);

                        	} else {

                        		FBU(raddr);		

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
                        
                        #line 468 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 462 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBNE(raddr);

                        	} else {

                        		FBE(raddr);		

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
                        
                        #line 643 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 637 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBLG(raddr);

                        	} else {

                        		FBUE(raddr);		

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
                        
                        #line 506 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 500 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUL(raddr);

                        	} else {

                        		FBGE(raddr);		

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
                        
                        #line 681 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 675 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBL(raddr);

                        	} else {

                        		FBUGE(raddr);		

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
                        
                        #line 545 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 538 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUG(raddr);

                        	} else {

                        		FBLE(raddr);		

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
                        
                        #line 719 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 713 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                        

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBG(raddr);		

                        	} else {

                        		FBULE(raddr);

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 15: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 605 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

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

                        	

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 599 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBU(raddr);

                        	} else {

                        		FBO(raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
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
            
            #line 430 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
             	 

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
                  
                  #line 109 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ADD( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 113 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   AND( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 865 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   OR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 945 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   XOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 912 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SUB( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 115 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ANDN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 867 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ORN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 943 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   XNOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 8: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 111 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 937 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   UMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 11: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 900 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 12: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 914 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SUBX( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 14: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 935 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   UDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 896 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 16: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 110 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ADDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 17: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 114 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ANDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 18: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 866 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 19: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 946 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   XORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 20: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 913 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SUBcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 21: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 116 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ANDNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 22: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 868 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ORNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 23: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 944 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   XNORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 24: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 112 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ADDXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 26: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 938 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   UMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 27: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 901 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 28: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 915 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SUBXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 30: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 936 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   UDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 31: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 897 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 919 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 933 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 863 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   MULScc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 899 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SLL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 903 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SRL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 902 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 872 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 906 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 869 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   RDPSR( rd );

                  
                  
                  
                }
                
                break;
              case 42: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 871 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   RDWIM( rd );

                  
                  
                  
                }
                
                break;
              case 43: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 870 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 942 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   WRY( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                
                break;
              case 49: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 939 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   WRPSR( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 50: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 941 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   WRWIM( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 940 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 748 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FMOVs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 752 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FNEGs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 454 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FABSs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 41: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 759 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FSQRTs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 42: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 757 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FSQRTd( fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 43: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 758 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 458 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FADDs( fs1s, fs2s, fds );

                        //| "FBA" (reloc) => 

                        //| "FBA,a" (reloc) => 

                        

                        
                        
                        
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
                        
                        #line 455 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 456 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 765 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 763 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 764 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 751 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 749 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 750 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 739 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 737 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 738 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 756 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 740 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FdMULq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 196: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 746 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 743 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FdTOs( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 199: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 755 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FqTOs( fs2q, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 200: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 744 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FiTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 201: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 760 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FsTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 203: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 753 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FqTOd( fs2q, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 204: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 745 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FiTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 205: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 762 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FsTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 206: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 742 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FdTOq( fs2d, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 209: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 761 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FsTOi( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 210: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 741 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FdTOi( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 211: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 754 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                        
                        #line 736 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FCMPs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 82: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 731 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FCMPd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 83: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 735 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FCMPq( fs1q, fs2q );

                        
                        
                        
                      }
                      
                      break;
                    case 85: 
                      { 
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 734 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FCMPEs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 86: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 732 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                         FCMPEd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 87: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 733 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                                
                                #line 790 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                                

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

                                

                                
                                
                                
                              } /*opt-block*//*opt-block+*/
                              else 
                                goto MATCH_label_a2;  /*opt-block+*/
                              
                              break;
                            case 31: 
                              if ((MATCH_w_32_0 & 0x1fff) 
                                      /* simm13 at 0 */ == 8) { 
                                nextPC = 4 + MATCH_p; 
                                
                                #line 767 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                                 

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
                  
                  #line 917 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 875 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   

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
                  
                  #line 873 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
                  
                  #line 853 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LD( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 861 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDUB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 862 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDUH( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 854 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDD( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 904 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   ST( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 905 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   STB( rd, encode_eaddr(eaddr));

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 911 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   STH( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 907 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   STD( rd, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 8: case 11: case 12: case 14: case 16: case 17: case 18: 
              case 19: case 20: case 21: case 22: case 23: case 24: case 25: 
              case 26: case 27: case 28: case 29: case 30: case 31: case 34: 
              case 38: case 40: case 41: case 42: case 43: case 44: case 45: 
              case 46: case 47: case 48: case 49: case 50: case 51: case 52: 
              case 53: case 54: case 55: case 56: case 57: case 58: case 59: 
              case 60: case 61: case 62: case 63: 
                goto MATCH_label_a0; break;
              case 9: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 858 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDSB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 859 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDSH( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 13: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 860 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDSTUB( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 916 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   SWAP_( encode_eaddr(eaddr), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 856 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDF( encode_eaddr(eaddr), fds );

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  nextPC = 4 + MATCH_p; 
                  
                  #line 857 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDFSR(encode_eaddr(eaddr));

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 855 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   LDDF( encode_eaddr(eaddr), fdd );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 909 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                    STF( fds, encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  nextPC = 4 + MATCH_p; 
                  
                  #line 910 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
                   STFSR( encode_eaddr(eaddr) );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 908 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
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
      
      #line 948 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
      

      	cerr << "Relocator can not generate this instructions." << endl;

      	unimplemented(pc);

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a1: (void)0; /*placeholder for label*/ 
    { 
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      unsigned val = (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ << 10;
      nextPC = 4 + MATCH_p; 
      
      #line 898 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       decode_sethi( val, rd );

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a2: (void)0; /*placeholder for label*/ 
    { 
      unsigned eaddr = addressToPC(MATCH_p);
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      nextPC = 4 + MATCH_p; 
      
      #line 813 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       

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

      	

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a3: (void)0; /*placeholder for label*/ 
    { 
      unsigned eaddr = addressToPC(MATCH_p);
      nextPC = 4 + MATCH_p; 
      
      #line 423 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
       

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

#line 952 "dynamic/pathfinder/sparc.NET.direct/sparc.NET.direct.extra.m"
the_end:
return nextPC;
}


