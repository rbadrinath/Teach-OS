#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

int g=0;

void handler(int signo) {
   struct timeval tv;
   struct timezone tz;


  if (signo ==  SIGALRM)
    printf("received SIGALRM when g is %d\n",g);
  gettimeofday(&tv,&tz); 
  printf("time is --> %ld\n",tv.tv_sec);
  g++;
}

void set_my_signal(){
  if (signal(SIGALRM, handler) == SIG_ERR) {
       perror("setting signal failed");
       exit(1);
  }
}


int main () {
  struct itimerval timer;
  struct itimerval timer2;
  
  // set handler
  set_my_signal();

  // set the initial timeout values 
  timer.it_value.tv_sec = 10;   // first expires after 10 sec
  timer.it_value.tv_usec = 0;   // Recall: 1,000,000 us is one second.
  // and after that timeout values
  timer.it_interval.tv_sec = 5; // expires every 5 sec thereafter
  timer.it_interval.tv_usec = 0;

  // print the time now.
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv,&tz); 
  printf("time is --> %ld\n",tv.tv_sec);

  // start the timer
  setitimer (ITIMER_REAL, &timer, NULL);

  while (1);


 /* Do busy work. */
}
