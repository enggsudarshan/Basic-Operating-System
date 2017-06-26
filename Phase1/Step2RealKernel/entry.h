// entry.h, 159

#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <spede/machine/pic.h>

#define TIMER_INTR 32

#define KERNEL_CODE 0x08         // kernel's code segment
#define KERNEL_DATA 0x10         // kernel's data segment
#define KERNEL_STACK_SIZE 16384  // kernel's stack byte size

// ISR Entries
#ifndef ASSEMBLER

__BEGIN_DECLS

#include "TF.h"

extern void ProcLoader(TF_t *);     // code defined in entry.S
extern void TimerEntry();        // code defined in entry.S

__END_DECLS

#endif

#endif
