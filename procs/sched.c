#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#include <sys/time.h>
#include <sys/resource.h>


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
	 // What is the priority range
	 int max = sched_get_priority_max(SCHED_FIFO);
	 int min = sched_get_priority_min(SCHED_FIFO);
	 printf("Max min sched get priority %d %d\n",max,min);

	 // What scheduler policy parameter (in particular the priority
	 struct sched_param param;
	 sched_getparam(mypid,&param);
	 printf("param.priority %d\n",param.sched_priority);
	 printf("From getpriority Process%d\n",getpriority(PRIO_PROCESS,mypid));
	 printf("From getpriority user   %d\n",getpriority(PRIO_USER,getuid()));

	 // setting priority of this process / all processes of this user
	 setpriority(PRIO_PROCESS,mypid,10);
	 // setpriority(PRIO_USER,getuid(),10);
	 getchar();

	 // attempt to set it to FIFO scheduler fails as normal user
	 //      but works as super user. Be careful because it is real time
	 param.sched_priority=1; // I found this is minimum for SCHED_FIFO
	 int ret = sched_setscheduler(0, SCHED_FIFO, &param);
	 if ( ret != 0 ) {
		 perror("**sched_setscheduler");
	 } else {
		 printf(" I set my priority, and going on an infinit loop\n");
		 while ( 1 ) ;
	 }

}
