#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
void main(){
	struct mystruct * s;	
	// create the new shm
	//
	int id = shmget(SHM_KEY,sizeof(struct mystruct),IPC_CREAT|0644);
	if ( id < 0 ) {
		perror("Trying to create shm :");
		exit(1);
	}
	// attach it to the current process
	// Note we decided we will use it as a certain structure
	s = shmat(id, (void * )NULL, 0);
	//
	s->count=0;
	srand(time(NULL));
	int salt1=rand()%100; // salt1 should be a randome from 0..99
	int salt=salt1*100;   // this is to get a seq of numbers ending 0..99
	for ( int i =0; i< salt1 ; i++){
	// Now we write whatever we wish
	    s->count ++;
	    s->val[i]=salt+i;
	}
	// Now done, so detach
	shmdt(s);
}
