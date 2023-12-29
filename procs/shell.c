#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND(X) (strcmp(argv[1],X)==0)
#define PROMPT "* "
#define MAXLENGTH 1000    // length of a command line
#define MAXARGS    98 	  // max num of arguments to a command on the command line
//#define DEBUG 1

// =========================================================================
// SOME UTILITIES TO HELP READ INPUT AND CONVERT IT TO COMMAND AND ARGUMENTS

// return number of args to cmd
// eg if input line =  ls -la
// then   *cmd = "ls"
//        args = [ "ls" , "-ls" ]
//        return value is 2 because that is the number of items in args[]
//  IMPORTANT: *cmd and args[i] are malloced by the function
//             **** Must be freed up later
//             *** You may use clean_parsedline()
// special case: If line is empty then cmd=NULL indicating empty line
//                                     and args[0]=NULL
//
int parseline(char *line, char * *cmd, char * args[]){
	int i=0;
	char * s=strtok(line," \n");
	if ( s != NULL ){
		*cmd = malloc(strlen(s)+1);
		strcpy(*cmd,s);
		args[0]=malloc(strlen(s)+1);
		strcpy(args[0],s);
	} else  {
		*cmd=NULL;
		args[0]=NULL;
		return 0;
	}

	s=strtok(NULL," \n");
	i=1;
	while( s != NULL && i <= MAXARGS){
		args[i]=malloc(strlen(s)+1);
		strcpy(args[i++],s);
		s=strtok(NULL," \n");
	}
	args[i]=NULL;
	return(i);
}
// Cleanup stuff allocated by parseline() 
void clean_parsedline(char **cmd, char *args[]){
	if(*cmd){
		free(*cmd);
		*cmd=NULL;
	}
	for(int i=0;args[i]!=NULL;i++){
		free(args[i]);
		args[i]=NULL;
	}
}

// For debug only : just prints n items in x[]
void printlist(char *x[], int n){
	for (int i=0; i<n ; i++)
		printf("%s ",x[i]);
}
// UTILITIES ENDED===========================================


// executed cmd with arguments {args[0], args[1], ... args[numargs-1], NULL} as required by execv
// current implementation does not use environment
//                        forks and waits on the child to complete
int runcmd(char *cmd, char * args[], int numargs){

	args[numargs]=NULL; // cautious - 
			    // make sure last argument in the array is NULL
			    
#ifdef DEBUG
	printf("Executing: %s ::",cmd); 
	printlist(args,numargs); 
	printf("\n");
#endif

	int id=fork();   	// call fork()
	if ( id == -1 ) {
		perror("Forking: ");
		return(1);	// if fork failed
	} else {
		if (id == 0 ) { // in child
			execv(cmd,args);  // call execv()
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

// This is the main behaviour of the shell 
// simply executes one command after the other
// It exits on the special "exit" command typed by the user.
int main(){
	// To hold a user input line
	char * line = (char * )malloc(MAXLENGTH);
	int size=MAXLENGTH;

	// To hold the command name and its arguments
	char * cmd ; 
	char * args[MAXARGS+2];
	int numargs;

	while(1){
		// print a prompt
		printf(PROMPT);

		// read  a line from the user
		getline(&line,(size_t *)&size,stdin);

		// get the command and arguments from the line
		// eg cmd = "/bin/ls" ; args[]= ["/bin/ls" , "-l", NULL]
		numargs=parseline(line,&cmd, args);

		if ( cmd==NULL )  // if no command given
			continue; // nothing to execute, just read the next line

		if (strcmp(cmd,"exit")==0) { // user wants to exit
			exit(0);
		}
		else    // run the user command
			// eg "/bin/ls",   ["/bin/ls", "-l", "NULL"],  2
			runcmd(cmd,args,numargs);

		// cleanup malloc-ed memory
		clean_parsedline(&cmd,args);

		// done, go to the top of the loop
	}
}
