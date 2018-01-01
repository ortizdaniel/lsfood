/***********************************************************************
*
* @Archivo:	lista_reserva.h lista_reserva.c
* @Finalidad: 	lista PDI para almacenar los platos reservados de cada cliente
				Picard que se conecta
* @Autor: Eduard de Torres - Mod by Daniel & Elena
* @Fecha: desconocida
*
************************************************************************/

#include "lista_reserva.h"

/***********************************************************************
*
* @Nombre: L_RESERVA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int L_RESERVA_crea (Lista_Reserva * l) {
	l->pri = (Nodo_Reserva*) malloc (sizeof(Nodo_Reserva));
	if (l->pri == NULL) {
		return 0;
	}
	else {
		l->ant = l->pri;
		l->pri->sig = NULL;
		return 1;
	}
}

/***********************************************************************
*
* @Nombre: L_RESERVA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			nombre, cantidad, y subtotal de la reserva
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int L_RESERVA_anadir (Lista_Reserva * l, char* nombre, int cantidad, int subtotal) {
	Nodo_Reserva * aux = (Nodo_Reserva*) malloc (sizeof(Nodo_Reserva));
	if (aux == NULL) {
		return 0;	}
	else {
		aux->nombre = strdup(nombre);
		aux->cantidad = cantidad;
		aux->subtotal = subtotal;
		aux->sig = l->ant->sig;
		l->ant->sig = aux;
		l->ant = l->ant->sig;
		return 1;
	}
}

/***********************************************************************
*
* @Nombre: L_RESERVA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void L_RESERVA_eliminar (Lista_Reserva * l) {
	Nodo_Reserva * aux;
	if (l->ant->sig == NULL) {
		//printf("Error. El pdi esta al final de la lista.\n");
	}
	else {
		aux = l->ant->sig;
		free(aux->nombre);
		l->ant->sig = l->ant->sig->sig;
		//picards->ant->sig = aux->sig;
		free(aux);
	}
}

/***********************************************************************
*
* @Nombre: L_RESERVA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
		Out: parámetros del plato consultado
* @Ret: -
************************************************************************/
int L_RESERVA_consultar (Lista_Reserva l, char** nombre, int* cantidad, int* subtotal) { 
	if (l.ant->sig == NULL) {
		//printf("Error, estamos en el final de la lista.\n");
		return -1;
	}
	else {
		Nodo_Reserva* aux = l.ant->sig;
		if (nombre != NULL) *nombre = aux->nombre;
		if (cantidad != NULL) *cantidad = aux->cantidad;
		if (subtotal != NULL) *subtotal = aux->subtotal;
		return 1;
	}
}

/***********************************************************************
*
* @Nombre: L_RESERVA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int  L_RESERVA_vacia (Lista_Reserva l) {
	return l.pri->sig == NULL;
}

/***********************************************************************
*
* @Nombre: L_RESERVA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_avanzar (Lista_Reserva * l) {
	if (l->ant->sig == NULL) {
		//printf("Error al avanzar. La lista esta al final.\n");
	}
	else {
		l->ant = l->ant->sig;
	}
}

/***********************************************************************
*
* @Nombre: L_RESERVA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int  L_RESERVA_final (Lista_Reserva l) {
	return l.ant->sig == NULL;
}

/***********************************************************************
*
* @Nombre: L_RESERVA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_irPrincipio (Lista_Reserva * l) {
	l->ant = l->pri;
}

/***********************************************************************
*
* @Nombre: L_RESERVA_destruir
* @Def: destruye un alista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_RESERVA_destruir (Lista_Reserva * l) {
	//Vaciar la lista de elementos validos
	L_RESERVA_irPrincipio(l);
	while (!L_RESERVA_final(*l)) {
		L_RESERVA_eliminar(l);
	}
	//Liberar fantasma
	free(l->pri); //free(picards->ant);
	l->pri = NULL;
	l->ant = NULL;
}