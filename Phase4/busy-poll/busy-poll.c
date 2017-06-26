// busy-poll.c
// A busy-poll device driver example, onto parallel port (printer)

#include "spede.h"
#define LOOP 1666667     // use this constant to alter loop number

int main() {
   int i, code;
   char key, *p,
        hello[] = "Hello, World!\n\r";   // newline (LF) carriage return (CR)

   cons_printf("\n************************************************************\n");
   cons_printf("* Check power/cable/paper. Press 1 to print (or q to end). *\n");
   cons_printf("************************************************************\n");

// ---------------------------------------------------------------------------------
// following resets printer which will jitter, make sure powered, cable plugged
   outportb(LPT1_BASE+LPT_CONTROL, PC_SLCTIN); // Printer Control, SeLeCT INterrupt
   code = inportb(LPT1_BASE+LPT_STATUS);
   for(i=0; i<50; i++) IO_DELAY();             // needs delay
//   outportb(LPT1_BASE+LPT_CONTROL, PC_INIT | PC_SLCTIN | PC_IRQEN); // IRQ 7 use
   outportb(LPT1_BASE+LPT_CONTROL, PC_INIT | PC_SLCTIN);  // no IRQ 7
   for(i=0; i<LOOP; i++) IO_DELAY();           // wait for 1 second
// ---------------------------------------------------------------------------------

   while(1) {
      for(i=0; i<LOOP; i++) IO_DELAY();        // wait 1 sec, or use Sleep
      if(!cons_kbhit()) continue;
      key = cons_getchar();
      if(key == 'q') break;
      if(key != '1') continue;

      p = hello;                               // start from beginning
      while(*p) {
         outportb(LPT1_BASE + LPT_DATA, *p);      // send char as data
         code = inportb(LPT1_BASE + LPT_CONTROL); // read current control code
         outportb(LPT1_BASE + LPT_CONTROL, code | PC_STROBE); // add strobe bit

         for(i=0; i<50; i++) IO_DELAY();          // Epson LP571 needs delay

         outportb(LPT1_BASE + LPT_CONTROL, code); // return original control

         for(i = 0; i < LOOP*3; i++) {
            code = PS_ACK & inportb(LPT1_BASE + LPT_STATUS);
            if(code == 0) break;      // printer ACK'ed
            IO_DELAY();               // otherwise, wait 0.6 us, and loop
         } 

         if(i == LOOP*3) {         // if 3 sec has passed
            cons_printf(">>> Time out while printing a char!\n");
            break;                    // timed out, return -1
         }

         p++;  // otherwise, moved to next char
      }
   }
   return 0;
}


