// syscall.c
// software interrupt/syscalls, i.e., kernel 
#include "syscall.h"
int GetPid() {                   // no input, has return
   int pid;

   asm("int $48; movl %%eax, %0" // CPU inst
       : "=g" (pid)              // output from asm("...")
       :                         // no input into asm("...")
       : "%eax");                // will get pushed before asm("..."), and popped after

   return pid;
}

void Sleep(int centi_seconds) {   // has input, no return

   asm("movl %0, %%eax; int $49" // CPU inst
       :                         // no output from asm("...")
       : "g" (centi_seconds)      // input into asm("...")
       : "%eax");                // will get pushed before asm("..."), and popped after
}


int SemReq(void)
{
	int semid;

   asm("int $50; movl %%eax, %0" // CPU inst
       : "=g" (semid)              // output from asm("...")
       :                         // no input into asm("...")
       : "%eax");                // will get pushed before asm("..."), and popped after

   return semid;
	
}

void SemWait(int semid)
{
	asm("movl %0, %%eax; int $51" // CPU inst
       :                         // no output from asm("...")
       : "g" (semid)      // input into asm("...")
       : "%eax");                // will get pushed before asm("..."), and popped after
}

void SemPost(int semid)
{
	asm("movl %0, %%eax; int $52" // CPU inst
       :                         // no output from asm("...")
       : "g" (semid)      // input into asm("...")
       : "%eax");                // will get pushed before asm("..."), and popped after
}

void TripTermIRQ()
{
	asm("int $35");
}


void Fstat(char *cmd_str, char *obj_data)
{
	asm("movl %0, %%eax; movl %1, %%ebx; int $53" // CPU inst
       :                         // no output from asm("...")
       : "g" ((int)cmd_str), "g" ((int)obj_data)        // input into asm("...") 
       : "%eax", "%ebx");               // will get pushed before asm("..."), and popped after
}

int Fopen(char *name) 
{        // name of the file to open
         int fd;
         asm("movl %1, %%eax; int $54; movl %%ebx, %0" // in-intr-out
            : "=g" (fd)              // return file descriptor
            : "g" ((int)name)        // type-cast to int
            : "%eax", "%ebx");
         return fd;
}

void Fread(int fd, char *read_data)
{
	asm("movl %0, %%eax; movl %1, %%ebx; int $55" // CPU inst
       :                         // no output from asm("...")
       : "g" (fd) , "g" ((int)read_data)      // input into asm("...") 
       : "%eax", "%ebx");               // will get pushed before asm("..."), and popped after
}

void Fclose(int fd)
{
	asm("movl %0, %%eax; int $56" // CPU inst
       :                         // no output from asm("...")
       : "g" (fd)       // input into asm("...")
       : "%eax");               // will get pushed before asm("..."), and popped after
}
