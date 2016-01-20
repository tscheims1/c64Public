/*
  flags.h
  bho1 26.11.2011
*/


#define SFLAG 0
#define NFLAG 0
#define VFLAG 1
#define OFLAG 1
#define BFLAG 3
#define DFLAG 4
#define IFLAG 5
#define ZFLAG 6
#define CFLAG 7
#define XFLAG 2


void clearCarryflag(char flags[]);

void setCarryflag(char flags[]);

char getCarryflag(char flags[]);

void clearZeroflag(char flags[]);

void setZeroflag(char flags[]);

char getZeroflag(char flags[]);

void clearIRQflag(char flags[]);

void setIRQflag(char flags[]);

char getIRQflag(char flags[]);

void clearDflag(char flags[]);

void setDflag(char flags[]);

char getDflag(char flags[]);

void clearBRKflag(char flags[]);

void setBRKflag(char flags[]);

char getBRKflag(char flags[]);

void clearOverflowflag(char flags[]);

void setOverflowflag(char flags[]);

char getOverflowflag(char flags[]);

void clearSignflag(char flags[]);

void setSignflag(char flags[]);

char getSignflag(char flags[]);

