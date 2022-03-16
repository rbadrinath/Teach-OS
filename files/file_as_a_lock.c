#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
// Global
static char * lockfile="lockfile";
int create_and_acquire_lock(int *fd){
	// return value 
	//       0  succesfully created, fd is the file descriptor
	//       1  unsuccessful
	*fd=open(lockfile,O_CREAT|O_EXCL|O_RDWR,S_IRWXU);
	if ((*fd) < 0 ) {
		if ( errno == EEXIST ) {
			printf("  create_and_aquire(): file already exists\n");
			return 1 ; // File already exists
		}
		perror("open failed unexpectedly ");
		exit (1);

	}
	return 0 ; // Created a new File 
}
void remove_lockfile(){
	int r = unlink(lockfile);
	if ( r < 0 ) {
		perror("RemoveLockfile() failed ");
		exit(1);
	}
}
void delete_and_release_lock(int fd){
	close(fd);
	remove_lockfile(); // cheating
}
int main(){
	int r;
	int fd;
	int successes=0;
	int i;
	for (i = 0 ; i < 10 ; i++) {
		printf("Attempting:...\n");
		r = create_and_acquire_lock(&fd);
		if (r==0) { //successfully created and acquired
			printf("   %d %d: Success, Got it! busy now... \n", i, (int) getpid());
			sleep(5);
			delete_and_release_lock(fd);
			printf("   ..Done\n");
			successes++;
			sleep(3); // simulating some fixed time work

		} else {    // unsuccessful in creating
			printf("  %d %d: **FAILED**\n", i, (int) getpid());
			// retry 
			int x=random()%3;
			sleep(x); // wait for a random time before retrying
		}

	}	
	printf("Succeeded %d out of %d times\n",successes,i);

}
