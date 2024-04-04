#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

int  main(int argc, char * argv[]){
	char pipename[20]=PIPENAME;
	if (argc >1)
		strcpy(pipename,argv[1]);
	int fd=open(pipename,O_RDONLY);
	if (fd < 0 ) {
		perror("open failed");
		exit(1);
	}
	int MAXLEN=100;
	char string[MAXLEN];
	while( 1 ) {
		printf("*\n");
		int len = read(fd,string,MAXLEN);
		if (len > 0 ) write(1,string,len);
		else // Now it is EOF
		       	break;
	}
}
