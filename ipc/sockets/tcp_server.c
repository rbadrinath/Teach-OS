#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commons.h" 

int main(int argc, char * argv[]){
	// declare socket vars
	int psock, fsock;
	int r;
	struct sockaddr_in local, remote;
	int remote_size=sizeof(remote);
	char str[100];
	strcpy(str,"Server message\n");
	//
	psock=socket(AF_INET, SOCK_STREAM , 0);
	if (psock < 0 ){
		perror("socket():");
		exit(1);
	}
	//
	local.sin_family = AF_INET;
    	local.sin_addr.s_addr = INADDR_ANY;
    	local.sin_port = htons( SRV_PORT );

	r = bind(psock,(struct sockaddr *)&local, sizeof(local));
	if (r < 0 ){
		perror("bind():");
		exit(1);
	}
	r = listen(psock,2);
	if (r < 0 ){
		perror("listen():");
		exit(1);
	}
	//     The below part repeats for each new connection
	int j=0;
	while(1){
		fsock=accept(psock,(struct sockaddr *)&remote,&remote_size);
		if (fsock < 0 ){
			perror("accept():");
			exit(1);
		}
		r = send(fsock,str,strlen(str),0);
		if (r < 0 ){
			perror("send():");
			exit(1);
		}
		r = recv(fsock,str,100,0);
		if (r < 0 ){
			perror("recv():");
			exit(1);
		}
		printf("Got %2d: %s\n",j,str);
		j++;
	}
}
