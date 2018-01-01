/***********************************************************************
*
* @Archivo:	lista_lista.h lista_lista.c
* @Finalidad: 	lista PDI para almacenar referencias a las listas de
				reservas de cada thread dedicado
* @Autor: Eduard de Torres - Mod by Daniel & Elena
* @Fecha: desconocida
*
************************************************************************/

#include "lista_lista.h"

/***********************************************************************
*
* @Nombre: L_LISTA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int L_LISTA_crea (Lista_Lista * l) {
	l->pri = (Nodo_L*) malloc (sizeof(Nodo_L));
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
* @Nombre: L_LISTA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			e = el elemento a añadir
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int L_LISTA_anadir (Lista_Lista * l, Lista_Reserva * e) {
	Nodo_L * aux = (Nodo_L*) malloc (sizeof(Nodo_L));
	if (aux == NULL) {
		return 0;	}
	else {
		aux->e = e;
		aux->sig = l->ant->sig;
		l->ant->sig = aux;
		l->ant = l->ant->sig;
		return 1;
	}
}

/***********************************************************************
*
* @Nombre: L_LISTA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void L_LISTA_eliminar (Lista_Lista * l) {
	Nodo_L * aux;
	if (l->ant->sig == NULL) {
		//printf("Error. El pdi esta al final de la lista.\n");
	}
	else {
		aux = l->ant->sig;
		//L_RESERVA_destruir(aux->e);
		//free(aux->e); //TO-DO revisar
		l->ant->sig = l->ant->sig->sig;
		//picards->ant->sig = aux->sig;
		free(aux);
	}
}

/***********************************************************************
*
* @Nombre: L_LISTA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
* @Ret: el elemento
************************************************************************/
Lista_Reserva* L_LISTA_consultar (Lista_Lista l) { 
	if (l.ant->sig == NULL) {
		//printf("Error, estamos en el final de la lista.\n");
		return NULL;
	}
	else {
		return l.ant->sig->e;
	}
}

/***********************************************************************
*
* @Nombre: L_LISTA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int  L_LISTA_vacia (Lista_Lista l) {
	return l.pri->sig == NULL;
}

/***********************************************************************
*
* @Nombre: L_LISTA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_avanzar (Lista_Lista * l) {
	if (l->ant->sig == NULL) {
		//printf("Error al avanzar. La lista esta al final.\n");
	}
	else {
		l->ant = l->ant->sig;
	}
}

/***********************************************************************
*
* @Nombre: L_LISTA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int L_LISTA_final (Lista_Lista l) {
	return l.ant->sig == NULL;
}

/***********************************************************************
*
* @Nombre: L_LISTA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_irPrincipio (Lista_Lista * l) {
	l->ant = l->pri;
}

/***********************************************************************
*
* @Nombre: L_LISTA_destruir
* @Def: destruye una lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void L_LISTA_destruir (Lista_Lista * l) {
	//Vaciar la lista de elementos validos
	L_LISTA_irPrincipio(l);
	while (!L_LISTA_final(*l)) {
		L_LISTA_eliminar(l);
	}
	//Liberar fantasma
	free(l->pri); //free(picards->ant);
	l->pri = NULL;
	l->ant = NULL;
}