/***********************************************************************
*
* @Archivo:	lista_lista.h lista_lista.c
* @Finalidad: 	lista PDI para almacenar referencias a las listas de
				reservas de cada thread dedicado
* @Autor: Eduard de Torres - Mod by Daniel & Elena
* @Fecha: desconocida
*
************************************************************************/

#ifndef _LISTA_LISTA_H
#define _LISTA_LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "lista_reserva.h"

typedef struct _nodo_l {
	Lista_Reserva * e;
	struct _nodo_l * sig;
} Nodo_L;

typedef struct {
	Nodo_L * pri;
	Nodo_L * ant;
} Lista_Lista;

/***********************************************************************
*
* @Nombre: L_LISTA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int L_LISTA_crea (Lista_Lista * l);

/***********************************************************************
*
* @Nombre: L_LISTA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			e = el elemento a añadir
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int L_LISTA_anadir (Lista_Lista * l, Lista_Reserva * e);

/***********************************************************************
*
* @Nombre: L_LISTA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void L_LISTA_eliminar (Lista_Lista * l);

/***********************************************************************
*
* @Nombre: L_LISTA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
* @Ret: el elemento
************************************************************************/
Lista_Reserva* L_LISTA_consultar (Lista_Lista l);

/***********************************************************************
*
* @Nombre: L_LISTA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int  L_LISTA_vacia (Lista_Lista l);

/***********************************************************************
*
* @Nombre: L_LISTA_destruir
* @Def: destruye un alista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_destruir (Lista_Lista * l);

/***********************************************************************
*
* @Nombre: L_LISTA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_avanzar (Lista_Lista * l);

/***********************************************************************
*
* @Nombre: L_LISTA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int L_LISTA_final (Lista_Lista l);

/***********************************************************************
*
* @Nombre: L_LISTA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_irPrincipio (Lista_Lista * l);

#endif

