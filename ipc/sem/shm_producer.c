#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "configs.h"
#include "safe_ops.h"
#include "mysemops.h"
#include <time.h>
#define SHM_KEY_VAL 8888
#define SEM_KEY_VAL 7777
#define SEM_KEY_EMPTY_SLOT 7776
#define SEM_KEY_FILLED_SLOT 7775
#define SLOW_DOWN 1
int main(){
	struct mystruct * s; // pointer to shh
	struct semid_ds buf; // needed for status from sem
	int semid;
	int shmid;
	semid=get_existing_sem(SEM_KEY_VAL);
	lock_down(semid);
	// first one to acquire lock will (a) create the shm & (b) initialize shm
	get_info_sem(semid, &buf);
	printf("buf.sem_otime = %ld\n", buf.sem_otime);
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

	// s = shmat(shmid, (void * )NULL, 0);
	//srand(23);
	srand(time(NULL));
	int salt=rand()%100; // salt should be a randome from 0..99
	salt=salt*100;
	int semid_empty_slot=get_existing_sem(SEM_KEY_EMPTY_SLOT);
	int semid_filled_slot=get_existing_sem(SEM_KEY_FILLED_SLOT);
	// safely add 10 successive numbers and then quit
	// for(int i=salt;i<salt+10;i++) {
	int i=salt;
	while(1) { i++;
		for (int j=0;j<=i%4;j++) putchar(' '); 
		printf("+\n");
		lock_down(semid_empty_slot);
		safely_put(i,s,semid);
		free_up(semid_filled_slot);
		if (SLOW_DOWN) sleep(rand()%3);
	}
	shmdt(s);
}
