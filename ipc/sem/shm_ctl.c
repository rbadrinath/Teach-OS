#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "safe_ops.h"
#include <stdio.h>
#include <string.h>
#include "configs.h"

int create_new_shm(int key, int size){
	struct mystruct * s; // pointer to shm
	int shmid;
	// (a) create the shm if needed
	shmid = shmget(key, size,
			IPC_CREAT|IPC_EXCL|0644);
	return shmid;
}

int get_existing_shm(int key){
	int shmid = shmget(key, 0, 0644);
	return shmid;
	// shmat(shmid, (void *) NULL,0);
}

int create_new_mystruct_shm(int key){
	return create_new_shm(key, sizeof(struct mystruct));
}

void init_mystruct(struct mystruct * s){
	s->free=s->used=0;
}

void print_mystruct(struct mystruct * s){
	printf("s->free = %d\n",s->free);
	printf("s->used = %d\n",s->used);
	for(int i = s->used; i != s->free ; i = (i+1)%MAX)
		printf(" %d ",s->val[i]);
	printf("\n");
}

void rm_shm(int shmid){
	struct shmid_ds sbuf;
	struct mystruct * s=shmat(shmid, (void * )NULL, 0);
	shmctl(shmid,IPC_RMID,&sbuf);
	shmdt(s);
}

void get_info_shm(int shmid){
	struct shmid_ds sbuf;
	shmctl(shmid,IPC_STAT,&sbuf);
	printf("size	=%ld\n",sbuf.shm_segsz);
	printf("cpid	=%d\n",sbuf.shm_cpid);
	printf("lpid	=%d\n",sbuf.shm_lpid);
	printf("nattch	=%ld\n",sbuf.shm_nattch);
}

void print_help(char * argv[]){
	printf("%s  <options> <arguments> \n",argv[0]);
	printf("	-n <KEY>       : create new mystruct shm \n");
	printf("	-i <KEY>       : init existg mystruct shm\n");
	printf("	-p <KEY>       : print mystruct shm\n");
	printf("	-I <KEY>       : print info shmid_ds\n");
	printf("	-r <KEY>       : remove shm\n");
	printf("	-h             : this help msg\n");
}

#define NEW_MYSTRUCT_SHM 	1
#define GET_SHM 		2
#define INIT_MYSTRUCT_SHM	3
#define PRINT_MYSTRUCT_SHM	4
#define INFO_SHM		5
#define RM_SHM			6
#define HELP			7

int main(int argc, char * argv[]){
	int cmd=-1,key=-1,shmid=-1;
	struct mystruct *s=NULL;
	if (argc > 2 && strcmp(argv[1],"-n")==0)
		cmd = NEW_MYSTRUCT_SHM;
	if (argc > 2 && strcmp(argv[1],"-i")==0)
		cmd = INIT_MYSTRUCT_SHM;
	if (argc > 2 && strcmp(argv[1],"-p")==0)
		cmd = PRINT_MYSTRUCT_SHM;
	if (argc > 2 && strcmp(argv[1],"-I")==0)
		cmd = INFO_SHM;
	if (argc > 2 && strcmp(argv[1],"-r")==0)
		cmd = RM_SHM;
	if (argc > 1 && strcmp(argv[1],"-h")==0)
		cmd = HELP;

	if (argc > 2 )
		key = atoi(argv[2]);

	switch(cmd){
	case NEW_MYSTRUCT_SHM:
		shmid = create_new_mystruct_shm(key);
		if (shmid < 0 ){
			perror("INFO: get()");
			exit(1);
		}
		printf("shmid= %d\n",shmid);
		break ;
	case INIT_MYSTRUCT_SHM:
		shmid = get_existing_shm(key);
		if (shmid < 0 ){
			perror("INIT: get()");
			exit(1);
		}
		s=shmat(shmid, (void * )NULL, 0);
		init_mystruct(s);
		shmdt(s);
		break ;
	case PRINT_MYSTRUCT_SHM:
		shmid = get_existing_shm(key);
		if (shmid < 0 ){
			perror("PRINT: get()");
			exit(1);
		}
		s=shmat(shmid, (void * )NULL, 0);
		print_mystruct(s);
		shmdt(s);
		break ;
	case INFO_SHM:
		shmid = get_existing_shm(key);
		if (shmid < 0 ){
			perror("INFO: get()");
			exit(1);
		}
		get_info_shm(shmid);
		printf("shmid= %d\n",shmid);
		break ;
	case RM_SHM:
		shmid = get_existing_shm(key);
		if (shmid < 0 ){
			perror("INFO: get()");
			exit(1);
		}
		rm_shm(shmid);
		break ;
	default: 
		printf("Unknown option/ Missing arguments:\n");
	case HELP:	
		print_help(argv);
		break;
	}
}
