#include "BaseFunctions.h"

void *serve_client(void * ptr) {
  int client_fd;
  while (1) {
  	pthread_mutex_lock(&mtx);
  	while (queue_count <= 0) {
  	   pthread_cond_wait(&queue_nonempty, &mtx);
  	}
    client_fd = get_qnode(&client_queue);
  	queue_count--;
    pthread_cond_signal(&queue_nonfull);
  	pthread_mutex_unlock(&mtx);
  }
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
