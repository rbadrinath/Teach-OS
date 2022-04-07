#include <stdio.h>
#include <stdlib.h>
// #define STRLEN  32 // 2^5=32
#define STRLEN  4096  // 4K Bytes
int main(int argc, char * argv[]){
	char * large=NULL;
	char * prev =NULL;
	for( int cnt=0; 1 ; cnt++) {
		prev=large;
		getchar();
		// Allocate 4096 bytes
		large=(char * ) malloc(STRLEN);
		// print the return address
		printf("now at %3d ->%p\n",cnt,large);
		if(cnt==0) continue;
		printf("\t diff is %ld\n",large-prev);
		// Notice that though we are allocating
		// 4K Bytes the difference is a bit more!  
		// Possibly the extra space to manage the allocated 4096 bytes
		// Use the pmap command on another terminal to check when
		// the heap [anon] grows
	}

}
