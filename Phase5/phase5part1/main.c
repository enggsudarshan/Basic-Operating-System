/*******************************************************************/
// main.c, 159
// this kernel is real 
// Team Name: RASOS (Members: Sudarshan Deo and Reena Dalavi)
/***************************************************************/
#include "spede.h"      // spede stuff
#include "main.h"       // main stuff
#include "isr.h"        // ISR's
#include "tools.h"      // handy functions for Kernel
#include "proc.h"       // processes such as IdleProc()
#include "typedef.h"    // data types
#include  "entry.h"

// kernel data stuff
int run_pid,system_time;             
q_t ready_q, avail_q,avail_sem_q;  
pcb_t pcb[PROC_NUM];     
char proc_stack[PROC_NUM][PROC_STACK_SIZE];
struct i386_gate *IDT_ptr;
sem_t sem[Q_SIZE];
term_t term[3];

int main() 
{
	int new_pid;
   	InitKernelData();
   	InitKernelControl();
   	new_pid=DeQ(&avail_q);
   	NewProcISR(new_pid,IdleProc);
	new_pid=DeQ(&avail_q);
	NewProcISR(new_pid,TermProc);
	new_pid=DeQ(&avail_q);
	NewProcISR(new_pid,TermProc);
   	ProcLoader(pcb[run_pid].TF_p);	
   	return 0;   
}



void SetEntry(int entry_num, func_ptr_t func_ptr)
{
     	struct i386_gate *gateptr = &IDT_ptr[entry_num];
     	fill_gate(gateptr, (int) func_ptr, get_cs(), ACC_INTR_GATE, 0);
}


void InitKernelData() 
{
	int i;
	system_time =0;
     	MyBzero((char *)&avail_q,sizeof(&avail_q));
     	MyBzero((char *)&ready_q,sizeof(&ready_q)); 
	MyBzero((char *)&ready_q,sizeof(&avail_sem_q));
	MyBzero((char *)&term, sizeof(term_t));
	term[0].io_base = 0x2f8;
	term[1].io_base = 0x3e8;
	term[2].io_base = 0x2e8;
     	for(i=0;i<=Q_SIZE-1;i++)
       	{
        	EnQ(i,&avail_q); 
		EnQ(i,&avail_sem_q); 
       	}
     	run_pid= 0; 

	
}

void InitKernelControl() 
{
	IDT_ptr = get_idt_base();
    	outportb(0x21, ~25);
    
     	SetEntry(32, TimerEntry);
     	SetEntry(48, GetPidEntry);
    	SetEntry(49, SleepEntry);  

  	SetEntry(50, SemReqEntry); 
	SetEntry(51, SemWaitEntry); 
	SetEntry(52, SemPostEntry);

	SetEntry(35, Term1Entry);
	SetEntry(36, Term2Entry); 
}

void ProcScheduler() 
{  
	if(run_pid >= 1) 
       		return;
     	if(run_pid == 0)   
       		pcb[run_pid].state=READY;   
      	if(((run_pid =DeQ(&ready_q)))==-1)
            	run_pid = 0;  
        pcb[run_pid].state = RUN;  
        pcb[run_pid].runtime=0;       
}

void KernelMain(TF_t *TF_p) 
{
	//int new_pid;
	char key; 
     	pcb[run_pid].TF_p=TF_p;	
 
	switch(TF_p->intr_id)
	{
		case TIMER_INTR:
			TimerISR();
		  	outportb(0x20, 0x60);
		  	break;
                  
      		case SLEEP_INTR:
            		SleepISR();
            		break;
       		case GETPID_INTR:
            		GetPidISR();
            		break;
		case SEMREQ_INTR:
			SemReqISR();
			break;
		case SEMWAIT_INTR:
			SemWaitISR(TF_p->eax);
			break;
		case SEMPOST_INTR:
			SemPostISR(TF_p->eax);
			break;

		case TERM1_INTR:
			TermISR();
			outportb(0x20, 0x63);
			break;
		case TERM2_INTR:
			TermISR();
			outportb(0x20, 0x64);
			break;
 	        default:
		  	cons_printf("Kernel Panic: no such intr # (%d)!\n", TF_p->intr_id);
		  	breakpoint();
	}

        if (cons_kbhit())
	{
        	key = cons_getchar();
            	switch(key) 
		{
                      case 'k':
                      		KillProcISR() ;
                      		break;
                      case 'b':
                       		breakpoint(); 
                      		break;
                      case 'q':
                        	exit(0);   
		}
	}
        ProcScheduler();
	ProcLoader(pcb[run_pid].TF_p);
}
