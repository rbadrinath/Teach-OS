#include "myincludes.h"
#include <string.h>
#define CMP_ARGV(I,X) (strcmp(argv[I],X)==0)

void print_help_exit(char * cmd){
	printf("%s  [-h|--help] redirect-outfile  "
	       	"command-to-execute   arg1 arg2 ...\n",
	       	cmd);
	exit(0);
}
int check_args(char * argv[]){
	int i=0;
	for(i=1;argv[i]!=NULL;i++) {
		if (CMP_ARGV(i,"-h") || CMP_ARGV(i,"--help")) {
			print_help_exit(argv[0]);
		}
	}
	return i;
}

void main(int argc, char * argv[]){
	int n = check_args(argv);
	if (n < 3 )
		print_help_exit(argv[0]);
	char * of=argv[1];
	char * cmd=argv[2];
	char ** newlist=&argv[2];
	// This is to setup redirection
	int ofd = open(of,O_WRONLY|O_CREAT,S_IWUSR|S_IRUSR);
	if (ofd < 0 ) {
		printf("** Open %s failed",of);
		exit(1);
	} else {
		dup2(ofd,1);
	}

	execvp(cmd,newlist);
}
