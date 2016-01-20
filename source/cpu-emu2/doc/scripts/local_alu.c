/* 
   alu.c 
   - 21.11.05/BHO1
   bho1 29.12.2006
   bho1 6.12.2007
   bho1 30.11.2007 - clean up
   bho1 24.11.2009 - assembler instruction
   bho1 3.12.2009 - replaced adder with full_adder
   bho1 20.7.2011 - rewrite: minimize global vars, ALU-operations are modeled with fct taking in/out register as parameter
   bho1 6.11.2011 - rewrite flags: adding flags as functional parameter. Now alu is truly a function
   bho1 26.11.2012 - remove bit declaration from op_alu_asl and op_alu_ror as they are unused (this may change later)
   bho1 20.9.2014 cleaned
   
   
   GPL applies

   -->> YOUR FULL NAME HERE  <<--
*/

#include <stdio.h>
#include <string.h>

#include "alu.h"
#include "alu-opcodes.h"
#include "register.h"
#include "flags.h"
int const max_mue_memory = 100;

char mue_memory[100]= "100 Byte - this memory is at your disposal"; /*mue-memory */
char* m = mue_memory;

unsigned int c = 0; 	/* carry bit address    */
unsigned int s = 1;	    /* sum bit address      */
unsigned int c_in = 2;	/* carry in bit address */



/*
  testet ob alle bits im akkumulator auf null gesetzt sind.
  Falls ja wird 1 returniert, ansonsten 0
*/
int zero_test(char accumulator[]){
  int i;
  for(i=0;accumulator[i]!='\0'; i++){
	if(accumulator[i]!='0')		
	  return 0;
  }
  return 1;
}

int apply_zero_flag(char accumulator[], char flags[]){
  int i;
  for(i=0;accumulator[i]!='\0'; i++){
    if(accumulator[i]!='0'){		
      flags[ZFLAG] = '0';
      return 0;
    }
  }
  flags[ZFLAG] = '1';
  return 1;
}


int apply_overflow_flag(char accumulator[], char flags[]){
  //Overflowflag
  if( ( rega[0] == '1' &&
	regb[0] == '1' &&
	regc[0] == '0' )  ||
      ( rega[0] == '0' &&
	regb[0] == '0' &&
	regc[0] == '1' ) ){
    flags[OFLAG] = '1';
  } else {
    flags[OFLAG] = '0';
  }
}

void zsflagging(char* flags,char *acc){
  //Zeroflag
  apply_zero_flag(acc, flags);
  
  //Signflag
  flags[SFLAG] = acc[0];
}

/*
  Halfadder: addiert zwei character p,q und schreibt in 
  den Mue-memory das summen-bit und das carry-bit.
*/
void half_adder(char p, char q){
  char result, carry;
  if (p == '0' && q == '0') {
    result = '0';
    carry = '0';
  }else if(p=='0' && q=='1') {
    result = '1';
    carry = '0';
  }else if(p=='1' && q=='0') {
    result = '1';
    carry = '0';
  } else // p=='1' && q=='1'
    {
      result = '0';
      carry = '1';
    }

  m[c] = carry;
  m[s] = result;
}

/*
  clear mue_memory
*/
void mu_memory_reset(){
  int i;
  
  for(i=0;i<max_mue_memory;i++)
    m[i] = '0';
}

/*
  Reset ALU
  resets registers and calls alu_op_reset 
*/
void op_alu_reset(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  /* clear rega, regb, accumulator, flags */
  for(i=0; i<REG_WIDTH; i++){
    rega[i] = '0';
    regb[i] = '0';
    accumulator[i] = '0';
    flags[i] = '0';
  }
}

/* 
   void adder(char pbit, char qbit, char cbit)
   Adder oder auch Fulladder:
   Nimmt zwei character bits und ein carry-character-bit
   und schreibt das Resultat (summe, carry) in den Mue-speicher
*/
void full_adder(char pbit, char qbit, char cbit){
  half_adder(pbit, qbit);
  if(m[c] == '0' ){
    half_adder(m[s], cbit);
  } else {
    m[s] = cbit;
  }
}

/*
  Invertieren der Character Bits im Register reg
  one_complement(char reg[]) --> NOT(reg)
*/

