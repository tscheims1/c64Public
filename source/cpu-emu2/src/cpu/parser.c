/*

  parser.c
  bho1 - init 27.8.2012 

  Compile with  `sdl-config --cflags --libs`
  

  
*/
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "memory.h"

#define MAX_CMD 10

int mem_pager_adr = 0;
int mem_pager_rows = 4;
int mem_pager_cols = 16;

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.
   Returns NULL on EOF. */
char *
rl_gets ()
{
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

  /* Get a line from the user. */
  line_read = readline("mon> ");

  /* If the line has any text in it,
     save it on the history. */
  if (line_read && *line_read)
    add_history(line_read);

  return (line_read);
}


char * read_line(){
  return rl_gets();
}

//   ##        ###### 
//   ##       ##    ##
//   ##       ##      
//   ##        ###### 
//   ##             ##
//   ##       ##    ##
//   ########  ###### 

int ls(struct _6510_cpu* cpu, char memory[][9], char* string){
  print_cpu(cpu);
  //      print_register_short();
  //print_register_exec();
}


//   ##     ## ##    ## ######## #### ##      
//   ##     ## ###   ##    ##     ##  ##      
//   ##     ## ####  ##    ##     ##  ##      
//   ##     ## ## ## ##    ##     ##  ##      
//   ##     ## ##  ####    ##     ##  ##      
//   ##     ## ##   ###    ##     ##  ##      
//    #######  ##    ##    ##    #### ########

int until_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  void (*mue_code_ptr)();
  int adr;
  //  char* buffer;
  sscanf(string, " %*s %4x ", &adr);
  if( !(adr>MEMORY_SIZE || adr < 0) ){
    while(adr!=pc2int(cpu)){
      fetch(cpu, memory);
      exec(cpu, memory);
      print_cpu_dis(cpu,memory);
    }
  }
}

int (*eval_fctptr)(struct _6510_cpu* cpu, char memory[][9], char* string);

int quit(struct _6510_cpu cpu, char *memory[][9], char* string){
  printf("quit repl and cpu ..\n");
  SDL_Quit();
  exit(0);
}


#define ACC 0
#define ABRL 1
#define ABRH 2
#define DBR 3
#define IDX 4
#define IDY 5
#define PCL 6
#define PCH 7
#define IR 8
#define SP 9

char *names[] = { "acc", "abrl", "abrh", "dbr","idx" ,"idy", "pcl", "pch", "sp", NULL};
int registers_cmp(const char* string,const char* stringb){
  if(string[0]=='r' && string[1]=='e' && string[2]=='g' && string[3]==' ')
	return 0;
  else
	return -1;
}


/*
  register setting:
  <reg-name> <value>
  <reg-name> ::= idx idy idz acc, pc abrl abrh ir
      
*/

int registers_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  char regname[5] = "1234";
  char buffer_reg[8+1] = "00000000";
  int key, content=-1;
  int i;
  // mon> mem af 02 ; mam[af] <- 02
  sscanf(string, " %*s %s %2x ", &(regname[0]), &content);

  key = -1;
  for(i=0;names[i]!=NULL;i++){
	if(strcmp(regname,names[i])==0){
	  key=i;
	  break;
	}
  }
  if(content!=-1){
	int2register(content, buffer_reg);
	switch(key)
	  {
	  case ACC:
		cp_register(buffer_reg, cpu->rega);
		break;
	  case ABRL:
		cp_register(buffer_reg, cpu->abrl);
		break;
	  case ABRH:
		cp_register(buffer_reg, cpu->abrh);
		break;
	  case DBR:
		cp_register(buffer_reg, cpu->dbr);
		break;
	  case IDX:
		cp_register(buffer_reg, cpu->regx);
		break;
	  case IDY:
		cp_register(buffer_reg, cpu->regy);
		break;
	  case PCL:
		cp_register(buffer_reg, cpu->pcl);
		break;
	  case PCH:
		cp_register(buffer_reg, cpu->pch);
		break;
	  case IR:
		cp_register(buffer_reg, cpu->ir);
		break;
	  case SP:
		cp_register(buffer_reg, cpu->sp);
		break;
	  default:
		break;
	  }
  }
  print_cpu(cpu);

}



