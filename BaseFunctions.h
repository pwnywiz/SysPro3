#ifndef BASEFUNCTIONS_H_
#define BASEFUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "ClientQueue.h"
#include "HashTable.h"

#define HASH_SIZE 100
#define MUTEX_SPLIT 10

int queue_size;
int queue_count;
pthread_mutex_t mtx;
pthread_mutex_t *transaction_mtx;
pthread_cond_t queue_nonempty;
pthread_cond_t queue_nonfull;
qnode *client_queue;

void *serve_client(void * ptr);
int write_command(int fd, char* message);
int read_command (int fd, char *buffer);
void perror_exit(char *message);

#endif /* BASEFUNCTIONS_H_ */
