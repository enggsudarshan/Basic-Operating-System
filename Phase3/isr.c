// isr.c, 159
/***********************************************************/
#include "spede.h"
#include "typedef.h"
#include "isr.h"
#include "tools.h"
#include "extern.h"
#include "proc.h"
/***********************************************************/
void NewProcISR(int new_pid, func_ptr_t p) 
{
	pcb[new_pid].state=READY;
	pcb[new_pid].runtime=pcb[new_pid].lifespan=0;
     	if(new_pid > 0)        
       		EnQ(new_pid,&ready_q);
     	MyBzero((char *)&proc_stack[new_pid], PROC_STACK_SIZE);
     	pcb[new_pid].TF_p = (TF_t *) &proc_stack[new_pid][PROC_STACK_SIZE-sizeof(TF_t)];

	
     	/*(if(new_pid==0)
     		pcb[new_pid].TF_p->eip=(unsigned int) p;
     	else
          	pcb[new_pid].TF_p->eip=(unsigned int) p;*/
	pcb[new_pid].TF_p->eip=(unsigned int) p;

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
    	pcb[run_pid].state=AVAIL;  //change state of running process to AVAIL
    	EnQ(run_pid,&ready_q);  //queue the running PID to available queue
    	run_pid=-1;//set running PID to -1 (now none running)
  
}        


void TimerISR() 
{
	int i; 
	//Not for phase 2
 	//if(run_pid == 0|| run_pid == -1)   //just return if run_pid is < 1 (can be 0 or -1)
 	// return;
 	system_time++;
	for ( i=0 ;i<Q_SIZE;i++)
	{
     		if((pcb[i].state ==SLEEP) && (pcb[i].wake_time<=system_time))  
 		{
 			EnQ(i, &ready_q);
        		pcb[i].state=READY;
        	}
	}
  

	pcb[run_pid].runtime++;             //upcount its runtime and lifespan (in its PCB)
     	pcb[run_pid].lifespan++;
   	if(pcb[run_pid].runtime==TIME_SLICE) 
   	{
      		pcb[run_pid].lifespan+=pcb[run_pid].runtime;
      		pcb[run_pid].runtime= 0;
      		pcb[run_pid].state=READY;     // change its state to READY
      		EnQ(run_pid,&ready_q);        //queue its PID (run_pid) to ready_q
      		run_pid=-1;                   //clear run_pid to -1 (none running now)
    	}  
}


void SleepISR()
{
	pcb[run_pid].wake_time = system_time + pcb[run_pid].TF_p->eax;
	pcb[run_pid].state = SLEEP; 
        run_pid = -1;
}


void GetPidISR()
{
	pcb[run_pid].TF_p->eax=run_pid;
}

void SemReqISR()
{
	int sem_num;

	sem_num = DeQ(&avail_sem_q);
	if(sem_num > -1)
		MyBzero((char *)&(sem[sem_num]) ,sizeof(sem_t));

	pcb[run_pid].TF_p->eax = sem_num;
}

void SemWaitISR(int sem_id)
{
	//sem_id = pbc[run_pid].TF_p->eax;
	if(sem[sem_id].count > 0)
		sem[sem_id].count--;
	else
	{
		//cons_printf("SemWaitISR: Blocking process # %d\n", run_pid);	
		EnQ(run_pid,&sem[sem_id].wait_q);
		pcb[run_pid].state = WAIT;
		run_pid=-1;
	}
}

void SemPostISR(int sem_id)
{
	int proc_id;
	//sem_id = pbc[run_pid].TF_p->eax;
	if(sem[sem_id].wait_q.size == 0)
		sem[sem_id].count++;
	else
	{
		proc_id = DeQ(&sem[sem_id].wait_q);
		//cons_printf("SemPostISR: freeing process # %d\n", proc_id);
		pcb[proc_id].state = READY;
		EnQ(proc_id,&ready_q);	
	}
	
}

