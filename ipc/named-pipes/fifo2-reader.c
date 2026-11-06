#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include "common.h"

int  main(int argc, char * argv[]){
	char pipename[20]=PIPENAME;
	if (argc >1)
		strcpy(pipename,argv[1]);
	int fd0=open(pipename,O_RDONLY);
	if (fd0 < 0 ) {
		perror("open failed 1");
		exit(1);
	}
	int fd1;
	if (argc >2){
		strcpy(pipename,argv[2]);
	    fd1=open(pipename,O_RDONLY);
	    if (fd1 < 0 ) {
		    perror("open failed 2");
		    exit(1);
	    }
    } else {
		printf("Only for 2 readers\n");
		exit(1);
	}
	printf("Now reading and printing strings from the pipe:\n");
	printf("Every read string is printed and *  and newline appear at the end\n");
	int MAXLEN=100;
	char string[MAXLEN];
	// fill in poll
	struct pollfd fdinfo[2];

	fdinfo[0].fd=fd0;
	fdinfo[0].events=POLLIN;   // poll for input from fd0

	fdinfo[1].fd=fd1;
	fdinfo[1].events=POLLIN;   // poll for input from fd1


	while( 1 ) {
		// getchar();  // uncomment this to manually control when the reader progresses

		if(fdinfo[0].fd==-1 && fdinfo[1].fd==-1)
			break;
		int ret = poll(fdinfo,2,-1);  // poll the given 2 fds for events
		if(ret < 1 )
			break;
		int len;
		if(fdinfo[0].revents&POLLIN){
			len = read(fd0,string,MAXLEN);
			string[len]=' ';
			string[len+1]='0';
			string[len+2]='\n';
			if (len > 0 ) write(1,string,len+3);
			else {
				printf(" 0 EOF\n"); // Now it is EOF
		       		break;
			}
		}
		if(fdinfo[1].revents&POLLIN){
			len = read(fd1,string,MAXLEN);
			string[len]=' ';
			string[len+1]='1';
			string[len+2]='\n';
			if (len > 0 ) write(1,string,len+3);
			else { // Now it is EOF
				printf(" 1 EOF\n"); // Now it is EOF
		       		break;
			}
		}
		if(fdinfo[0].revents&POLLHUP || fdinfo[0].events&POLLERR) {
		   fdinfo[0].fd=-1;
		   printf("wr0 closed\n");
		}
		if(fdinfo[1].revents&POLLHUP || fdinfo[1].events&POLLERR){
		   fdinfo[1].fd=-1;
		   printf("wr1 closed\n");
		}
	}
}
