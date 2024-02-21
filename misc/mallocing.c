#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEAP 1
#define STACK 0
int heap_or_stack = HEAP;

#define LEN  4096  // 4K?, 4M Bytes
#define SLEEP 1
#define WAIT 0
long datalen=LEN;
int sleep_or_wait=WAIT;

int run_test(){
	char * large=NULL;
	char * prev =NULL;
	for( int cnt=0; 1 ; cnt++) {
		prev=large;
		if (sleep_or_wait == SLEEP)
			sleep(1);
		else {
			int n;
			while( (n = getchar()) != '\n') {
			if ( n == 'l' ) // increase datalen 4K->4M->4G
				datalen=datalen*1024;
			if ( n == 'h' )
				heap_or_stack=HEAP;
			if ( n == 's' )
				heap_or_stack=STACK;
			}
		}
		if ( heap_or_stack == HEAP ) {
			// Allocate datalen bytes on heap
			large=(char * ) malloc(datalen);
			// print the return address
			printf("now at %3d ->%p\n",cnt,large);
			if(cnt==0) continue;
			printf("\t diff is %ld\n",large-prev);
			// Notice that though we are allocating
			// 4KiBytes the difference is a bit (16B) more!  
			// Possibly the extra space to manage the allocated 4096 bytes
			// Use the pmap command on another terminal to check when
			// the heap [anon] grows
		} else { // STACK
			int large[datalen];
			printf("Stacked %ld\n",datalen);
			run_test();
		}
	}

}
void print_help(){
	printf(
	"Program expects user to hit enter each time before an\n" \
	" allocation. Additionally the following is useful:\n" \
	" l     to allocated 1024 times more memory (default is 4KiB)\n" \
	" h     to allocate on the heap space (default)\n" \
	" s     to allocate on the stack\n" 
	);
}
int main(){
	print_help();
	run_test();
}