/*
;; ##     ##  ######## ##     ##
;; ###   ### ##        ###   ###
;; #### #### ##        #### ####
;; ## ### ## ######### ## ### ##
;; ##  #  ## ##        ##  #  ##
;; ##     ## ##        ##     ##
;; ##     ##  ######## ##     ##
*/

/*
  memory
  memarr <adr1> <adr2>
  mem <adr> <value>
  mem <adr>
      
      
*/

int memory_cmp(const char* string,const char *stringb){
  if(string[0]=='m' && string[1]=='e' && string[2]=='m' && string[3]==' ')
	return 0;
  else
	return 1;
} 
int memory_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  int adr=-1;
  int content=-1;
  char reg[8+1];

  sscanf(string, "%*s %4x %2x ", &adr, &content);
  if(adr>MEMORY_SIZE || adr < 0){
    adr = 0;
  } 
  if(content!=-1){
    int2register(content,reg);
    cp_register(reg, memory[adr]);
  }
  print_memory_page0(cpu, memory, adr);
}


/*
 #######  #######  #######  ######
 #           #     #        #     #
 #           #     #        #     #
 #######     #     #######  ######
       #     #     #        #     
       #     #     #        #     
 #######     #     #######  #
*/

int step_cmp(const char* string,const char *stringb){
  if(string[0]=='s' && string[1]=='t' && string[2]=='e' && string[3]=='p' )
	return 0;
  else
	return 1;
} 

int step_counter;

int step(struct _6510_cpu* cpu, char memory[][9], char* string){
  int steps=0;
  sscanf(string, " %*s %i ", &steps);
  int silentRunning =0;
  if(steps==-1)
	silentRunning = 1;
		
	
  while(silentRunning||steps--){

    fetch(cpu, memory);
    exec(cpu, memory);

	if(!silentRunning)
		print_cpu_dis(cpu,memory);
	//else
	//	usleep(2);//need 8-us for SID sync

  }
  
  


}

//   ######## ##     ## ########  ###### 
//   ##        ##   ##  ##       ##    ##
//   ##         ## ##   ##       ##      
//   ######      ###    ######   ##      
//   ##         ## ##   ##       ##      
//   ##        ##   ##  ##       ##    ##
//   ######## ##     ## ########  ###### 
//  cp value into mem[pc] and exec
int execute_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  int op, lb, hb;
  int pc=pc2int(cpu);
  sscanf(string, "x %2x %2x %2x", &op, &lb, &hb);
  int2register(op, &memory[pc]);
  int2register(lb, &memory[pc+1]);
  int2register(hb, &memory[pc+2]);  
  fetch(cpu, memory);
  exec(cpu, memory);
  print_cpu_dis(cpu,memory);
  
}

int load_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  char path[255]; 
  int modus, lb, hb;
  int pc=pc2int(cpu);
  sscanf(string, "load %d %s", &modus, &path);
  read_memorydump(path, memory);
  cp_register("00000110",cpu->pch);
  cp_register("00000000",cpu->pch);
}
int dis_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  int adr=-1;
  //  int content=-1;
  char reg[8+1];

  sscanf(string, "%*s %4x ", &adr); //, &content);
  if(adr>MEMORY_SIZE || adr < 0){
    adr = 0;
  } 
  print_dis_memory( memory, adr);
}

