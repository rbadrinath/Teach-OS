#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <semaphore.h>

//NAMED_SEM not yet tested
//#define NAMED_SEM

#define SLEEPTIME 10000
struct timespec random_time(){
	struct timespec ts;
	ts.tv_sec=0;
        ts.tv_nsec=SLEEPTIME+random()%SLEEPTIME;
	return ts;
}
// struct timespec ts=random_time();
// nanosleep(&ts,NULL);


#define MAX 9

// shared structure which holds all the values in a queue
struct mystruct {
	int val[MAX];
	int free;     // pointer to next free slot
	int used;     // pointer to next filled slot - initially both are 0
};
// initialize will malloc this
struct mystruct * s; 

pthread_mutex_t mutex;


sem_t filledsem_sem;
sem_t freesem_sem;
sem_t * filledsem;
sem_t * freesem;

#define INIT_FREE MAX

#ifdef NAMED_SEM
#define FREESEM_NAME "/free-sem"
#define FILLEDSEM_NAME "/filled-sem"
#endif

// FOR THIS CODE NUMPRODUCERS == NUMCONSUMERS
#define NUMCONSUMERS 5
#define NUMPRODUCERS NUMCONSUMERS 

void initializations(){
	// allocated the shared structure
	s = malloc(sizeof(struct mystruct));
	s->free=s->used=0;

	// initialize mutex 
	if (pthread_mutex_init(&mutex, NULL) != 0) { 
        	printf("Failed to init mutex\n"); 
        	exit(1); 
    	} 
	
	// create semaphores
#ifdef NAMED_SEM
	filledsem=sem_open(FILLEDSEM_NAME,O_RDWR|O_CREAT,0644,0);
	if (filledsem == SEM_FAILED ){
		perror("Filled sem creation");
		exit(1);
	}

	freesem=sem_open(FREESEM_NAME,O_RDWR|O_CREAT,0644,INIT_FREE);
	if (freesem == SEM_FAILED ){
		perror("Free sem creation");
		exit(1);
	}
#else
	freesem=&freesem_sem;
	filledsem=&filledsem_sem;
	if(sem_init(filledsem,0,0)!= 0 ){   // initially no filled slots
		perror("Seminit failed");
		exit(1);
	}
	if(sem_init(freesem,0,INIT_FREE)!=0){	    // initially all free slots
		perror("Seminit failed");
		exit(1);
	}
#endif
	// checking
	if(freesem == filledsem){
		printf("Same address both sems?"); exit(1);
	}
	int r;
	sem_getvalue(filledsem,&r);
	if(r != 0 ){
		printf("r = %d\n",r);
		perror("filledsem not initialized well");
		exit(1);
	}
	sem_getvalue(freesem,&r);
	if(r != INIT_FREE){
		printf("r = %d\n",r);
		perror("freesem not initialized well");
		exit(1);
	}

	/*
	while(r != 0 ){
		printf("r = %d\n",r);
		perror("filledsem not initialized well");
		sem_wait(filledsem);
		sem_getvalue(filledsem,&r);

		//exit(1);
	}
	sem_getvalue(freesem,&r);
	while(r != INIT_FREE){
		printf("r = %d\n",r);
		perror("freesem not initialized well");
		sem_post(filledsem);
		sem_getvalue(filledsem,&r);
		//exit(1);
	}
	*/


}

void put(int val, struct mystruct * s){
	s->val[s->free]=val;
	s->free=(s->free+1)%MAX;
}
int get(struct mystruct * s){
	int x=s->val[s->used];
	s->used=(s->used+1)%MAX;
	return x;
}

void * thread_consumer(void * n){
        int t = (int) n + 1;

        while(1){
		// entry only if it gets a semaphore
		int r = sem_wait(filledsem);
		if( r != 0 ) {
			perror("sem_wait(filled) failed");
			exit(1);
		}
		
                // acquire mutex on the buffer
                pthread_mutex_lock(&mutex);

		// read a value and update the pointers in s
                int x = get(s);

		// wakeup anyone waiting for a free slot
		sem_post(freesem);

                // release mutex
                pthread_mutex_unlock(&mutex);

                printf("C #%2d v%4d \n",t,x);
		if (x==0){
			printf("Problem in C\n");
			exit(1);
		}
		if (x==-1)
			break;
		struct timespec ts=random_time();
		nanosleep(&ts,NULL);
		//sleep(1);
        }
        printf("C #%2d Exiting\n",t);
        return NULL ;
}

void * thread_producer(void * n){
        int t=(int ) n + 1;
        int i=0;
        int salt=t*100;

        for(int i=0;i<MAX+1;i++) {
                // new value to put
                int val=salt+(i%100);
		if (i==MAX) 
			val=-1;

		// Check if there is free space
		int r = sem_wait(freesem);
		if( r != 0 ) {
			perror("sem_wait(free) failed");
			exit(1);
		}

                // acquire mutex on the buffer
                pthread_mutex_lock(&mutex);

                // put new val & update indexes
                put(val,s);

		// wakeup anyone waiting on filledsem
		sem_post(filledsem);

                // release mutex
                pthread_mutex_unlock(&mutex);

                printf("P-------------#%2d v%4d\n",t,val);
		struct timespec ts=random_time();
		nanosleep(&ts,NULL);
        }
        printf("P-------------#%2d Exiting\n",t);
        return NULL ;
}

int main(){

        int i,r;
	initializations();

        pthread_t c_tid[NUMCONSUMERS];
        for(i=0;i < NUMCONSUMERS; i++)
                r = pthread_create(&c_tid[i],NULL,thread_consumer,(void *)i);

        // Create the producer threads
        pthread_t p_tid[NUMPRODUCERS];
        for(i=0;i < NUMPRODUCERS; i++)
                r = pthread_create(&p_tid[i],NULL,thread_producer,(void *)i);

        // This is the main thread. Let it exit, has nothing to do.
        //printf("Main thread done: Exiting!\n");
	
	for(int i=0;i<NUMCONSUMERS;i++)
		if ( pthread_join(c_tid[i],NULL) !=  0 ){
			perror("Joining C");
			exit(1);
		}
	for(int i=0;i<NUMPRODUCERS;i++)
		if ( pthread_join(p_tid[i],NULL) !=  0 ){
			perror("Joining P");
			exit(1);
		}

	//sleep(1);
        pthread_exit(NULL);

}
