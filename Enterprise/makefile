CC = gcc

all: enterprise clean

enterprise: main.o util.o networkpic.o networkdata.o network.o lista.o lista_reserva.o lista_lista.o
	$(CC) $^ -o $@ -Wall -Wextra -g -lpthread

%.o: %.c %.h
	$(CC) $< -c -Wall -Wextra

clean: 
	rm *.o