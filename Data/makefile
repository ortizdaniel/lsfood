CC = gcc

all: main clean

main: main.o util.o
	$(CC) $^ -o $@ -Wall -Wextra -g

%.o: %.c %.h

clean: 
	rm *.o

run:
	./main