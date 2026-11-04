#include <stdio.h>

#define main0 main

int ga=2;
int gb=40;
int gc=1;


int main0(){
	int a=2;
	int b=40;
	int c;
	
    __asm__ __volatile__ (
	"movl %1, %0;"
	"addl %2, %0;"
	: "=r" (c)
	: "r" (a),
	  "r" (b)
	:   // no clobbers
	);
    printf("Result: %d\n", c); // Outputs 42
	return 0;
}

// main1 illustrates use of register based access
// "r" says 'choose whichever register you want for this variable'
// "+" indicates that variables is both an input and an output
int main1() {
    int a = 2;
    int b = 40;
    int c=1;

	// trying to do c+=100+a+b
	// __volatile__ tells gcc not to optimize or reorder instructions in this block of code
    __asm__ __volatile__ (
	"movl $100,%%eax;"
	"addl %%eax,%0;"
	"addl %1, %0;"
	"addl %2, %0;"
	:   "+r" (c)         // output  %0
	:   "r" (a),         // input %1(a)
	    "r" (b)          // input %2(b)
	:  "%eax"            // clobbered regs - we are overwriting %eax, could be empty
	);

    printf("Result: %d\n", c); // Outputs 143
    return 0;
}

// main2 illustrates use of memory access
// "m" says 'access memory for this variable'
// "+" indicates that variables is both an input and an output
int main2() {
    int a = 2;
    int b = 40;
    int c=1;

	// trying to do c+=100+a+b
	// __volatile__ tells gcc not to optimize or reorder instructions in this block of code
    __asm__ __volatile__ (
	"movl $100,%%eax;"
	"addl %%eax,%0;"
	"addl %1, %0;"
	"addl %2, %0;"
	:   "+r" (c)         // output  %0
	:   "m" (a),         // input %1(a)
	    "m" (b)          // input %2(b)
	:  "%eax"                  // clobbered regs - we are overwriting %eax, could be empty
	);

    printf("Result: %d\n", c); // Outputs 143
    return 0;
}

// main3 illustrates use of clobbered registers
// It explicitly indicates to the assembler that the code modifies %eax
int main3() {

	// trying to do c+=100+a+b
	// __volatile__ tells gcc not to optimize or reorder instructions in this block of code
    __asm__ __volatile__ (
	"movl $100,%%eax;"
	"addl %%eax,%0;"
	"addl %1, %0;"
	"addl %2, %0;"
	:   "+r" (gc)         // output  %0
	:   "m" (ga),         // input %1(a)
	    "m" (gb)          // input %2(b)
	:  "%eax"                  // clobbered regs - we are overwriting %eax, could be empty
	);

    printf("Result: %d\n", gc); // Outputs 143
    return 0;
}
