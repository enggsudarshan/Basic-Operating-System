# hello.s
#
# print a msg to terminal, exit with msg addr
# 
# Once this runs, it subtracts 4096 from ESP to get where its DRAM starts.
# $msg is given location 2G+x (a virtual address given by gcc link386).
# Subtract 2G from $msg to get the difference/offset x.
# Add x to starting DRAM addr, that's DRAM addr of msg.

.text                       # code segment
.global _start              # _start is public

_start:                     # _start is main()

   pushl %esp               #push the stack pointer
   pop %ebx                 #pop into register ebx
   sub $0x1000, %ebx        #subtract 4096 from it (this is the base, real addr of the page)

   movl $msg, %ecx          #copy $msg to register ecx
   sub $0x80000000, %ecx     #subtract 2G from it, get x (offset)

   add %ecx, %ebx           #add  x (offset) to ebx (base of page) -- where msg really is
   pushl %ebx               #save a copy (push it to stack)
   pushl %ebx
   popl %ebx
   int   $57
                   #pop to ebx (get a copy of real msg addr)
   popl %ebx
   
   int   $60

.data                       # data segment follows code segment in memory layout
msg:                        # msg
   .ascii "(Team Name): Hello! Good things need no arguments, bad things worth no arguments.\n\r"
   .rept 13                #101-87 = 14
   .ascii "\0"             #null chars
   .endr                   #end repeat
