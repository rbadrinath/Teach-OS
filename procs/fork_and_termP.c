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
		// child aims to terminate the parent process
		int parent_pid=getppid();
		printf("Process ready to %d send a kill to Process %d \n",
				getpid(), parent_pid);
		getchar();
		kill(parent_pid,SIGTERM);
		printf("Check that process %d exited\n", parent_pid);
		getchar();
	} else {
		// parent loops indefinitely
		 while (1) {}
	}
}
