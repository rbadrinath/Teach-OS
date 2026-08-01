# This code is generated with the help of an LLM

# Some useful info about x86-64:
# Often the way code is written the operations are on values in registers
# rax (a) holds reults of many arith ops
#     (b) holds the function return value
# rdi and rsi contain the first and second argument respectively
# rcx is used as a loop counter or some kind of loop control

.global gcd
.type gcd, @function

# assumes two arguments passed via rdi and rsi
gcd:
    # rdi = a, rsi = b	   
    movq %rdi, %rax        # Move 'a' into %rax (dividend)
    movq %rsi, %rcx        # Move 'b' into %rcx (divisor)

.L_loopstart:
    testq %rcx, %rcx       # Check if b == 0
    jz .L_done             # If the reult of the test is zero, finish loop

    xorq %rdx, %rdx        # Clear %rdx for 64-bit division (%rdx:%rax)
    idivq %rcx             # Divide %rdx:%rax by %rcx (rax = rax / rcx, rdx = rax % rcx)

    movq %rcx, %rax        # Move old divisor 'b' into %rax
    movq %rdx, %rcx        # Move remainder into %rcx
    jmp .L_loopstart       # Repeat - Go to the beginning of the loop

.L_done:
    ret                    # GCD is in %rax
