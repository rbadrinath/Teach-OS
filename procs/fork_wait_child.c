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
#define WaitForever while(1)
#define WaitForEnter do { printf("Hit Enter to proceed :"); getchar();} while(0)

// set handler to SIG_DFL for experimentation
void (*handler)(int) =SIG_DFL ;

// Note: Setting the handler for SIGCHLD to SIG_IGN tells the kernel:
//    "when a child terminates, reap it immediately"
void set_sig_child( void (*handler)(int) ){

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags=0;

	if (sigaction(SIGCHLD,&sa,NULL)== -1) {
       perror("setting SIGCHLD failed");
       exit(1);
	}
    printf("Parent set SIGCHLD to ** ");
    if( handler == SIG_DFL)
	    printf("SIG_DFL ** \n");
    else
	    printf("SIG_IGN ** \n");
}

void main(int argc, char *argv[]){
	int child_pid;	// return value of fork
	int status;		// used by wait

	// process the argv and set eh handlerto SIG_DFL or SIG_IGN
	if (argc>1 && strcmp(argv[1],"SIG_IGN")==0){
		handler=SIG_IGN;
	} else if (argc>1 && strncmp(argv[1],"-h",2)==0) {
		printf("%s [SIG_IGN|SIG_DFL]\n",argv[0]);
		exit(0);
	} else
		handler=SIG_DFL;
	// set the handler for the SIGCHLD signal
	set_sig_child(handler);
    // print a confirmation message of which we are testing
	printf("Testing with signal = %ld\n", (long ) handler);

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
