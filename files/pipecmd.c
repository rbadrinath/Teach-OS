#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(){
	int p[2];
	if ( pipe(p) < 0 ) {
		perror("pipe creation: ");
		exit(1);
	}
	int c=fork();
	// alternately
	if ( c == 0 ) {
		dup2(p[1],1);  // from now on in child 1 is same as p[1]
		close(p[0]);  // dont need p[0] in child
		char string[]="This is the house that jack built";
		printf("%s\n",string);
	} else if ( c > 0 ){
		dup2(p[0],0); // from now on in parent 0 is same as p[0]
		close(p[1]);  // dont need p[1] in parent
		char word[100];
		while( scanf("%s",word) != EOF )
			printf("GOT:%s\n",word);
	} else
		perror("Failed fork:");
}
