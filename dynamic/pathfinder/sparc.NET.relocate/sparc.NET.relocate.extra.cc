#define sign_extend(N,SIZE) (((int)((N) << (sizeof(unsigned)*8-(SIZE)))) >> (sizeof(unsigned)*8-(SIZE)))
#include <assert.h>

#line 2 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
#include "sparc.NET.relocate.cc"

reg_or_imm_Instance encode_roi( sint32_t pc )
{


#line 5 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 5 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) { 
      int /* [~4096..4095] */ simm13 = 
        sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
      
      #line 6 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
       return imode(simm13);

      
      
      
    } /*opt-block*//*opt-block+*/
    else { 
      unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
      
      #line 7 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
       return rmode(rs2);

      
      
      
    } /*opt-block*//*opt-block+*/
    
  }goto MATCH_finished_f; 
  
  MATCH_finished_f: (void)0; /*placeholder for label*/
  
}

#line 11 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
}


void process_eaddr(sint32_t pc, unsigned scratch) 
{


#line 15 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 15 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 16 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        		mov(imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 18 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        		ADD(rs1, imode(simm13), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 20 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        		mov(rmode(rs1), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 22 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        		ADD(rs1, rmode(rs2), scratch);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_e; 
  
  MATCH_finished_e: (void)0; /*placeholder for label*/
  
}

#line 27 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
}

	
address__Instance relocate_eaddr(sint32_t pc, sint32_t scratch)
{

if (delay_slot_relocated) {
	delay_slot_relocated = 0;
	scratch = 6; 


#line 35 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 35 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 36 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        		return dispA(scratch, simm13);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 38 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        		return dispA(scratch, simm13);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 40 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        		return indexA(scratch, rs1);

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 42 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        		return indexA(scratch, rs2);

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_d; 
  
  MATCH_finished_d: (void)0; /*placeholder for label*/
  
}

#line 46 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"

}

// if scratch is %g0, we are in big trouble!
//if (scratch == 0) 
	scratch = 6;   // in cases like ld %o0, %o0!!!!
// store instruction don't have a scratch register!! so they should be 0
	


#line 53 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 53 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
      if ((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */ == 0) { 
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 55 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        	if (do_relocate_eaddr(pc)) {

        		decode_sethi((unsigned)mem, scratch);

        //		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 

        		return dispA(scratch, simm13);

        	} else {

        		abort();

        	}

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        int /* [~4096..4095] */ simm13 = 
          sign_extend((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */, 13);
        
        #line 63 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
         

        	if (do_relocate_eaddr(pc)) {

        		decode_sethi((unsigned)mem, scratch);

        //		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 

        		ADD(scratch, rmode(rs1), scratch); 	

        		return dispA(scratch, simm13);

        	} else {

        		return dispA(rs1, simm13);		

        	} 

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    else 
      if ((MATCH_w_32_0 & 0x1f) /* rs2 at 0 */ == 0) { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        
        #line 72 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        	if (do_relocate_eaddr(pc)) {

        		decode_sethi((unsigned)mem, scratch);

        //		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 

        		return indexA(scratch, rs1);

        	} else {	

        		return indirectA(rs1);

        	} 

        
        
        
      } /*opt-block*//*opt-block+*/
      else { 
        unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
        unsigned rs2 = (MATCH_w_32_0 & 0x1f) /* rs2 at 0 */;
        
        #line 80 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
        

        	if (do_relocate_eaddr(pc)) {

        		decode_sethi((unsigned)mem, scratch);

        //		ADD(scratch, imode((unsigned)mem & 0x000003ff), scratch); 

        		ADD(scratch, rmode(rs1), scratch); 		

        		return indexA(scratch, rs2);

        	} else {		

        		return indexA(rs1, rs2);

        	} 

        
        
        
      } /*opt-block*//*opt-block+*/ /*opt-block+*/
    
  }goto MATCH_finished_c; 
  
  MATCH_finished_c: (void)0; /*placeholder for label*/
  
}

