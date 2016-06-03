#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ClientQueue.h"
#include "HashTable.h"

#define HASH_SIZE 10

void child_server(int newsock);
int write_command(int fd, char* message);
int read_command (int fd, char *buffer);
void perror_exit(char *message);

int main(int argc, char *argv[]) {
  if (argc != 7) {
    printf("Not enough arguments\n");
    exit(1);
  }

  int i;
  int port;
  int queue_size;
  int server_socket;
  int threadpool_size;
  int newsock;
  struct sockaddr_in server, client;
  socklen_t clientlen = sizeof(client);
  struct sockaddr *serverptr=(struct sockaddr *)&server;
  struct sockaddr *clientptr=(struct sockaddr *)&client;
  // struct hostent *rem;

  //  Save the -p, -s and -q arguments in any order
  for (i = 1; i < argc; i++) {
    if (!strcmp("-p", argv[i])) {
      port = atoi(argv[i+1]);
    }
    if (!strcmp("-s", argv[i])) {
      threadpool_size = atoi(argv[i+1]);
    }
    if (!strcmp("-q", argv[i])) {
      queue_size = atoi(argv[i+1]);
    }
  }

  /* Create socket */
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      perror_exit("socket");
  server.sin_family = AF_INET;       /* Internet domain */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(port);      /* The given port */
  /* Bind socket to address */
  if (bind(server_socket, serverptr, sizeof(server)) < 0)
      perror_exit("bind");
  /* Listen for connections */
  if (listen(server_socket, 5) < 0) {
    perror_exit("listen");
  }

  while (1) {
      /* accept connection */
  	if ((newsock = accept(server_socket, clientptr, &clientlen)) < 0) perror_exit("accept");
  	/* Find client's address */
  	printf("Accepted connection\n");
  	switch (fork()) {    /* Create child for serving client */
  	case -1:     /* Error */
  	    perror("fork"); break;
  	case 0:	     /* Child process */
  	    close(server_socket); child_server(newsock);
  	    exit(0);
  	}
  	close(newsock); /* parent closes socket to client */
  }
}

void child_server(int newsock) {
  char buf[2048];
  // read(newsock, size, 4);
  // while(read(newsock, buf, 1) > 0) {  /* Receive 1 char */
  // 	putchar(buf[0]);           /* Print received char */
  // 	/* Capitalize character */
  // 	buf[0] = toupper(buf[0]);
  // 	/* Reply */
  // 	if (write(newsock, buf, 1) < 0)
  // 	    perror_exit("write");
  // }
  read_command(newsock,buf);

  printf("Closing connection.\n");
  close(newsock);	  /* Close socket */
}

int write_command(int fd, char* message) {/* Write formated commands */
  char temp;
  int length = 0;

	length = strlen(message) + 1;	/* Find length of string */
	temp = length;
	if(write (fd, &temp, 1) < 0) {	/* Send length first */
		exit (-2);
  }
	if(write (fd, message, length) < 0) {	/* Send string */
		exit (-2);
  }

	return length;		/* Return size of string */
}

int read_command (int fd, char *buffer) {/* Read formated commands */
	char temp;
  int i = 0;
  int length = 0;

	if ( read ( fd, &temp, 1 ) < 0 )	{/* Get length of string */
		exit (-3);
  }
	length = temp;
	while ( i < length ) { /* Read $length chars */
		if ( i < ( i+= read (fd, &buffer[i], length - i))) {
			exit (-3);
    }
  }

	return i;	/* Return size of string */
}

void perror_exit(char *message) {
  perror(message);
  exit(EXIT_FAILURE);
}
