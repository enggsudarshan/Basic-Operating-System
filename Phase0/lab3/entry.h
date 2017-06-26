//******************************************************
//entry.h of entry.S Phase 0 ,Excersie 3 -- Timer Lab
//****************************************************
#ifndef _ENTRY_H_
#define _ENTRY_H_

#define TIMER_INTR 32 

#ifndef ASSEMBLER                  //skip if ASSEMBLER defined (in assembly code)
void TimerEntry();                 //defined in entry.S assembly won't take this
#endif
#endif

