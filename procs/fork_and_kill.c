#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>

void main(){
	int child_pid;
	int status;
	printf("1 PID = %d\n",(int)getpid());   // See this once only
	child_pid = fork();   // This is where all the aciton is
	printf("2 PID = %d, %d\n",(int)getpid(),child_pid); // See this twice
	if (child_pid == 0 ) {
		printf("Child looping\n");
		while (1) {} // short form for an infinite loop
	} else {
		// printf("Parent looping\n");
		// while (1) {}
		// alternately
		printf("Process ready to %d send a kill to Process %d \n",
				getpid(), child_pid);
		getchar();
		kill(child_pid,SIGTERM);
		printf("Check that process %d exited\n", child_pid);
		getchar();
		wait(&status);
	}
}
