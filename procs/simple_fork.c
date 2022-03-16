#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

/* Simple program that creates a copy process by using fork*/
void main(){
	int child_pid;
	int x;
	int status;
	printf("1 PID = %d\n",(int)getpid());   // See this once only
	child_pid = fork();   // This is where all the aciton is
	printf("2 PID = %d, %d\n",(int)getpid(),child_pid); // See this twice
// Uncomment this for both processes to loop
	if (x == 0 ) {
		printf("Child looping\n");
		while (1) {} // short form for an infinite loop
	} else {
		printf("Parent looping\n");
		// while (1) {}
		// alternately
		wait(&status);
	}
}
