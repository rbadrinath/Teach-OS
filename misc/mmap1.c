#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include<signal.h>


void handler(int signo) {
  if (signo ==  SIGUSR1)
    printf("received SIGUSR1\n");
}

void set_my_signal(){
  if (signal(SIGUSR1, handler) == SIG_ERR) {
       perror("setting signal failed");
       exit(1);
  }
}

int g;

int main(){
  	set_my_signal();
  	pause();
    	char * new_region = mmap(NULL, 4096, 
		/* readable and writable */ PROT_READ|PROT_WRITE, 
		/* shared among procs */ MAP_SHARED |MAP_ANONYMOUS, 
		/* no fd and offset */ -1, 0); 
    	if (new_region == MAP_FAILED ) {
	    	perror("mmap for new_region");
	    	exit(1);
    	}
    	int status;
    	strcpy(new_region,"Joker\n");
    	if(fork()==0){ 
	    	// Child write hello world into the new_region
	    	printf("In child\n");
		sleep(1000);
	    	strcpy(new_region, "hello from child!\n"); 
	    	exit(0); 
    	}
    	printf("In parent\n");
    	wait (&status); 
    	// Parent reads the new_region
    	printf("Found-%s\n",new_region);
}
