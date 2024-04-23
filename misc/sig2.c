#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

// This code simply prints some global value when SIGUSR1 is delivered to it
// any other signal (eg SIGUSR2) will terminate the proces

// When SIGUSR2 is used, the handler for SIGUSR1 flips between handler() and SIG_IGN

int g=99;

int sig1on=1;

void handler(int signo) {
  if (signo ==  SIGUSR1)  {
    printf("received SIGUSR1 when g=%d\n",g);
    g++;
  } 
  else if (signo == SIGUSR2) {
    if (sig1on)
	    signal(SIGUSR1, SIG_IGN);
    else
	    signal(SIGUSR1, handler);
    sig1on=1-sig1on;
  }
}

void set_my_signal(){
  if (signal(SIGUSR1, handler) == SIG_ERR) {
       perror("setting signal failed");
       exit(1);
  }
  if (signal(SIGUSR2, handler) == SIG_ERR) {
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
//
/* You may use the following script to get the pid of this process
mypid=`ps -x | grep sig2| grep -v grep |  cut -c1-8`
   and use this to send 1 and 2 signals to the process. Try with and without USESIGUSR2 defined
while : ; do read; if [ "x$REPLY" == "x1" ]; then kill -s SIGUSR1 $mypid; else kill -s SIGUSR2 $mypid; fi; done
*/


