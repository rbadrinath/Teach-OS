#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

// #define DEBUG 1

// Small macro to test if argv[1] is X
#define COMMAND(X) (strcmp(argv[1],X)==0) 
// future:: COMMAND(i,X) (strcmp(argv[i],X)==0)

int main(int argc, char ** argv){


// Process arguments - here just printing for fun	
#ifdef DEBUG
	printf("%d\n", argc);
	for( int i = 0; i<argc ; i++ ){ 
	 printf("%s  ", argv[i]);
	}
	printf("\n");
#endif

	int r;
	if (argc < 4) {
		printf("%s [--help]\n",argv[0]);
		printf("%s -[h|s] filename linkname\n",argv[0]);
		exit(0);
	}
	int k=2;
	if (argc==4) {
	    if COMMAND("-h") {
		r=link(argv[k],argv[k+1]);
	    	if ( r ) { 
		     perror("Linking error");
		     exit(1);
		} else
			exit(0);
	    }
	    if COMMAND("-s"){
		r=symlink(argv[k],argv[k+1]);
	    	if ( r ) { 
		     perror("Linking error");
		     exit(1);
		} else
			exit(0);
		exit(1);
	    }
	}
	printf("Invalid argument\n");
	exit(1);
}


