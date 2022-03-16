#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void main(){
	struct shmid_ds sbuf;
	struct mystruct * s;
	// create the new shm if it doesnt exist
	//
	int id = shmget(SHM_KEY,sizeof(struct mystruct),IPC_CREAT|0644);
	if ( id < 0 ) {
		perror("Trying to create shm:");
		exit(1);
	}
	// attach it to the current process
	// Note we decided we will use it as a certain structure
	s = shmat(id, (void * ) NULL, 0);
	//
	for (int i=0;i < s->count;i++) {
	// Now we will read as much as we can whatever we wish
	    printf("%d\n",s->val[i]);
	}
	// Now done, so detach
	/* 
	shmctl(id,IPC_RMID,&sbuf);
	shmctl(id,IPC_STAT,&sbuf);
	printf("size	=%ld\n",sbuf.shm_segsz);
	printf("cpid	=%d\n",sbuf.shm_cpid);
	printf("lpid	=%d\n",sbuf.shm_lpid);
	printf("nattch	=%ld\n",sbuf.shm_nattch);
	*/
	shmdt(s);

}
