#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "common.h"

int  main(){
	int fd=open(PIPENAME,O_RDONLY);
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
