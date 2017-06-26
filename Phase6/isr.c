// isr.c, 159
/***********************************************************/
#include "spede.h"
#include "typedef.h"
#include "isr.h"
#include "tools.h"
#include "extern.h"
#include "proc.h"
/***********************************************************/
void NewProcISR(int new_pid, func_ptr_t p) 
{
	MyBzero((char *)&pcb[new_pid], sizeof(pcb_t));
	pcb[new_pid].state=READY;
	pcb[new_pid].runtime=pcb[new_pid].lifespan=0;
     	if(new_pid > 0)        
       		EnQ(new_pid,&ready_q);
     	MyBzero((char *)&proc_stack[new_pid], PROC_STACK_SIZE);
	
     	pcb[new_pid].TF_p = (TF_t *) &proc_stack[new_pid][PROC_STACK_SIZE-sizeof(TF_t)];

	pcb[new_pid].TF_p->eip=(unsigned int) p;

    	pcb[new_pid].TF_p->eflags = EF_DEFAULT_VALUE|EF_INTR;
    	pcb[new_pid].TF_p->cs = get_cs();                     
    	pcb[new_pid].TF_p->ds = get_ds();                
    	pcb[new_pid].TF_p->es = get_es();                   
    	pcb[new_pid].TF_p->fs = get_fs();              
    	pcb[new_pid].TF_p->gs = get_gs();                   
}

void KillProcISR() 
{
	if(run_pid == 0)   
     		return;
    	pcb[run_pid].state=AVAIL;  //change state of running process to AVAIL
    	EnQ(run_pid,&ready_q);  //queue the running PID to available queue
    	run_pid=-1;//set running PID to -1 (now none running)
  
}        


void TimerISR() 
{
	int i; 
 	system_time++;
	for ( i=1 ;i<Q_SIZE-1;i++)
	{
     		if((pcb[i].state ==SLEEP) && (pcb[i].wake_time<=system_time))  
 		{
 			EnQ(i, &ready_q);
        		pcb[i].state=READY;
        	}
	}
	pcb[run_pid].runtime++;             //upcount its runtime and lifespan (in its PCB)
     	pcb[run_pid].lifespan++;
   	if(pcb[run_pid].runtime==TIME_SLICE) 
   	{
      		pcb[run_pid].lifespan+=pcb[run_pid].runtime;
      		pcb[run_pid].runtime= 0;
      		pcb[run_pid].state=READY;     // change its state to READY
      		EnQ(run_pid,&ready_q);        //queue its PID (run_pid) to ready_q
      		run_pid=-1;                   //clear run_pid to -1 (none running now)
    	}  
}


void SleepISR()
{
	pcb[run_pid].wake_time = system_time + pcb[run_pid].TF_p->eax;
	pcb[run_pid].state = SLEEP; 
        run_pid = -1;
}


void GetPidISR()
{
	pcb[run_pid].TF_p->eax=run_pid;
}

void SemReqISR()
{
	int sem_num;

	sem_num = DeQ(&avail_sem_q);
	if(sem_num > -1)
		MyBzero((char *)&(sem[sem_num]) ,sizeof(sem_t));

	pcb[run_pid].TF_p->eax = sem_num;
}

void SemWaitISR(int sem_id)
{
	if(sem[sem_id].count > 0)
		sem[sem_id].count--;
	else
	{	
		EnQ(run_pid,&sem[sem_id].wait_q);
		pcb[run_pid].state = WAIT;
		run_pid=-1;
	}
}

void SemPostISR(int sem_id)
{
	int proc_id;
	if(sem[sem_id].wait_q.size == 0)
		sem[sem_id].count++;
	else
	{
		proc_id = DeQ(&sem[sem_id].wait_q);
		pcb[proc_id].state = READY;
		EnQ(proc_id,&ready_q);	
	}
	
}

void TermISR()
{
	int status,i;
	
	for(i=0;i<3;i++)
	{
		status =  inportb(term[i].io_base+IIR);
		switch(status)
		{
        		case IIR_TXRDY : TermOutHandler(i);
	 		  	 	 break;
			case IIR_RXRDY : TermInHandler(i);
			  	 	 break;
		}
		if(term[i].out_flag == 1)
		{
			TermOutHandler(i);
		}
	}
}

