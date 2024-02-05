#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NOFLUSH "noflush"
#define FFLUSH "fflush"
#define SCANF "scanf"
#define GETCHAR "getchar"

// set FLUSH to be one of the four above
#define FLUSH NOFLUSH

#define CMP(X,Y) strcmp(X,Y)==0

// C Langauge buffering.. more experimentation
// It appears the buffered read(scanf getchar etc) sometimes
// causes stdout to be flushed.

void main(){
	int j;
	for(int i=0;i<10;i++) {
		// do strace and see what system calls are executed
		// does write happen repeatedly?
		printf(" %d ,",i);
		// Without any of the below flushing ways 0, 1, 2, 3,...
		// get printed at the end (see the sleep below)
		// different ways to flush:
		if ( CMP(FLUSH,FFLUSH) ) {
			// This one is the standard way
			// Call fflush for event time i%4==0
			if (i%4==0)
				fflush(stdout);
		} else if (CMP(FLUSH,SCANF) ) {
			// It so happens that doing a scanf when the
			// input buffer is empty also casuses an flushing
			//   .. give multiple integers here for fun
			//   .. also for fun give a non integer as input
			if (scanf("%d",&j)==EOF)
		 		exit(0);
		} else if ( CMP(FLUSH,GETCHAR) ) {
			// A getchar too: try multiple characters here for fun
			getchar();
		}
		sleep(1);
	}
}
