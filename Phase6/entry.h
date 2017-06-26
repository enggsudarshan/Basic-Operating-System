// entry.h, 159

#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <spede/machine/pic.h>


#define TIMER_INTR 32
#define GETPID_INTR 48
#define SLEEP_INTR 49
#define SEMREQ_INTR 50
#define SEMWAIT_INTR 51
#define SEMPOST_INTR 52
#define TERM1_INTR 35
#define TERM2_INTR 36
#define FSTAT_INTR 53
#define FOPEN_INTR 54
#define FREAD_INTR 55
#define FCLOSE_INTR 56

#define KERNEL_CODE 0x08         // kernel's code segment
#define KERNEL_DATA 0x10         // kernel's data segment
#define KERNEL_STACK_SIZE 16384  // kernel's stack byte size

#define IRQ0_INTR 0x60 
#define IRQ3_INTR 0x63 
#define IRQ4_INTR 0x64 

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
extern void Term1Entry();
extern void Term2Entry();
extern void FstatEntry();
extern void FopenEntry();
extern void FreadEntry();
extern void FcloseEntry();
__END_DECLS

#endif

#endif
