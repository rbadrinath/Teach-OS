#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define SLEEPTIME 100

int gettid();

void * thread_function(void * tid){
	printf("T=%d , P=%d sleeping...\n", gettid(), getpid());
	// pthread_exit(NULL);
	sleep(SLEEPTIME);
	putchar('_');
	printf("... %d woke up\n", gettid());
	return NULL ;
}

int main(int argc, char * argv[]){
	pthread_t tid; // This is not an integer. non portable structure type

	printf("Starting with T=%d P=%d\n",gettid(), getpid());

	int r = pthread_create(&tid,NULL,thread_function,(void *)tid);

	// pthread_cancel(tid);
	sleep(SLEEPTIME);
	putchar('1');
	pthread_join(tid,NULL);
	exit(0);

}
