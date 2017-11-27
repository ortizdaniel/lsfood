/***********************************************************************
*
* @Archivo:	lista.h lista.c
* @Finalidad: 	lista PDI para almacenar los file descriptors de cada cliente
				Picard que se conecta, tomada de Projectes de Programació 1
* @Autor: Eduard de Torres
* @Fecha: desconocida
*
************************************************************************/

#ifndef _LISTA_H
#define _LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_IP 16
#define INIT_SIZE 4

typedef struct _nodo {
	int e;
	struct _nodo * sig;
} Nodo;

typedef struct {
	Nodo * pri;
	Nodo * ant;
} Lista;

/***********************************************************************
*
* @Nombre: LISTA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int LISTA_crea (Lista * l);

/***********************************************************************
*
* @Nombre: LISTA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			e = el elemento a añadir
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int LISTA_anadir (Lista * l, int e);

/***********************************************************************
*
* @Nombre: LISTA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void LISTA_eliminar (Lista * l);

/***********************************************************************
*
* @Nombre: LISTA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
* @Ret: el elemento
************************************************************************/
int LISTA_consultar (Lista l);

/***********************************************************************
*
* @Nombre: LISTA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int  LISTA_vacia (Lista l);

/***********************************************************************
*
* @Nombre: LISTA_destruir
* @Def: destruye un alista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_destruir (Lista * l);

/***********************************************************************
*
* @Nombre: LISTA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_avanzar (Lista * l);

/***********************************************************************
*
* @Nombre: LISTA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int  LISTA_final (Lista l);

/***********************************************************************
*
* @Nombre: LISTA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_irPrincipio (Lista * l);

#endif

