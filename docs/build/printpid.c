#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

/* Simple program that executes (overlays) anoher program */
void main(){
	printf("My PID = %d\n",(int)getpid());
}
