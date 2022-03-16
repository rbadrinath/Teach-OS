#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#define COMMAND(X) (strcmp(argv[1],X)==0)
int main(int argc, char ** argv){
	
	printf("%d\n", argc);
	for( int i = 0; i<argc ; i++ ){ 
	 printf("%s  ", argv[i]);
	}
	printf("\n");
	if (argc == 1 )
		exit(0);

	int k=2;
	if COMMAND("ul")
		exit(unlink(argv[k]));
	if COMMAND("sl")
		exit(symlink(argv[k],argv[k+1]));
	if COMMAND("hl")
		exit(link(argv[k],argv[k+1]));
}


