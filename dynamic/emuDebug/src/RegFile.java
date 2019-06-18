import java.io.*;
class RegFile implements Serializable{ 
int r_NF;
int r_ZF;
int r_OF;
int r_CF;
int r_pc;
int r_npc;
int r_AF;
int r_CTI;
int r_CWP;
int r_FGF;
int r_FLF;
int r_FSR;
int r_FZF;
int r_NEXT;
int r_PSR;
int r_TBR;
int r_WIM;
int r_Y;
int rd[] = new int[32]; 
public RegFile(
int I_r_NF, 
int I_r_ZF, 
int I_r_OF, 
int I_r_CF,
int I_r_pc, 
int I_r_npc,
int I_r_AF, 
int I_r_CTI, 
int I_r_CWP, 
int I_r_FGF, 
int I_r_FLF, 
int I_r_FSR, 
int I_r_FZF, 
int I_r_NEXT, 
int I_r_PSR, 
int I_r_TBR, 
int I_r_WIM, 
int I_r_Y,
int I_rd[]) 
{
r_NF = I_r_NF; 
r_ZF = I_r_ZF; 
r_OF = I_r_OF; 
r_CF = I_r_CF; 
r_pc = I_r_pc; 
r_npc = I_r_npc; 
r_AF = I_r_AF; 
r_CTI = I_r_CTI; 
r_CWP = I_r_CWP; 
r_FGF = I_r_FGF; 
r_FLF = I_r_FLF; 
r_FSR = I_r_FSR; 
r_FZF = I_r_FZF; 
r_NEXT = I_r_NEXT; 
r_PSR = I_r_PSR; 
r_TBR = I_r_TBR; 
r_WIM = I_r_WIM; 
r_Y = I_r_Y; 
for (int i = 0; i < I_rd.length; i++) { 
rd[i] = I_rd[i];
 }}
}
