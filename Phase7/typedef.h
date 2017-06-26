// typedef.h, 159

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include "TF.h"              // trapframe type TF_t is defined here

#define TIME_SLICE 10       // max timer count, then rotate process
#define PROC_NUM 20          // max number of processes
#define Q_SIZE 20            // queuing capacity
#define PROC_STACK_SIZE 4096 // process runtime stack in bytes

#define MAX_FD 20                             // # of avail FD
#define VALID_FD_RANGE(fd) (-1 < fd && fd < MAX_FD) // FD #-range check

#define A_MT  0xF000 // Attribute Identifier Flag Mask Table for mask & compare
#define A_DIR 0x4000 // DIRectory type
#define A_REG 0x8000 // REGular file type

#define A_RWXU 00700 // owner: read, write, execute
#define A_RWXG 00070 // group: read, write, execute
#define A_RWXO 00007 // other: read, write, execute
#define A_ROTH 00004 // other: read
#define A_WOTH 00002 // other: write
#define A_XOTH 00001 // other: execute

// macros to extract informaton from "mode" field of "attr_t"
// is it a directory or regular file?
#define A_ISDIR(mode) (((mode) & A_MT) == A_DIR)
#define A_ISREG(mode) (((mode) & A_MT) == A_REG)

#define MODE_EXEC (0777 | A_REG)
#define MODE_FILE (0666 | A_REG)
#define MODE_READFILE (0444 | A_REG)
#define MODE_DIR (0777 | A_DIR)

#define END_DIR_INODE ((unsigned int)(~0)) // end marker of directory content

#define BUF_SIZ 100
#define PAGE_NUM 100
#define PAGE_SIZE 4096

typedef void (*func_ptr_t)(); // void-return function pointer type

// this is the same as constants defines: AVAIL=0, READY=1, etc.
typedef enum {AVAIL, READY, RUN, SLEEP, WAIT, ZOMBIE, WAIT2CHILD} state_t;

typedef struct {             // PCB describes proc image
   state_t state;            // state of process
   int runtime,              // runtime since loaded to run
       lifespan,             // total runtime since created
       ppid,                 // parent PID
       cpid;  
    	int wake_time;              //Phase-2
   TF_t *TF_p;               // points to trapframe of process
} pcb_t;

typedef struct {             // generic queue type
   int q[Q_SIZE];            // integers are queued in q[] array
   int size;                 // size is also where the tail is for new data
} q_t;

typedef struct
{
	q_t wait_q;
	int count;	
} sem_t;

typedef struct 
{             // phase 5, terminal
	q_t in_q,
	out_q,
	echo_q;
	int in_q_sem,out_q_sem,echo_flag,out_flag,io_base;
	char stdout_q[BUF_SIZ+1];
} term_t;


typedef struct {
   int inode,   // inode on the device
       mode,    // file access mode
       dev,     // PID of file system
       nlink,   // # of links to the file
       size;    // file size
   char *data;  // file content
} attr_t;

typedef struct { // directory type
   int inode,
       mode,
       size;
   char *name,
        *data;
} dir_t;

typedef struct {      // file descriptor type
   int owner,         // PID, -1 means not used
       offset;        // can be negative
   dir_t *item,
         *owning_dir; // dir where `item' resides
} fd_t;

typedef struct {
	int owner, addr;
} page_info_t;

#endif
