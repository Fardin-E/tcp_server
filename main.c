#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

static volatile sig_atomic_t running = 1;

void intHandler(int err) {
	running = 0;
}

int main() {
	int sfd, cfd, result;
	socklen_t client_addr_size;
	struct sockaddr_in server_addr, client_addr;
	char buffer[1024] = {0};

	// Set up signal handler for graceful shutdown
	signal(SIGINT, intHandler);

	// Create socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		handle_error("socket");
	}	

	// Set up server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

	// Bind socket
	result = bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (result == -1) {
		handle_error("bind");
	}
	
	// Listen for connections
	result = listen(sfd, 10);
	if (result == -1) {
		handle_error("listen");
	}
	
	printf("Server listening on port 8000...\n");

	// Accept client connection
	client_addr_size = sizeof(client_addr);
	cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
	if (cfd == -1) {
		handle_error("accept");
	}

	// Send initial message
	write(cfd, "Hello\n", 6);

	// Main communication loop
	while(running) {
		// Clear buffer before reading
		memset(buffer, 0, sizeof(buffer));

		// Read from client
		ssize_t bytes_read = read(cfd, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0) {
			if (bytes_read == 0) {
				printf("Client disconnected\n");
			} else {
				perror("read");
			}
			break;
		}

		// Print received message
		printf("client: %s\n", buffer);
	}

	// Cleanup
	close(cfd);
	close(sfd);

	return 0;
}