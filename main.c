#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main() {
	
	int sfd, cfd, result;
	socklen_t client_addr_size;
	struct sockaddr_in server_addr, client_addr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sfd == -1) {
		handle_error("socket");
	}	

	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);
	result = bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (result == -1) {
		handle_error("bind");
	}
	
	result = listen(sfd, 10);
	if (result == -1) {
		handle_error("listen");
	}
	
	client_addr_size = sizeof(client_addr);
	cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
	if (cfd == -1) {
		handle_error("accept");
	}

	//send messages

	result = close(sfd);
	if (result == -1) {
		handle_error("close");
	}

	return 0;
}