#line 91 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
}



void encode_delay_load_store( sint32_t pc ) 
{
	sint32_t nextPC;


#line 97 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 97 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
    pc
    ;
  unsigned MATCH_w_32_0;
  { 
    MATCH_w_32_0 = getDword(MATCH_p); 
    
      switch((MATCH_w_32_0 >> 19 & 0x3f) /* op3 at 0 */) {
        case 0: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 98 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 1: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 105 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr,0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 2: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 106 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr,0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 3: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 99 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 4: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 107 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 5: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 108 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 6: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 112 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 7: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 109 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr( eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 8: case 11: case 12: case 14: case 15: case 16: case 17: 
        case 18: case 19: case 20: case 21: case 22: case 23: case 24: 
        case 25: case 26: case 27: case 28: case 29: case 30: case 31: 
        case 33: case 34: case 37: case 38: case 40: case 41: case 42: 
        case 43: case 44: case 45: case 46: case 47: case 48: case 49: 
        case 50: case 51: case 52: case 53: case 54: case 55: case 56: 
        case 57: case 58: case 59: case 60: case 61: case 62: case 63: 
          goto MATCH_label_b0; break;
        case 9: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 102 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 10: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 103 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr,0 ); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 13: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 104 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr,0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 32: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 101 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 35: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 100 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 36: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 111 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        case 39: 
          if (0 <= (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ && 
            (MATCH_w_32_0 >> 30 & 0x3) /* op at 0 */ < 3) 
            goto MATCH_label_b0;  /*opt-block+*/
          else { 
            unsigned eaddr = addressToPC(MATCH_p);
            unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
            nextPC = 4 + MATCH_p; 
            
            #line 110 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             relocate_eaddr(eaddr, 0); delay_slot_relocated = 1;

            
            
            
          } /*opt-block*//*opt-block+*/
          
          break;
        default: assert(0);
      } /* (MATCH_w_32_0 >> 19 & 0x3f) -- op3 at 0 --*/ 
    
  }goto MATCH_finished_b; 
  
  MATCH_label_b0: (void)0; /*placeholder for label*/ 
    { 
      nextPC = MATCH_p; 
      
      #line 114 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
      

      		// other instructions

      		;

      
      
      
    } 
    goto MATCH_finished_b; 
    
  MATCH_finished_b: (void)0; /*placeholder for label*/
  
}

#line 118 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
}


