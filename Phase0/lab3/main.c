//**************************************************************
//Name : Reena Dalavi
//Phase 0 :Excersie 3 -- Timer Lab
//main.c
//**************************************************************

//Header file inclusion
#include<spede/stdio.h>                               //putchar funtion
#include<spede/flames.h>                              //cons_printf function
#include<spede/machine/io.h>                          //ouportb function
#include<spede/machine/proc_reg.h>                    //struct defined i386_gate and functionget_idt_base()
#include<spede/machine/seg.h>                         //fill_gate function
#include<spede/machine/pic.h>                         //PIC control
#include "entry.h"                                    //needs addr of TimerEntry

typedef void (* func_ptr_t)();
struct i386_gate *IDT_ptr;

void SetEntry (int entry_num, func_ptr_t func_ptr) {
  struct i386_gate *gateptr = &IDT_ptr[entry_num];
  fill_gate(gateptr, (int)func_ptr, get_cs(),ACC_INTR_GATE,0);
}
int main(){
  int i;

  IDT_ptr =get_idt_base();                           //get where IDT is
  cons_printf("IDT is at %u.\n",IDT_ptr);

  SetEntry(32,TimerEntry);                          //prime IDT entry
  outportb(0x21,~1);                                //0x21 is PIC mask ,~1 is mask
  asm("sti");                                       //set/enable intr in CPU Eflags reg

  while(1){                                        //forever loop
    for(i=0;i<833000;i++) IO_DELAY();              //Delay CPU half second
    cons_putchar('Z');                             //putting char of the flash
    if (cons_kbhit()) break;                       //keyboard hit ,breaks loop
  }
  return 0;                                       //end of main
}


