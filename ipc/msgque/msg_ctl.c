#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "common.h"


int print_help(char * argv[]){
	printf("%s  <option> \n",argv[0]);
	printf("	-n [<PATH> <PROJECT>] : create newly msgq \n");
	// printf("	-i [<PATH> <PROJECT>] : init existing msgq\n");
	// printf("	-v        : current sem value\n");
	// printf("	-u        : up sem value\n");
	// printf("	-d        : down sem value\n");
	printf("	-r [<PATH> <PROJECT>] : remove msgq\n");
	printf("	-I [<PATH> <PROJECT>] : print info msqid_ds msgq\n");
	printf("	-h        : this help msg\n");
	printf(" NB: Default values of PATH and PROJECT are %s %d\n",MYPROJECTPATH, MYPROJECTID);

}

int get_existing_msq(key_t token){
	int msqid= msgget (token, 0644);
	if (msqid < 0 ){
		perror("getting msqid failed");
		exit(1);
	}
	return msqid;
}

int get_info_msq(key_t token, struct msqid_ds * buf){
	int msqid=get_existing_msq(token);
	if ( msqid < 0 ){
		perror("getting msqid failed:");
		exit(1);
	}
        int r = msgctl(msqid,IPC_STAT,buf );
	if ( r < 0 ){
		perror("getting msq info failed:");
		exit(1);
	}
        return r;
}

int print_info(struct msqid_ds * buf){
	printf("msg_stime=%ld\n",buf->msg_stime);
	printf("msg_rtime=%ld\n",buf->msg_rtime);
	printf("msg_qnum =%ld\n",buf->msg_qnum);
	printf("msg_lspid=%d\n",buf->msg_lspid);
	printf("msg_lrpid=%d\n",buf->msg_lrpid);

}

void rm_msq(key_t token){
	int msqid=get_existing_msq(token);
	if ( msqid < 0 ){
		perror("getting msqid failed:");
		exit(1);
	}
        int r = msgctl(msqid,IPC_RMID,NULL );
	if ( r < 0 ){
		perror("removing msq failed:");
		exit(1);
	}
}

#define NEW_MSGQ   1
#define INFO_MSGQ  4
#define RM_MSGQ    5


int main(int argc, char * argv[]){
	int cmd=-1;
	key_t token=-1;
	char *path=NULL;
	int  projectid=-1;
	int  msqid=-1;
	struct msqid_ds buf;
	// get the token for my queue
	if (argc == 4 ) {
		path=argv[2];
		projectid=atoi(argv[3]);
	} else {
		path=MYPROJECTPATH; projectid=MYPROJECTID;
	}
	printf("Project path = %s, project id = %d\n", path, projectid);
	struct stat statbuf;
	if (stat(path, &statbuf) < 0 ){
		perror("Failed to stat the path for ftok");
		exit(1);
	}
	token = ftok (path, projectid);
	if ( token <= 0 ){
		perror("Failed to get token");
		exit(1);
	}
	printf("token = %d\n",token);
        if (argc > 1 ) {
		if (strcmp(argv[1],"-n")==0)
			cmd=NEW_MSGQ;
		if (strcmp(argv[1],"-r")==0)
			cmd=RM_MSGQ;
		if (strcmp(argv[1],"-I")==0)
			cmd=INFO_MSGQ;
	}
	

	switch(cmd){
		case NEW_MSGQ: 
			msqid = msgget (token, IPC_CREAT|IPC_EXCL|0644);
			if (msqid < 0 ){
				perror("Reader failed to create new  queue");
				exit(1);
			}
		        break;
		case INFO_MSGQ:
		     msqid = get_info_msq(token, &buf);
		     print_info(&buf);
		     break;
		case RM_MSGQ:
		     rm_msq(token);
		     break;
		default:
		     printf("Unknown/Unimplemented Option\n");
		     print_help(argv);
	}
}
