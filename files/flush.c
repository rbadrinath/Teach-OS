#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]){
	printf("Hello ");
	fflush(stdout);  // try with and without this line
	sleep(3);
}
