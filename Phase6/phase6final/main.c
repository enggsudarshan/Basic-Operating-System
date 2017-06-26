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
#include "filesys_data.h"

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
     	MyBzero((char *)&avail_q,sizeof(avail_q));
     	MyBzero((char *)&ready_q,sizeof(ready_q)); 
	MyBzero((char *)&ready_q,sizeof(avail_sem_q));
	MyBzero((char *)&term[0], sizeof(term_t));
	MyBzero((char *)&term[1], sizeof(term_t));
	MyBzero((char *)&term[2], sizeof(term_t));

	term[0].io_base = 0x2f8;
	term[1].io_base = 0x3e8;
	term[2].io_base = 0x2e8;
     	for(i=0;i<=Q_SIZE-1;i++)
       	{
        	EnQ(i,&avail_q); 
		EnQ(i,&avail_sem_q); 
       	}
     	run_pid= 0; 
   	root_dir[0].size = sizeof( root_dir );
   	bin_dir[0].size = sizeof( bin_dir );
   	bin_dir[1].size = root_dir[0].size;
   	www_dir[0].size = sizeof( www_dir );
   	www_dir[1].size = root_dir[0].size;
	for(i=0; i<MAX_FD; i++) 
	{
      		  fd_array[i].owner = -1;
	}
	
}

void InitKernelControl() 
{
	IDT_ptr = get_idt_base();
    	outportb(0x21, ~25);
    
     	SetEntry(TIMER_INTR, TimerEntry);
     	SetEntry(GETPID_INTR, GetPidEntry);
    	SetEntry(SLEEP_INTR, SleepEntry);  

  	SetEntry(SEMREQ_INTR, SemReqEntry); 
	SetEntry(SEMWAIT_INTR, SemWaitEntry); 
	SetEntry(SEMPOST_INTR, SemPostEntry);

	SetEntry(TERM1_INTR, Term1Entry);
	SetEntry(TERM2_INTR, Term2Entry); 

	SetEntry(FSTAT_INTR, FstatEntry);
	SetEntry(FOPEN_INTR, FopenEntry);
	SetEntry(FREAD_INTR, FreadEntry);
	SetEntry(FCLOSE_INTR, FcloseEntry);
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
     	pcb[run_pid].TF_p=TF_p;	
 
	switch(TF_p->intr_id)
	{
		case TIMER_INTR:
			TimerISR();
		  	outportb(0x20, IRQ0_INTR);
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
			outportb(0x20, IRQ3_INTR);
			break;
		case TERM2_INTR:
			TermISR();
			outportb(0x20, IRQ4_INTR);
			break;
		case FSTAT_INTR:
			FstatISR();
			break;
		case FOPEN_INTR:
			FopenISR();
			break;
		case FREAD_INTR:
			FreadISR();
			break;
		case FCLOSE_INTR:
			FcloseISR();
			break;
 	        default:
		  	cons_printf("Kernel Panic: no such intr # (%d)!\n", TF_p->intr_id);
	}
        ProcScheduler();
	ProcLoader(pcb[run_pid].TF_p);
}
