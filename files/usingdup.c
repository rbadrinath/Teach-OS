// open the file twice
#include "myincludes.h"
#include <string.h>
#define CMP_ARGV(I,X) (strcmp(argv[I],X)==0)
void withoutdup(){
	char buf[100];

	// open same file twice
	int fd1 = open("xyz",O_RDONLY);
	int fd2 = open("xyz",O_RDONLY);

	// read from first fd and print on screen
	int n = read(fd1,buf,10);
	write(1,buf,10);
	
	// read from second fd and print on screen
	n = read(fd2,buf,10);
	write(1,buf,10);

	// they have two different internal structures/information
}
void withdup(){
	char buf[100];

	// open the file only once
	int fd1 = open("xyz",O_RDONLY);
	// duplicate the fd ... (NOT open again)
	int fd2 = dup(fd1);

	// read using fd1 and print on the screen
	int n = read(fd1,buf,10);
	write(1,buf,10);

	// read using fd2 and print on the screen
	n = read(fd2,buf,10);
	write(1,buf,10);

	// the two fds share the same internal structures
}
void main(int argc, char *argv[]){
	if (argc > 1 ) {
		if (CMP_ARGV(1,"--nodup") || CMP_ARGV(1,"-n"))
			withoutdup();
	} else
			withdup();
}
