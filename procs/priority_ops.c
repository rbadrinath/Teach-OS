#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
/*
       int getpriority(int which, id_t who);
       int setpriority(int which, id_t who, int prio);
*/
void main(){
	int pid=getpid();
	printf("My PID is %d\n",pid);
	int prio=getpriority(PRIO_PROCESS,pid);
	printf("My Priority is %d\n",prio);
	getchar();
	printf("Setting my priority\n");
	int r = setpriority(PRIO_PROCESS,pid, prio+4);
	if ( r == -1)
		printf("Error setting priority**\n");
	prio=-99;
	prio=getpriority(PRIO_PROCESS,pid);
	printf("My Priority is %d\n",prio);
	getchar();
}


