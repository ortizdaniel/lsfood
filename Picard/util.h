/***********************************************************************
*
* @Archivo:	util.h util.c
* @Finalidad: proporcionar distintas utilidades para Picard
* @Autor: Daniel y Elena
* @Fecha: 22/10/17
*
************************************************************************/

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


/***********************************************************************
*
* @Nombre: print
* @Def: implementacion de mala calidad de printf, para file descriptors
* @Arg: In: fd = file descriptor donde escribir
* 			format = el formato con el cual printar
* @Ret: -
************************************************************************/
void print(int fd, const char* format, ...);

/***********************************************************************
*
* @Nombre: read_until
* @Def: lee de un file descriptor hasta que se encuentra con un caracter
		deseado
* @Arg: In: fd = file descriptor del cual leer
* 			stop = el caracter comodin con el cual detenerse
* @Ret: puntero donde se ha leido esta informacion, NULL si error
************************************************************************/
char* read_until(int fd, char stop);

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
int read_config(const char* file, Config* config);

/***********************************************************************
*
* @Nombre: destroy_config
* @Def: destruye una Config previamente obtenida con read_config
* @Arg: In: config = configuracion a ser destruida
* @Ret: -
************************************************************************/
void destroy_config(Config config);

/***********************************************************************
*
* @Nombre: strcmpign
* @Def: string compare ignore case
* @Arg: In: a = string 1
* 			b = string 2
* @Ret: 0 si las cadenas son iguales, -1 si no
************************************************************************/
int strcmpign(const char* a, const char* b);

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
ssize_t read_clean(int fd, char* buffer, size_t max);

void remove_extra_spaces(char* str);

#endif