/* 
   flags.c 
   bho1: 26.11.2011 init

*/

#include "register.h"
#include "flags.h"

void clearCarryflag(char flags[]){
  flags[CFLAG] = '0';
}
void setCarryflag(char flags[]){
  flags[CFLAG] = '1';
}
char getCarryflag(char flags[]){
  return flags[CFLAG];
}
void clearZeroflag(char flags[]){
  flags[ZFLAG] = '0';
}
void setZeroflag(char flags[]){
  flags[ZFLAG] = '1';
}
char getZeroflag(char flags[]){
  return flags[ZFLAG];
}
void clearIRQflag(char flags[]){
  flags[IFLAG] = '0';
}
void setIRQflag(char flags[]){
  flags[IFLAG] = '1';
}
char getIRQflag(char flags[]){
  return flags[IFLAG];
}
void clearDflag(char flags[]){
  flags[DFLAG] = '0';
}
void setDflag(char flags[]){
  flags[DFLAG] = '1';
}
char getDflag(char flags[]){
  return flags[DFLAG];
}
/* void clearBRKflag(char flags[]){ */
/*   flags[BFLAG] = '0'; */
/* } */
/* void setBRKflag(char flags[]){ */
/*   flags[BFLAG] = '1'; */
/* } */
char getBRKflag(char flags[]){ 
  return flags[BFLAG] ;
 }
void clearOverflowflag(char flags[]){
  flags[VFLAG] = '0';
}
void setOverflowflag(char flags[]){
  flags[VFLAG] = '1';
}
char getOverflowflag(char flags[]){
  return flags[VFLAG];
}
void clearSignflag(char flags[]){
  flags[SFLAG] = '0';
}
void setSignflag(char flags[]){
  flags[SFLAG] = '1';
}
char getSignflag(char flags[]){
  return flags[SFLAG];
}
