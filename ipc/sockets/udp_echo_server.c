#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "commons.h"
#define BUFFSIZE 100
	
// Driver code
int main() {
	int sockfd;
	char rcvstr[BUFFSIZE];
	char sendstr[500] = "Server Message\n";
	struct sockaddr_in cli_add, myadd;  // rcv cli_add, fill myadd for bind
		
	// Creating socket file descriptor
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0 ) {
		perror("socket()");
		exit(1);
	}

	fillzero(cli_add);
	fillzero(myadd);
		
	myadd.sin_family = AF_INET;
	myadd.sin_addr.s_addr = INADDR_ANY;
	myadd.sin_port = htons(SRV_PORT);
		
	int r = bind(sockfd, (struct sockaddr *)&myadd,
			sizeof(myadd));
	if( r < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	
	int len  = sizeof(myadd);
	
	while(1){
		printf("Receiving from client...\n");
		r = recvfrom(sockfd, (char *)rcvstr, BUFFSIZE-1,
				MSG_WAITALL, ( struct sockaddr *) &cli_add,
				&len); // get rcvstr, cli_add, read/write: len
		if ( r < 0 ) {
			perror("recvfrom()");
			exit(1);
		}
		printf("....Received\n");
		rcvstr[r] = '\0';
		printf("Message got from client: %s\n", rcvstr);
		strcpy(sendstr,rcvstr);
		printf("Sending to client...\n");
		r = sendto(sockfd, (char *)sendstr, strlen(sendstr),
			MSG_CONFIRM, (const struct sockaddr *) &cli_add,
			len); // use cli_add, len
		if ( r < 0 ) {
			perror("sendto()");
			exit(1);
		}
		printf("...Done!\n");
	}
		
	return 0;
}

