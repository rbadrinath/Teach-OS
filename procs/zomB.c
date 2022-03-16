#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void main(int argc, char ** argv){
	if (argc <= 1 ) printf("*Too few arguments:  %s somename \n",argv[0]),
		exit(1);
	int r = fork();
	if ( r < 0 )  {
		perror("Couldnt create new processes, exiting");
		exit(1);
	}
	if ( r == 0) {
		// child simply exits
		exit(0);
	} else {
		// parent simply loops
		for (int i=0 ; i < 100 ; i ++)
			printf("%s\t%d \n",argv[1],i),
			sleep(1);
	}

}

