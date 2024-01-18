#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#include <sys/time.h>
#include <sys/resource.h>

int new_priority=10;

void show_and_get(char *msg){
	printf(msg);
	printf("\n Press enter to continue ...");
	getchar();
}

void main(){
	// What scheduler policy is my system using for me?
	 int mypid=getpid();
	 int sdlr = sched_getscheduler(mypid);
	 printf("My scheduler is: ");
	 switch (sdlr){ 
		 // Normal policies
		 case SCHED_OTHER: 
			 printf("SCHED_OTHER\n");
			 break;
		 default:
			 printf("Unexpected policy %d\n",sdlr);
	 }
	 // What scheduler policy parameter (in particular the priority
	 struct sched_param param;
	 sched_getparam(mypid,&param);

	 // change my process priority ie nice value since it is SCHED_OTHER
	 printf("setting my priority to : %d\n",new_priority);
	 setpriority(PRIO_PROCESS,mypid,new_priority);
	 // we can also change priotity in one short for 
	 //       all process for a user.
	 // setpriority(PRIO_USER,getuid(),new_priority);
	 
	 printf("Now my process priority is : %d\n",getpriority(PRIO_PROCESS,0));
	 show_and_get("");
}
