#include <stdio.h>
#include <unistd.h>

// which test read or write ?
#define DOREADTEST 0

// for write test use write vs printf ?
#define USEWRITE 0

// when using printf, do you want to flush after printf before fork?
#define USEFLUSH 1
#define FLUSHCMD  fflush(stdout)


int forked_read(),forked_write();

int main(int argc, char * argv[]){
	if ( DOREADTEST )
		forked_read();
	else
		forked_write();
}

int forked_read(){
	// check read buffering
	int i1;
	
	printf("Test read buffering\n");
	// read and print first number in the file
	FILE * f = fopen("/tmp/myfile","r");
	fscanf(f,"%d ",&i1);
	printf("1st integer before fork: %d.. waiting for an enter... \n",i1);

	// wait and then fork
	getchar();
	int r = fork();
	
	// read the second number from the file and print
	fscanf(f,"%d ",&i1);
	printf("Next From %d: , %d\n",getpid(),i1);

	fclose(f);
}

int forked_write(){
	int i1;
	// Try with printf vs try with write
	if (USEWRITE == 1 )
		write(1,"Test write buffers: before fork().. ",35);
	else {
		printf("Test printf buffers: flush=%d before fork().. ",USEFLUSH);
		if (USEFLUSH)
			FLUSHCMD;
	}

	// Use without flush and try both printf and write above

	int r = fork();
	if (r == 0 ) { // This is child
	       printf("From C %d ",getpid());
	       sleep(2);		// Just to see from Parent first
	}else{		
		printf("From P %d ",getpid());
	}
       	printf("\n");
}
