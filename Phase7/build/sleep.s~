# sleep.s
#
# below as a demo code, it calls Sleep(300), change it to:
#    call Sleep for a period of seconds equal to its PID, and
#    call Exit with addr of its 1st inst as the exit code
#
# Enter "./build.pl sleep.s" to compile and convert to text sleep.bin
# build.pl does:
#    as --32 sleep.s -o sleep.o 
#    link386 -nostartfiles -userapp sleep.o -o sleep
# sleep.bin is to be included into file sys

# If curious about relative addr of identifiers below: as -a sleep.s

.text               	# code segment 
.global _start      	# _start is main()

_start:             	# instructions begin

   movl %esp, %ecx	# ESP starts atop of this DRAM page
   add $128, %ecx	# add 128B

   sub $4096, %ecx	# subtract 2G (0x80000000), get x
   int  $48		# call GetPid

   mov  $100, %ebx 	# move 100 into eax
   mul %ebx		# Multiply 100 to Pid

   int  $49         	# call Sleep(Pid * 100)

   movl %ecx, %eax	# address of the 1st instruction
   int  $60         	# call Exit


.data               	# examples on how to declare data

msg1:
   .ascii "Hello, World!\n\r"  # null-terminated string
msg2:                   # another string
   .ascii "Hey, Jude"
   .rept 20             # repeat 20 times
      .ascii "!"        # filled with exclamation marks
   .endr                # end of repeat
   .ascii "\n\r"


