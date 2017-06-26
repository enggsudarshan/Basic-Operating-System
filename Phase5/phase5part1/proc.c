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
#define add (0x0f00+'0')
void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && (cons_getchar() == 'q' || cons_getchar() == 'Q')) exit(0);
        		cons_printf("0..");
        	for(i=0;i< 1666667;i++)
        		IO_DELAY();
      	}
}




void TermProc()
{
	char a_str[101];            // a handy string
   int  i,k,baud_rate,remote,local, divisor;
  local=GetPid()-1;
   remote=(local+1)%2;
   term[local].out_q_sem=SemReq();
   for(i = 0; i <Q_SIZE ; i++)
     {
       	SemPost(term[local].out_q_sem);
//SemPost(term[remote].out_q_sem);

   }
      term[local].in_q_sem=SemReq();
      term[local].out_flag =1;
      term[local].echo_flag=1;
term[remote].out_flag =1;
      term[remote].echo_flag=1;
for( k =0;k<2;k++)
{
 //  initialize the port data structure (term_t term[x]):
 //  A. request for out_q_sem, boost the count to the out_q size
  // B. request for in_q_sem (the count is zero to begin with)
   //C. set echo_flag to 1 (the default is to echo back to terminal)
  // D. set out_flag to 1 (assuming 1st TXRDY event was missed)
  
 //  reset the serial port with these code:
   // A. set baud rate 9600
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
//leep(50);
}
for(i=0;i<24;i++)
TermOut("\n",local);
   //(Run DemoTerm1.dli to see exactly what it is doing in below.)
   while(1) {

      MyStrcpy(a_str, "\n\n\nHello\n" );
     TermOut(a_str,local);// given a_str to prompt msg to terminal
       MyStrcpy(a_str,  "\nWhat is your name?\n\n");// to a _str
      TermOut(a_str,local);//prompt it to terminal again
     //get terminal KB input into a_str by calling TermIn()
TermIn(a_str,local);
      cons_printf( "\nTerm 1 Name: %s...", a_str);
 
TermOut("(Local)",local);
      MyStrcpy(a_str, "\nHow's your father?\n\n" );
        TermOut(a_str,local);//to the terminal
      //get from terminal KB input
        TermIn(a_str,local);
     cons_printf( "\nTerm 1 Father Status: ...%s" ,a_str);
     MyStrcpy(a_str,"\nCan you elaborate that?\n\n");// to the terminal
      //get from terminal KB input
      TermOut(a_str,local);
TermIn(a_str,local);      
cons_printf( "\nTerm 1 Replied: ...%s", a_str);
   }
}


void TermIn(char *str,int which)
{
	char ch, *p;
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
	}		
	*p = '\0';
}

void   TermOut(char *str,int which) //is coded (as subroutine of TermProc):
   {
char *p;
   p=str;
   while(*p)
{
     SemWait(term[which].out_q_sem);
EnQ(*p,&term[which].out_q);
if(*p=='\n')
{SemWait(term[which].out_q_sem);
EnQ('\r',&term[which].out_q);
}
TripTermIRQ();
p++;
}

}










