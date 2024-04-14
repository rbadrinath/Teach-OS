#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]){
	int j=0;
	char pipename[20]=PIPENAME;
	if (argc >1)
		strcpy(pipename,argv[1]);

	int fd = open(pipename,O_WRONLY);
	if ( fd < 0 ) {
		perror("Open failed");
		exit(1);
	}
	char n[2]=" ";  // this will hold one char '0'..'9'
	char * string[]={n,"  This ","is ","server ", "sentence","."};
	while(1) {
	   string[0][0]='0'+j;  // each string starts with a sequence digit
	   for (int i=0;i<5;i++) {
		   if( write(fd,string[i],strlen(string[i]))< 0){
			   perror("writer write():");
			   exit(1);
		   }
		   // sleep(1);
	   }
	   printf("sent %d\n",j);
	   j=(j+1)%10;
	   sleep(2);
	}
}
