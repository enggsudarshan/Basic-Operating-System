// isr.c, 159

#include "spede.h"
#include "typedef.h"
#include "isr.h"
#include "tools.h"
#include "extern.h"
#include "proc.h"

void NewProcISR(int new_pid, func_ptr_t p) 
{
	pcb[new_pid].state=READY;
	pcb[new_pid].runtime=pcb[new_pid].lifespan=0;
  	if(new_pid > 0)
		EnQ(new_pid,&ready_q);
   	MyBZero(proc_stack[new_pid], PROC_STACK_SIZE);
	pcb[new_pid].TF_p = (TF_t *) &proc_stack[new_pid][PROC_STACK_SIZE-sizeof(TF_t)];
	if (new_pid == 0)
	{
		pcb[new_pid].TF_p->eip=(unsigned int) p;
   	} 
	else 
	{
		pcb[new_pid].TF_p->eip=(unsigned int) p;
   	}
   	pcb[new_pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR; // set INTR flag
   	pcb[new_pid].TF_p->cs = get_cs();                     // standard fair
   	pcb[new_pid].TF_p->ds = get_ds();                     // standard fair
   	pcb[new_pid].TF_p->es = get_es();                     // standard fair
   	pcb[new_pid].TF_p->fs = get_fs();                     // standard fair
   	pcb[new_pid].TF_p->gs = get_gs();                     // standard fair
}

void KillProcISR() 
{
	if(run_pid == 0)
		return;
	pcb[run_pid].state=AVAIL;
	EnQ(run_pid,&ready_q);
	run_pid=-1;
}        

void TimerISR() 
{
	if(run_pid == 0|| run_pid == -1)
		return;
	pcb[run_pid].runtime++;
	pcb[run_pid].lifespan++;
	if(pcb[run_pid].runtime == TIME_SLICE)
	{
		pcb[run_pid].runtime= 0;
   		pcb[run_pid].state=READY;
   		EnQ(run_pid,&ready_q);
   		run_pid=-1;
	}
}
