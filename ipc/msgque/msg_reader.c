#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(){
	// get the token for my queue
	key_t token = ftok (MYPROJECTPATH, MYPROJECTID);
	if ( token <= 0 ){
		perror("Reader failed to get token");
		exit(1);
	}
	printf("token = %d\n",token);
	// get or create the queue
	int qid = msgget (token, IPC_CREAT|0644);
	if (qid < 0 ){
		perror("Reader failed to get queue");
		exit(1);
	}
	struct msgbuf m1;
	int n = msgrcv(qid, &m1, MSGSIZE, 0, 0);
	if (n < 0 ){
		perror("Reader failed to receive");
		exit(1);
	}
	printf("%s",m1.mtext);

}
