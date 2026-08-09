#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>

// Some Macros
#define WaitForever while(1);
#define WaitForEnter printf("Hit Enter to proceed :"); getchar();

// set HANDLER to SIG_IGN or SIG_DFL for experimentation
#define HANDLER SIG_DFL

// Note: Setting the handler for SIGCHLD to SIG_IGN tells the kernel:
//    "when a child terminates, reap it immediately"
void set_sig_child(){
  if (signal(SIGCHLD, HANDLER) == SIG_ERR) {
       perror("setting SIGCHLD failed");
       exit(1);
  }
  else
	  printf("Parent set SIGCHLD.\n");
}
void main(){
	int child_pid;	// return value of fork
	int status;		// used by wait

	printf("Testing with signal = %d\n", (int ) HANDLER);

	set_sig_child();

	printf("Parent PID = %d\n",(int)getpid());

	child_pid = fork(); 

	if (child_pid == 0 ) {
		printf("Child PID = %d\n",(int)getpid());
		sleep(1);
		printf("Child exits\n");
	} else {

		sleep(2);
		// By this time child has exited
		// It is in either Zombie state or has been reaped off
		// Wait while the user does a ps -x
		WaitForEnter;
		if( wait(&status) == -1 )
			perror("Parent's Waiting failed....:-(\n");
		else
			printf("Parent Reaped child successfully:-)!\n");

		// One way or the other child has been reaped off by this time
		// Wait while the user does a ps -x
		WaitForever;
	}
}
