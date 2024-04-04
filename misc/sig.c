#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int g=99;

void handler(int signo) {
  if (signo ==  SIGUSR1)
    printf("received SIGUSR1 when g=%d\n",g);
  g++;
}

void set_my_signal(){
  if (signal(SIGUSR1, handler) == SIG_ERR) {
       perror("setting signal failed");
       exit(1);
  }
}
int main() {
  set_my_signal();
  printf("g is at %p\n",&g);
  int x=1;
  while(1) {
	  sleep(1);
	  for(int i=0;i<x;i++) printf(".");
	  printf("\n");
	  x++;
	  if( x > 10 ) x=1;
  }
  printf("Finished g=%d\n",g);
}
