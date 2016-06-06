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

    while (1) {
      if (TransactionsExecute(client_fd, hash) == 1) {
        break;
      }
    }
  }
}

int TransactionsExecute(int fd, Hashtable *ht) {
  const char s[2] = " ";
  char command[2048];
  char token_array[40][400];
  char *token;
  char *tempchar;
  char *tempname1;
  char *tempname2;
  int tables[HASH_SIZE/MUTEX_SPLIT];
  int balance_array[40];
  int i;
  int amount;
  int balance;
  int delay = 0;
  int counter = 0;
  int array_counter = 0;
  int delay_flag = 0;
  int wrong_command = -1;
  int wrong_commandDelay = -2;
  int success_command = 1;
  int success_commandDelay = 2;

  read_command(fd, command);
  token = strtok_r(command, s, &tempchar);

  if (strcmp("add_account", token) == 0) {
    token = strtok_r(NULL, s, &tempchar);
    amount = atoi(token);

    token = strtok_r(NULL, s, &tempchar);
    tempname1 = token;

    token = strtok_r(NULL, s, &tempchar);
    if (token != NULL) {
      if ((delay = atoi(token)) > 0) {
        delay_flag = 1;
        usleep(delay*1000);
      }
    }

    if (hash_search(ht, tempname1)) {
      if (delay_flag) {
        write (fd, &wrong_commandDelay, 4);
        write (fd, &delay, 4);
      }
      else {
        write (fd, &wrong_command, 4);
      }
      write_command(fd, tempname1);
      write(fd, &amount, 4);
      return 0;
    }

    tables[0] = hash_fillBucket(HASH_SIZE, tempname1);
    pthread_mutex_lock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    hash_insert(ht, amount, tempname1);
    pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);

    if (delay_flag) {
      write(fd, &success_commandDelay, 4);
      write(fd, &delay, 4);
    }
    else {
      write(fd, &success_command, 4);
    }
    write_command(fd, tempname1);
    write(fd, &amount, 4);
  }
  else if (strcmp("add_transfer", token) == 0) {
    token = strtok_r(NULL, s, &tempchar);
    amount = atoi(token);

    token = strtok_r(NULL, s, &tempchar);
    tempname1 = token;

    token = strtok_r(NULL, s, &tempchar);
    tempname2 = token;

    token = strtok_r(NULL, s, &tempchar);
    if (token != NULL) {
      if ((delay = atoi(token)) > 0) {
        delay_flag = 1;
        usleep(delay*1000);
      }
    }

    if (hash_search(ht, tempname1) == 0 || hash_search(ht, tempname2) == 0) {
      if (delay_flag) {
        write (fd, &wrong_commandDelay, 4);
        write (fd, &delay, 4);
      }
      else {
        write (fd, &wrong_command, 4);
      }
      write_command(fd, tempname1);
      write_command(fd, tempname2);
      write(fd, &amount, 4);
      return 0;
    }

    tables[0] = hash_fillBucket(HASH_SIZE, tempname1);
    tables[1] = hash_fillBucket(HASH_SIZE, tempname2);
    if (tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT)) == tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))) {
      pthread_mutex_lock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    }
    else {
      pthread_mutex_lock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
      pthread_mutex_lock(&transaction_mtx[tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    }
    if (hash_getBalance(ht, tempname1) < amount) {
      if (delay_flag) {
        write (fd, &wrong_commandDelay, 4);
        write (fd, &delay, 4);
      }
      else {
        write (fd, &wrong_command, 4);
      }
      write_command(fd, tempname1);
      write_command(fd, tempname2);
      write(fd, &amount, 4);
      if (tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT)) == tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))) {
        pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
      }
      else {
        pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
        pthread_mutex_unlock(&transaction_mtx[tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
      }
      return 0;
    }
    hash_transfer(ht, amount, tempname2, tempname1);
    if (tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT)) == tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))) {
      pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    }
    else {
      pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
      pthread_mutex_unlock(&transaction_mtx[tables[1]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    }

    if (delay_flag) {
      write(fd, &success_commandDelay, 4);
      write(fd, &delay, 4);
    }
    else {
      write(fd, &success_command, 4);
    }
    write_command(fd, tempname1);
    write_command(fd, tempname2);
    write(fd, &amount, 4);
  }
  else if (strcmp("add_multi_transfer", token) == 0) {
    token = strtok_r(NULL, s, &tempchar);
    amount = atoi(token);

    token = strtok_r(NULL, s, &tempchar);
    while(token != NULL) {
      strcpy(token_array[array_counter],token);
      array_counter++;
      token = strtok_r(NULL, s, &tempchar);
    }
    if ((delay = atoi(token_array[array_counter-1])) > 0) {
      delay_flag = 1;
      array_counter--;
      usleep(delay*1000);
    }

    for (i = 0; i < array_counter; i++) {
      if (hash_search(ht, token_array[i]) == 0) {
        if (delay_flag) {
          write (fd, &wrong_commandDelay, 4);
          write (fd, &delay, 4);
        }
        else {
          write (fd, &wrong_command, 4);
        }
        write_command(fd, token_array[0]);
        write(fd, &amount, 4);
        return 0;
      }
    }

    //  Find all the mutexes that need to be locked and save them
    //  in increasing order, preventing any possible deadlock or conflict
    for (i = 0; i < array_counter; i++) {
      if (insert_sorted(tables, counter, hash_fillBucket(HASH_SIZE, token_array[i])/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT)))) {
        counter++;
      }
    }
    for (i = 0; i < counter; i++) {
      pthread_mutex_lock(&transaction_mtx[tables[i]]);
    }
    if (hash_getBalance(ht, token_array[0]) < amount*(array_counter-1)) {
      if (delay_flag) {
        write (fd, &wrong_commandDelay, 4);
        write (fd, &delay, 4);
      }
      else {
        write (fd, &wrong_command, 4);
      }
      write_command(fd, token_array[0]);
      write(fd, &amount, 4);
      for (i = 0; i < counter; i++) {
        pthread_mutex_unlock(&transaction_mtx[tables[i]]);
      }
      return 0;
    }
    for (i = 1; i < array_counter; i++) {
      hash_transfer(ht, amount, token_array[i], token_array[0]);
    }
    for (i = 0; i < counter; i++) {
      pthread_mutex_unlock(&transaction_mtx[tables[i]]);
    }

    if (delay_flag) {
      write (fd, &success_commandDelay, 4);
      write (fd, &delay, 4);
    }
    else {
      write (fd, &success_command, 4);
    }
    write_command (fd, token_array[0]);
    write (fd, &amount, 4);
  }
  else if (strcmp("print_balance", token) == 0) {
    token = strtok_r(NULL, s, &tempchar);
    tempname1 = token;

    if (hash_search(ht, tempname1) == 0) {
      write (fd, &wrong_command, 4);
      write_command(fd, tempname1);
      return 0;
    }

    tables[0] = hash_fillBucket(HASH_SIZE, tempname1);
    pthread_mutex_lock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);
    balance = hash_getBalance(ht, tempname1);
    pthread_mutex_unlock(&transaction_mtx[tables[0]/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT))]);

    write(fd, &success_command, 4);
    write_command(fd, tempname1);
    write(fd, &balance, 4);
  }
  else if (strcmp("print_multi_balance", token) == 0) {
    token = strtok_r(NULL, s, &tempchar);
    while(token != NULL) {
      strcpy(token_array[array_counter],token);
      array_counter++;
      token = strtok_r(NULL, s, &tempchar);
    }

    for (i = 0; i < array_counter; i++) {
      if (hash_search(ht, token_array[i]) == 0) {
        write (fd, &wrong_command, 4);
        write(fd, &array_counter, 4);
        for (i = 0; i < array_counter; i++) {
          write_command(fd, token_array[i]);
        }
        return 0;
      }
    }

    //  Find all the mutexes that need to be locked and save them
    //  in increasing order, preventing any possible deadlock or conflict
    for (i = 0; i < array_counter; i++) {
      if (insert_sorted(tables, counter, hash_fillBucket(HASH_SIZE, token_array[i])/(HASH_SIZE/(HASH_SIZE/MUTEX_SPLIT)))) {
        counter++;
      }
    }

    for (i = 0; i < counter; i++) {
      pthread_mutex_lock(&transaction_mtx[tables[i]]);
    }
    for (i = 0; i < array_counter; i++) {
      balance_array[i] = hash_getBalance(ht, token_array[i]);
    }
    for (i = 0; i < counter; i++) {
      pthread_mutex_unlock(&transaction_mtx[tables[i]]);
    }

    write(fd, &success_command, 4);
    write(fd, &array_counter, 4);
    for (i = 0; i < array_counter; i++) {
      write_command(fd, token_array[i]);
      write(fd, &balance_array[i], 4);
    }
  }
  else if (strcmp("exit", token) == 0) {
    return 1;
  }

  return 0;
}

