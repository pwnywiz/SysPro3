#Makefile
OBJS1 = bankserver.o
OBJS2 = bankclient.o InputFunctions.o
SOURCE1 = bankserver.c
SOURCE2 = bankclient.c InputFunctions.c
HEADER = InputFunctions.h
COMPHEADER = InputFunctions.h.gch
OUT1 = bankserver
OUT2 = bankclient
CC = gcc
FLAGS = -g -c

# -g option enables debugging mode
# -c flag generates object code for separate files
all: $(OBJS1) $(OBJS2)
	gcc -o $(OUT1) $(SOURCE1)
	gcc -o $(OUT2) $(SOURCE2)

# create / compile the individual files separately
bankserver.o: bankserver.c
	$(CC) $(FLAGS) bankserver.c

bankclient.o: bankclient.c
	$(CC) $(FLAGS) bankclient.c

InputFunctions.o: InputFunctions.c
	$(CC) $(FLAGS) InputFunctions.c

# clean
clean:
	rm -f $(OBJS1) $(OBJS2) $(OUT1) $(OUT2) $(COMPHEADER)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
