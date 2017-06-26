// proc.c, 159
// processes are coded here

#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions


void IdleProc() 
{
	int i;
	while(1)
	{
		if(cons_kbhit() && cons_getchar() == 'b') breakpoint();
   		cons_printf("0..");
   		for(i=0;i< 16666670/10;i++)
			IO_DELAY();
	}
}

void UserProc() 
{
	int i;
	while(1)
	{
	cons_printf("%d..", run_pid);
   	for(i=0;i< 16666670/10;i++)
		IO_DELAY();
	}	
}
