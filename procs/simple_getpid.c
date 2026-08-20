#include <stdio.h>
#include <string.h>
#include <unistd.h>



void main(){
	printf("My pid is: %d\n",getpid());
	int i;
	// try a busy loop with different things to do
	// correspondingly check pidstat -p <pid> 1
	while (i=1){
		// doing nothing is a busy while loop
		//strcmp(" "," ");	// library call
		//getpid();       	// system call
		//sleep(100);	    // system call but not actively consume any CPU
		//printf("hi\n");  // a library call leading to a system call
	}
}
