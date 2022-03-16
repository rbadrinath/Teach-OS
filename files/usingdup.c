// open the file twice
#include "myincludes.h"
void main1(){
	char buf[100];
	int fd1 = open("xyz",O_RDONLY);
	int fd2 = open("xyz",O_RDONLY);
	int n = read(fd1,buf,10);
	write(1,buf,10);
	n = read(fd2,buf,10);
	write(1,buf,10);
}
void main2(){
	char buf[100];
	int fd1 = open("xyz",O_RDONLY);
	int fd2 = dup(fd1);
	int n = read(fd1,buf,10);
	write(1,buf,10);
	n = read(fd2,buf,10);
	write(1,buf,10);
}
void main(){
	main2();
}
