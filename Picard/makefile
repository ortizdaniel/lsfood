CC = gcc

all: picard clean

picard: main.o util.o menu.o network.o
	$(CC) $^ -o $@ -Wall -Wextra -g -lpthread

%.o: %.c %.h
	$(CC) $< -c -Wall -Wextra -g

clean: 
	rm *.o