/***********************************************************************
*
* @Archivo:	util.h util.c
* @Finalidad: proporcionar distintas utilidades para Enterprise
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
			//free(buffer);
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