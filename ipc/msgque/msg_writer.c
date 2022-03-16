#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

int main(){
	// get the token for my queue
	key_t token = ftok (MYPROJECTPATH, MYPROJECTID);
	if ( token <= 0 ){
		perror("Writer failed to get token");
		exit(1);
	}
	printf("token = %d\n",token);
	// get or create the queue
	int qid = msgget (token, IPC_CREAT|0644);
	if (qid < 0 ){
		perror("writer failed get queue:");
		exit(1);
	}
	struct msgbuf m1;
	// fill in the send message text
	m1.mtype=TYPEA;
	strcpy(m1.mtext,"Hellow World\n");
	int n = msgsnd(qid, &m1, MSGSIZE, 0);
	if (n < 0 ){
		perror("sending failed:");
		exit(1);
	}

}
