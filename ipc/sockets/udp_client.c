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
	
int main() {
	int sockfd;
	char rcvstr[BUFFSIZE];
	char sendstr[50] = "Client Message\n";
	struct sockaddr_in srv_add, myadd;
	int len;
		
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&srv_add, 0, sizeof(srv_add));
		
	// Filling server information
	srv_add.sin_family = AF_INET;
	srv_add.sin_port = htons(SRV_PORT);
	srv_add.sin_addr.s_addr = INADDR_ANY;
		
		
	printf("Sending to server...\n");
	int n = sendto(sockfd, (const char *)sendstr, strlen(sendstr),
		MSG_CONFIRM, (const struct sockaddr *) &srv_add,
			sizeof(srv_add)); 
	if ( n < 0 ){
		perror("cli:sendto()");
		exit(1);
	}
	printf("...Done!\n");
			
	printf("Receiving from server...\n");
	n = recvfrom(sockfd, (char *)rcvstr, BUFFSIZE,
				MSG_WAITALL, (struct sockaddr *) &srv_add,
				&len);
	if ( n < 0 ){
		perror("cli:sendto()");
		exit(1);
	}
	printf("...Received!\n");

	rcvstr[n] = '\0';
	printf("Message got from server : %s\n", rcvstr);
	
	close(sockfd);
	return 0;
}
