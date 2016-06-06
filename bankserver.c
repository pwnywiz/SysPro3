#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#include "BaseFunctions.h"

int main(int argc, char *argv[]) {
  if (argc != 7) {
    printf("Not enough arguments\n");
    exit(1);
  }

  int i;
  int port;
  int new_client;
  int server_socket;
  int threadpool_size;
  pthread_t *consumers;
  struct sockaddr_in server, client;
  socklen_t clientlen = sizeof(client);
  struct sockaddr *serverptr=(struct sockaddr *)&server;
  struct sockaddr *clientptr=(struct sockaddr *)&client;

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
  client_queue = NULL;
  queue_count = 0;

  hash = hash_create(HASH_SIZE);

  //  Create a thread array of size threadpool_size
  //  and a mutex array with 1 mutex per 10 hashtable buckets
  consumers = malloc(threadpool_size*sizeof(pthread_t));
  transaction_mtx = malloc((HASH_SIZE/MUTEX_SPLIT)*sizeof(pthread_mutex_t));

  //  Initialization of mutexes and condition variables
  for (i = 0; i < HASH_SIZE/MUTEX_SPLIT; i++) {
    pthread_mutex_init(&transaction_mtx[i], 0);
  }
  pthread_mutex_init(&mtx, 0);
	pthread_cond_init(&queue_nonempty, 0);
	pthread_cond_init(&queue_nonfull, 0);

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

  for (i = 0; i < threadpool_size; i++) {
    pthread_create(&consumers[i], 0, serve_client, 0);
  }

  //  Master Thread accepts connections and adds them to the queue
  while (1) {
    if ((new_client = accept(server_socket, clientptr, &clientlen)) < 0) {
      perror_exit("accept");
    }
    pthread_mutex_lock(&mtx);
    while (queue_count >= queue_size) {
      pthread_cond_wait(&queue_nonfull, &mtx);
    }
    add_qnode(&client_queue,new_client);
    queue_count++;
    pthread_cond_broadcast(&queue_nonempty);
    pthread_mutex_unlock(&mtx);
  }
}
