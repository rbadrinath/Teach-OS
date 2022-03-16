#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int create_new_sem(int key) ; // Create a semaphore
int get_existing_sem(int key) ; // get an exisittng semaphore
void init_sem(int semind, int val); // Iniialize a semaphore
int get_info_sem(int semid, struct semid_ds * buf); // getinfo
int get_val_sem(int semid); // getinfo
void remove_sem(int semid); // Remove semaphore from the system

void lock_down(int semid);
void free_up(int semid);

