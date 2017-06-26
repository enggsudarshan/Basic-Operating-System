// entry.h, 159

#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <spede/machine/pic.h>

#define TIMER_INTR 32
#define PRINTER_INTR 39

#define GETPID_INTR 48
#define SLEEP_INTR 49
#define SEMREQ_INTR 50
#define SEMWAIT_INTR 51
#define SEMPOST_INTR 52

#define KERNEL_CODE 0x08         // kernel's code segment
#define KERNEL_DATA 0x10         // kernel's data segment
#define KERNEL_STACK_SIZE 16384  // kernel's stack byte size

// ISR Entries
#ifndef ASSEMBLER

__BEGIN_DECLS

#include "TF.h"
//#include "syscall.h"

extern void ProcLoader(TF_t *);     // code defined in entry.S
extern void TimerEntry();        // code defined in entry.S
extern void GetPidEntry();
extern void SleepEntry();
extern void SemReqEntry();
extern void SemWaitEntry();
extern void SemPostEntry();
extern void PrinterEntry();
__END_DECLS

#endif

#endif
