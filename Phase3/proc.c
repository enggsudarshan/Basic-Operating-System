// proc.c, 159
// processes are coded here
//In Real Kernel (Kernel Process and User Process)
//Code defined here
/********************************************************************/
#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions
#include "syscall.h"
/********************************************************************/
unsigned short  *vid_mem_ptr = (unsigned short * )0XB8000+13*80+40;
unsigned short  *vid_mem_ptr1 = (unsigned short * )0XB8000+13*80+39;
unsigned short  *vid_mem_ptr2 = (unsigned short * )0XB8000+13*80+38;
#define add (0x0f00+'0')
void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && cons_getchar() == 'b') breakpoint();
        		cons_printf("0..");
        	for(i=0;i< 1666667;i++)
        		IO_DELAY();
      	}
}


void UserProc() 
{
	int pid;
	int  sleep_seconds;
   	while(1)
	{
   		
		pid = GetPid();
       		sleep_seconds = 75 + ((pid-1)%4)*75;
      		cons_printf("%d..",pid); 
		Sleep(sleep_seconds);
      }
}


void Producer()
{
    int temp,temp1,temp2,temp3,temp4,temp5;
	int i,pid;
	pid = GetPid();
	if(product_num == -1)
	{
		product_sem = SemReq();
		SemPost(product_sem);
		product_num = 0;
	}
	while(1)
	{
		SemWait(product_sem);
		cons_printf("\n++ Producer %d producing... ", pid);
		for(i=0; i<1666667;i++)
			IO_DELAY();
		product_num++;
		if(product_num < 10)
		{
			*vid_mem_ptr=add + product_num;
			*vid_mem_ptr1=add + 0;
			*vid_mem_ptr2=add + 0;
		}
		else
		{
			temp = product_num/10;
			temp5 = product_num%10;
			if(temp < 10)
			{
				if(product_num%10==0)
				{
					*vid_mem_ptr=add + 0;
					*vid_mem_ptr1=add + temp;
					*vid_mem_ptr2=add + 0;
				}
				else
				{
					*vid_mem_ptr=add + temp5;
					*vid_mem_ptr1=add + temp;
					*vid_mem_ptr2=add + 0;
				}
				
			}
			else
			{
				temp1 = product_num/100;
				temp2 = product_num%10;
				temp3 = product_num/10;
				temp4 = temp3%10;
				if(product_num%10==0)
				{
					*vid_mem_ptr=add + 0;
					*vid_mem_ptr1=add + 0;
					*vid_mem_ptr2=add + temp1;
				}	
				else
				{
					*vid_mem_ptr=add + temp2;
					*vid_mem_ptr1=add + temp4;
					*vid_mem_ptr2=add + temp1;
				}
			}
			
		}
		cons_printf("  product # is now %d++ ", product_num);
		SemPost(product_sem);
	}
}

void Consumer()
{
	int i,pid;
	    int temp,temp1,temp2,temp3,temp4,temp5;
	pid = GetPid();
	if(product_num == -1)
	{
		product_sem = SemReq();
		SemPost(product_sem);
		product_num = 0;
	}
	while(1)
	{
		SemWait(product_sem);
		cons_printf("\n-- Consumer %d consuming... ", pid);
		for(i=0; i<1666667;i++)
			IO_DELAY();
		product_num--;
		if(product_num < 10)
		{
			*vid_mem_ptr=add + product_num;
			*vid_mem_ptr1=add + 0;
			*vid_mem_ptr2=add + 0;
		}
		else
		{
			temp = product_num/10;
			temp5 = product_num%10;
			if(temp < 10)
			{
				if(product_num%10==0)
				{
					*vid_mem_ptr=add + 0;
					*vid_mem_ptr1=add + temp;
					*vid_mem_ptr2=add + 0;
				}
				else
				{
					*vid_mem_ptr=add + temp5;
					*vid_mem_ptr1=add + temp;
					*vid_mem_ptr2=add + 0;
				}
				
			}
			else
			{
				temp1 = product_num/100;
				temp2 = product_num%10;
				temp3 = product_num/10;
				temp4 = temp3%10;
				if(product_num%10==0)
				{
					*vid_mem_ptr=add + 0;
					*vid_mem_ptr1=add + 0;
					*vid_mem_ptr2=add + temp1;
				}	
				else
				{
					*vid_mem_ptr=add + temp2;
					*vid_mem_ptr1=add + temp4;
					*vid_mem_ptr2=add + temp1;
				}
			}
			
		}
		cons_printf("  product # is now %d--", product_num);
		SemPost(product_sem);
	}
		
}
