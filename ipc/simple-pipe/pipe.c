#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(){
	int pfdA[2];
	if ( pipe(pfdA) < 0 ) {
		perror("pipe creation: ");
		exit(1);
	}
	int c=fork();
	// alternately
	if ( c == 0 ) {
		// Child is the writer 
		char * string[]={"This ","is ","my ", "house ","."};
		for (int i=0;i<5;i++) {
			write(pfdA[1],string[i],strlen(string[i]));
			// sleep(1); 
			// for convenience I made child sleep
		}
	} else if ( c > 0 ){
		//  Parent is the reader //  Is this important??   close(pfdA[1]);
		int MAXLEN=100;
		char string[MAXLEN];
		while( 1 ) {
			printf("*\n");
			int len = read(pfdA[0],string,MAXLEN);
			if (len > 0 ) write(1,string,len);
			else // Now it is EOF
			       	break;
		}
	} else
		perror("Failed fork:");
}

