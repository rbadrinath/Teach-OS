#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <math.h>

#define SLEEPTIME 10000
#define MAX 20 
#define NTHRD 100 // keep it even

int gettid();

//Normally balance=balance-100 is one statement
//However to illustrate the problem better we can break it up 
//   into multiple statements
//#define ONESTATEMENT



struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=random()%SLEEPTIME;
	return ts;
}


int balance = 0;


void * update_decrementer(void * tid){
	struct timespec ts=random_time();
	nanosleep(&ts,NULL);

#ifdef ONESTATEMENT
	balance = balance-100;
#else
	int ax =balance;
	ax = ax-100;
	nanosleep(&ts,NULL);
	balance=ax;
#endif


	return NULL;
}

void * update_incrementer( void * tid){
	struct timespec ts=random_time();
	nanosleep(&ts,NULL);


#ifdef ONESTATEMENT
	balance=balance+100;
#else
	int ax =balance;
	ax = ax+100;
	nanosleep(&ts,NULL);
	balance=ax;
#endif

	return NULL;
}


int main(int argc, char * argv[]){
	pthread_t tid[NTHRD]; // tid is not an integer... non portable structure pointer type

	// Initialize
	srand(getpid());

	for(int i=0;i<NTHRD;i+=2) {
		int r = pthread_create(&tid[i],NULL,update_decrementer,(void *)tid[i]);
		if (r != 0 ) {
			perror("couldn't create 1");
			exit(1);
		}
		r = pthread_create(&tid[i+1],NULL,update_incrementer,(void *)tid[i+1]);
		if (r != 0 ) {
			perror("couldn't create 2");
			exit(2);
		}
	}
	printf("main GENERATED, waiting for workers\n");

	for(int i=0;i<NTHRD;i++)
			pthread_join(tid[i],NULL);
	printf("main DONE. balance = %d\n",balance);
	exit(0);

}
