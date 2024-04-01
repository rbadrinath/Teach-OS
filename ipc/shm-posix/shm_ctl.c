#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

void print_help(char * argv[]){
	printf("%s [-n]    : The optoinal u argument means do not initialize\n",argv[0]);
	printf("option: choose from:\n");
	printf("    o		open shm and map to process\n");
	printf("    p		print count value in shm\n");
	printf("    i		increment the count value in shm\n");
	printf("    d		decrement the count value in shm\n");
	printf("    u		unlink the shm\n");
	printf("    c		close the shm\n");
	printf("    h		print this help message\n");
	printf("    q		quit this program\n");
}
void main(int argc, char *argv[]){
	struct mystruct {
		int count;
		int val[MAX];
	} * s;	
	int shm_fd;
	int mode=1; // by default initialize

	if (argc > 1  && strcmp(argv[1],"-n")==0){
		printf("Setting uninitialized mode for opening\n");
		mode=0;
	}

	printf("My pid is %d\n",getpid());
	printf("Using SHM_NAME for operations is = %s\n",SHM_NAME);

   while(1) {
	char options[10];
	scanf("%s",options);

	if (strcmp(options,"q")==0)
		exit(0);

	if (strcmp(options,"h")==0){
		print_help(argv);
		continue;
	}

	if (strcmp(options,"o")==0){
		// create the new shm assuming it doesnt exist
		// if it exists, just get an fd to it. Like open() 
		shm_fd = shm_open(SHM_NAME, O_CREAT|O_RDWR,0644);
		if ( shm_fd < 0 ) {
			perror("Trying to open shm:");
			exit(1);
		} else {
			printf("Opened shm; ");
			ftruncate(shm_fd, sizeof(struct mystruct));
		}
		s = mmap(NULL, sizeof(struct mystruct), PROT_READ | PROT_WRITE, 
				MAP_SHARED, shm_fd, 0);
		if (s == NULL) {
			perror("couldn't map new");
			exit(1);
		} else {
			printf("Mapped shm to address space; ");
		}

		if (mode) {
			s->count=0;
			printf("set count field to 0\n");
		} else
			printf("\n");
		continue;
	}
	// print the content
	if (strcmp(options,"p")==0){
		printf("Current Value of count = %d\n",s->count);
		continue;
	}
	// increment the content
	if (strcmp(options,"i")==0){
		printf("Incremented count...\n");
		s->count++;
		printf("Current Value of count = %d\n",s->count);
		continue;
	}
	// decrement the content
	if (strcmp(options,"d")==0){
		printf("Decremented count...\n");
		s->count--;
		printf("Current Value of count = %d\n",s->count);
		continue;
	}

	// to remove the shm ... 
	if (strcmp(options,"u")==0)
		if ( shm_unlink(SHM_NAME) != 0 )
			perror("unlink failed");
		else
			printf("unlinked\n");
	
	if (strcmp(options,"c")==0)
		if ( close(shm_fd) != 0 )
			perror("close failed");
		else
			printf("closed\n");
   }
}
