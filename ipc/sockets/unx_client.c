#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "commons.h" 

int main(int argc, char * argv[]){
	// declare socket vars
	int fsock;
	struct sockaddr_un server;
	char str[100];
	char suffix[100]="Client Message from  ";
	sprintf(str,"%s %u\n",suffix, getpid());
	
	fillzero(server);
	
	fsock=socket(AF_UNIX, SOCK_STREAM , 0);
	if (fsock < 0 ){
		perror("socket():");
		exit(1);
	}

	server.sun_family = AF_UNIX;
	strcpy( server.sun_path, UNIX_SOCK_PATH );

	// connect to the server on the sock
	printf("Connecting....\n");
	int r = connect(fsock,(struct sockaddr *) &server, sizeof(server));
	if (r < 0 ){
		perror("connect():");
		exit(1);
	}
	printf("....Connected\n");

	// send
	printf("Sending....\n");
	r = send(fsock,str,strlen(str),0);
	if (r < 0 ){
		perror("send():");
		exit(1);
	}
	printf("....Sent\n");

	// receive
	printf("Receiving....\n");
	r = recv(fsock,str,100,0);
	if (r < 0 ){
		perror("recv():");
		exit(1);
	}
	printf("....Received\n");
	str[r]='\0';
	printf("Got: %s\n",str);

	// exit
	exit(0);  // automatically socket is closed from client end
}
