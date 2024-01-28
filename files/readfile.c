// open the file twice
#include "myincludes.h"
#include <string.h>
#define CMP_ARGV(I,X) (strcmp(argv[I],X)==0)
void main(int argc, char * argv[]){
	char buf[100];
	char fname[50]="xyz";

	if (argc > 1)
		strcpy(fname,argv[1]);

	// open same file twice
	int fd1 = open(fname,O_RDONLY);

	// read from first fd and print on screen
	int n = read(fd1,buf,10);
	write(1,buf,10);
	
	close(fd1);

	// The below part shows how file descriptors get allocated:
	// Uncomment to see it work
/*

	// close the STDIN
	close(0);
	// open the same file
	fd1 = open(fname,O_RDONLY);
	printf("\nfd1 is %d\n",fd1);

	// now try to read from this file
	n = read(fd1,buf,10);
	write(1,buf,10);

	// Lets try scanf
	scanf("%s",buf);
	printf("\n %s \n",buf);

	// What is your conclusion ?
*/
	
}
