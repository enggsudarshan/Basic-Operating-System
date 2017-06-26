// main.c, 159
// this kernel is simulated, not real yet
//
// Team Name: ??????? (Members: ?????? and ??????)

#include "spede.h"      // spede stuff
#include "main.h"       // main stuff
#include "isr.h"        // ISR's
#include "tools.h"      // handy functions for Kernel
#include "proc.h"       // processes such as IdleProc()
#include "typedef.h"    // data types

// kernel data stuff:
int run_pid;             // currently-running PID, if -1, none running
q_t ready_q, avail_q;    // processes ready to run and ID's un-used
pcb_t pcb[PROC_NUM];     // process table
char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks

int main() {
   int new_pid;

   call InitKernelData()  to initialize kernel data

   call DeQ() to dequeue avail_q to get an un-used pid, to
   call NewProcISR(new_pid, IdleProc) (to create IdleProc)

   an infinite loop to alternate two functions:
      call ProcLoader() which is to simulate loading a process to run
      call KernelMain() to simulate kernel run periodically

   return 0;   // not reached, but compiler needs it for syntax
}

void InitKernelData() {
   int i;

   call MyBzero() to clear the two queues (code this function in tools.h/.c)

   loop on i, 0 ~ Q_SIZE-1:
      call EnQ() to enqueue i to avail_q

   set run_pid to 0  // IdleProc is chosen to run first
}

void ProcScheduler() {  // to choose a run PID
   simply return if run_pid currently is 1 or greater
   (or continue, since run_pid is 0 or possibly -1)

   if run_pid is 0 (meaning IdleProc is being run),
   change its state (in its PCB) from RUN to READY

   get run_pid by dequeuing ready_q
   if run_pid returned from DeQ() is -1 (ready_q is empty),
   then set it to 0 (so fall back to run IdleProc)

   set selected process (run_pid) state to RUN, and
   clear its runtime to zero (both in its PCB)
}

void KernelMain() {
   int new_pid;
   char key;

   call TimerISR() to service timer (as if it just occurred)

   if a key has been pressed on PC {
      read key = cons_getchar()

      switch(key) {
         case 'n':
            dequeue avail_q for a new_pid
            if new_pid is -1, show a msg on the target PC:
               "Kernel Panic: no more process!\n"
            else
               call NewProcISR(new_pid, UserProc) to create a new process
            break;

         case 'k':
            call KillProcISR() to kill the run_pid process
            break;

         case 'b':
            call breakpoint(); which brings up GDB prompt
            break;

         case 'q':
            call exit(0) to quit the whole thing, MyOS.dli run
     }
   }
   call ProcScheduler() to choose a new run_pid if needed
}

