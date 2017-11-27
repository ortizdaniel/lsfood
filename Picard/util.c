/***********************************************************************
*
* @Archivo:	util.h util.c
* @Finalidad: proporcionar distintas utilidades para Picard
* @Autor: Daniel y Elena
* @Fecha: 22/10/17
*
************************************************************************/

#include "util.h"

/***********************************************************************
*
* @Nombre: print
* @Def: implementacion de mala calidad de printf, para file descriptors
* @Arg: In: fd = file descriptor donde escribir
* 			format = el formato con el cual printar
* @Ret: -
************************************************************************/
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

/***********************************************************************
*
* @Nombre: read_until
* @Def: lee de un file descriptor hasta que se encuentra con un caracter
		deseado
* @Arg: In: fd = file descriptor del cual leer
* 			stop = el caracter comodin con el cual detenerse
* @Ret: puntero donde se ha leido esta informacion, NULL si error
************************************************************************/
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

/***********************************************************************
*
* @Nombre: read_config
* @Def: lee la configuracion del fichero para Picard
* @Arg: In: file = path donde se encuentra el fichero de configuracion
* 		Out: config = tipo de Config de salida
* @Ret: 0 si funcionamiento correcto.
		1 si no se pudo encontrar el fichero.
		2 si no se pudo leer el nombre de Picard.
		4 si no se pudo leer el dinero.
		6 si no se pudo leer la IP donde conectarse.
		8 si no se pudo leer el puerto donde contectarse.
************************************************************************/
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

/***********************************************************************
*
* @Nombre: destroy_config
* @Def: destruye una Config previamente obtenida con read_config
* @Arg: In: config = configuracion a ser destruida
* @Ret: -
************************************************************************/
void destroy_config(Config config) {
	free(config.nombre);
	free(config.ip);
}

/***********************************************************************
*
* @Nombre: strcmpign
* @Def: string compare ignore case
* @Arg: In: a = string 1
* 			b = string 2
* @Ret: 0 si las cadenas son iguales, -1 si no
************************************************************************/
int strcmpign(const char* a, const char* b) {
	unsigned int i;
	unsigned int len = strlen(a);
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

/***********************************************************************
*
* @Nombre: read_clean
* @Def: lee del file descriptor deseado, y limpia el ultimo caracter.
		Similar a read_until, pero con un tamano máximo. Recomendable
		usar para el lectura por terminal.
* @Arg: In: fd = file descriptor donde leer
*			max = numero de caracteres maximos a leer
* 		Out: buffer = string de salida
* @Ret: numero de bytes leidos totales
************************************************************************/
ssize_t read_clean(int fd, char* buffer, size_t max) {
	ssize_t bytes = read(fd, buffer, max);
	buffer[bytes - 1] = '\0';
	return bytes;
}