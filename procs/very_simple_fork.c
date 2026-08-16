#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

/* Simple program that creates a copy process by using fork*/
void main(){
	getchar();
	// call fork, once, twice or thrice to see how it appears 
	// in pstree <pid>  - here <pid> id the process id of corresponding bach
	// in pidstat -p ALL | grep a.out - shows the CPU on the last column.
	fork();
	fork();
	// Try either the while(1) or the sleep(1000) for fun
	while (1) ;
	sleep(1000);
}
