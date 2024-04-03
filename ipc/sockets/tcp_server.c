#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commons.h" 

int main(int argc, char * argv[]){
	// declare socket vars
	int l_sock, conn_sock;
	int r;
	struct sockaddr_in local, remote;
	int remote_size=sizeof(remote);
	char str[100];
	char suffix[100]="Sever message  ";


	// get an internet socket
	l_sock=socket(AF_INET, SOCK_STREAM , 0);
	if (l_sock < 0 ){
		perror("socket():");
		exit(1);
	}

	// define a structure with a local address and 
	// parameters for server port
	local.sin_family = AF_INET;
    	local.sin_addr.s_addr = htonl(INADDR_ANY);
    	local.sin_port = htons( SRV_PORT );

	// associate the server socket to the address and port above
	r = bind(l_sock,(struct sockaddr *)&local, sizeof(local));
	if (r < 0 ){
		perror("bind():");
		exit(1);
	}

	// set the socket to listening for an incoming connection request
	r = listen(l_sock,2);
	if (r < 0 ){
		perror("listen():");
		exit(1);
	}

	//     The below part repeats for each new connection
	int j=0;
	int i=0;
	while(1){
		// wait for a connection ... get a new "full" sock
		conn_sock=accept(l_sock,(struct sockaddr *)&remote,&remote_size);
		if (conn_sock < 0 ){
			perror("accept():");
			exit(1);
		}
		// send
		sprintf(str,"%s %u %d\n",suffix,getpid(),i++);
		r = send(conn_sock,str,strlen(str),0);
		if (r < 0 ){
			perror("send():");
			exit(1);
		}
		// receive
		r = recv(conn_sock,str,100,0);
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		printf("Got %2d: %s\n",j,str);
		j++;
		// close the new sock
		close(conn_sock);
	}
}
