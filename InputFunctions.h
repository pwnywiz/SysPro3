#ifndef INPUTFUNCTIONS_H_
#define INPUTFUNCTIONS_H_

// #include "BaseFunctions.h"

int FileExecute(int fd, char *str);
int write_command(int fd, char* message);
int read_data (int fd, char *buffer);
// int linesize(FILE *const fp);

#endif /* INPUTFUNCTIONS_H_ */
