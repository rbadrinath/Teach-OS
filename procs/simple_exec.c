#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND   "/bin/ps"
#define ARGS      "/bin/ps", "-l", NULL 
/* Simple program that executes (overlays) anoher program */
void main(){

	// Execution of a general command with args
	char * overlaycmd = COMMAND ;
	char * params[] = { ARGS }; // looks like {"/bin/ps", "-l", NULL}

	printf("I am the executable....Before  exec call: PID = %d\n",(int)getpid());
	sleep(2);
	execv(overlaycmd,params);

	printf("After ... NEVER PRINT THIS if success..... PID = %d\n",(int)getpid());
	perror("Failed execution:");
}
