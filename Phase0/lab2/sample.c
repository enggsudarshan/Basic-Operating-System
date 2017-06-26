#include<spede/stdio.h>
#include<spede/flames.h>

void DisplayMsg(int i)
{
  int j;
   for(j=0;j<=4;j++)
   {
    printf(" %d Hello world %d \nECS",i,2*i);
    cons_printf("--> Hello World <--\nCPE/CSC");
   i++;
   }
}
int main(void)
{
   int i;
   //int j;
   i=128;
   DisplayMsg(i);
   return 0;
}
