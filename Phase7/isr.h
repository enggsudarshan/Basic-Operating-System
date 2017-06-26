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
void TermISR();
void TermOutHandler(int which);
void TermInHandler(int which);
void FstatISR();
void FopenISR(void);
void FcloseISR(void);
void FreadISR(void);
int CanAccessFD( int fd, int owner );
int AllocFD( int owner );
void FreeFD( int fd );
dir_t *FindName( char *name );
dir_t *FindNameSub( char *name, dir_t *this_dir );
void Dir2Attr( dir_t *dir_p, attr_t *attr_p );
void ForkISR(void);
void WaitISR(void);
void ExitISR(void);
void SysWriteISR(void);
#endif
