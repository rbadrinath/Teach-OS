#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int g=0;

void handler(int signo) {
  if (signo ==  SIGUSR1)
    printf("received SIGUSR1 when g=%d\n",g);
  g=100;
}

int main() {
  if (signal(SIGUSR1, handler) == SIG_ERR) {
       perror("setting signal failed");
       exit(1);
  }
  sleep(15);
  printf("Finished g=%d\n",g);
}