int go_cmd(struct _6510_cpu* cpu, char memory[][9], char* string){
  int adr = -1;

  sscanf(string, "%*s %4x ", &adr);
  printf("%s %4x ", string, adr);

  if(adr>MEMORY_SIZE || adr < 0){
    adr = 0;
  }
  int hb, lb;
  lb = adr & 0xff;
  hb = (adr>>8) & 0xff;
  int2register(adr & 0xff, cpu->pcl);
  int2register((adr>>8) & 0xff, cpu->pch);
}
//   ########     ###    ########   ######  ######## ######## 
//   ##     ##   ## ##   ##     ## ##    ## ##       ##     ##
//   ##     ##  ##   ##  ##     ## ##       ##       ##     ##
//   ########  ##     ## ########   ######  ######   ######## 
//   ##        ######### ##   ##         ## ##       ##   ##  
//   ##        ##     ## ##    ##  ##    ## ##       ##    ## 
//   ##        ##     ## ##     ##  ######  ######## ##     ##

struct map {
  int (*eval_fctptr)(struct _6510_cpu* cpu, char memory[][9], char* string); 
  char cmd[200];
  int (*cmp_fctptr)(const char* string,const char*cmd); 
};

struct map ptable[20] = { // { .eval_fctptr=&ls, "qwe", .cmp_fctptr=&strcmp}
  { &ls , "ls", &strcmp },
  { &execute_cmd , "x ", &strcmp },
  { &step, "step", &step_cmp},
  { &quit, "quit", &strcmp},
  { &registers_cmd, "reg", &registers_cmp},
  { &until_cmd, "until", &strcmp},
  { &memory_cmd, "mem", &memory_cmp},
  { &load_cmd, "load", &strcmp},
  { &dis_cmd, "dis", &strcmp},
  { &go_cmd, "go", &strcmp}
  //  { &set_cmd, "set", &set_cmp}
};

int parse_line(struct _6510_cpu* cpu, char memory[][9], char* string){
  int i;
  for(i=0;i<MAX_CMD; i++){
    if(!strncmp(string, ptable[i].cmd,2)){
      return ptable[i].eval_fctptr(cpu, memory, string);
    }
  }
}

//    ######  ######## ######## ######## ######## ######## 
//   ##    ## ##          ##       ##    ##       ##     ##
//   ##       ##          ##       ##    ##       ##     ##
//    ######  ######      ##       ##    ######   ######## 
//         ## ##          ##       ##    ##       ##   ##  
//   ##    ## ##          ##       ##    ##       ##    ## 
//    ######  ########    ##       ##    ######## ##     ##

int set_cmp(const char* string,const char *stringb){
  while(*string &&*stringb){
	if(*string++!=*stringb++)
	  return 1;
  }
  return 0;
}
void* var_tbl[10][2] = { { "mem_pager_adr", &mem_pager_adr },
						 { "mem_pager_rows", &mem_pager_rows },
						 { "mem_pager_cols", &mem_pager_cols },
						 { NULL, NULL}
};

int set_name_cmp(const char* string,const char *stringb){
  if(string && stringb){
	while(*string &&*stringb){
	  if(*(string++)!=*(stringb++))
		return 1;
	}
	return 0;
  } else {
	return 1;
  }
}

char* parse_6502_register_name(struct _6510_cpu* cpu, char* name){

  if(!strcmp(name,"a")){
	return cpu->rega;
  }
  if(!strcmp(name,"idx")){
	return cpu->regx;
   }

  if(!strcmp(name,"idy")){
	return cpu->regy;
	}
  if(!strcmp(name,"pch")){
	return cpu->pch;
  }
  if(!strcmp(name,"pcl")){
	return cpu->pcl;
  }
  if(!strcmp(name,"sp")){
	return cpu->sp;
  }
  if(!strcmp(name,"flg")){
	return cpu->flags;
  }
  return NULL;
}

void set_cmd(char* string){
  unsigned int value;
  char name[30];
  char*cp;
  int *ip;
  sscanf(string, "%*s %s %x", name, &value);
  int i=0;
  for(i=0;var_tbl[i][0]!=0;i++){
	cp=var_tbl[i][0];
	if(!set_name_cmp(name, cp)){
	  ip = var_tbl[i][1];
	  *ip =  value;
	  break;
	}
  }
}
