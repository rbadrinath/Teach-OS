#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include<signal.h>

#define EG_ANON 0
#define EG_FILE 1
#define FILENAME "Mmap-test-file"

int test_type = EG_FILE;

void pmap_file(){
	printf("FILE:Check my pmap at start:\npmap %d; " \
			"make sure file named %s exists\n",getpid(),FILENAME);
	getchar();
	printf("Continuing...\n");
	int fd=open(FILENAME,O_RDWR);
    	if (fd < 0 ) {
	    	perror("Opening File "FILENAME"  failed");
	    	exit(1);
    	}
    	char * new_region = mmap(NULL, 4096, 
	/* readable,writeable */ PROT_READ|PROT_WRITE,
	/* shared among procs, file mapped */ MAP_SHARED, 
	/* fd and offset */ fd, 0); 
    	if (new_region == MAP_FAILED ) {
	    	perror("mmap for new_region");
	    	exit(1);
    	}
	printf("Check my pmap at after mapping:\npmap %d\n",getpid());
	getchar();
	printf("Continuing...\n");
	int i;
	char s[10];
	strncpy(s,new_region,5);
	s[5]='\0';
    	printf("new_region is at %p and contains: %s\n",new_region, s);
	/* also try writing
	char hello[]="hello";
	strcpy(new_region,hello);
    	printf("new_region changed, see file\n");
    	printf("Notice that all this is virtual address space\n");
	i = msync(new_region,5,MS_SYNC);
	if( i < 0 )
		perror("msync");
	munmap(new_region,4096);
	*/

}
void pmap_anon(){
	printf("ANON:Check my pmap at start:\npmap %d\n",getpid());
	getchar();
	printf("Continuing...\n");
    	char * new_region = mmap(NULL, 4096, 
	/* readable and writable */ PROT_READ|PROT_WRITE, 
	/* shared among procs, not file mapped */ MAP_PRIVATE |MAP_ANONYMOUS, 
	/* no fd and offset */ -1, 0); 
    	if (new_region == MAP_FAILED ) {
	    	perror("mmap for new_region");
	    	exit(1);
    	}
	printf("Check my pmap at after mapping:\npmap %d\n",getpid());
	getchar();
	printf("Continuing...\n");
	int i;
	for(i=0;i<10;i++)
		new_region[i]='0'+i;
	new_region[i]='\0';
    	printf("new_region is at %p contains: %s\n",new_region, new_region);
    	printf("Notice that all this is virtual address space\n");
}

void main(){
	if (test_type == EG_FILE)
		pmap_file();
	else
		pmap_anon();
}
