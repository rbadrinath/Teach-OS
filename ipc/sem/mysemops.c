#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "mysemops.h"
#define MAX 10
#include "configs.h"

int create_new_sem(int key) {
// Create a semaphore
   int semid = semget(key,1,IPC_CREAT|IPC_EXCL|0644);  //nsems=1, flags is create
   if (semid < 0 ) {
	perror("Couldnt create/ acquire:");
   }
   return semid;
}
int get_existing_sem(int key) {
// Create a semaphore
   int semid = semget(key,1,0);
   return semid;
}
// Iniialize a semaphore
// semctl(semid,SEMINDX,CMD,VAL)
void init_sem(int semid, int val){
        if ( semctl(semid,0,SETVAL,val)   < 0 ){
		perror("Couldnt initialize:");
	}
}

int get_info_sem(int semid, struct semid_ds * buf){
// semctl(semid,SEMINDX,CMD,INFO)
        int r = semctl(semid,0,IPC_STAT,buf );
	return r;
}

int get_val_sem(int semid){
// getinfo
// semctl(semid,SEMINDX,CMD,INFO)
        int r = semctl(semid,0,GETVAL);
	return r;
}


void lock_down(int semid){
	//struct sembuf un/lock={SEMINDX,SEMOP(+1 or -1),IPC_NOWAIT?}
	struct sembuf wait_and_acquire_op={0,-1,0};
	semop(semid,&wait_and_acquire_op,1);
}

void free_up(int semid){
	//struct sembuf un/lock={SEMINDX,SEMOP(+1 or -1),IPC_NOWAIT?}
	struct sembuf free_and_signal_op={0,1,0};
	semop(semid,&free_and_signal_op,1);
}

void remove_sem(int semid){ // Remove semaphore from the system
        int r = semctl(semid,0,IPC_RMID);
}
