#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/resource.h>

#define YIELD

void main(){
	// What scheduler policy is my system using for me?
	 int mypid=getpid();
	 printf("Getting my schduler policy: ");
	 int sdlr = sched_getscheduler(mypid);
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
		 // real time (high static priority) policies
		 case SCHED_FIFO:
			 printf("SCHED_FIFO\n");
			 break;
		 case SCHED_RR:
			 printf("SCHED_RR\n");
			 break;
		 default:
			 printf("Unknown policy %d\n",sdlr);
	 }

	 // What is the possible priority range
	 int max = sched_get_priority_max(SCHED_FIFO);
	 int min = sched_get_priority_min(SCHED_FIFO);
	 printf("Allowed priority range for policy %d %d\n",max,min);

	 // What scheduler policy parameter (in particular the priority=nice)
	 printf("Getting scheduler parameters for policy:\n");
	 struct sched_param param;
	 sched_getparam(mypid,&param);


	 printf("      param.sched_priority %d\n",param.sched_priority);
	 errno=0;
	 printf("      Process  nice value %d\n",getpriority(PRIO_PROCESS,0));
	 if(errno==-1)
		 printf("**Error in getpriority(PRIO_PROCESS,...\n");
	 errno=0;
	 printf("      User     nice value %d\n",getpriority(PRIO_USER,0));
	 if(errno==-1)
		 printf("**Error in getpriority(PRIO_USER,...\n");
}
