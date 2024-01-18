#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#include <sys/time.h>
#include <sys/resource.h>

#define YIELD

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
	/* My system doesnt support these normal mode scheduler policies
		 case SCHED_BATCH:
			 printf("SCHED_BATCH\n");
			 break;
		 case SCHED_IDLE:
			 printf("SCHED_IDLE\n");
			 break;
	*/
		 // real tiem policies
		 case SCHED_FIFO:
			 printf("SCHED_FIFO\n");
			 break;
		 case SCHED_RR:
			 printf("SCHED_RR\n");
			 break;
		 default:
			 printf("Unknown policy %d\n",sdlr);
	 }
	 // What scheduler policy parameter (in particular the priority
	 struct sched_param param;
	 sched_getparam(mypid,&param);

	 // change my process priority ie nice value
	 setpriority(PRIO_PROCESS,mypid,10);
	 // we can also change priotity in one short for 
	 //       all process for a user.
	 // setpriority(PRIO_USER,getuid(),10);
	 getchar();
}