int encode( sint32_t pc )
{
RAddr raddr;
sint32_t nextPC;


#line 124 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
{ 
  dword MATCH_p = 
    
    #line 124 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 193 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 185 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 303 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 295 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 281 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 273 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 325 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 317 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 171 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 163 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 403 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 395 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BPOS(raddr);

                        	} else {

                        		BNEG(raddr);		

                        	}

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      
                      break;
                    case 8: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 0) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 134 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	// emliminate this branch, but can't remove this if it is the back branch!!

                        	if (reloc < pc)

                        		BA(relocate(reloc, pc));	

                        //| "BA,a" (reloc) => 

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    case 9: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 381 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 341 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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
                        
                        #line 215 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 207 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 237 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 229 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 259 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                        

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 251 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 149 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 141 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

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
                        
                        #line 425 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                        

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 417 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         	

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		BNEG(raddr);

                        	} else {

                        		BPOS(raddr);		

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
                  
                  #line 586 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 513 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUGE(raddr);		

                        	} else {

                        		FBUL(raddr);

                        	}

                        //| "FBUL,a" (reloc) => FBULA(relocate_target(reloc));

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    case 5: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 0) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 482 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBULE(raddr);

                        	} else {

                        		FBUG(raddr);		

                        	}

                        //| "FBUG,a" (reloc) => FBUGA(relocate_target(reloc));

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else 
                        goto MATCH_label_a0;  /*opt-block+*/
                      
                      break;
                    case 12: 
                      if ((MATCH_w_32_0 >> 29 & 0x1) /* a at 0 */ == 1) { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 499 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                        

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

                        
                        
                        
                      } /*opt-block*//*opt-block+*/
                      else { 
                        unsigned reloc = 
                          4 * sign_extend(
                                      (MATCH_w_32_0 & 0x3fffff) 
                                            /* disp22 at 0 */, 22) + 
                          addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 491 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUL(raddr);

                        	} else {

                        		FBUGE(raddr);		

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
                        
                        #line 522 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                        

                        	if (relocate_delay_slot(nextPC))

                        		encode_delay_load_store(nextPC);

                        	if (relocate_target(reloc, pc, raddr)) {

                        		FBUG(raddr);		

                        	} else {

                        		FBULE(raddr);

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
            
            #line 444 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
             	 

            	//if (relocate_delay_slot(nextPC))

            	//	encode_delay_load_store(nextPC);

            	//call(relocate(reloc));

            	

            	// now just generate: set guest pc => %o7

            	int o7 = 15; 

            	decode_sethi((unsigned)pc, o7);

            	ADD(o7, imode((unsigned)pc & 0x000003ff), o7); 

            		

            
            
            
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
                  
                  #line 125 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ADD( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 129 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   AND( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 587 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   OR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 683 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   XOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 650 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SUB( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 131 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ANDN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 589 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ORN( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 681 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   XNOR( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 8: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 127 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                  
                  #line 675 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   UMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 11: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 638 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SMUL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 12: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 652 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SUBX( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 14: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 673 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   UDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 634 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SDIV( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 16: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 126 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ADDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 17: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 130 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ANDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 18: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 588 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 19: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 684 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   XORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 20: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 651 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SUBcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 21: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 132 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ANDNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 22: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 590 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ORNcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 23: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 682 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   XNORcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 24: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 128 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ADDXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 26: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 676 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   UMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 27: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 639 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SMULcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 28: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 653 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SUBXcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 30: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 674 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   UDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 31: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 635 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SDIVcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 656 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   TADDcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 33: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 670 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   TSUBcc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 585 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   MULScc( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 37: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 637 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SLL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 38: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 641 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SRL( rs1, encode_roi( reg_or_imm ), rd );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 640 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 594 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 644 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                  
                  #line 591 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   RDPSR( rd );

                  
                  
                  
                }
                
                break;
              case 42: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 593 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   RDWIM( rd );

                  
                  
                  
                }
                
                break;
              case 43: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 592 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                  
                  #line 680 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   WRY( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                } /*opt-block*//*opt-block+*/
                
                break;
              case 49: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 677 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   WRPSR( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 50: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 679 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   WRWIM( rs1, encode_roi( reg_or_imm ) );

                  
                  
                  
                }
                
                break;
              case 51: 
                { 
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 678 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 547 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FMOVs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 551 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FNEGs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 453 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FABSs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 41: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 558 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FSQRTs( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 42: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 556 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FSQRTd( fs2d, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 43: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 557 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 457 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 454 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 455 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 564 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 562 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 563 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 550 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 548 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 549 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 538 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 536 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 537 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 555 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 539 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FdMULq( fs1q, fs2q, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 196: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 545 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FiTOs( fs2s, fds );

                        //| "FLUSH" (eaddr) => FLUSH( relocate_eaddr( eaddr ) ); 

                        
                        
                        
                      }
                      
                      break;
                    case 198: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 542 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FdTOs( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 199: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 554 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FqTOs( fs2q, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 200: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 543 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FiTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 201: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 559 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FsTOd( fs2s, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 203: 
                      { 
                        unsigned fdd = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 552 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FqTOd( fs2q, fdd );

                        
                        
                        
                      }
                      
                      break;
                    case 204: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 544 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FiTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 205: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 561 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FsTOq( fs2s, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 206: 
                      { 
                        unsigned fdq = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fdq at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 541 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FdTOq( fs2d, fdq );

                        
                        
                        
                      }
                      
                      break;
                    case 209: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 560 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FsTOi( fs2s, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 210: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 540 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FdTOi( fs2d, fds );

                        
                        
                        
                      }
                      
                      break;
                    case 211: 
                      { 
                        unsigned fds = 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 553 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                        
                        #line 535 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 82: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 530 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 83: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 534 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPq( fs1q, fs2q );

                        
                        
                        
                      }
                      
                      break;
                    case 85: 
                      { 
                        unsigned fs1s = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1s at 0 */;
                        unsigned fs2s = (MATCH_w_32_0 & 0x1f) /* fs2s at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 533 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPEs( fs1s, fs2s );

                        
                        
                        
                      }
                      
                      break;
                    case 86: 
                      { 
                        unsigned fs1d = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1d at 0 */;
                        unsigned fs2d = (MATCH_w_32_0 & 0x1f) /* fs2d at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 531 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPEd( fs1d, fs2d );

                        
                        
                        
                      }
                      
                      break;
                    case 87: 
                      { 
                        unsigned fs1q = 
                          (MATCH_w_32_0 >> 14 & 0x1f) /* fs1q at 0 */;
                        unsigned fs2q = (MATCH_w_32_0 & 0x1f) /* fs2q at 0 */;
                        nextPC = 4 + MATCH_p; 
                        
                        #line 532 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         FCMPEq( fs1q, fs2q );

                        
                        
                        
                      }
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 5 & 0x1ff) -- opf at 0 --*/ 
                break;
              case 56: 
                if ((MATCH_w_32_0 >> 13 & 0x1) /* i at 0 */ == 1) 
                  
                    switch((MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */) {
                      case 0: case 1: case 2: case 3: case 4: case 5: case 6: 
                      case 7: case 8: case 9: case 10: case 11: case 12: 
                      case 13: case 14: case 16: case 17: case 18: case 19: 
                      case 20: case 21: case 22: case 23: case 24: case 25: 
                      case 26: case 27: case 28: case 29: case 30: 
                        goto MATCH_label_a2; break;
                      case 15: 
                        if ((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */ == 8 && 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) { 
                          nextPC = 4 + MATCH_p; 
                          
                          #line 566 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                           ;

                          
                          
                          
                        } /*opt-block*//*opt-block+*/
                        else 
                          goto MATCH_label_a2;  /*opt-block+*/
                        
                        break;
                      case 31: 
                        if ((MATCH_w_32_0 & 0x1fff) /* simm13 at 0 */ == 8 && 
                          (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */ == 0) { 
                          nextPC = 4 + MATCH_p; 
                          
                          #line 565 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                           ;

                          
                          
                          
                        } /*opt-block*//*opt-block+*/
                        else 
                          goto MATCH_label_a2;  /*opt-block+*/
                        
                        break;
                      default: assert(0);
                    } /* (MATCH_w_32_0 >> 14 & 0x1f) -- rs1 at 0 --*/  
                else 
                  goto MATCH_label_a2;  /*opt-block+*/
                break;
              case 58: 
                
                  switch((MATCH_w_32_0 >> 25 & 0xf) /* cond at 0 */) {
                    case 0: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 666 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTN( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 1: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 659 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTE( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 2: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 664 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTLE( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 3: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 663 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTL( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 4: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 665 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTLEU( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 5: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 658 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTCS( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 6: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 668 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTNEG( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 7: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 672 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTVS( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 8: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 655 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         TA( relocate_eaddr(eaddr, 0) );

                        
                        
                        
                      }
                      
                      break;
                    case 9: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 667 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTNE( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 10: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 660 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTG( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 11: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 661 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTGE( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 12: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 662 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTGU( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 13: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 657 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //TCC( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 14: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 669 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTPOS( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    case 15: 
                      { 
                        unsigned eaddr = addressToPC(MATCH_p);
                        nextPC = 4 + MATCH_p; 
                        
                        #line 671 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                         abort(); //executeTVC( relocate_eaddr( eaddr ) );

                        
                        
                        
                      }
                      
                      break;
                    default: assert(0);
                  } /* (MATCH_w_32_0 >> 25 & 0xf) -- cond at 0 --*/ 
                break;
              case 60: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 597 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   

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

                  
                  
                  
                }
                
                break;
              case 61: 
                { 
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  unsigned reg_or_imm = addressToPC(MATCH_p);
                  unsigned rs1 = (MATCH_w_32_0 >> 14 & 0x1f) /* rs1 at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 595 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
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
                  
                  #line 575 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LD( relocate_eaddr(eaddr, rd), rd );

                  
                  
                  
                }
                
                break;
              case 1: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 583 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDUB( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 2: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 584 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDUH( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 3: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 576 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDD( relocate_eaddr(eaddr, rd), rd );

                  
                  
                  
                }
                
                break;
              case 4: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 642 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   ST( rd, relocate_eaddr( eaddr,0 ) );

                  
                  
                  
                }
                
                break;
              case 5: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 643 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   STB( rd, relocate_eaddr( eaddr,0 ) );

                  
                  
                  
                }
                
                break;
              case 6: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 649 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   STH( rd, relocate_eaddr( eaddr,0 ) );

                  
                  
                  
                }
                
                break;
              case 7: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 645 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   STD( rd, relocate_eaddr( eaddr,0 ) );

                  
                  
                  
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
                  
                  #line 580 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDSB( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 10: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 581 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDSH( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 13: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 582 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDSTUB( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 15: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 654 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   SWAP_( relocate_eaddr( eaddr,rd ), rd );

                  
                  
                  
                }
                
                break;
              case 32: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 578 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDF( relocate_eaddr(eaddr, 0), fds );

                  // | "LDFSR" (eaddr) => LDFSR( relocate_eaddr( eaddr ) );

                  
                  
                  
                }
                
                break;
              case 35: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 577 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   LDDF( relocate_eaddr(eaddr, 0), fdd );

                  
                  
                  
                }
                
                break;
              case 36: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fds = (MATCH_w_32_0 >> 25 & 0x1f) /* fds at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 647 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                    STF( fds, relocate_eaddr(eaddr, 0) );

                  //| "STFSR" (eaddr) => executeSTFSR( relocate_eaddr( eaddr ) );

                  
                  
                  
                }
                
                break;
              case 39: 
                { 
                  unsigned eaddr = addressToPC(MATCH_p);
                  unsigned fdd = (MATCH_w_32_0 >> 25 & 0x1f) /* fdd at 0 */;
                  nextPC = 4 + MATCH_p; 
                  
                  #line 646 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
                   STDF( fdd, relocate_eaddr(eaddr, 0) );

                  
                  
                  
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
      
      #line 686 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
      

      	cerr << "Relocator can not generate this instructions." << endl;

      	unimplemented(pc);

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a1: (void)0; /*placeholder for label*/ 
    { 
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      unsigned val = (MATCH_w_32_0 & 0x3fffff) /* imm22 at 0 */ << 10;
      nextPC = 4 + MATCH_p; 
      
      #line 636 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
       decode_sethi( val, rd );

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_label_a2: (void)0; /*placeholder for label*/ 
    { 
      unsigned eaddr = addressToPC(MATCH_p);
      unsigned rd = (MATCH_w_32_0 >> 25 & 0x1f) /* rd at 0 */;
      nextPC = 4 + MATCH_p; 
      
      #line 568 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
       

      	if (relocate_delay_slot(nextPC))

      		encode_delay_load_store(nextPC);

      		//		abort();  // err load / store could change eaddr

      	int scratch = 5;

      	//LD(relocate_eaddr(eaddr, scratch), scratch);

      	process_eaddr(eaddr, scratch);

      	indirect_jump(pc, scratch);

      
      
      
    } 
    goto MATCH_finished_a; 
    
  MATCH_finished_a: (void)0; /*placeholder for label*/
  
}

#line 689 "dynamic/sparc.NET.relocate/sparc.NET.relocate.extra.m"
return nextPC;
}


