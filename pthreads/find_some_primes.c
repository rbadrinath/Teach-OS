#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <math.h>

#define SLEEPTIME 100
#define NEXT(X) ((X==MAX-1) ? 0 :X+1)
#define END_OF_INPUT -1
int gettid();



// A simple FIFO queue on an array of size MAX
#define MAX 20 
int front=0; 
int rear=0;
int number[MAX];

int genRN(){
	while(1){
		int r = random()%10000;
		if (r >1000 ) {
			return r;
		}
	}
}

int isprime(int n) {
	for(int i=2;i<sqrt(n);i++)
		if( n%i == 0 )
			return 0;
	return 1;
}

#define wait_for_something   while(front == rear) 

void * read_items(void * tid){
	wait_for_something;
	while( front != rear ) {

		int n = number[NEXT(rear)];
		rear = NEXT(rear);

		// printf("Fetched %d\n",n);
		if ( n == END_OF_INPUT )
			break;

		if ( isprime(n) )
			printf("%d\n",n);

		wait_for_something;
	}
	printf("read_items Done\n");
	return NULL ;
}

int main(int argc, char * argv[]){
	pthread_t tid; // This is not an integer. non portable structure type

	//printf("Starting with T=%d P=%d\n",gettid(), getpid());


	int r = pthread_create(&tid,NULL,read_items,(void *)tid);
	for(int i=0;i<=100;i++) {
		for (int i=0;NEXT(front) == rear ;i=1) 
			if(i==0) printf(".\n");

		int r;
		if (i==100)
			r=END_OF_INPUT;
 		else 
			r=genRN();

		number[NEXT(front)]=r;
		front=NEXT(front);

		// printf("Pushed %d\n",r);
		if ( r == END_OF_INPUT)
			break;
	}
	printf("main GENERATED, waiting for worker\n");

	pthread_join(tid,NULL);
	printf("main DONE\n");
	exit(0);

}
