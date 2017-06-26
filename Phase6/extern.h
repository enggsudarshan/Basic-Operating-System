// extern.h, 159

#ifndef _EXTERN_H_
#define _EXTERN_H_

#include "typedef.h"                // defines q_t, pcb_t, MAX_PROC_NUM, PROC_STACK_SIZE

extern int run_pid;                 // PID of currently-running process, -1 means none
extern q_t ready_q, avail_q,avail_sem_q;        // created/ready to run, and available/un-used PID's
extern pcb_t pcb[PROC_NUM];         // process table
extern char proc_stack[PROC_NUM][PROC_STACK_SIZE]; // process runtime stacks
extern int system_time;
extern sem_t sem[Q_SIZE];
extern term_t term[3];

///////////////////////////////////// phase 6 file system
extern char help_txt_data[];
extern char note_txt_data[];
extern char index_html_data[];
extern char hello_html_data[];
extern dir_t bin_dir[4];
extern dir_t www_dir[8];
extern dir_t root_dir[11];
extern fd_t fd_array[MAX_FD];

#endif  
