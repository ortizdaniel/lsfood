#ifndef _ENTERPRISE_LIST_H
#define _ENTERPRISE_LIST_H

#define MAX_IP 16
#define INIT_SIZE 8

typedef struct {
	char ip[MAX_IP]; //ip es como mucho 255.255.255.255\0
	int port;
	int users_connected;
	int alive;
} Enterprise;

int enterprises_init();

Enterprise* enterprises_get();

int enterprises_size();

int enterprises_add(Enterprise e);

void enterprises_end();

#endif