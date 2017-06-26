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
	MyBzero((char *)&pcb[new_pid], sizeof(pcb_t));
	pcb[new_pid].state=READY;
	pcb[new_pid].runtime=pcb[new_pid].lifespan=0;
     	if(new_pid > 0)        
       		EnQ(new_pid,&ready_q);
     	MyBzero((char *)&proc_stack[new_pid], PROC_STACK_SIZE);
	
     	pcb[new_pid].TF_p = (TF_t *) &proc_stack[new_pid][PROC_STACK_SIZE-sizeof(TF_t)];

	pcb[new_pid].TF_p->eip=(unsigned int) p;

    	pcb[new_pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR;
    	pcb[new_pid].TF_p->cs = get_cs();                     
    	pcb[new_pid].TF_p->ds = get_ds();                
    	pcb[new_pid].TF_p->es = get_es();                   
    	pcb[new_pid].TF_p->fs = get_fs();              
    	pcb[new_pid].TF_p->gs = get_gs();                   
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
 	system_time++;
	for ( i=1 ;i<Q_SIZE-1;i++)
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
	if(sem[sem_id].count > 0)
		sem[sem_id].count--;
	else
	{	
		EnQ(run_pid,&sem[sem_id].wait_q);
		pcb[run_pid].state = WAIT;
		run_pid=-1;
	}
}

void SemPostISR(int sem_id)
{
	int proc_id;
	if(sem[sem_id].wait_q.size == 0)
		sem[sem_id].count++;
	else
	{
		proc_id = DeQ(&sem[sem_id].wait_q);
		pcb[proc_id].state = READY;
		EnQ(proc_id,&ready_q);	
	}
	
}

void TermISR()
{
	int status,i;
	
	for(i=0;i<2;i++)
	{
		status =  inportb(term[i].io_base+IIR);
		switch(status)
		{
        		case IIR_TXRDY : TermOutHandler(i);
	 		  	 	 break;
			case IIR_RXRDY : TermInHandler(i);
			  	 	 break;
		}
		if(term[i].out_flag == 1)
		{
			TermOutHandler(i);
		}
	}
}

void TermOutHandler(int which)
{
	char ch = 0; 
	if(term[which].echo_q.size >0)
	{
		ch = DeQ(&term[which].echo_q);
	}
	else
	{
		if(term[which].out_q.size > 0)
		{
			ch = DeQ(&term[which].out_q);
			SemPostISR(term[which].out_q_sem);
		}
	}
	if (ch != 0)
	{
		outportb(term[which].io_base+DATA, ch);
		term[which].out_flag = 0;
	} 
	else 
	{
		term[which].out_flag = 1;
	}
}

void TermInHandler(int which)
{
	char ch;
	ch = inportb(term[which].io_base+DATA) & 0x7F; 
	EnQ(ch,&term[which].in_q);
	SemPostISR(term[which].in_q_sem);
	
	if (ch == '\r')
	{
		EnQ('\r',&term[which].echo_q);
		EnQ('\n',&term[which].echo_q);
	}
	else 
	{
		if (term[which].echo_flag == 1)
		{	
			EnQ(ch,&term[which].echo_q);
		}
	}
}
