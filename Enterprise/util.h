/***********************************************************************
*
* @Archivo:	util.h util.c
* @Finalidad: proporcionar distintas utilidades para Enterprise
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
#include <sys/stat.h>
#include <semaphore.h>

#define MAX_IP 16

typedef struct {
	char* nombre;
	int t_act;
	char ip_data[MAX_IP]; //para conectarse a DATA
	int port_data;
	char ip_pic[MAX_IP]; //donde se conectan los PICs
	int port_pic;
} Config;

typedef struct {
	char* nombre;
	int cantidad;
	int precio;
} Plato;

/***********************************************************************
*
* @Nombre: util_init
* @Def: inicializa un semaforo para que el método print sea sincrono
* @Arg: -
* @Ret: -
************************************************************************/
void util_init();

/***********************************************************************
*
* @Nombre: util_end
* @Def: libera los recursos usados por el semaforo
* @Arg: -
* @Ret: -
************************************************************************/
void util_end();

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

#endif