void TermOutHandler(int which)
{
	char ch = 0; 
	if(term[which].echo_q.size >0)
	{
		ch = DeQ(&term[which].echo_q);
	}
	else
	{
		if(term[which].out_q.size > 0)
		{
			ch = DeQ(&term[which].out_q);
			SemPostISR(term[which].out_q_sem);
		}
	}
	if (ch != 0)
	{
		outportb(term[which].io_base+DATA, ch);
		term[which].out_flag = 0;
	} 
	else 
	{
		term[which].out_flag = 1;
	}
}

void TermInHandler(int which)
{
	char ch;
	ch = inportb(term[which].io_base+DATA) & 0x7F; 
	EnQ(ch,&term[which].in_q);
	SemPostISR(term[which].in_q_sem);
	
	if (ch == '\r')
	{
		EnQ('\r',&term[which].echo_q);
		EnQ('\n',&term[which].echo_q);
	}
	else 
	{
		if (term[which].echo_flag == 1)
		{	
			EnQ(ch,&term[which].echo_q);
		}
		else
		{
			EnQ('*',&term[which].echo_q);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////

void FstatISR() {
   char *name, *obj_data;
   attr_t *attr_p;
   dir_t *dir_p;

   name = (char *)pcb[run_pid].TF_p->eax;
   obj_data = (char *)pcb[run_pid].TF_p->ebx;

   dir_p = FindName( name );

   if( ! dir_p ) {      // dir_p == 0, not found
      obj_data[0] = 0;  // null terminated, not found, return
      return;
   }

   attr_p = (attr_t *)obj_data;
   Dir2Attr( dir_p, attr_p ); // copy what dir_p points to to where attr_p points to

// should include filename (add 1 to length for null char)
   MyMemcpy( (char *)( attr_p + 1 ), dir_p->name, MyStrlen( dir_p->name ) + 1 );
}

void FopenISR(void) {
   char *name;
   int fd;
   dir_t *dir_p;

   name = (char *)pcb[run_pid].TF_p->eax;

   fd = AllocFD( run_pid );  // run_pid is owner of fd allocated

   if( fd == -1 ) {
      cons_printf( "FopenISR: no more File Descriptor!\n" );
      pcb[run_pid].TF_p->ebx = -1;
      return;
   }

   dir_p = FindName( name );
   if( ! dir_p ) {
      cons_printf( "FopenISR: name not found!\n" );
      pcb[run_pid].TF_p->ebx = -1;
      return;
   }

   fd_array[fd].item = dir_p;    // dir_p is the name
   pcb[run_pid].TF_p->ebx = fd;  // process gets this to future read
}

void FcloseISR(void) {
   int fd, result;

   fd = pcb[run_pid].TF_p->eax;

   result = CanAccessFD( fd, run_pid ); // check if run_pid owns this FD
   if( result == 0 ) FreeFD( fd );
   else  cons_printf( "FcloseISR: cannot close FD!\n" );
}

// Copy bytes from file into user's buffer. Returns actual count of bytes
// transferred. Read from fd_array[fd].offset (initially given 0) for
// buff_size in bytes, and record the offset. may reach EOF though...
void FreadISR(void) {
   int fd, result, remaining;
   char *read_data;
   dir_t *lp_dir;

   fd = pcb[run_pid].TF_p->eax;
   read_data = (char *)pcb[run_pid].TF_p->ebx;

   result = CanAccessFD( fd, run_pid ); // check if run_pid owns this FD

   if( result != 0 ) {
      cons_printf( "FreadISR: cannot read from FD!\n" );
      read_data[0] = 0;  // null-terminate it
   }

   lp_dir = fd_array[fd].item;

   if( A_ISDIR(lp_dir->mode ) ) {  // it's a dir
// if reading directory, return attr_t structure followed by obj name.
// a chunk returned per read. `offset' is index into root_dir[] table.
      dir_t *this_dir = lp_dir;
      attr_t *attr_p = (attr_t *)read_data;
      dir_t *dir_p;

      if( 101 < sizeof( *attr_p ) + 2) {
         cons_printf( "FreadISR: read buffer size too small!\n" );
         read_data[0] = 0;  // null-terminate it
         return;
      }

// use current dir, advance to next dir for next time when called
      do {
         dir_p = ( (dir_t *)this_dir->data );
         dir_p += fd_array[fd].offset ;

         if( dir_p->inode == END_DIR_INODE ) {
            read_data[0] = 0;  // EOF, null-terminate it
            return;
         }
         fd_array[fd].offset++;   // advance
      } while( dir_p->name == 0 );

// MyBzero() fills buff with 0's, necessary to clean buff
// since Dir2Attr may not completely overwrite whole buff...
      MyBzero( read_data, 101 );
      Dir2Attr( dir_p, attr_p );

// copy obj name after attr_t, add 1 to length for null
      MyMemcpy((char *)( attr_p + 1 ), dir_p->name, MyStrlen( dir_p->name ) + 1);

   } else {  // a file, not dir
// compute max # of bytes can transfer then MyMemcpy()
      remaining = lp_dir->size - fd_array[fd].offset;

      if( remaining == 0 ) {
         read_data[0] = 0;  // EOF, null-terminate it
         return;
      }

      MyBzero( read_data, 101 );  // null termination for any part of file read

      result = remaining<100?remaining:100; // -1 saving is for last null

      MyMemcpy( read_data, &lp_dir->data[ fd_array[ fd ].offset ], result );

      fd_array[fd].offset += result;  // advance our "current" ptr
   }
}

// check ownership of fd and the fd is valid within range
int CanAccessFD( int fd, int owner ) {
   if( VALID_FD_RANGE(fd) && fd_array[fd].owner == owner) return 0;
   return -1;     // not good
}

// Search our (fixed size) table of file descriptors. returns fd_array[] index
// if an unused entry is found, else -1 if all in use. if avail, then all
// fields are initialized.
int AllocFD( int owner ) {
   int i;

   for(i=0; i<MAX_FD; i++) {
      if( -1 == fd_array[i].owner ) {
         fd_array[i].owner = owner;
         fd_array[i].offset = 0;
         fd_array[i].item = 0;     // NULL is (void *)0, spede/stdlib.h

         return i;
      }
   }

   return -1;   // no free file descriptors
}

void FreeFD( int fd ) {  // mark a file descriptor as now free
   fd_array[fd].owner = -1;
}

dir_t *FindName( char *name ) {
   dir_t *starting;

// assume every path relative to root directory. Eventually, the user
// context will contain a "current working directory" and we can possibly
// start our search there
   if( name[0] == '/' ) {
      starting = root_dir;

      while( name[0] == '/' ) name++;

      if( name[0] == 0 ) return root_dir; // client asked for "/"
   } else {
// path is relative, so start off at CWD for this process
// but we don't have env var CWD, so just use root as well
      starting = root_dir; // should be what env var CWD is
   }

   if( name[0] == 0 ) return 0;

   return FindNameSub( name, starting );
}

// go searching through a single dir for a name match. use MyStrcmp()
// for case-insensitive compare. use '/' to separate directory components
// if more after '/' and we matched a dir, recurse down there
// RETURN: ptr to dir entry if found, else 0
// once any dir matched, don't return name which dir was matched
dir_t *FindNameSub( char *name, dir_t *this_dir ) {
   dir_t *dir_p = this_dir;
   int len = MyStrlen( name );
   char *p;

// if name is '.../...,' we decend into subdir
   if( ( p = strchr( name, '/' ) ) != 0) len = p - name;  // p = to where / is (high mem)

   for( ; dir_p->name; dir_p++ ) {
//      if((unsigned int)dir_p->name > 0xdfffff) return 0; // tmp bug-fix patch

      if( 1 == MyStrcmp( name, dir_p->name, len ) ) {
         if( p && p[1] != 0 ) { // not ending with name, it's "name/..."
// user is trying for a sub-dir. if there are more components, make sure this
// is a dir. if name ends with "/" we don't check. thus "hello.html/" is legal
            while( *p == '/' ) {
               p++;                           // skipping trailing /'s in name
               if( '\0' == *p ) return dir_p; // name "xxx/////" is actually legal
            }

// altho name given is "xxx/yyy," xxx is not a directory
            if(dir_p->mode != MODE_DIR) return 0; // bug-fix patch for "cat h/in"

            name = p;
            return FindNameSub( name, (dir_t *)dir_p->data );
         }
         return dir_p;
      }
   }

   return 0;   // no match found
}

// copy what dir_p points to (dir_t) to what attr_p points to (attr_t)
void Dir2Attr( dir_t *dir_p, attr_t *attr_p ) {
   attr_p->dev = run_pid;            // run_pid manages this i-node

   attr_p->inode = dir_p->inode;
   attr_p->mode = dir_p->mode;
   attr_p->nlink = ( A_ISDIR( attr_p->mode ) ) + 1;
   attr_p->size = dir_p->size;
   attr_p->data = dir_p->data;
}

