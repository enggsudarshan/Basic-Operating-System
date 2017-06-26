//***********************************************************
//isr.c Phase 0 ,Excerise 3 -- Timer Lab
//**********************************************************
//header file inclusion
#include<spede/flames.h>
#include<spede/machine/io.h>
#include<spede/machine/pic.h>
// 2byte ptr point to video memory location
// row 20 ,col 10 from upper left corner (b8000)
unsigned short *vid_mem_ptr = (unsigned short * )0XB8000+20*80+9;  
#define A (0x0f00 + 'A')                 //0x0f000 ,VGA attr :bold W on B 

void TimerISR(){                        //envoked from TimerEntry
  static unsigned short ch = A;         //start with Letter A
  static int timer_count = 0;          //count timer -intr occurances

//dismiss timer intr (IRQ 0),otherwise ,new intr signal from timer 
//won't be recognized by CPU since circuit uses edge trigger flipflop 
  outportb(0x20,0x60);                  //0x20 is PIc control reg, 0x60 dismisses IRQ 0

  if(++timer_count % 75 == 0) //every 75 second (1/3 second)
  {
    *vid_mem_ptr = ch;        //display ascii values
    ch++;                    //next char in ASCII table
    if(ch == A + 26 )       //once beyond last pritable char 
      ch = A;              //move back to A
  }
}


