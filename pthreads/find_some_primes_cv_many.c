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
#define NONE -2
// new
#define NREADERS 15 
#define NWRITERS 2
#define COUNT 20 

/*
 * There are NWRITERS number of threads that write random integers to the
 *   FIFO queue. Each of them writes COUNT number of integers.
 *   Each is implemented in a   write_items()   thread
 *
 * There are NREADERS number of threads that repeatedly read from the queue
 *   Each of them repeatedly reads from the queue and prints primality info
 *   Each is implemented in a   read_items()   thread
 *   Look at the reader code to see how end of input it handled
 *
 * main() starts the writers and the readers
 *        Then it waits for all the writers to finish
 *        Then it inserts an END_OF_INPUT into the queue
 *        Then it waits for all the readers to finish
 * 
 */
int gettid();
struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=random()%SLEEPTIME;
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

pthread_cond_t free_slot;   // writers wait for a free slot
pthread_cond_t filled_slot; // readers wait for a filled slot
pthread_mutex_t mutex;	    // mutex for all operations to the queue and to stop

int stop=0;	// some reader sets it to 1 to indicate 
		// it has read an END_OF_INPUT and so 
		// no more input is expected to come

#define wait_for_something_conditionally   while(front == rear && stop == 0) \
	pthread_cond_wait(&filled_slot,&mutex) 


int read_value(int mytid){
	int n=NONE;
	struct timespec ts=random_time();
	pthread_mutex_lock(&mutex);      // |-together these two
	wait_for_something_conditionally;// |-stmts do cond lock
	if ( stop == 0 ) { 		// |
	   n = number[NEXT(rear)];	// | - CS
	   nanosleep(&ts,NULL);		// |
	   rear = NEXT(rear);		// |
					// |
	   if ( n == END_OF_INPUT ){	// |
		stop=1;			// |
		printf("r END_OF_INPUT found %d\n", mytid); // |
	   }				// |
	} 				// |

	// We have different cases here!
	if ( n==NONE ) // nothing was there to read, just get out
		n = END_OF_INPUT;  // Just set the return value

	else if ( n == END_OF_INPUT ) // found the END_OF_INPUT
		// wake up reads who are waiting for something
		pthread_cond_broadcast(&filled_slot); // NOTE: filled_slot!!
						      // NOTE: cond_broadcast
						      // TRY: cond_signal

	else 	// normal read of a valid value
		// wakeup any writers
		pthread_cond_signal(&free_slot); 

	pthread_mutex_unlock(&mutex);	 //  in all cases unlock
	nanosleep(&ts,NULL); // additional sleep for fun - create race
	return n;
}
void * read_items(void * id){
	int mytid=gettid();
	// printf("r read_items starting with TID=%d\n",mytid);
	while(1) {

		int n = read_value(mytid);

		// printf("Fetched %d\n",n);
		if ( n == END_OF_INPUT )
		 	break;

		if ( isprime(n) )
			printf("r %d %d\n",mytid, n);
		else
			printf("r %d * %d\n",mytid, n);

	}
	printf("w Done TID=%d\n",mytid);
	return NULL ;
}

#define waitforspace_conditionally while(NEXT(front) == rear ) \
					pthread_cond_wait(&free_slot,&mutex)

//				 if(i==0) printf(".\n")
//
int write_value(int v){
		pthread_mutex_lock(&mutex);
		waitforspace_conditionally;

		number[NEXT(front)]=v;
		front=NEXT(front);

		pthread_cond_signal(&filled_slot);
		pthread_mutex_unlock(&mutex);
		
		return v;
}
void * write_items(void * id){
	int mytid=gettid();
	for(int i=0;i<COUNT;i++) {

		// generate the next item to enqueue
		int r=genRN();

		// put it in the queue
		write_value(r);

		printf("w Pushed %d\n",r);
	}
	printf("w Done \n");
	return NULL ;
}

int main(int argc, char * argv[]){
	pthread_t rtid[NREADERS]; // This is not an integer.
	pthread_t wtid[NWRITERS]; // This is not an integer.

	//printf("m Starting with T=%d\n",gettid());

	// Create all the readers
	for(int i=0; i <NREADERS ;i++){
		int r = pthread_create(&rtid[i],NULL,read_items,(void *)NULL);
		if (r !=0 ) {
			perror("m Thread create(r) failed");
			exit(1);
		}
	}

	// Create all the writers
	for(int i=0; i <NWRITERS ;i++){
		int r = pthread_create(&wtid[i],NULL,write_items,(void *)NULL);
		if (r !=0 ) {
			perror("m Thread create(w) failed");
			exit(1);
		}
	}

	printf("m GENERATED all workers, waiting for worker\n");

	// Now wait for all the writers to finish
	for(int i=0; i <NWRITERS ;i++){
		int r = pthread_join(wtid[i],NULL);
		if(r != 0 ) {
			perror("m Thread join(w) failed");
			exit(1);
		}
	}

	printf("m Writers done Pushing END_OF_INPUT\n");

	// all writers are done, so put a -1 and signal the end.
	write_value(END_OF_INPUT);

	// Now wait for all the readers to finish
	for(int i=0; i <NREADERS ;i++){
		int r = pthread_join(rtid[i],NULL);
		if(r != 0 ) {
			perror("m Thread join(r) failed");
			exit(1);
		}
	}

	printf("m Readers done\n");


	// printf("m DONE\n");
	exit(0);

}
