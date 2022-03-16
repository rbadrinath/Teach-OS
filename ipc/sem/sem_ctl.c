#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mysemops.h"

#define HELP		0
#define MY_CREATE_NEW	1
#define MY_INIT      	2
#define MY_VAL	     	3
#define MY_UP 	     	4
#define MY_DOWN     	5
#define MY_RM		6
#define MY_INFO		7


void print_help(char * argv[]){
	printf("%s  <option> \n",argv[0]);
	printf("	-n <KEY>       : create newly \n");
	printf("	-i <KEY> <VAL> : init existing\n");
	printf("	-v <KEY>       : current sem value\n");
	printf("	-u <KEY>       : up sem value\n");
	printf("	-d <KEY>       : down sem value\n");
	printf("	-r <KEY>       : remove sem\n");
	printf("	-I <KEY>       : print info semid_ds sem\n");
	printf("	-h        : this help msg\n");

	
}

int print_info(struct semid_ds * buf){
	/*
	   struct semid_ds {
               struct ipc_perm sem_perm; 
               time_t          sem_otime;
               time_t          sem_ctime;
               unsigned long   sem_nsems;
           };
	*/
	printf("sem_otime=%ld\n",buf->sem_otime);
	printf("sem_ctime=%ld\n",buf->sem_ctime);

}

void main(int argc, char * argv[]){
	int cmd=0,semid=-1,key=0, initval=1;
	struct semid_ds buf;
	if ( (argc > 1 && strcmp(argv[1],"-h")==0) || (argc != 3) )
		cmd=HELP;
	if ( argc > 2 ) {
		key = atoi(argv[2]);
		if (key == 0 ){
			cmd=HELP;
			argc=1;
		} else 
			printf("key is %d\n",key);
	} 
	if ( argc > 3 ) 
		initval = atoi(argv[3]);
	if (argc == 3 && strcmp(argv[1],"-n")==0)
		cmd=MY_CREATE_NEW;
	if (argc == 4 && strcmp(argv[1],"-i")==0)
		cmd=MY_INIT;
	if (argc == 3 && strcmp(argv[1],"-v")==0)
		cmd=MY_VAL;
	if (argc == 3 && strcmp(argv[1],"-u")==0)
		cmd=MY_UP;
	if (argc == 3 && strcmp(argv[1],"-d")==0)
		cmd=MY_DOWN;
	if (argc == 3 && strcmp(argv[1],"-r")==0)
		cmd=MY_RM;
	if (argc == 3 && strcmp(argv[1],"-I")==0)
		cmd=MY_INFO;
        switch(cmd){
		case MY_CREATE_NEW:
		     semid = create_new_sem(key);
		     printf("semid is %d\n",semid);
		     return ;
		case MY_INIT:
		     semid = get_existing_sem(key);
		     init_sem(semid,initval);
		     printf("semid is %d initialized to %d\n",semid,initval);
		     return ;
		case MY_VAL:
		     semid = get_existing_sem(key);
		     int v = get_val_sem(semid);
		     printf("semid is %d\n",semid);
		     printf("semval is %d\n",v);
		     return ;
		case MY_UP:
		     semid = get_existing_sem(key);
		     free_up(semid);
		     printf("semid is %d\n",semid);
		     return;
		case MY_DOWN:
		     semid = get_existing_sem(key);
		     lock_down(semid);
		     printf("semid is %d\n",semid);
		     return;
		case MY_RM:
		     semid = get_existing_sem(key);
		     remove_sem(semid);
		     printf("semid is %d\n",semid);
		     return;
		case MY_INFO:
		     semid = get_existing_sem(key);
		     get_info_sem(semid, &buf);
		     printf("semid is %d\n",semid);
		     print_info(&buf);
		     return;
		default:
		     printf("unsupported command\n");
		case HELP:
		     print_help(argv);
		     return;
	}
}
