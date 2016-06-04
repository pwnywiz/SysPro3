#Makefile
OBJS1 = bankserver.o ClientQueue.o HashTable.o AccountList.o BaseFunctions.o
OBJS2 = bankclient.o InputFunctions.o
SOURCE1 = bankserver.c ClientQueue.c HashTable.c AccountList.c BaseFunctions.c
SOURCE2 = bankclient.c InputFunctions.c
HEADER = InputFunctions.h ClientQueue.h HashTable.h AccountList.h BaseFunctions.h
COMPHEADER = InputFunctions.h.gch ClientQueue.h.gch HashTable.h.gch AccountList.h.gch BaseFunctions.h.gch
OUT1 = bankserver
OUT2 = bankclient
CC = gcc
FLAGS = -g -c
THREAD = -lpthread

# -g option enables debugging mode
# -c flag generates object code for separate files
all: $(OBJS1) $(OBJS2)
	gcc -o $(OUT1) $(SOURCE1) $(THREAD)
	gcc -o $(OUT2) $(SOURCE2)

# create / compile the individual files separately
bankserver.o: bankserver.c
	$(CC) $(FLAGS) bankserver.c

bankclient.o: bankclient.c
	$(CC) $(FLAGS) bankclient.c

InputFunctions.o: InputFunctions.c
	$(CC) $(FLAGS) InputFunctions.c

ClientQueue.o: ClientQueue.c
	$(CC) $(FLAGS) ClientQueue.c

HashTable.o: HashTable.c
	$(CC) $(FLAGS) HashTable.c

AccountList.o: AccountList.c
	$(CC) $(FLAGS) AccountList.c

BaseFunctions.o: BaseFunctions.c
	$(CC) $(FLAGS) BaseFunctions.c

# clean
clean:
	rm -f $(OBJS1) $(OBJS2) $(OUT1) $(OUT2) $(COMPHEADER)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
