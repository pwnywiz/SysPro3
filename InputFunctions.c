#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "InputFunctions.h"

//  Pick a str string, seperate it into tokens
//  find the corresponding command and write it to the socket
int FileExecute(int fd, char *str) {
  const char s[2] = " ";
  char temp[2048];
  char token_array[40][400];
  char *token;
  int i;
  int balance_array[40];
  int array_counter;
  int result;
  int delay;
  int amount;

  strcpy(temp,str);
  temp[strcspn(temp, "\n")] = 0;

  token = strtok(str, s);

  if (strcmp("add_account", token) == 0) {
    write_command(fd,temp);
    read(fd,&result,4);

    if (result == 1) {
      read_command(fd,temp);
      read(fd,&amount,4);
      printf("Success.  Account creation (%s:%d)\n", temp, amount);
    }
    else if (result == 2){
      read(fd,&delay,4);
      read_command(fd,temp);
      read(fd,&amount,4);
      printf("Success.  Account creation (%s:%d[:%d])\n", temp, amount, delay);
    }
    else if (result == -1){
      read_command(fd,temp);
      read(fd,&amount,4);
      printf("Error.    Account creation failed (%s:%d)\n", temp, amount);
    }
    else {
      read(fd,&delay,4);
      read_command(fd,temp);
      read(fd,&amount,4);
      printf("Error.    Account creation failed (%s:%d[:%d])\n", temp, amount, delay);
    }
  }
  else if (strcmp("add_transfer", token) == 0) {
    write_command(fd,temp);
    read(fd,&result,4);

    if (result == 1) {
      read_command(fd,token_array[0]);
      read_command(fd,token_array[1]);
      read(fd,&amount,4);
      printf("Success.  Transfer addition (%s:%s:%d)\n", token_array[0], token_array[1], amount);
    }
    else if (result == 2){
      read(fd,&delay,4);
      read_command(fd,token_array[0]);
      read_command(fd,token_array[1]);
      read(fd,&amount,4);
      printf("Success.  Transfer addition (%s:%s:%d[:%d])\n", token_array[0], token_array[1], amount, delay);
    }
    else if (result == -1){
      read_command(fd,token_array[0]);
      read_command(fd,token_array[1]);
      read(fd,&amount,4);
      printf("Error.    Transfer addition failed (%s:%s:%d)\n", token_array[0], token_array[1], amount);
    }
    else {
      read(fd,&delay,4);
      read_command(fd,token_array[0]);
      read_command(fd,token_array[1]);
      read(fd,&amount,4);
      printf("Error.    Transfer addition failed (%s:%s:%d[:%d])\n", token_array[0], token_array[1], amount, delay);
    }
  }
  else if (strcmp("add_multi_transfer", token) == 0) {
    write_command(fd,temp);
    read(fd,&result,4);

    if (result == 1) {
      read_command(fd, token_array[0]);
      read(fd, &amount, 4);
      printf("Success.  Multi-Transfer addition (%s:%d)\n", token_array[0], amount);
    }
    else if (result == 2){
      read(fd,&delay,4);
      read_command(fd, token_array[0]);
      read(fd, &amount, 4);
      printf("Success.  Multi-Transfer addition (%s:%d[:%d])\n", token_array[0], amount, delay);
    }
    else if (result == -1){
      read_command(fd,token_array[0]);
      read(fd,&amount,4);
      printf("Error.    Multi-Transfer addition failed (%s:%d)\n", token_array[0], amount);
    }
    else {
      read(fd,&delay,4);
      read_command(fd,token_array[0]);
      read(fd,&amount,4);
      printf("Error.    Multi-Transfer addition failed (%s:%d[:%d])\n", token_array[0], amount, delay);
    }
  }
  else if (strcmp("print_balance", token) == 0) {
    write_command(fd,temp);
    read(fd,&result,4);

    if (result == 1) {
      read_command(fd, token_array[0]);
      read(fd, &amount, 4);
      printf("Success.  Balance (%s:%d)\n", temp, amount);
    }
    else {
      read_command(fd,token_array[0]);
      printf("Error.    Balance (%s)\n", token_array[0]);
    }
  }
  else if (strcmp("print_multi_balance", token) == 0) {
    write_command(fd,temp);
    read(fd,&result,4);

    if (result == 1) {
      read(fd, &array_counter, 4);
      for (i = 0; i < array_counter; i++) {
        read_command(fd, token_array[i]);
        read(fd, &balance_array[i], 4);
      }
      printf("Success.  Multi-Balance (");
      for (i = 0; i < array_counter; i++) {
        printf("%s/%d:", token_array[i], balance_array[i]);
      }
      printf(")\n");
    }
    else {
      read(fd, &array_counter, 4);
      for (i = 0; i < array_counter; i++) {
        read_command(fd, token_array[i]);
      }
      printf("Error.    Multi-Balance (");
      for (i = 0; i < array_counter; i++) {
        printf("%s:", token_array[i]);
      }
      printf(")\n");
    }
  }
  else if (strcmp("sleep", token) == 0) {
    token = strtok(NULL, s);
    usleep(atoi(token)*1000);
  }
  else if (strcmp("exit\n", str) == 0) {
    printf("Exiting\n");
    strcpy(temp, "exit aaaa");
    write_command(fd,temp);

    return 1;
  }
  else {
    printf("failure: passed wrong command\n");
  }

  return 0;
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

	if ( read (fd, &temp, 4) < 0 ) {/* Get length of string */
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
