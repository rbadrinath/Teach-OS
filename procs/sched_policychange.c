#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <limits.h>

#include <sys/time.h>
#include <sys/resource.h>

int policy=SCHED_FIFO;  // -o fifo   OR  -o rr
int priority=1;		// -p n           MUST BE AT LEAST 1, careful!
int yield=0;		// -y 0  OR -y 1  Weather it yields or no
int limitedtime=0;	// -t 0  OR -t 1  Whether to limit time or not

void show_and_exit(char *msg){
	printf("%s",msg);
	exit(1);
}

void parse_args_set_config(int argc, char * argv[]){

	if (argc > 1)
	if (strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0)
		show_and_exit("OPTIONS: -o rr|fifo|normal  schedling policy\n"
			      "         -p 0,1..99         static priority 0 for normal 1..99 for rr|fifo\n"
			      "         -y 0|1             yield no or yes\n"
			      "         -t 0|1             unlimited or limited time loop\n"
			      "Default:  -o fifo   -p 1   -y 0 -t 0\n"
			      "Use priority 0 for normal, else 1-99\n");

	if (argc%2==0) 
		show_and_exit("**Bad num args, must be even\n");

	int i=1;
	while(argv[i]!=NULL){
		if (strcmp(argv[i],"-o")==0) {
			if(strcmp(argv[i+1],"fifo")==0) policy=SCHED_FIFO;
			else if(strcmp(argv[i+1],"rr")==0) policy=SCHED_RR;
			else if(strcmp(argv[i+1],"normal")==0) policy=SCHED_OTHER;
			else show_and_exit("**Bad policy\n");
			i+=2;
		} else if (strcmp(argv[i],"-p")==0) {
			priority=atoi(argv[i+1]);
			if(priority<0 || priority > 99)
				show_and_exit("**Bad priority value\n");
			i+=2;
		} else if (strcmp(argv[i],"-y")==0) {
			yield=atoi(argv[i+1]);
			i+=2;
		} else if (strcmp(argv[i],"-t")==0) {
			limitedtime=atoi(argv[i+1]);
			i+=2;
		}
		else
			show_and_exit("**Bad arg, exit\n");
	}
	printf("Setting policy: %d, priority: %d, yield: %c, timelimit: %c\n",
			policy, priority, yield?'Y':'N', limitedtime?'Y':'N');
}

void show_current_scheduler(){
	 int mypid=getpid();
	 int sdlr = sched_getscheduler(mypid);
	 printf("My scheduler is: ");
	 switch (sdlr){ 
		 // Normal policies
		 case SCHED_OTHER: 
			 printf("SCHED_OTHER\n");
			 break;
		 // real time policies
		 case SCHED_FIFO:
			 printf("SCHED_FIFO\n");
			 break;
		 case SCHED_RR:
			 printf("SCHED_RR\n");
			 break;
		 default:
			 printf("Unknown policy %d\n",sdlr);
			 exit(1);
	 }
}
void main(int argc, char * argv[]){
	parse_args_set_config(argc,argv);

	// What scheduler policy is my system using for me?
	show_current_scheduler();
	// What scheduler policy parameter (in particular the priority
	struct sched_param param;
	sched_getparam(0,&param);

	// set scheduler policy and priority
	printf("Setting policy and priority ...\n");
	param.sched_priority=priority;
	int ret = sched_setscheduler(0, policy, &param);
	printf(" ... done. ");
	show_current_scheduler();
	if ( ret != 0 ) {
		 perror("**sched_setscheduler");
		 exit(1);
	} 
	if (limitedtime){
		printf(" Now a limited loop\n");
		for(int j=0;j<5;j++)
		for(int i=0;i<INT_MAX;i++)
			if (yield ) sched_yield();
	} else {
		printf(" Now in an infinite loop\n");
		while ( 1 ) 
			if (yield ) sched_yield();
	 }

}
