#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

/* Simple program that executes (overlays) anoher program */
void main(){
	struct timeval tv; struct timezone tz;
	printf("My PID = %d\n",(int)getpid());
	gettimeofday(&tv,&tz);  // To understand why this syscall is not seen see man vdso..... it shortcuts and reduces a syscall to a funciton call.
}
