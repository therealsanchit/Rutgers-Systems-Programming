	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <pthread.h>
	
	#define PORT "9879"
	#define BACKLOG 5
	#define MAX_THREADS 5
	/*
		This program runs on a separate iLab machine other than that which the client code runs on.
		
		Listens for client requests from the client socket and handles those requests. The port
		must be greater than 8k(2^13) but less than 64k(2^16). It can be hard-coded.
		
		Creates threads to run requests and passes the client socket the result of those requests... the responses 
		can be: success + data or an error code and error values
		
		Should assign each client a unique file descriptor to tell them apart
		
		Only needs to support the flags: O_RDONLY, O_WRONLY, and O_RDWR... all other flags should be an error
		
	*/
	
	/*
	struct node{
		char *pathname;
		char *client_sockaddr;
		int flags;
		int fd;
		struct node *next;
	};
	
	node *head = NULL;
	*/
	
	struct serverHandler {
		int socketfd;
	};
	
	int serverHandler(void *arg) {
		//int *connection_id = (int *) arg;
		//printf("conn id: %d\n", *(connection_id));
	
	//	printf("HElLO");
		//decode the message that gets sent to the sevrer
		int fd = 0; //this might change but I'm gonna play around with it a bit	
		if(fd == 1){} //this is only there to temporarily get rid of the compilation error
		char** messages;
		char* message = (char*) arg;

		messages = (char**) malloc(sizeof(char*) *3);
		messages[0] = strtok(message, "&");
		messages[1] = strtok(NULL, "&");
		messages[2] = strtok(NULL, "&");
		

		if(strcmp(messages[0], "0") == 0){
			//call a function to simlate netOpen
		}else if(strcmp(messages[0], "R") == 0){
			//call a function to simulate netRead
		}else if(strcmp(messages[0], "W") == 0){
			//call a function to simulate netWrite
		}else if(strcmp(messages[0], "C") == 0){
			//call a function to simulate netWrite
		}else{
			printf("invalid request\n");
			return -1;
		}
		return 0;
	}
	
	
	
	int main(int argc, char **argv){
		
		// create a socket for the server
		int s;
		int server_socket;
		//server_socket = socket(AF_INET, SOCK_STREAM, 0);
		
	/*	if(server_socket < 0){
		perror("problem creating socket!");
		return -1;
		}
	*/	
		struct addrinfo hints, *res, *rp;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		
		s = getaddrinfo(NULL, PORT, &hints, &res);
		if(s != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	 }
		
		for(rp = res; rp != NULL; rp = rp->ai_next){
		
		server_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(server_socket < 0){
		fprintf(stderr,"problem creating socket!\n");
		continue;
		}
		
		if(bind(server_socket, res->ai_addr, res->ai_addrlen) != 0){
		perror("bind issue\n");
		continue;
		}
		
		break;	
		}
		
		if(rp == NULL){
		fprintf(stderr, "failed to connect\n");
		}
		
		freeaddrinfo(res);
		
		if(listen(server_socket, BACKLOG != 0)){
		fprintf(stderr,"listening problem!\n");
		return -1;
		} 
		
		printf("listening...\n");
		
		int connection_id;
		struct sockaddr_storage client_addr;
		socklen_t client_addr_len = sizeof(struct sockaddr_storage);
		pthread_t worker;
		
	/*	connection_id = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if(connection_id == -1){
		fprintf(stderr, "the connection cannot be made!\n");
		return -1;
		}
	*/	
		int limit;
		for(limit = 0; limit < MAX_THREADS; limit++){
		
		connection_id = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
		if(connection_id == -1){
		fprintf(stderr, "the connection cannot be made!\n");
		return -1;
		}
		//struct serverHandler tempfd = malloc(sizeof(struct serverHandler));
		//tempfd->socket = connection_id;
		if(pthread_create(&worker, NULL, (void*) serverHandler, &connection_id) != 0){
		fprintf(stderr, "could not create thread!\n");
		return -1;
		}
		printf("limit: %d\n", limit); 
		}
		
		close(server_socket);
	
		
		
	
	
	
	
	
	/*	
		
		if(listen(server_socket, 10) != 0){
		perror("listening...");
		exit(1);
		}
		
		struct sockaddr_in *result_addr = (struct sockaddr_in *) res->ai_addr;
	 printf("Listening on file descriptor %d, port %d\n", server_socket, ntohs(result_addr->sin_port));
	 
	 printf("Waiting for connection...\n");
	 
	 
	 int client_fd = accept(server_socket, NULL, NULL);
	 printf("Connection made: client_fd=%d\n", client_fd);
	 
	 char buf[1000];
	 int len = read(client_fd, buf, sizeof(buf) - 1);
	 buf[len] = '\0';
	 
	 printf("read %d chars\n", len);
	 printf("===\n");
	 printf("%s\n", buf);
	 
		*/
	
	
		return 0;
	}
