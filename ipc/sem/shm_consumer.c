#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "mysemops.h"
#include "safe_ops.h"
#include "configs.h"

int main(){
	struct mystruct * s; // pointer to shh
	struct semid_ds buf; // needed for status from sem
	int semid;
	int shmid;
	int x;
	semid=get_existing_sem(SEM_KEY_VAL);
	lock_down(semid);
	// first one to acquire lock will (a) create the shm & (b) initialize shm
	get_info_sem(semid,& buf);
	if (buf.sem_otime == 0 ){
		printf("Firstone\n");
		// I am the first one to get the semaphore
		// (a) create the shm
		shmid = shmget(SHM_KEY_VAL,sizeof(struct mystruct),IPC_CREAT|IPC_EXCL|0644);
		s=shmat(shmid, (void * )NULL, 0);
		// (b) initializ the shm
		s->free=s->used=0;
		// (c) free up the lock
		free_up(semid);
	} else {
		// I am not the first
		// (a) get the shm
		shmid = shmget(SHM_KEY_VAL,sizeof(struct mystruct),0644);
		s=shmat(shmid, (void * )NULL, 0);
		// (b) free up the lock
		free_up(semid);
	}

	int semid_empty_slot=get_existing_sem(SEM_KEY_EMPTY_SLOT);
	printf("ID SEM EMPTY %d; val = %d\n",
			semid_empty_slot,get_val_sem(semid_empty_slot));
	int semid_filled_slot=get_existing_sem(SEM_KEY_FILLED_SLOT);
	printf("ID SEM FILLED %d; val = %d\n",
			semid_filled_slot,get_val_sem(semid_filled_slot));
	// safely read 10 successive numbers and then quit
	// for(int i=0;i<10;i++) {
	while (1) {
		lock_down(semid_filled_slot);
		int x = safely_get(s,semid);
		printf("%d \n",x);
		free_up(semid_empty_slot);
	}
	// }
	shmdt(s);
}
