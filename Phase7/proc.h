// proc.h, 159

#ifndef _PROC_H_
#define _PROC_H_

//void ProcLoader();
void IdleProc();
void TermProc();
void TermIn(char *,int which);
void TermOut(char *,int which);
void TermCatCmd(char *cmd_str, int which);
void TermLsCmd(char *cmd_str, int which);
void Attr2Str(attr_t *attr_p, char *str);
#endif
