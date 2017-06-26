# hello.s
#
# print a msg to terminal, exit with msg addr
# 
# Once this runs, it subtracts 4096 from ESP to get where its DRAM starts.
# $msg is given location 2G+x (a virtual address given by gcc link386).
# Subtract 2G from $msg to get the difference/offset x.
# Add x to starting DRAM addr, that's DRAM addr of msg.

.text                       	# code segment
.global _start              	# _start is public

_start:                     	# _start is main()


   movl $msg, %eax  	    	# addr of msg is 0x80000080+y since
   int   $57			# Call SysWriteISR		
   mov $0x80000090, %eax	# Address of 1st instruction 
   int   $60			# call exit

.data                       # data segment follows code segment in memory layout
msg:                        # msg
   .ascii "\n(RASOS): Hello! Good things need no arguments, bad things worth no arguments.\n\r"

   

