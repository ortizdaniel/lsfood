CC = gcc

all: main clean

main: main.o util.o networkpic.o
	$(CC) $^ -o $@ -Wall -Wextra -g -lpthread

%.o: %.c %.h
	$(CC) $< -c -Wall -Wextra

clean: 
	rm *.o

run:
	./main