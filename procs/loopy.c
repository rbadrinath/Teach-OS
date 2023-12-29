#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
//Possible values for option
#define BUSY   1
#define SLEEPY 2
#define KERNEL 3
//run this on one terminal with different options
//on another terminal run top, see where this program is in the list
//check the amount of time consumed by the PROCESS (%cpu)
//check also the amount of time the SYSTEM is in user space(us) vs kernel space(sy)
void main(int argc, char ** argv){
	int option=SLEEPY;
	// Find and print the option
	if (argc <= 1 ) 
		printf("*Too few arguments:  %s [sleepy|busy|kernel] \n",argv[0]),
		exit(1);
	if (strcmp(argv[1],"busy") == 0 ) 
		option=BUSY;
	else if (strcmp(argv[1],"kernel") == 0) 
		option=KERNEL;

	printf("%s = %d\n", argv[1], (int)getpid());

	int i=0;
	while (1) { // An infinite loop
		if (option==SLEEPY) {
			//  a "sleepy" process will be in sleep most of the time, waiting for a second to pass
			printf("%s\t%d \n",argv[1],i);
			sleep(1); 
			i++;
		} else if (option==BUSY) { // option is BUSY
			//it will simply be running through the loop, consuming cpu
			//also a great way to see the effect of renice -n 19 -p PID
			// printf("%s %d ",argv[1],i);
			// i++;
			// BUT all this is in user land
		}else {
			// make it consume kernel time - some syscall
			struct rusage usage;
			getrusage(RUSAGE_SELF, &usage);
		}
	}

}

