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
#define LOOP 1666667 
void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && cons_getchar() == 'b') breakpoint();
        		cons_printf("0..");
        	for(i=0;i< LOOP;i++)
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
	int i,pid ;

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

void PrinterProc() {
   int i, code;
   char key, *p;
   char hello[] = "Hello, World!\n\r";
   cons_printf("\n************************************************************\n");
   cons_printf("* Check power/cable/paper. Press 1 to print (or q to end). *\n");
   cons_printf("************************************************************\n");
   printer_sem =SemReq();
   outportb(LPT1_BASE+LPT_CONTROL, PC_SLCTIN);   
   code = inportb(LPT1_BASE+LPT_STATUS);         
   for(i = 0; i < 50; i++) IO_DELAY();                  
   outportb(LPT1_BASE+LPT_CONTROL, PC_INIT|PC_SLCTIN|PC_IRQEN); 

 
   while(1) 
   {
 
      Sleep(10);//for(i=0; i<LOOP; i++) IO_DELAY();
      if (!cons_kbhit())  continue;
      key = cons_getchar();
      if (key == 'q') exit(0);
      if( key == 'b') breakpoint();
       cons_printf("Key Pressed %c\n",key);

     // if((key != '1' || key != '2'))  continue;
       p=hello;
       while(*p) {
         
        outportb(LPT1_BASE+LPT_DATA, *p);       // write char to DATA reg
       
         code = inportb(LPT1_BASE+LPT_CONTROL);  // read CONTROL reg

	
	if(key == '1') {
	
         SemWait(printer_sem);
       } //work for 1 but not for 2
         outportb(LPT1_BASE+LPT_CONTROL, code|PC_STROBE); // write CONTROL, STROBE added
         for(i = 0; i < 50; i++) IO_DELAY();              // needs delay
         outportb(LPT1_BASE+LPT_CONTROL, code);  // send back original CONTROL

	if(key == '1') 
           {
for(i = 0; i < LOOP*3; i++) {

            code = PS_ACK & inportb(LPT1_BASE + LPT_STATUS);
        
            if(code == 0) break;      // printer ACK'ed
            IO_DELAY(); 
             // otherwise, wait 0.6 us, and loop
         } 

         if(i == LOOP*3) {         // if 3 sec has passed
            cons_printf(">>> Time out while printing a char!\n");
            break;                    // timed out, return -1
         }
}
if(key == '2')
{

SemWait(printer_sem);
}
                 	
         
               p++;
         }//end while(*p)


         }//while 1
   

}
      
