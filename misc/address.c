#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
int g=0;
void main() {
	int x;
	int * a;
	int z[100];
 	a = (int *)malloc(10);
   	printf("writecode) is at %p\n",write);
   	printf("main(code) is at %p\n",main);
   	printf("g   (data) is at %p\n",&g);
   	printf("a   (heap) is    %p\n",a);
   	printf("x   (stak) is at %p\n",&x);
   	printf("a   (stak) is at %p\n",&a);
   	printf("z   (stak) is at %p\n",&z);
	printf(" ......... you may want to use pmap <PID> to see the address space\n");
	if ( mlock(a,10) < 0 ){
		perror("main.mlock():");
		exit(1);
	}
	sleep(10000);
}