void one_complement(char reg[]){
  int i;
  for(i=0; i<REG_WIDTH; i++)
	if(reg[i]=='0')
	  reg[i] = '1';
	else
	  reg[i] = '0';
}

/*
  Das zweier-Komplement des Registers reg wird in reg geschrieben
  reg := K2(reg)
*/
void two_complement(char reg[]){
  int i;
  
  one_complement(reg);
  
  for(i=REG_WIDTH-1; i>=0;i--){
	if(reg[i]=='1'){
	  reg[i]='0';
	}else{
	  reg[i] = '1';
	  return;
	}
  }
}

/*
  Die Werte in Register rega und Register regb werden addiert, das
  Resultat wird in Register accumulator geschrieben. Die Flags cflag, 
  oflag, zflag und sflag werden entsprechend gesetzt
  
  accumulator := rega + regb
*/
void op_add(char rega[], char regb[], char accumulator[], char flags[]){
  //adding rega with regb
  int i;

  half_adder(rega[REG_WIDTH-1], regb[REG_WIDTH-1]);
  //  full_adder(rega[REG_WIDTH-1], regb[REG_WIDTH-1], m[c_in]);
  accumulator[REG_WIDTH-1] = m[s];
  for(i=REG_WIDTH-2; i>-1; i--){
	full_adder(rega[i], regb[i], m[c]);
	accumulator[i] = m[s];
  }

  //setting the flags accordingly

  //Carryflag
  if(m[c] == '1')
	setCarryflag(flags);
  else
	clearCarryflag(flags);

  //Zeroflag
  apply_zero_flag(accumulator, flags);

  //Signflag
  flags[SFLAG] = accumulator[0];
  
  // overflow flag
  apply_overflow_flag(char rega[], char regb[], char regc[], char flags[])


}

/*
  op:      ORA
  ex:      25 ORA af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   or with accumulator
  form1:   A:=A or {adr}
  form2:   A <- (A) V M
  form3:   *       A = A | M
  *       P.N = A.7
  *       P.Z = (A==0) ? 1:0
  *

  */

void alu_op_OR(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  for(i=0; i<REG_WIDTH; i++)
    if(rega[i] == '1' || regb[i] == '1')
      accumulator[i] = '1';
    else
      accumulator[i] = '0';

  flags[SFLAG] = accumulator[0]

  apply_zero_flag(regouta, flags);
  
}



/*
  op:      ASL
  ex:      25 ASL af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   arithmetic shift left
  form1:   {adr}:={adr}*2
  form2:   C <- A7, A <- (A) << 1
  form3:   *       P.C = B.7
  *       B = (B << 1) & $FE
  *       P.N = B.7
  *       P.Z = (B==0) ? 1:0
  *

  */

void alu_op_ASL(char rega[], char regb[], char accumulator[], char flags[]){
  int i;

  for(i=1;i<REG_WIDTH;i++)
    regouta[i-1] = regina[i];
  regouta[i-1] = '0';
  if(regina[0] == '1')
    setCarryflag(flags);
  else
    clearCarryflag(flags);
  apply_zero_flag(regouta, flags);

  flags[SFLAG] = accumulator[0];
}




/*
  op:      AND
  ex:      25 AND af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   and (with accumulator)
  form1:   A:=A\&{adr}
  form2:   A <- (A) /\ M
  form3:   *       A = A & M
  *       P.N = A.7
  *       P.Z = (A==0) ? 1:0
  *

  */

void alu_op_AND(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  for(i=0; i<REG_WIDTH; i++){
    if(rega[i] == '1' && regb[i] == '1')
      accumulator[i] = '1';
    else
      accumulator[i] = '0';
  }

  flags[SFLAG] = accumulator[0];
  
  setZeroflag(flags);
  for(i=0; i<REG_WIDTH; i++){
    if(accumulator[i] == '1'){
      clearZeroflag(flags);
      break;
    }
  }

}


/*
  op:      ROL
  ex:      25 ROL af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   rotate left
  form1:   {adr}:={adr}*2+C
  form2:   C <- A7 & A <- A << 1 + C
  form3:   *       t = B.7
  *       B = (B << 1) & $FE
  *       B = B | P.C
  *       P.C = t
  *       P.Z = (B==0) ? 1:0
  *       P.N = B.7
  *  

  */

