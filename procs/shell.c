#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND(X) (strcmp(argv[1],X)==0)
#define MAXLENGTH 1000
// #define DEBUG 1

// return number of args to cmd
// eg if input line =  ls -la
// then   cmd = ls
//        args = [ "ls" , "-ls" ]
//        spl = (undefined)
//        return value is 2 because that is the number of items in args[]
int parseline(char *line, char * cmd, char * args[], char * spl){
	int i=0;
	char * s=strtok(line," \n");
	if ( s != NULL ){
		strcpy(cmd,s);
		strcpy(args[0],s);
	}

	s=strtok(NULL," \n");
	i=1;
	while( s != NULL ){
		strcpy(args[i++],s);
		s=strtok(NULL," \n");
	}
	// strcpy(args[i],"\0");
	return(i);
}

// For debug only : just prints n items in x[]
void printlist(char *x[], int n){
	for (int i=0; i<n ; i++)
		printf("%s ",x[i]);
}

// executed cmd with arguments {args[0], args[1], ... args[numargs-1], NULL} as required by execv
// current implementation does not use environment
//                        forks and waits on the child to complete
int runcmd(char *cmd, char * args[], int numargs, char * spl){
	int j=0;
	char * paramlist[10] ;
	for(j=0;j<numargs; j++) {
		paramlist[j]=(char *)malloc(MAXLENGTH);
		strcpy(paramlist[j],args[j]);
	}
	paramlist[numargs]=NULL;
#ifdef DEBUG
	printf("Executing: %s ::",cmd); printlist(paramlist,numargs); printf("\n");
#endif
	int id=fork();
	if ( id == -1 ) {
		perror("Forking: ");
		return(1);
	} else {
		if (id == 0 ) {
			// in child
			execv(cmd,paramlist);
			char errorline[]="                            ";
			sprintf(errorline,"Error executing %s ",cmd);
			perror(errorline);
			exit(1);
		}
		// in parent
		int wstatus;
		waitpid(id,&wstatus,0);
	}
}

// This is the main behaviour of the shell, simply executes one command after the other
// It exits on the special "exit" command.
int main(int argc, char * argv[]){
	char * line = (char * )malloc(MAXLENGTH);
	char * cmd  = (char *)malloc(20);
	char * args[10];
	int i ;
	for(i=0;i<10;i++)
		args[i]=(char *)malloc(MAXLENGTH),
		strcpy(args[i],"Two ");
	char *spl=NULL;
	int size;
	int numargs;
	while(1){
		printf("+ ");
		getline(&line,(size_t *)&size,stdin);
		numargs=parseline(line,cmd, args, spl);
		if (strcmp(cmd,"exit")==0) {
			// printf("end shell\n");
			exit(0);
		}
		runcmd(cmd,args,numargs,spl);
	}
}
