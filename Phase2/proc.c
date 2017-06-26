// proc.c, 159
// processes are coded here
//In Real Kernel (Kernel Process and User Process)
//Code defined here
/********************************************************************/
#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions
#include "syscall.h"
/********************************************************************/
void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && cons_getchar() == 'b') breakpoint();
        		cons_printf("0..");
        	for(i=0;i< 1666667;i++)
        		IO_DELAY();
      	}
}


void UserProc() 
{
	int pid;
	int  sleep_seconds;
   	while(1)
	{
   		//if(cons_kbhit() && cons_getchar() == 'b') breakpoint();
		pid = GetPid();
       		sleep_seconds = 75 + ((pid-1)%4)*75;
      		cons_printf("%d..",pid); 
		Sleep(sleep_seconds);
      }
}
