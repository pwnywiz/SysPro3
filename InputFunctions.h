#ifndef INPUTFUNCTIONS_H_
#define INPUTFUNCTIONS_H_

int FileExecute(int fd, char *str);
int write_command(int fd, char* message);
int read_command(int fd, char *buffer);

#endif /* INPUTFUNCTIONS_H_ */
