CC = gcc

all: enterprise clean

enterprise: main.o util.o networkpic.o networkdata.o network.o lista.o
	$(CC) $^ -o $@ -Wall -Wextra -g -lpthread

%.o: %.c %.h
	$(CC) $< -c -Wall -Wextra

clean: 
	rm *.o