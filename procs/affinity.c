/* This is a simplistic implementation of what schedtool  implements more elaborately */

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

extern char * optarg;
extern int optind, opterr, optopt;

void print_help(char* cmd){
	fprintf(stderr,"%s: Usage [-g | -s CPU ] -p PID\n",cmd);
	fprintf(stderr,"    -g : gets the affinity of the process identified by PID,\n"
			       "    -s : sets CPU as the affinity for the process identified by PID\n");
	return;
}
int main(int argc, char * argv[]){
	pid_t pid=0;
	int flag=0;
	cpu_set_t cpumask;
	CPU_ZERO (&cpumask);
	int opt;
	if ( (opt=getopt(argc,argv,"gs:"))!=  -1) {
		switch(opt){
			case 'g':
				// do the get stuff
				flag=1;
				break;
			case 's':
				flag=2;
				// do the set stuff
				int a_cpu = atoi(optarg);
				CPU_SET(a_cpu, &cpumask);
				break;
			default:
				exit(1);
		}
	} else {
		print_help(argv[0]);
		exit(1);
	}
	if ( (opt=getopt(argc,argv,"p:"))!=  -1) {
		switch(opt){
			case 'p':
				// do the set stuff
				pid = (pid_t) atoi(optarg);
				break;
			default:
				print_help(argv[0]);
				exit(1);
		}
	} else {
		print_help(argv[0]);
		exit(1);
	}
	if (flag==1){
		// this is get
		int r = sched_getaffinity(pid,sizeof(cpumask),&cpumask);
		for(int i=0;i<4;i++)
			if(CPU_ISSET(i,&cpumask))
				printf("%d ",i);
		printf("\n");
	} else {
		// this is set
		int r = sched_setaffinity(pid, sizeof(cpumask),&cpumask);
	}
	return 0;

}
