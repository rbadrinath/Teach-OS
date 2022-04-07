#include <stdio.h>
#include <unistd.h>
#define TEST main_write
int main_read(),main_write();

int main(int argc, char * argv[]){
	TEST();
}
int main_read(){
	// check read buffering
	int i1;
	
	// read and print first number in the file
	FILE * f = fopen("/tmp/myfile","r");
	fscanf(f," %d ",&i1);
	printf("--- %d \n",i1);

	// wait and then fork
	getchar();
	// int r = fork();
	
	// read the second number from the file and print
	fscanf(f," %d ",&i1);
	printf("From %d: , %d\n",getpid(),i1);

	fclose(f);
}

int main_write(){
	int i1;
	printf("Hello before fork() ");
	// getchar();
	// fflush(stdout);  // try with and without this line
	int r = fork();
	if (r == 0 ) {
	       printf("From %d ",getpid());
	       sleep(6);
	}else{		
		sleep(1);
		printf("From %d ",getpid());
		sleep(3);
	}
       	printf("\n");
}
