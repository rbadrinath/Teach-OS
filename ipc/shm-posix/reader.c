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
	int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT,0644);
	if ( fd < 0 ) {
		perror("Trying to create shm :");
		exit(1);
	}
	// set size
	// ftruncate(fd, sizeof(struct mystruct));
	// Note we decided we will use it as a certain structure
	s = mmap(NULL, sizeof(struct mystruct), PROT_READ, 
		MAP_SHARED, fd, 0);
	if (s == NULL) {
		perror("couldn't map new");
		exit(1);
	} else {
		printf("Mapped shm to address space; ");
	}
	printf("Reading %d numbers:\n",s->count);
	for (int i=0;i < s->count;i++) {
	// Now we will read as much as we can whatever we wish
	    printf("%d\n",s->val[i]);
	}

	// Now done, so close
	close(fd);
}