//  Insert a unique value sorted into a given array
int insert_sorted(int *arr, int size, int value) {
  int i;
  int j;

  for (i = 0; i < size; i++) {
    if (value == arr[i]) {
      return 0;
    }
    if (value < arr[i]) {
      for (j = size; j > i; j--) {
        arr[j] = arr[j-1];
      }
      arr[i] = value;
      return 1;
    }
  }
  arr[size] = value;

  return 1;
}

int write_command(int fd, char* message) {/* Write formated commands */
  int temp;
  int length = 0;

	length = strlen(message) + 1;	/* Find length of string */
	temp = length;
	if(write (fd, &temp, 4) < 0) {	/* Send length first */
		exit (-2);
  }
	if(write (fd, message, length) < 0) {	/* Send string */
		exit (-2);
  }

	return length;		/* Return size of string */
}

int read_command(int fd, char *buffer) {/* Read formated commands */
	int temp;
  int i = 0;
  int length = 0;

	if (read (fd, &temp, 4) < 0) {/* Get length of string */
		exit (-3);
  }
	length = temp;
	while (i < length) { /* Read $length chars */
		if (i < (i+= read (fd, &buffer[i], length - i))) {
			exit (-3);
    }
  }

	return i;	/* Return size of string */
}

void perror_exit(char *message) {
  perror(message);
  exit(EXIT_FAILURE);
}
