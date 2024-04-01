#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
void main(){
	struct mystruct * s;	
	// create the new shm
	//
	int fd = shm_open(SHM_NAME,O_RDWR | O_CREAT,0644);
	if ( fd < 0 ) {
		perror("Trying to create shm :");
		exit(1);
	}
	// set size
	ftruncate(fd, sizeof(struct mystruct));
	// Note we decided we will use it as a certain structure
	s = mmap(NULL, sizeof(struct mystruct), PROT_READ | PROT_WRITE, 
		MAP_SHARED, fd, 0);
	if (s == NULL) {
		perror("couldn't map new");
		exit(1);
	} else {
		printf("Mapped shm to address space; ");
	}
	s->count=0;
	srand(time(NULL));
	int nums=rand()%100; // nums should be a randome from 0..99, say 85
	int start=nums*100;   // the seq is 8500 8501 ... 8504
	for ( int i =0; i< nums ; i++){
	// Now we write whatever we wish
	    s->count ++;
	    s->val[i]=start+i;
	}
	printf("First num: %d ;  last num: %d\n",start,start+(nums-1));
	// Now done, so close
	close(fd);
}
