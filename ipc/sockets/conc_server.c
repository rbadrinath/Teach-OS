#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commons.h" 

void test_send_recv(int conn_sock){
	char str[100];
	char suffix[100]="Sever message ";
	// send and receive 5 messages
	int i=0;
	while(1) {
	     int r;

	     // receive
	     r = recv(conn_sock,str,100,0);
	     if (r < 0 ){
		     perror("recv():");
		     exit(1);
	     }
	     if (r == 0 ){
	     // client closed connection (EOF)
		     printf("Server %u: Client exited, connection closed\n",getpid());
		     exit(0);
	     }
	     str[r]='\0';
	     printf("Server %u recvd: %s\n",getpid(),str);

	     // send
	     sprintf(str,"%s from pid %u # %d\n",suffix,getpid(),i++);
	     r = send(conn_sock,str,strlen(str),0);
	     if (r < 0 ){
		     perror("send():");
		     exit(1);
	     }
	     sleep(1);
	}
}


int main(int argc, char * argv[]){
	// declare socket vars
	int l_sock, conn_sock;
	int r;
	struct sockaddr_in local, remote;
	int remote_size;
	char str[100];
	strcpy(str,"Server message\n");
	//
	l_sock=socket(AF_INET, SOCK_STREAM , 0);
	if (l_sock < 0 ){
		perror("socket():");
		exit(1);
	}
	//
	fillzero(local);
	local.sin_family = AF_INET;
    	local.sin_addr.s_addr = INADDR_ANY;
    	local.sin_port = htons( SRV_PORT );

	r = bind(l_sock,(struct sockaddr *)&local, sizeof(local));
	if (r < 0 ){
		perror("bind():");
		exit(1);
	}
	r = listen(l_sock,1);
	if (r < 0 ){
		perror("listen():");
		exit(1);
	}
	//     The below part repeats for each new connection
	while(1){
		fillzero(remote); remote_size = sizeof(remote);
		conn_sock=accept(l_sock,(struct sockaddr *)&remote,&remote_size);
		if (conn_sock < 0 ){
			perror("accept():");
			exit(1);
		}
		if ( (r = fork()) < 0 ){
			perror("fork():");
			exit(1);
		} else if ( r == 0 ) {
			// if child then do the send_recv activity
			close(l_sock);
			test_send_recv(conn_sock);
			exit(1);
		} else  // if parent then simply close the socket and go for next client
			close(conn_sock);
		sleep(1);
	}
}

