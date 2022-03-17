#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "safe_ops.h"

#define SLEEPTIME 1
#define NUMCONSUMERS 2
#define NUMPRODUCERS  5

// Declare all glabal shared stuff
// 1. Data structure
struct mystruct s;  // PS: This includes a mutex too! 
		    // Perhaps we could include the CVs as well?
// 2. Conds Mutex for the list
pthread_cond_t emptyslot_cond, filledslot_cond;


void * thread_consumer(void * n){
	int t = (int) n + 1;
	
	while(1){
		// acquire mutex on the buffer
		pthread_mutex_lock(&s.mutex);
		while (ISEMPTY(s)) { // if not empty, then greedily consume
			// wait till there is a FILLED element in the buffer 
			pthread_cond_wait(&filledslot_cond,&s.mutex);
		}
		// read that element & update indexes
		int x = get(&s);
		// signal anyone waiting for EMPTY condition
		pthread_cond_signal(&emptyslot_cond);
		// release mutex
		pthread_mutex_unlock(&s.mutex);
		printf("C #%2d v%4d \n",t,x);
	}
	return NULL ;
}

void * thread_producer(void * n){
	int t=(int ) n + 1;
	int i=0;
	//int salt=rand()%100; // salt should be a random from 0..99
	int salt=t;
	salt=salt*100;
	while(1) {
		// new value to put
		int val=salt+(i%100);
		// acquire mutex on the buffer
		pthread_mutex_lock(&s.mutex);
		// wait till there is a EMPTY element in the buffer 
		while (ISFULL(s)) { // If not full just go ahead and put
			pthread_cond_wait(&emptyslot_cond,&s.mutex);
		}
		// put new val & update indexes
		put(val,&s);
		printf("P-------------#%2d v%4d\n",t,val);
		// signal anyone waiting for FILLED condition
		pthread_cond_signal(&filledslot_cond);
		// release mutex
		pthread_mutex_unlock(&s.mutex);
		i++;
		sleep(SLEEPTIME);
	}
	return NULL ;
}


int main(int argc, char * argv[]){
	// initialize randomnumber generator
	srand(time(NULL));
	// initialize the mutex and the cvs
	pthread_mutex_init(&s.mutex,NULL);
	pthread_cond_init(&filledslot_cond,NULL);
	pthread_cond_init(&emptyslot_cond,NULL);
	
	// Initialize the shared data buffer
	s.free=s.used=0; // this is empty condition
	// but s.free+1==s.used is the full condition

	// Create the consumer threads
	int i,r;
	pthread_t c_tid[NUMCONSUMERS];
	for(i=0;i < NUMCONSUMERS; i++) 
		r = pthread_create(&c_tid[i],NULL,thread_consumer,(void *)i);

	// Create the producer threads
	pthread_t p_tid[NUMPRODUCERS];
	for(i=0;i < NUMPRODUCERS; i++) 
		r = pthread_create(&p_tid[i],NULL,thread_producer,(void *)i);

	// This is the main thread. Let it exit, has nothing to do.
	printf("Main thread done: Exiting!\n");
	pthread_exit(NULL);
}
