// proc.c, 159
// processes are coded here
//In Real Kernel (Kernel Process and User Process)
//Code defined here
/********************************************************************/
#include "spede.h"      // for IO_DELAY() needed here below
#include "extern.h"     // for currently-running pid needed below
#include "proc.h"       // for prototypes of process functions
#include "syscall.h"
#include "tools.h"
#include "typedef.h"
/********************************************************************/

void IdleProc() 
{
	int i;
	while(1)
      	{
		if(cons_kbhit() && cons_getchar() == 'b') 
			breakpoint();
        	cons_printf("0..");
        	for(i=0;i< 1666667;i++)
        		IO_DELAY();
      	}
}


void TermProc()
{
   	int i, baud_rate, divisor,local,k,length;
   	char login[101], passwd[101], cmd_str[101];
  	local=GetPid()-1;
   	term[local].out_q_sem=SemReq();
   	for(i = 0; i <Q_SIZE ; i++)
     	{
       		SemPost(term[local].out_q_sem);
	}
      	term[local].in_q_sem=SemReq();
      	term[local].out_flag =1;
      	term[local].echo_flag=1;
	for(k =0;k<3;k++)
	{
   		baud_rate = 9600;              // Mr. Baud invented this
   		divisor = 115200 / baud_rate;  // time period of each baud
   		outportb(term[k].io_base+CFCR, CFCR_DLAB);          // CFCR_DLAB 0x80
	   	outportb(term[k].io_base+BAUDLO, LOBYTE(divisor));
   		outportb(term[k].io_base+BAUDHI, HIBYTE(divisor));
   		outportb(term[k].io_base+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);
   		outportb(term[k].io_base+IER, 0);
   		outportb(term[k].io_base+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
   		IO_DELAY();
   		outportb(term[k].io_base+IER, IER_ERXRDY|IER_ETXRDY); // enable TX, RX events
		Sleep(50);
	}
	for(i=0;i<24;i++)
	{
		TermOut("\n",local);
	}
	MyBzero((char *)cmd_str, 101);
   	while(1) 
	{
		while(1)
		{

			TermOut("Team RASOS Login: ",local);
			TermIn(login,local);
			TermOut("\n",local);

			TermOut("Password:",local);
			term[local].echo_flag=0;
			TermIn(passwd,local);
			TermOut("\n",local);
			term[local].echo_flag=1;
			if(!MyStrcmp(passwd, "pizza", 5))
			{
				TermOut("Stop hacking 'round!!!\n",local);
	 		}
			else
			{
				cons_printf("Term %d Login: %s   Passwd: %s\n",GetPid(),login,passwd);
				break;
			}

   		}
		while(1)
		{
			TermOut("(RASOS) UserShell -> ",local);
			TermIn(cmd_str,local);
			length = MyStrlen(cmd_str);
			if(length == 0)
				continue;
			if (MyStrcmp(cmd_str, "logout", 6) || MyStrcmp(cmd_str, "000000", 6))
			{
	 			break;
	 		} 
			else if (MyStrcmp(cmd_str, "cat", 3) || MyStrcmp(cmd_str, "222", 3))
			{
	 			TermCatCmd(cmd_str,local);
	 		} 
			else if (MyStrcmp(cmd_str, "ls", 2) || MyStrcmp(cmd_str, "11", 2))
			{
				TermLsCmd(cmd_str,local);
	 		} 
			else
			{
				TermOut("Command Not Found\n",local);
			}
		}
	}
}

void TermCatCmd(char *cmd_str, int which) 
{ 
	char obj_data[101], read_data[101];
      	attr_t *attr_p;
	int fd;
     	cmd_str += 4;
	Fstat(cmd_str,obj_data);	

      	if(obj_data[0] ==0)
	{
		TermOut("Fstat: No such file(stat failed)!\n",which);
		return;
	}
    
        attr_p = (attr_t *)obj_data;     
        if(A_ISDIR(attr_p->mode)) 
	{        
        	TermOut("\aUsage: cat a file, not directory!\n", which);
            	return;
        }
      	fd =Fopen(cmd_str);
      	while(1)
	{
        	Fread(fd,read_data);
         	if (read_data[0] == '\0') 
			break;
      		TermOut(read_data, which);
	}
     	Fclose(fd);
}


void TermLsCmd(char *cmd_str, int which) 
{ 
	char obj_data[101],read_data[101];
      	attr_t *attr_p;
	int fd;
     	cmd_str += 2;
	if(cmd_str[0] == 0)
	{
		cmd_str[0] = '/';
		cmd_str[1] = (char)0;
	}
	else
	{
		cmd_str++;
	}

	Fstat(cmd_str,obj_data);
	
      	if  (obj_data[0] ==0)
	{
		TermOut("Fstat: No such file(stat failed)!\n",which);
		return;
	}
        	
	attr_p = (attr_t *)obj_data;
	if(A_ISREG(attr_p->mode)) 
	{
		Attr2Str(attr_p,read_data);
		TermOut(read_data, which);
		return;
	}
//	Attr2Str(attr_p,read_data);
	//printf("attr_p->size = %d\n",attr_p->size);
	//TermOut(read_data, which);
	fd =Fopen(cmd_str);
	
      	while(1)
	{
        	Fread(fd,obj_data);
         	if (obj_data[0] == '\0') 
			break;
		attr_p = (attr_t *)obj_data;
		Attr2Str(attr_p,read_data);
      		TermOut(read_data, which);
 		//Fstat(read_data,obj_data);
	}
     	Fclose(fd);
}

   void Attr2Str(attr_t *attr_p, char *str) { // make str from attributes in str
   // str contains: attr_t and name, p+1 points to name
      char *name = (char *)(attr_p + 1);
	
   // make str from the attributes attr_p points
      sprintf(str, " ---- size=%4d  name=%s\n", attr_p->size, name); // cheats
      if ( A_ISDIR(attr_p->mode) ) str[1] = 'd';         // mode is directory
      if ( QBIT_ON(attr_p->mode, A_ROTH) ) str[2] = 'r'; // mode is readable
      if ( QBIT_ON(attr_p->mode, A_WOTH) ) str[3] = 'w'; // mode is writable
      if ( QBIT_ON(attr_p->mode, A_XOTH) ) str[4] = 'x'; // mode is executable
   }

void TermIn(char *str,int which)
{
	char ch, *p;
	int ch_count=0;
	p = str;
	while(1)
	{
		SemWait(term[which].in_q_sem);
		ch = DeQ(&term[which].in_q);
		if(ch == '\r')
		{
			break;
		}
		else
		{
			*p++ = ch;
		}
		ch_count++;
		if(ch_count==100)
			break;	
	}		
	*p = '\0';
}

void TermOut(char *str,int which)
{
	char *p;
   	p=str;
   	while(*p)
	{
     		SemWait(term[which].out_q_sem);
		EnQ(*p,&term[which].out_q);
		if(*p=='\n')
		{
			SemWait(term[which].out_q_sem);
			EnQ('\r',&term[which].out_q);
		}
		TripTermIRQ();
		p++;
	}
}









