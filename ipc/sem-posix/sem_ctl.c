#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>


// Default semaphore name
#define DEF_SEM_NAME "/sem-1"


void print_help(char * argv[]){
	printf("%s [-r]    : The optional -r argument means remove the sem\n",argv[0]);
	printf("option: choose from:\n");
	printf("    i   n       set initial value for next opento the number n\n");
	printf("    n   s       set semaphore name to the string s (must follow convention)\n");
	printf("    o		open sem\n");
	printf("    p		post sem\n");
	printf("    w		wait sem\n");
	printf("    c		close sem\n");
	printf("    u		unlink sem\n");
	printf("    v		get sem value\n");
	printf("    h		print this help message\n");
	printf("    q		quit this program\n");
}
void main(int argc, char *argv[]){
	sem_t * sem=NULL;
	int mode=1; // by default initialize
	unsigned int init_value=0;
	char sem_name[100]=DEF_SEM_NAME;

	printf("My pid is %d\n",getpid());
	printf("Using sem_name for operations is = %s\n",sem_name);
	printf("Get help using \"h\"\n");

	if (argc > 1  && strcmp(argv[1],"-r")==0){
		printf("Removing existing semaphore, new one will start at set initial value[0]\n");
		sem_unlink(sem_name);
	} else 
		printf("Will reuse any existing semaphore\n");

   while(1) {
	char options[10];
	scanf("%s",options);

	if (strcmp(options,"q")==0)
		exit(0);

	if (strcmp(options,"h")==0){
		print_help(argv);
		continue;
	}

	if (strcmp(options,"i")==0){
		scanf("%u",&init_value);
		continue;
	}

	if (strcmp(options,"n")==0){
		scanf("%s",sem_name);
		continue;
	}

	if (strcmp(options,"o")==0){
		// create the new sem assuming it doesnt exist

		sem=sem_open(sem_name,O_RDWR|O_CREAT,0644,init_value);
		if (sem == SEM_FAILED ){
			perror("Filled sem creation");
			exit(1);
		} else 
			printf("Opened sem\n");
		continue;
	}
	
	if (strcmp(options,"p")==0){
		// post
		int r = sem_post(sem);
		if ( r < 0 )
			perror("Post failed");
		else 
			printf("Post done\n");
		continue;
	}
	if (strcmp(options,"w")==0){
		// wait
		int r = sem_wait(sem);
		if ( r < 0 )
			perror("Wait failed");
		else 
			printf("Wait done\n");
		continue;
	}

	if (strcmp(options,"v")==0){
	        // Read the value of the semaphore
		int r;
		if ( sem_getvalue(sem,&r)!= 0 )
			perror("Getvalue failed");
		else
        		printf("sem value is: %d\n", r);
		continue;
	}

	if (strcmp(options,"u")==0)
		if ( sem_unlink(sem_name) != 0 )
			perror("unlink failed");
		else
			printf("unlinked\n");
	
	// to close the sem ...
	if (strcmp(options,"c")==0)
		if ( sem_close(sem) != 0 )
			perror("close failed");
		else
			printf("closed\n");
   }
}
