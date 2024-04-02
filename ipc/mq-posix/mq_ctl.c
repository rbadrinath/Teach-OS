#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>


#define MQ_NAME "/mq-1"
#define MQ_MSGSIZE 1024
#define MQ_MSGCOUNT 10

char g_msg[50]="hello world A";
char rcv_buf[MQ_MSGSIZE*2];

void print_help(char * argv[]){
	printf("%s [-n]    : The optoinal u argument means do not initialize\n",argv[0]);
	printf("option: choose from:\n");
	printf("    n		open new mq\n");
	printf("    r		receive from the mq\n");
	printf("    s		send to the mq\n");
	printf("    c		close the mq\n");
	printf("    u		unlink the mq\n");
	printf("    a		howmany messages in the mq\n");
	printf("    h		print this help message\n");
	printf("    q		quit this program\n");
}
void main(int argc, char *argv[]){
	mqd_t mq;
	mqd_t rmq;
	struct mq_attr attr;
	attr.mq_msgsize=MQ_MSGSIZE;
	attr.mq_maxmsg=MQ_MSGCOUNT;

	int mode=1; // by default initialize

	if (argc > 1  && strcmp(argv[1],"-n")==0){
		printf("Setting uninitialized mode for opening\n");
		mode=0;
	}

	printf("My pid is %d\n",getpid());
	printf("Using MQ_NAME for operations is = %s\n",MQ_NAME);

   while(1) {
	char options[10];
	scanf("%s",options);

	if (strcmp(options,"q")==0)
		exit(0);

	if (strcmp(options,"h")==0){
		print_help(argv);
		continue;
	}

	if (strcmp(options,"o")==0){
		// create the new shm assuming it doesnt exist
		// if it exists, just get an fd to it. Like open() 
		// use attribs here

		mq= mq_open(MQ_NAME, O_CREAT|O_RDWR,0644,NULL);// struct mq_attr *attr);
		if ( (int) mq < 0 ) {
			perror("Trying to open mq:");
			exit(1);
		} else 
			printf("Opened mq\n");
		continue;
	}
	
	if (strcmp(options,"r")==0){
		rmq= mq_open(MQ_NAME, O_RDONLY);
		if (rmq < 0 ) 
			perror("open for Receive failed");
		ssize_t n= mq_receive(rmq, rcv_buf, attr.mq_msgsize, NULL); //unsigned int *msg_prio);
		close(rmq);
		if ( n < 0 )
			perror("Failed in Receive");
		else 
			printf("Current Message is %d bytes long = %s\n",(int)n,rcv_buf);
		continue;
	}

	if (strcmp(options,"s")==0){
		rmq= mq_open(MQ_NAME, O_WRONLY);
		int r= mq_send(rmq, g_msg,strlen(g_msg)+1, 0 );//unsigned int msg_prio);
		close(rmq);
		if ( r != 0 )
			perror("Send problem");
		else {
			printf("Sent a Message of %d bytes \"%s\"\n",(int)strlen(g_msg)+1,g_msg);
			// update the message for next time
			g_msg[strlen(g_msg)-1]+=1;
		}
		continue;
	}
	if (strcmp(options,"a")==0){
	        // Read the number of messages in the queue
        	if ( mq_getattr(mq, &attr) != 0 )
			perror("Getting attribures");
		else {
        		printf("Number of messages in the queue: %d\n", (int)attr.mq_curmsgs);
        		printf("msgsize: %d\n", (int)attr.mq_msgsize);
        		printf("n_msgs: %d\n", (int)attr.mq_maxmsg);
		}
	}
	// to remove the mq ... 
	if (strcmp(options,"u")==0)
		if ( mq_unlink(MQ_NAME) != 0 )
			perror("unlink failed");
		else
			printf("unlinked\n");
	
	// to close the mq ...
	if (strcmp(options,"c")==0)
		if ( mq_close(mq) != 0 )
			perror("close failed");
		else
			printf("closed\n");
   }
}
