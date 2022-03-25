#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * see man sysconf
 *    These are system limits (not user or group specific)
 *
 * see man getrlimit
 *    These are user or process specific resource limits
 *
 * Just to see system and process limits
 */

void printhelp(int argc,char * argv[]){
	printf("Usage: %s [-r|-s] NAME\n",argv[0]);
	printf("Eg.    %s -s PAGESIZE\n",argv[0]);
	printf("Eg.    %s -r STACK\n",argv[0]);
}
void syscon(int argc, char * argv[]){
	long var=-1;
	for (int i=2; i< argc; i ++){
		char *name=argv[i];
		if (strcmp(name,"ARG_MAX")==0){
			var=_SC_ARG_MAX;
		}
		if (strcmp(name,"PAGESIZE")==0){
			var=_SC_PAGESIZE;
		}
		if (strcmp(name,"NUMPHYPAGES")==0){
			var=_SC_PHYS_PAGES;
		}
		if (var != -1 ){
			int r = sysconf(var);
			if ( r < 0 ){
				perror("Calling sysconf");
				exit(1);
			}
			printf(" %s is %d \n",name,r);
		}
	}

}
void reslimit(int argc, char * argv[]){
	long var=-1;
	struct rlimit rlim;
	for (int i=2; i< argc; i ++){
		char name[20]="RLIMIT_";
		strcat(name,argv[i]);

		if (strcmp(name,"RLIMIT_AS")==0){
			var=RLIMIT_AS;
		}
		if (strcmp(name,"RLIMIT_CORE")==0){
			var=RLIMIT_CORE;
		}
		if (strcmp(name,"RLIMIT_CPU")==0){
			var=RLIMIT_CPU;
		}
		if (strcmp(name,"RLIMIT_DATA")==0){
			var=RLIMIT_DATA;
		}
		if (strcmp(name,"RLIMIT_FSIZE")==0){
			var=RLIMIT_FSIZE;
		}
		if (strcmp(name,"RLIMIT_LOCKS")==0){
			var=RLIMIT_LOCKS;
		}
		if (strcmp(name,"RLIMIT_MEMLOCK")==0){
			var=RLIMIT_MEMLOCK;
		}
		if (strcmp(name,"RLIMIT_MSGQUEUE")==0){
			var=RLIMIT_MSGQUEUE;
		}
		if (strcmp(name,"RLIMIT_NICE")==0){
			var=RLIMIT_NICE;
		}
		if (strcmp(name,"RLIMIT_NOFILE")==0){
			var=RLIMIT_NOFILE;
		}
		if (strcmp(name,"RLIMIT_NPROC")==0){
			var=RLIMIT_NPROC;
		}
		if (strcmp(name,"RLIMIT_RSS")==0){
			var=RLIMIT_RSS;
		}
		if (strcmp(name,"RLIMIT_STACK")==0){
			var=RLIMIT_STACK;
		}
		if (var != -1 ){
			int r = getrlimit(var,&rlim);
			if ( r < 0 ){
				perror("Calling sysconf");
				exit(1);
			}
			printf(" %15s : cur is %ld\n"
			       "               max is %ld\n",
				name,rlim.rlim_cur,rlim.rlim_max);
		}
	}
	printf("Note %ld means RLIM_INFINITY\n",RLIM_INFINITY);
}

int main(int argc, char * argv[]){
	if (argc == 1 ) {
		printhelp(argc,argv);
		exit(0); // Nothing to do
	}
	if (strcmp(argv[1],"-r")==0){
		reslimit(argc,argv);
		exit(0);
	}
	if (strcmp(argv[1],"-s")==0)
		syscon(argc,argv);
	else
		printhelp(argc,argv);
	exit(0);

}
