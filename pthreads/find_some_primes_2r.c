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
struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec= random()%SLEEPTIME;
	return ts;
}
// Usage:
//     struct timespec ts=random_time();
//     nanosleep(&ts,NULL);


// Purpose:
// This program adds an additional reader to find_some_primes.c
// use diff find_some_primes.c find_some_primes_2r.c
// Note that this fails to work as intended!
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
	int mytid=gettid();
	// printf("read_items starting with T=%d\n",mytid);
	while(1) {
		struct timespec ts=random_time();
		wait_for_something;

		int n = number[NEXT(rear)];
		nanosleep(&ts,NULL);
		rear = NEXT(rear);

		// printf("Fetched %d\n",n);
		if ( n == END_OF_INPUT )
			break;

		if ( isprime(n) )
			printf("%d %d\n",mytid, n);
		else
			printf("%d * %d\n",mytid, n);

	}
	// printf("read_items Done with T=%d\n",mytid);
	return NULL ;
}

#define waitforspace  for (int i=0;NEXT(front) == rear ;i=1) 
//				 if(i==0) printf(".\n")

int main(int argc, char * argv[]){
	pthread_t tid[2]; // This is not an integer. non portable structure type

	//printf("main starting with T=%d\n",gettid());

	int r1 = pthread_create(&tid[0],NULL,read_items,(void *)tid[0]);
	int r2 = pthread_create(&tid[1],NULL,read_items,(void *)tid[1]);
	if (r1 !=0 || r2 !=0 ) {
		perror("Thread create failed");
		exit(1);
	}
	for(int i=0;i<=101;i++) {

		// generate the next item to enqueue
		int r;
		if (i==100 || i==101)
			r=END_OF_INPUT;
 		else 
			r=genRN();


		waitforspace;

		number[NEXT(front)]=r;
		front=NEXT(front);

		// printf("Pushed %d\n",r);
	}
	// printf("main GENERATED, waiting for worker\n");

	r1 = pthread_join(tid[0],NULL);
	r2 = pthread_join(tid[1],NULL);
	if(r1 != 0 || r2 != 0 ) {
		perror("Thread join failed");
		exit(1);
	}
	// printf("main DONE\n");
	exit(0);

}