void alu_op_ROL(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  char bit = regina[0];
  for(i=1;i<REG_WIDTH;i++)
    regouta[i-1] = regina[i];
  regouta[i-1] = 	getCarryflag(flags);
  if(bit == '1')
    setCarryflag(flags);
  else
    clearCarryflag(flags);
  apply_zero_flag(regouta, flags);

  flags[SFLAG] = regouta[0];
  

}


/*
  op:      EOR
  ex:      25 EOR af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   exclusive or (with accumulator)
  form1:   A:=A exor {adr}
  form2:   A <- (A) \-/ M
  form3:   *       A = A ^ M
  *       P.N = A.7
  *       P.Z = (A==0) ? 1:0
  *

  */

void alu_op_EOR(char rega[], char regb[], char accumulator[], char flags[]){
  int i;
  for(i=0; i<REG_WIDTH; i++)
    if((rega[i] == '1' && regb[i] == '1') || (rega[i] == '0' && regb[i] == '0'))
      accumulator[i] = '0';
    else
      accumulator[i] = '1';

  flags[SFLAG] = accumulator[0];
  
  setZeroflag(flags);
  for(i=0; i<REG_WIDTH; i++){
    if(accumulator[i] == '1'){
      clearZeroflag(flags);
      break;
    }
  }

}


/*
  op:      LSR
  ex:      25 LSR af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   logical shift right
  form1:   {adr}:={adr}/2
  form2:   C <- A0, A <- (A) >> 1
  form3:   *       P.N = 0
  *       P.C = B.0
  *       B = (B >> 1) & $7F
  *       P.Z = (B==0) ? 1:0
  *

  */

void alu_op_LSR(char rega[], char regb[], char accumulator[], char flags[]){

}


/*
  op:      ADC
  ex:      25 ADC af -> fe and flags setting
  flags:   **---**
  NVBDIZC
  short:   add with carry
  form1:   A:=A+{adr}
  form2:   A <- (A) + M + C
  form3:   *       t = A + M + P.C
  *       P.V = (A.7!=t.7) ? 1:0
  *       P.N = A.7
  *       P.Z = (t==0) ? 1:0
  *       IF (P.D)
  *         t = bcd(A) + bcd(M) + P.C
  *         P.C = (t>99) ? 1:0
  *       ELSE
  *         P.C = (t>255) ? 1:0
  *       A = t & 0xFF
  *

  */

void alu_op_ADC(char rega[], char regb[], char accumulator[], char flags[]){
  //  m[c_in] = getCarryflag(flags);
  char one[REG_WIDTH+1] = "00000001";
  char saveacc[REG_WIDTH+1] = "00000001";
  char oflag, cflag;
  if(getCarryflag(flags)=='0'){
    op_add(rega, regb, accumulator, flags);
  }else{
    op_add(rega, regb, accumulator, flags);
    cp_register(accumulator, saveacc);
    oflag = flags[OFLAG];
    cflag = flags[CFLAG];
    op_add(saveacc, one, accumulator, flags);

    if(oflag =='1'){
      flags[OFLAG] = '1'
    }
    if(cflag == '1'){
      flags[CFLAG] = '1'
    }
  }
}




/*
  op:      ROR
  ex:      25 ROR af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   rotate right
  form1:   {adr}:={adr}/2+C*128
  form2:   C<-A0 & A<- (A7=C + A>>1)
  form3:   *       t = B.0
  *       B = (B >> 1) & $7F
  *       B = B | ((P.C) ? $80:$00)
  *       P.C = t
  *       P.Z = (B==0) ? 1:0
  *       P.N = B.7
  *

  */

void alu_op_ROR(char rega[], char regb[], char accumulator[], char flags[]){
  int i;

  regouta[0] = getCarryflag(flags);
  for(i=1;i<REG_WIDTH;i++)
    regouta[i] = regina[i-1];

  if(regina[i-1]=='1')
    setCarryflag(flags);
  else
    clearCarryflag(flags);
  apply_zero_flag(regouta, flags);

  flags[SFLAG] = regouta[0];
  

}


