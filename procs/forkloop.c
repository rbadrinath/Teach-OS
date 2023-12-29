#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


/* Simple program that creates a copy process by using fork*/
void main(){
	int child_pid;
	int x=0;
	int status;
	char spaces[]={0,0,0,0,0,0,0,0,0,0,0,0};

	printf("PID=%d\n",getpid());
	for (int i=0;i<3;i++){
		int r = fork();
		if (r == 0 ) // each child prints its info once!
		{
		   spaces[x++]='.';
		   printf("%s PID=%d\n",spaces,getpid());
		}
	}
	int i=rand()%5;
	sleep(i+5);
}
// printf("i=%d  ; %d child of %d\n",i,getpid(),getppid()); 
