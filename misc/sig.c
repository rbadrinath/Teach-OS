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
  while(1);
  printf("Finished g=%d\n",g);
}
