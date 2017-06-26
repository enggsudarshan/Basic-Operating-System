// tools.c, 159
//EnQueue and Dequeue functions are defined 

/********************************************************************/
#include "spede.h"
#include "typedef.h"
#include "extern.h"
/********************************************************************/

void MyBzero(char *p, int size) 
{
	while(size--)
  	{
   		*p++=0;   
  	}
}


void EnQ(int data, q_t *p) 
{
	if (p->size == Q_SIZE)
      	{
       		cons_printf("\nQueue is Full\n"); 
       		return;
      	}
      		p->q[p->size] =data;
      		p->size++;
}


int DeQ(q_t *p) 
{
	int  i ,data;
	if(p->size == 0)
	{
       		return -1;
     	}

     	data =p->q[0];
     	for( i= 0;i<p->size-1;i++) 
         	p->q[i]=p->q[i+1];
     	p->size--;
     	return data;
}


