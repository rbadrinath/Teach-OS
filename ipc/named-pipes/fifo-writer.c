#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
		   write(fd,string[i],strlen(string[i]));
		   // sleep(1);
	   }
	   printf("%d\n",j);
	   j=(j+1)%10;
	   sleep(2);
	}
}
