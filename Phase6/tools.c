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

void MyStrcpy(char *d, char *s){

	while (*s)
	{
		*d++  = *s++;
	}
	*d = '\0';


}

int MyStrcmp(char *p, char *q, int size)
{
	while (size > 0){
		if (*p == '\0' && *q == '\0') return 1;
		if (*p != *q) return 0;
		p++;
		q++;
		size--;
	}
	return 1;
}

int MyStrlen(char *s)
{
	int length=0;
	while(*s++)
	{
		length++;
	}
	return length;

}

void MyMemcpy(char *dest, char *src, int size)
{
	while(size--)
	{
		*dest = *src;
		dest++;
		src++;
	}
}
