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
q_t ready_q, avail_q;  
pcb_t pcb[PROC_NUM];     
char proc_stack[PROC_NUM][PROC_STACK_SIZE];
struct i386_gate *IDT_ptr;

int main() 
{
	int new_pid;
   	InitKernelData();
   	InitKernelControl();
   	new_pid=DeQ(&avail_q);
   	NewProcISR(new_pid,IdleProc);
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
     	for(i=0;i<=Q_SIZE-1;i++)
       	{
        	EnQ(i,&avail_q); 
       	}
     	run_pid= 0; 
}

void InitKernelControl() 
{
	IDT_ptr = get_idt_base();
    	outportb(0x21, ~1);
    
     	SetEntry(32, TimerEntry);
     	SetEntry(48, GetPidEntry);
    	SetEntry(49, SleepEntry);    
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
	int new_pid;
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

 	        default:
		  	cons_printf("Kernel Panic: no such intr # (%d)!\n", TF_p->intr_id);
		  	breakpoint();
	}

        if (cons_kbhit())
	{
        	key = cons_getchar();
            	switch(key) 
		{
                	case 'n':
                        	new_pid=DeQ(&avail_q);
                          	if(new_pid == -1)
                            	{
                            		cons_printf("Kernel Panic: no more process!\n");
	                    		exit(0);
                            	}
                           	else
                           		NewProcISR(new_pid, UserProc);
                       			break;

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
