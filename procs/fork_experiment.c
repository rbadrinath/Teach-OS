#include <stdio.h>
#include <unistd.h>

void main(){
	int x; int i=500;
	// printf("Have fun...");
	x=fork();
	if ( x == 0 ) {
		i++;
		printf("B i is %d at %p\n",i,&i);
	} else {
		i--;
		printf("C i is %d at %p\n",i,&i);
	}
}
