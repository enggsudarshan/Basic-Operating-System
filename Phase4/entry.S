// entry.S, 159
// entry points at interrupt

#include <spede/machine/asmacros.h>
#include "entry.h"

// set DS and ES to Kernel's data segment
#define SET_KERNEL_SEGS movw $(KERNEL_DATA), %ax; mov %ax, %ds; mov %ax, %es

.comm kernel_stack, KERNEL_STACK_SIZE, 1 // define kernel stack space
.text

// Loader loads trapframe of a process to CPU registers, to run it
ENTRY(ProcLoader)
   movl 4(%esp), %eax   // load stack pointer from eax
   movl %eax, %esp
   popl %gs
   popl %fs
   popl %es
   popl %ds
   popa                 // restore general registers
   add $4, %esp         // skip 4 bytes that stored intr_id
   iret

// push intr type number then jump to common handler
ENTRY(TimerEntry)
   pushl $TIMER_INTR
   jmp EnterKernel
ENTRY(GetPidEntry)
   pushl $GETPID_INTR
   jmp EnterKernel
ENTRY(SleepEntry)
   pushl $SLEEP_INTR
   jmp EnterKernel
ENTRY(SemReqEntry)
   pushl $SEMREQ_INTR
   jmp EnterKernel
ENTRY(SemWaitEntry)
   pushl $SEMWAIT_INTR
   jmp EnterKernel
ENTRY(SemPostEntry)
   pushl $SEMPOST_INTR
   jmp EnterKernel
ENTRY(PrinterEntry)
   pushl $PRINTER_INTR
   jmp EnterKernel
// save CPU context, call KernelMain()
EnterKernel:
   pusha
   pushl %ds
   pushl %es
   pushl %fs
   pushl %gs
   movl %esp, %edx
   cld
   SET_KERNEL_SEGS      // set kernel data segments
   leal kernel_stack + KERNEL_STACK_SIZE, %esp
   pushl %edx
   call CNAME(KernelMain)

