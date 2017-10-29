#include "util.h"

void print(int fd, const char* format, ...) {
	va_list list;
	va_start(list, format);
	char buff[128];
	char *s;
	double f;
	int i;
	int len = strlen(format);
	int num;
	char c;

	for (i = 0; i < len; i++) {
		c = format[i];
		if (c != '%') {
			write(fd, &c, sizeof(char));
		} else {
			c = format[++i];
			switch (c) {
				case 'd':
					num = va_arg(list, int);
					sprintf(buff, "%d", num);
					write(fd, buff, strlen(buff));
					break;
				case 'f':
					f = va_arg(list, double);
					sprintf(buff, "%f", f);
					write(fd, buff, strlen(buff));
					break;
				case 'c':
					num = va_arg(list, int);
					write(fd, &c, sizeof(char));
					break;
				case 's':
					s = va_arg(list, char*);
					write(fd, s, strlen(s));
					break;
			}
		}
	}
	va_end(list);
}

char* read_until(int fd, char stop) {
	char* ptr = (char *) malloc(sizeof(char));
	if (ptr == NULL) return NULL;

	int count = 0;
	while (read(fd, ptr + count, sizeof(char))) {
		if (ptr[count] == stop) {
			ptr[count] = '\0';
			break;
		}
		char* tmp = (char *) realloc(ptr, sizeof(char) * (++count));
		if (tmp == NULL) {
			free(ptr);
			return NULL;
		} else {
			ptr = tmp;
		}
		//if (!buffer) return 0;
	}
	return ptr;
}

int read_config(const char* file, Config* config) {
	int fd = open(file, O_RDONLY);
	if (fd < 0) return 1;

	config->nombre = read_until(fd, '\n');
	if (config->nombre == NULL) {
		close(fd);
		return 2;
	}

	char* buff = read_until(fd, '\n');
	if (buff == NULL) {
		free(config->nombre);
		close(fd);
		return 4;
	}
	config->dinero = atoi(buff);
	free(buff);
	
	config->ip = read_until(fd, '\n');
	if (config->ip == NULL) {
		free(config->nombre);
		close(fd);
		return 6;
	}
	
	buff = read_until(fd, '\n');
	if (buff == NULL) {
		free(config->nombre);
		free(config->ip);
		close(fd);
		return 8;
	}
	config->port = atoi(buff);
	
	free(buff);
	close(fd);
	return 0;
}

void destroy_config(Config config) {
	free(config.nombre);
	free(config.ip);
}

int strcmpign(const char* a, const char* b) {
	int i;
	int len = strlen(a);
	if (len != strlen(b)) return -1;
	
	char c, d;
	for (i = 0; i < len; i++) {
		c = toupper(a[i]);
		d = toupper(b[i]);
		if (c != d) {
			return -1;
		}
	}
	return 0;
}

ssize_t read_clean(int fd, char* buffer, size_t max) {
	ssize_t bytes = read(fd, buffer, max);
	buffer[bytes - 1] = '\0';
	return bytes;
}