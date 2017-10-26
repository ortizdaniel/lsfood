#ifndef _UTIL_H
#define _UTIL_H

#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	char* nombre;
	int dinero;
	char* ip;
	int port;
} Config;

void print(int fd, const char* format, ...);

char* read_until(int fd, char stop);

int read_config(const char* file, Config* config);

void destroy_config(Config config);

int strcmpign(const char* a, const char* b);

ssize_t read_clean(int fd, char* buffer, size_t max);

#endif