/*
  op:      CMP
  ex:      25 CMP af -> fe and flags setting
  flags:   *----**
  NVBDIZC
  short:   compare (with accumulator)
  form1:   A-{adr}
  form2:   (A - M) -> NZC
  form3:   *       t = A - M
  *       P.N = t.7
  *       P.C = (A>=M) ? 1:0
  *       P.Z = (t==0) ? 1:0
  *

  */

void alu_op_CMP(char rega[], char regb[], char accumulator[], char flags[]){
  char dummy[] ="00000000";
  op_sub(regina, reginb, dummy, flags);  

}


/*
  op:      DEC
  ex:      25 DEC af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   decrement
  form1:   {adr}:={adr}-1
  form2:   M <- (M) - 1
  form3:   *       M = (M - 1) & $FF
  *       P.N = M.7
  *       P.Z = (M==0) ? 1:0
  *

  */

void alu_op_DEC(char rega[], char regb[], char accumulator[], char flags[]){

}


/*
  op:      SBC
  ex:      25 SBC af -> fe and flags setting
  flags:   **---**
  NVBDIZC
  short:   subtract with carry
  form1:   A:=A-{adr}
  form2:   A <- (A) - M - ~C
  form3:   *       IF (P.D)
  *         t = bcd(A) - bcd(M) - !P.C
  *         P.V = (t>99 OR t<0) ? 1:0
  *       ELSE
  *         t = A - M - !P.C
  *         P.V = (t>127 OR t<-128) ? 1:0
  *       P.C = (t>=0) ? 1:0
  *       P.N = t.7
  *       P.Z = (t==0) ? 1:0
  *       A = t & 0xFF
  *

  accumulator =
  a - b - !c  =
  a - b - !c + 256 =
  a - b - (1-c) + 256 =
  a + (255 - b) + c =
  a + !b + c
  accumulator := rega - regb = rega + NOT(regb) +carryflag

*/

void alu_op_SBC(char rega[], char regb[], char accumulator[], char flags[]){
  char reg[REG_WIDTH+1]= "00000000";
  cp_register(regb, reg);
  one_complement(reg);
  op_adc(rega, reg, accumulator, flags);
}


/*
  op:      INC
  ex:      25 INC af -> fe and flags setting
  flags:   *----*-
  NVBDIZC
  short:   increment
  form1:   {adr}:={adr}+1
  form2:   M <- (M) + 1
  form3:   *       M = (M + 1) & $FF
  *       P.N = M.7
  *       P.Z = (M==0) ? 1:0
  *

  */

void alu_op_INC(char rega[], char regb[], char accumulator[], char flags[]){

}

/*

  Procedural approach to ALU with side-effect:
  Needed register are already alocated and may be modified
  mainly a switchboard
  
  alu_fct(int opcode, char reg_in_a[], char reg_in_b[], char reg_out_accu[], char flags[])

*/
void alu(unsigned int alu_opcode, char reg_in_a[], char reg_in_b[], char reg_out_accu[], char flags[]){
  char dummyflags[9] = "00000000";  
  mu_memory_reset();
  switch ( alu_opcode ){
  case ALU_OP_ADD :
    op_add(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_ADD_WITH_CARRY :
    op_adc(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_SUB :
    op_sub(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_SUB_WITH_CARRY :
    op_sbc(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_AND :
    op_and(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_OR:
    op_or(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_XOR :
    op_xor(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_NEG_A :
    op_neg_a(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_NEG_B :
    op_neg_b(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_NOT_A :
    op_not_a(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_NOT_B :
    op_not_b(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_ASL :
    op_alu_asl(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_LSR :
    op_alu_lsr(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_ROL: 
    op_alu_rol(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_ROR: 
    op_alu_ror(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_CMP: 
    op_alu_cmp(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);
    break;
  case ALU_OP_RESET :
    op_alu_reset(reg_in_a, reg_in_b, reg_out_accu, (flags==NULL)?dummyflags:flags);   
    break;
  default:
    printf("ALU(%i): Invalide operation %i selected", alu_opcode, alu_opcode); 
  }	
}
