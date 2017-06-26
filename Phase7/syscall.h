#ifndef _SYSCALL_H_
#define _SYSCALL_H_

int GetPid(void);  // no input, 1 return
void Sleep(int);   // 1 input, no return

int SemReq(void);
void SemWait(int);
void SemPost(int);
void TripTermIRQ();
void Fstat(char *cmd_str, char *obj_data);
int Fopen(char *name);
void Fread(int fd, char *read_data);
void Fclose(int fd);
void SysWrite(char *p);  
void Fork(char *code_addr, int code_size); 
int Wait(int *exit_status); 
void Exit(int exit_status); 

#endif
