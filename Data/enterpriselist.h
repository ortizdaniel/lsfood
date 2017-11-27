/***********************************************************************
*
* @Archivo:	enterpriselist.h enterpriselist.c
* @Finalidad: semi-implementación de un "ArrayList" para almacenar
			  la información de los Enterprises
* @Autor: Daniel y Elena
* @Fecha: 26/10/17
*
************************************************************************/

#ifndef _ENTERPRISE_LIST_H
#define _ENTERPRISE_LIST_H

#include <pthread.h>
#include <stdlib.h>

#define MAX_IP 16
#define INIT_SIZE 4

typedef struct {
	char* nombre;
	char ip[MAX_IP]; //ip es como mucho 255.255.255.255\0
	int port;
	int users_connected;
	int alive;
} Enterprise;

/***********************************************************************
*
* @Nombre: enterprises_init
* @Def: inicializa la lista
* @Arg: -
* @Ret: 1 si se creó, 0 si no
************************************************************************/
int enterprises_init();

/***********************************************************************
* PRE: pos >= 0 && pos < enterprises_size()
* @Nombre: enterprises_get
* @Def: retorna un elemento i de la lista
* @Arg: In: pos = posición deseada
* @Ret: puntero al Enterprise deseado
************************************************************************/
Enterprise* enterprises_get(int pos);

/***********************************************************************
*
* @Nombre: enterprises_size
* @Def: informa del tamaño de la lista actual
* @Arg: -
* @Ret: el tamaño de la lista
************************************************************************/
int enterprises_size();

/***********************************************************************
*
* @Nombre: enterprises_add
* @Def: añade un Enterprise* a la lista
* @Arg: In: e = puntero al Enterprise a añadir
* @Ret: 1 si se añadió correctamente, 0 si no
************************************************************************/
int enterprises_add(Enterprise* e);

/***********************************************************************
*
* @Nombre: enterprises_order
* @Def: ordena el array de Enterprises de tal manera que los que tengan
		menos usuarios estén al principio de la lista
* @Arg: -
* @Ret: -
************************************************************************/
void enterprises_order();

/***********************************************************************
*
* @Nombre: enterprises_end
* @Def: finaliza la lista y libera todos los recursos
* @Arg: -
* @Ret: -
************************************************************************/
void enterprises_end();

#endif