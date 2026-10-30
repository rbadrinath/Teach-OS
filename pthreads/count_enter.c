#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <math.h>


// TEMPLATE CODE

/* In this examples NTHREADS threads compete
   Only count_free number are allowed to enter the critical section
*/
#define NTHREADS 100
int count_free=20;

// Declare the mutexes and conditional variables and 
//   their initialization functions

pthread_cond_t free_slot;
pthread_mutex_t m;

void init_cvs(){
	if (pthread_cond_init(&free_slot,NULL) != 0 ){
		perror("Failed to initialize CV: free_slot\n");
		exit(1);
	}
}

void init_mutex(){
	if (pthread_mutex_init(&m, NULL) != 0) { 
        	printf("Failed to init mutex: m\n"); 
        	exit(1); 
    	} 
}


/* This is the thread function
	The parameter x is actually receiving an integer value
*/

void * worker_function(void * x){ 
	pthread_mutex_lock(&m);
	while (count_free==0) 
	  pthread_cond_wait(&free_slot,&m);

	count_free--;
	printf("%2d:  %d\n",(int )x,count_free);
	pthread_mutex_unlock(&m);

	// do work - simulated by the sleep below
	sleep(1);

	pthread_mutex_lock(&m);
	count_free++;
	pthread_cond_signal(&free_slot);
	pthread_mutex_unlock(&m);
}


// TEMPLATE CODE
int main(){
	// for Mutexes and CVs
	/*
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&free_slot,NULL);
	*/
	init_cvs();
	init_mutex();
	// Declare the array to hold tids
	pthread_t tid[NTHREADS];

	// CREATE THREADS
	for(int i=0;i<NTHREADS;i++){
    	int r = pthread_create(&tid[i],NULL,worker_function,(void *)i);
		if (r !=0 ) {
			perror("m Thread create() failed");
			exit(1);
		}
	}


	// WAIT FOR THREADS to finish
	for(int i=0; i <NTHREADS ;i++){
		int r = pthread_join(tid[i],NULL);
		if(r != 0 ) {
			perror("m Thread join() failed");
			exit(1);
		}
	}
}
