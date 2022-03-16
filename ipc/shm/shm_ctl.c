#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

void print_help(char * argv[]){
	printf("%s  <option>\n",argv[0]);
	printf("    -n		Create  new shm\n");
	printf("    -I		Print status info of shm\n");
	printf("    -s		show count value in shm\n");
	printf("    -u		increment the count value in shm\n");
	printf("    -d		decrement the count value in shm\n");
	printf("    -r		remove the shm\n");
	printf("    -h		print this help message\n");
}
void main(int argc, char * argv[]){
	struct shmid_ds sbuf;
	struct mystruct {
		int count;
		int val[MAX];
	} * s;	
	printf("My pid is %d\n",getpid());

	if (argc > 1  && strcmp(argv[1],"-h")==0){
		print_help(argv);
		exit(0);
	}

	printf("Using SHM_KEY for operations is = %d\n",SHM_KEY);

	if (argc > 1  && strcmp(argv[1],"-n")==0){
		// create the new shm assuming it doesnt exist
		//
		int id = shmget(SHM_KEY,sizeof(struct mystruct),
					IPC_CREAT|IPC_EXCL|0644);
		if ( id < 0 ) {
			perror("Trying to create new :");
			exit(1);
		} else {
			printf("Created new shm; \n");
			s = shmat(id, (void * ) NULL, 0);
			if (s == (void *) -1 ){
				perror("Trying to attach :");
				exit(1);
			} else {
				printf("..... Attached; \n");
				s->count=0;
				printf("..... Initialized;\n");
			}
			exit(0);
		}

	}
	// get existing shm assuming it already exists
	int id = shmget(SHM_KEY,sizeof(struct mystruct),0);
	if ( id < 0 ) {
		perror("Trying to get existing :");
		exit(1);
	}
	
	// attach it to the current process - just for fun
	s = shmat(id, (void * ) NULL, 0);
	if (s == (void *) -1 ){
		perror("Trying to attach :");
		exit(1);
	} else {
		printf("++ Attached the shm\n");
	}
	
	// print the content
	if (argc > 1  && strcmp(argv[1],"-s")==0){
		printf("Current Value of count = %d\n",s->count);
	}
	// increment the content
	if (argc > 1  && strcmp(argv[1],"-u")==0){
		printf("Incremented count...\n");
		s->count++;
		printf("Current Value of count = %d\n",s->count);
	}
	// decrement the content
	if (argc > 1  && strcmp(argv[1],"-d")==0){
		printf("Decremented count...\n");
		s->count--;
		printf("Current Value of count = %d\n",s->count);
	}

	// to remove the shm ... set the IPC_RMID cmd, final detatch removes it
	if (argc > 1 && strcmp(argv[1],"-r")==0)
		shmctl(id,IPC_RMID,&sbuf);
	
	// get the shm status info
	if (argc > 1 && strcmp(argv[1],"-I")==0) {
		shmctl(id,IPC_STAT,&sbuf);
		printf("shm status information:\n");
		printf("size	=%ld\n",sbuf.shm_segsz);
		printf("cpid	=%d\n",sbuf.shm_cpid);
		printf("lpid	=%d\n",sbuf.shm_lpid);
		printf("nattch	=%ld\n",sbuf.shm_nattch); 
				// should be 1 if attached
	}

	// Now done, so detach - just for fun. You can see that the count was 1
	// It will get removed if this is the only attached process and
	// IPC_RMID is set
	shmdt(s);
	printf("-- Deteched the shm\n");
}
