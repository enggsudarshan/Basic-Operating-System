// isr.c, 159

#include "spede.h"
#include "type.h"
#include "isr.h"
#include "tools.h"
#include "extern.h"
#include "proc.h"

void NewProcISR(int new_pid, func_ptr_t p) {
   clear the PCB and proc_stack for this new process
   (use your own MyBzero to do that!)
   set the state in its PCB to READY

   enqueue new_pid to ready_q if it's > 0
   (we don't queue IdleProc since it must be available)
}

void KillProcISR() {
   just "return" if run_pid is 0 (IdleProc is exempted)
      
   change state of running process to AVAIL
   queue the running PID to available queue
   set running PID to -1 (now none running)
}        

void TimerISR() {
   just return if run_pid is < 1 (can be 0 or -1)

   upcount its runtime and lifespan (in its PCB)

   if its runtime is == TIME_SLICE:
      change its state to READY
      queue its PID (run_pid) to ready_q
      clear run_pid to -1 (none running now)
      (ProcScheduler() will later be called to look into ready_q
       for another; if none there, it sets run_pid to 0)
}
