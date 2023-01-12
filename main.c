#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 3001

int main(void)
{
	// socket - bind - listen - connect 
	int sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (sock == -1) {
		perror("socket broke");
		return 1;
	}
	printf("socket works");

	// bind socket to address
	struct sockaddr_in HostAddress;
	HostAddress.sin_family = AF_INET;
	HostAddress.sin_port = htons(PORT);
	HostAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&HostAddress, sizeof(HostAddress)) != 0){

		perror("binding broke");
		return 1;
	}
	printf("bind success");

	// TODO listen to connection
		
}
