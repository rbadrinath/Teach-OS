#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

// Generic Petersons implementation - a linear implementation
// Not very space efficient, but...
/* 
MAIN IDEA
   1. The threads competing at each level and the one who wins
      all n levels acquires the lock
   2. at each level the incoming competitor tries to be the nice guy
      at that level, letting others go up to the next level
   3. when there are no one to compete with or someone else becomes nice
      guy, you get to move to the next level
IMPLEMENTATION
   There is a matrix with one row per thread
   Each thread fills its row from 0..nthreads-1
      Each such row item is a level
   There is a niceguy for each level and each thread puts its ID competing
   to be the nice guy.
*/


#define SLEEPTIME 100000
struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=random()%SLEEPTIME;
	return ts;
}

// Shared
#define MAX_THREADS 100

int niceguy[MAX_THREADS];  // one niceguy at each level
int competing[MAX_THREADS][MAX_THREADS];  // bool: One row for each thread

#define DFL_NTHREADS 10
int nthreads=0;

#define START_LEVEL 0
#define END_LEVEL nthreads

// returns the count of all threads that have competing[i][l]==1
// This is cumulative of all threads in this and the higher levels
// because of the way competing[i][l] is erased only by unlock
int count_atlevel(int l){
	int count=0;
	for(int i=0;i<nthreads;i++)
		if(competing[i][l]==1)count++; // ith thread is competing at level l
	return count;
}
void unlock(int self){
	// just assert that self is not competing at any of the levels
	for(int l=START_LEVEL;l<=END_LEVEL;l++)
		competing[self][l]=0;
	return;
}
void lock(int self){
	for( int l=START_LEVEL;l<=END_LEVEL;l++){
		competing[self][l]=1;  // self competing at level l
		niceguy[l]=self;       // self willing to be the niceguy at level l
		// count_atlevel(l) indicates number of people at level l or more
		while( (count_atlevel(l) > 1 && niceguy[l] == self) )
			;
	}
	// The one who makes it here - he still is at that top level
}


// This is the function using the lock and unlock
// Printing after entering and before exiting
// we should expect to see x in and x ....out lines one after another, 
//        not mixed between threads!
void * do_job(int * arg){
	int self=*arg;

	// lock and print entry
	lock(self);
	printf("%2d in\n",self);

	// wait a bit - simulating the CS
	struct timespec ts=random_time();
	nanosleep(&ts,NULL);

	// exit and print exiting
	printf("%2d .....out\n",self);
	unlock(self);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if(argc > 1 ) {
		nthreads=atoi(argv[1]);
		if (nthreads==0 || nthreads > MAX_THREADS)
			nthreads=DFL_NTHREADS;
	} else 
		nthreads=DFL_NTHREADS;
		
	pthread_t tid[nthreads];
	int simple_id[nthreads];


	for(int i=0;i<nthreads;i++){
		unlock(i);
	}

	for(int i=0;i<nthreads;i++) {
		/*
		if (i==nthreads/2)
		   sleep(1);
		   */
		simple_id[i]=i;
		pthread_create(&tid[i],NULL,do_job,&simple_id[i]);
	}
	for(int i=0;i<nthreads;i++)
		pthread_join(tid[i],NULL);
}
