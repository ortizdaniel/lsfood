CC = gcc

all: data clean

data: main.o util.o network.o networkpic.o networkent.o enterpriselist.o
	$(CC) $^ -o $@ -Wall -Wextra -g -lpthread

%.o: %.c %.h
	$(CC) $< -c -Wall -Wextra

clean: 
	rm *.o