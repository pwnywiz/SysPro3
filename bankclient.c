#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

void perror_exit(char *message);

int main(int argc, char *argv[]) {
  if (argc != 7) {
    printf("Not enough arguments\n");
    exit(1);
  }

  int i;
  int port;
  int server_socket;
  char str[300];
  char comm[300];
  char server_host[300];
  char buf[256];
  struct sockaddr_in server;
  struct sockaddr *serverptr = (struct sockaddr*)&server;
  struct hostent *rem;

  //  Save the -h, -p and -i arguments in any order
  for (i = 1; i < argc; i++) {
    if (!strcmp("-h", argv[i])) {
      strcpy(server_host, argv[i+1]);
    }
    if (!strcmp("-p", argv[i])) {
      port = atoi(argv[i+1]);
    }
    if (!strcmp("-i", argv[i])) {
      strcpy(comm, argv[i+1]);
    }
  }

/* Create socket */
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  	perror_exit("socket");
/* Find server address */
  if ((rem = gethostbyname(server_host)) == NULL) {
   herror("gethostbyname"); exit(1);
  }
  server.sin_family = AF_INET;       /* Internet domain */
  memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
  server.sin_port = htons(port);         /* Server port */
  /* Initiate connection */
  if (connect(server_socket, serverptr, sizeof(server)) < 0)
   perror_exit("connect");
  printf("Connecting to %s port %d\n", argv[1], port);
  do {
  	printf("Give input string: ");
  	fgets(buf, sizeof(buf), stdin);	/* Read from stdin*/
  	for(i=0; buf[i] != '\0'; i++) { /* For every char */
  	    /* Send i-th character */
      	if (write(server_socket, buf + i, 1) < 0)
      	   perror_exit("write");
          /* receive i-th character transformed */
      	if (read(server_socket, buf + i, 1) < 0)
      	    perror_exit("read");
  	}
  	printf("Received string: %s", buf);
  } while (strcmp(buf, "END\n") != 0); /* Finish on "end" */
  close(server_socket);                 /* Close socket and exit */
}

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
