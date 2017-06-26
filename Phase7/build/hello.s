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
   movl %esp, %eax		# ESP starts atop of this DRAM page
   sub $4096, %eax         	# minus 4KB --> start of page

   movl $msg, %ebx  		# msg is x bytes into this page
   sub $0x80000000, %ebx 	# subtract 2G (0x80000000), get x

   add %ebx, %eax  		# add x to start of page = msg addr
   pushl %eax			# save a copy of msg addr into stack (pushl)

   int   $57			# call SysWrite to output to terminal

   popl %eax        		# get the saved copy of msg addr
   int   $60			# call Exit(msg addr)

.data                       	# data segment follows code segment in memory layout
msg:                        	# msg
   .ascii "\n\r(RASOS): Hello! Good things need no arguments, bad things worth no arguments.\n\r"

   

