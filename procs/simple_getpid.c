#include <stdio.h>

#include <unistd.h>



void main(){
	printf("My pid is: %d\n",getpid());
	sleep(100);
}
