// main.c, 159

#include "spede.h"      // spede stuff
#include "main.h"       // main stuff
#include "isr.h"        // ISR's
#include "tools.h"      // handy functions for Kernel
#include "proc.h"       // processes such as IdleProc()
#include "typedef.h"    // data types
#include  "entry.h"

// kernel data stuff
int run_pid;             // currently-running PID, if -1, none running
q_t ready_q, avail_q;    // processes ready to run and ID's un-used
pcb_t pcb[PROC_NUM];     // process table
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks
struct i386_gate *IDT_ptr;


void SetEntry(int entry_num, func_ptr_t func_ptr)
{
	struct i386_gate *gateptr = &IDT_ptr[entry_num];
	fill_gate(gateptr, (int) func_ptr, get_cs(), ACC_INTR_GATE, 0);
}


void InitKernelData() 
{
	int i;
	MyBZero((char *)&avail_q,sizeof(&avail_q));
	MyBZero((char *)&ready_q,sizeof(&ready_q));  
	for(i=0;i<=Q_SIZE-1;i++)
	{
    		EnQ(i,&avail_q);
	}
	run_pid= 0;   
}


void InitKernelControl() 
{
    //locate IDT
    IDT_ptr = get_idt_base();
    //program PIC mask
    outportb(0x21, ~1);
    //fill out IDT timer entry
    SetEntry(32, TimerEntry);
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
 		default:
			cons_printf("Kernel Panic: no such intr # (%d)!\n", TF_p->intr_id);
			breakpoint();
    	}

   	if (cons_kbhit())
	{
      		key = cons_getchar();
      		//printf("Key Press : %c\n", key);
		//printf("\nLife Span of Process %d is %d\n",run_pid,pcb[run_pid].lifespan);
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
               				NewProcISR(new_pid, UserProc);//call NewProcISR(new_pid, UserProc) to create a new process
            				break;
         		case 'k':
            			KillProcISR() ;//to kill the run_pid process
            			break;
         		case 'b':
             			breakpoint(); //which brings up GDB prompt
            			break;
         		case 'q':
             			exit(0); //to quit the whole thing, MyOS.dli run
     		}
   	}
   	ProcScheduler();// to choose a new run_pid if needed
	ProcLoader(pcb[run_pid].TF_p);
}

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
