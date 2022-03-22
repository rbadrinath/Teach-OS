#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"
#include <stdio.h>

int main(){
	int j=0;
	int fd = open(PIPENAME,O_WRONLY);
	char n[2]=" ";
	char * string[]={n,"  This ","is ","my ", "sentence","."};
	while(1) {
	   string[0][0]='0'+j;
	   for (int i=0;i<5;i++) {
		   if( write(fd,string[i],strlen(string[i]))< 0){
			   perror("writer write():");
			   exit(1);
		   }
		   // sleep(1);
	   }
	   printf("%d\n",j);
	   j=(j+1)%10;
	   sleep(2);
	}
}
