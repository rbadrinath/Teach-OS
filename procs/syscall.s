#########################
# taken from: https://cs.lmu.edu/~ray/notes/gasexamples/
# See the above site for more on x86-64 assembly
# Note: This is pure assembly and uses no C libraries
# We dont want gcc to invoke ld in the usual way, so compile differently.
# To compile:
#     $ gcc -c syscall.s
#     $ ld syscall.o
# Then execute:
#     $ ./a.out
########################
        .global _start

        .text
_start:
        # write(1, message, length of message)		# see man page WRITE(2)
        mov     $1, %rax                # system call 1 is write
        mov     $1, %rdi                # file handle 1 is stdout
        mov     $message, %rsi          # address of string to output
        mov     $len, %rdx               # number of bytes
        syscall                         # invoke operating system to do the write

        # exit(0)			# see man page EXIT(2)
        mov     $60, %rax               # system call 60 is exit
        xor     %rdi, %rdi              # we want return code 0
        syscall                         # invoke operating system to exit
message:
        .ascii  "Hello, from assembly  world\n"
	len = . - message

