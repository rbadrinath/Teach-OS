#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "myincludes.h"

void main1(){

	char string[]="Hellow world\n";
	char errstr[]="EE\n";
	// write(1,string,strlen(string));

	// to open and write to a new file :
	// int newfd = open("tempfile",O_CREAT|O_RDWR,S_IRWXU);

			// duplicate the stdout  as a new fd
			int newfd = dup(1); 
			close(1);
			    printf("newfd = %d \n",newfd);//shouldnt work~ but..
			    int r = fflush(stdout);
			    fprintf(stderr,"newfd Found %d ", r);
			write(newfd,string,strlen(string));
	
	// write(newfd,string,strlen(string));
	// int r = write(newfd,string,strlen(string));
	// printf("Return value is %d",r);
} /* more below */

void main2(){
	char string[]="Hellow world\n";
	char errstr[]="EE\n";
	write(1,string,strlen(string));
	int fd = dup(1);
	int r = fork();
	if ( r == 0 ){
		close(1);
		write(1,errstr,strlen(errstr));
		exit(0);
	} else if ( r > 0 ) {
		int s;
		wait(&s);
		write(1,string,strlen(string));
	} else
	{
		perror("fork(): ");
		exit(1);
	}


}

void main3()
{
	char string[]="Hellow world\n";

	// to open a new file :
	int newfd = open("tempfile",O_CREAT|O_RDWR,S_IRWXU);
	// create a newfd that also points to the same place as newfd
	// and "here is the trick" - make its fd 1 (over writing the old 1)
	int duplicate = dup2(newfd,1);
	close(newfd);

	// write(1,string,strlen(string));
	printf("%s",string);

}

void main4(){
	// delayed write and sync
	char string[]="Hellow World\n";
	// to open an existing file :
        int fd = open("permfile",O_WRONLY);
        // Write to that file
        write(fd,string,strlen(string));
	sleep(20);
	close(fd);

}


void main(){
	main4();
}
