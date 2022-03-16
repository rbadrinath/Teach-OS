#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Possible values for option
#define BUSY   1
#define SLEEPY 2
void main(int argc, char ** argv){
	int option=SLEEPY;
	// Find and print the option
	if (argc <= 1 ) 
		printf("*Too few arguments:  %s [sleepy|busy] \n",argv[0]),
		exit(1);
	if (strcmp(argv[1],"busy") == 0 ) 
		option=BUSY;
	else 
		option=SLEEPY;

	printf("%s = %d\n", argv[1], (int)getpid());

	int i=0;
	while (1) { // An infinite loop
		if (option==SLEEPY) {
			//  a "sleepy" process will be in sleep most of the time, waiting for a second to pass
			printf("%s\t%d \n",argv[1],i);
			sleep(1); 
			i++;
		} else { // option is BUSY
			//it will simply be running through the loop, consuming cpu
			//also a great way to see the effect of renice -n 19 -p PID
			// printf("%s %d ",argv[1],i);
			// i++;
		}
	}

}

