// proc.c, 159
// processes are coded here
//In Real Kernel (Kernel Process and User Process)
//Code defined here
/********************************************************************/
#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions
#include "syscall.h"
#include "tools.h"
/********************************************************************/

void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && cons_getchar() == 'b') 
			breakpoint();
        	cons_printf("0..");
        	for(i=0;i< 1666667;i++)
        		IO_DELAY();
      	}
}


void TermProc()
{
	char a_str[101];         
	int  i,k=0,baud_rate,remote,local, divisor;
  	local=GetPid()-1;
   	remote=(local+1)%2;
   	term[local].out_q_sem=SemReq();
   	for(i = 0; i <Q_SIZE ; i++)
     	{
       		SemPost(term[local].out_q_sem);
	}
      	term[local].in_q_sem=SemReq();
      	term[local].out_flag =1;
      	term[local].echo_flag=1;
	term[remote].out_flag =1;
      	term[remote].echo_flag=1;
	for(k =0;k<2;k++)
	{
   		baud_rate = 9600;              // Mr. Baud invented this
   		divisor = 115200 / baud_rate;  // time period of each baud
   		outportb(term[k].io_base+CFCR, CFCR_DLAB);          // CFCR_DLAB 0x80
	   	outportb(term[k].io_base+BAUDLO, LOBYTE(divisor));
   		outportb(term[k].io_base+BAUDHI, HIBYTE(divisor));
   		outportb(term[k].io_base+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);
   		outportb(term[k].io_base+IER, 0);
   		outportb(term[k].io_base+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
   		IO_DELAY();
   		outportb(term[k].io_base+IER, IER_ERXRDY|IER_ETXRDY); // enable TX, RX events
		Sleep(50);
	}
	for(i=0;i<24;i++)
		TermOut("\n",local);
   	while(1) 
	{
		TermIn(a_str,local);
		cons_printf( "\nTerm %d Name: %s...",GetPid(),a_str);
		TermOut("(Remote)",remote);
		TermOut(a_str,remote);
		TermOut("\n",remote);
   	}
}


void TermIn(char *str,int which)
{
	char ch, *p;
	int ch_count=0;
	p = str;
	while(1)
	{
		SemWait(term[which].in_q_sem);
		ch = DeQ(&term[which].in_q);
		if(ch == '\r')
		{
			break;
		}
		else
		{
			*p++ = ch;
		}
		ch_count++;
		if(ch_count==100)
			break;	
	}		
	*p = '\0';
}

void TermOut(char *str,int which)
{
	char *p;
   	p=str;
   	while(*p)
	{
     		SemWait(term[which].out_q_sem);
		EnQ(*p,&term[which].out_q);
		if(*p=='\n')
		{
			SemWait(term[which].out_q_sem);
			EnQ('\r',&term[which].out_q);
		}
		TripTermIRQ();
		p++;
	}
}










