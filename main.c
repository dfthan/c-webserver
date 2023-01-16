#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 3002
#define BUFFER_SIZE 9999 

int main(void)
{
	char response_file[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>BLAZINGLY fast webserver in C</html>\r\n";
	// socket - bind - listen - connect 
	int server_socket = socket(AF_INET, SOCK_STREAM, 0); 
	if (server_socket == -1) {
		perror("socket broke");
		return 1;
	}

	// bind socket to address
	struct sockaddr_in server_address;
	int server_addresslen = sizeof(server_address);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_socket, (struct sockaddr*)&server_address, server_addresslen) != 0){

		perror("binding broke");
		return 1;
	}

		
	if (listen(server_socket, SOMAXCONN) != 0) {
		perror("listen broke");
		return 1;
	}

	// TODO nonblocking socket? 
	while (1) {
		int new_socket = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t *)&server_addresslen);


		if (new_socket < 0) {
			perror("accept broke");
			continue;
		}


		char buffer[BUFFER_SIZE];
		int value_read = read(new_socket, buffer, BUFFER_SIZE);
		if (value_read < 0) {
			perror("read broke");
			continue;
		}

		// print client ip, http version, path
		char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
		sscanf(buffer, "%s %s %s", method, uri, version);
		printf("%s:%u %s %s %s\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), method, version, uri);

		int value_write = write(new_socket, response_file, strlen(response_file));
		if (value_write < 0) {
			perror("write broke");
			continue;
		}

		close(new_socket);

	}
	return 0;
}
