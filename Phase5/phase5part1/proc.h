// proc.h, 159

#ifndef _PROC_H_
#define _PROC_H_

//void ProcLoader();
void IdleProc();
void UserProc();
void TermProc();
void TermIn(char *,int which);
void TermOut(char *,int which);
#endif
