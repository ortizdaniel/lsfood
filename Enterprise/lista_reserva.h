/***********************************************************************
*
* @Archivo:	lista_reserva.h lista_reserva.c
* @Finalidad: 	lista PDI para almacenar los platos reservados de cada cliente
				Picard que se conecta
* @Autor: Eduard de Torres - Mod by Daniel & Elena
* @Fecha: desconocida
*
************************************************************************/

#ifndef _LISTA_RESERVA_H
#define _LISTA_RESVERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct _nodo_reserva {
	char* nombre;
	int cantidad;
	int subtotal;
	struct _nodo_reserva * sig;
} Nodo_Reserva;

typedef struct {
	Nodo_Reserva * pri;
	Nodo_Reserva * ant;
} Lista_Reserva;

/***********************************************************************
*
* @Nombre: L_RESERVA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int L_RESERVA_crea (Lista_Reserva * l);

/***********************************************************************
*
* @Nombre: L_RESERVA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			nombre, cantidad, y subtotal de la reserva
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int L_RESERVA_anadir (Lista_Reserva * l, char* nombre, int cantidad, int subtotal);

/***********************************************************************
*
* @Nombre: L_RESERVA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void L_RESERVA_eliminar (Lista_Reserva * l);

/***********************************************************************
*
* @Nombre: L_RESERVA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
		Out: parámetros del plato consultado
* @Ret: -
************************************************************************/
int L_RESERVA_consultar (Lista_Reserva l, char** nombre, int* cantidad, int* subtotal);

/***********************************************************************
*
* @Nombre: L_RESERVA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int L_RESERVA_vacia (Lista_Reserva l);

/***********************************************************************
*
* @Nombre: L_RESERVA_destruir
* @Def: destruye un alista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_destruir (Lista_Reserva * l);

/***********************************************************************
*
* @Nombre: L_RESERVA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_avanzar (Lista_Reserva * l);

/***********************************************************************
*
* @Nombre: L_RESERVA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int L_RESERVA_final (Lista_Reserva l);

/***********************************************************************
*
* @Nombre: L_RESERVA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_irPrincipio (Lista_Reserva * l);

#endif

