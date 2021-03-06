// typedef.h, 159

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include "TF.h"              // trapframe type TF_t is defined here

#define TIME_SLICE 10       // max timer count, then rotate process
#define PROC_NUM 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define PROC_STACK_SIZE 4096 // process runtime stack in bytes

typedef void (*func_ptr_t)(); // void-return function pointer type

// this is the same as constants defines: AVAIL=0, READY=1, etc.
typedef enum {AVAIL, READY, RUN, SLEEP, WAIT, ZOMBIE, WAIT2CHILD} state_t;

typedef struct {             // PCB describes proc image
   state_t state;            // state of process
   int runtime,              // runtime since loaded to run
       lifespan,             // total runtime since created
       ppid,                 // parent PID
       cpid;  
    	int wake_time;              //Phase-2
   TF_t *TF_p;               // points to trapframe of process
} pcb_t;

typedef struct {             // generic queue type
   int q[Q_SIZE];            // integers are queued in q[] array
   int size;                 // size is also where the tail is for new data
} q_t;

typedef struct
{
	q_t wait_q;
	int count;	
} sem_t;

typedef struct 
{             // phase 5, terminal
	q_t in_q,
	out_q,
	echo_q;
	int in_q_sem,out_q_sem,echo_flag,out_flag,io_base;
} term_t;


#endif
