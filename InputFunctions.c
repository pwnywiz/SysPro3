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
  char *token;

  strcpy(temp,str);

  token = strtok(str, s);

  if (strcmp("add_account", token) == 0 || strcmp("add_transfer", token) == 0 || strcmp("add_multi_transfer", token) == 0
      || strcmp("print_balance", token) == 0 || strcmp("print_multi_balance", token) == 0) {
    write_command(fd,temp);

    //  ToDo Add read from the server
  }
  else if (strcmp("sleep", token) == 0) {
    token = strtok(NULL, s);
    usleep(atoi(token));
  }
  else if (strcmp("exit\n", str) == 0) {
    printf("Exiting\n");

    return 1;
  }
  else {
    printf("failure: passed wrong command\n");
  }

  return 0;
}

int write_command(int fd, char* message) {/* Write formated data */
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

int read_data (int fd, char *buffer) {/* Read formated data */
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

// int linesize(FILE *const fp) {
//   int ch;
//   int counter;
//
//   counter = 0;
//   while(1) {
//     ch = fgetc(fp);
//     if(ch == EOF || ch == '\n') {
//       break;
//     }
//     counter++;
//   }
//
//   return counter;
// }
