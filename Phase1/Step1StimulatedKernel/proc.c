// proc.c, 159
// processes are coded here

#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions

void ProcLoader() {     // load a process to run, here simulated
   if run_pid is 0:
      call IdleProc()   // as if loads it and it runs
   else (it's one of the other processes)
      call UserProc()   // as if loads it and it runs
}

void IdleProc() {
   show this on PC: "0.."
   loop 1666667 times calling IO_DELAY() (for VirtualBox, perhaps 1/10 of that)
}

void UserProc() {
   show this on target PC: cons_printf("%d..", run_pid);
   loop 1666667 times calling IO_DELAY() (for VirtualBox, perhaps 1/10 of that)
}
