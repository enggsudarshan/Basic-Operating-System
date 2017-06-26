// typedef.h, 159

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#define TIME_SLICE 3         // 3 seconds then rotate to next process
#define PROC_NUM 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define PROC_STACK_SIZE 4096 // process runtime stack in bytes

typedef void(*func_ptr_t)(); // function pointer type (void-return/arg)

// this is the same as constants defines: AVAIL=0, READY=1, etc.
typedef enum {AVAIL, READY, RUN, SLEEP, WAIT, ZOMBIE, WAIT4CHILD} state_t;

typedef struct {             // PCB describes proc image
   state_t state;            // state of process
   int runtime,              // runtime since loaded
       lifespan,             // total process time since created
       ppid,                 // parent PID
       cpid;                 // child PID
} pcb_t;

typedef struct {             // generic integer queue
   int q[Q_SIZE];            // int array q[] to queue integers
   int size;                 // current size of things queued
} q_t;

#endif
