// isr.h, 159

#ifndef _ISR_H_
#define _ISR_H_

#include "typedef.h" // needed to define func_ptr_t

void NewProcISR(int, func_ptr_t);
void KillProcISR();
void TimerISR();
void GetPidISR();
void SleepISR();
void SemReqISR();
void SemPostISR(int sem_id);
void SemWaitISR(int sem_id);
#endif
