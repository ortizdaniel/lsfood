#ifndef _NETWORK_H
#define _NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_TYPE sizeof(uint8_t)
#define MAX_HEADER_REAL 10
#define MAX_HEADER_FAKE 11
#define MAX_LENGTH sizeof(uint16_t)

typedef struct {
	uint8_t type;
	char header[MAX_HEADER_FAKE];
	uint16_t length;
	char* data;
} Packet;

int net_connect(char* ip, int port);

int is_connected();

void net_disconnect();

size_t send_packet(int sfd, Packet p);

int create_packet(Packet* p, int data_length);

size_t read_packet(int sfd, Packet* p);

#endif