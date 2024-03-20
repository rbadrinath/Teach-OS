#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define SLEEPTIME 100

int gettid();

char global[20]="Hello How are you?";

void * thread_function(void * tid){
	printf("T=%d , P=%d sleeping...\n", gettid(), getpid());
	// pthread_exit(NULL);
	sleep(SLEEPTIME);
	printf("... %d woke up\n", gettid());
	// to demonstrate they share the same address space
	strcpy(global,"bye bye"); 
	printf("Child Wrote:%s\n",global);
	return NULL ;
}

int main(int argc, char * argv[]){
	pthread_t tid; // This is not an integer. non portable structure type

	printf("Starting with T=%d P=%d\n",gettid(), getpid());

	int r = pthread_create(&tid,NULL,thread_function,(void *)tid);

	// pthread_cancel(tid);
	sleep(SLEEPTIME);
	// Try this for fun for(;;) ;
	pthread_join(tid,NULL);
	printf("Parent Sees:%s\n",global);
	exit(0);

}
