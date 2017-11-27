/***********************************************************************
*
* @Archivo:	lista.h lista.c
* @Finalidad: 	lista PDI para almacenar los file descriptors de cada cliente
				Picard que se conecta, tomada de Projectes de Programació 1
* @Autor: Eduard de Torres
* @Fecha: desconocida
*
************************************************************************/

#include "lista.h"

/***********************************************************************
*
* @Nombre: LISTA_crea
* @Def: crea la lista
* @Arg: Out: l = lista para crear
* @Ret: 1 si se realizo correctamente, 0 si no
************************************************************************/
int LISTA_crea (Lista * l) {
	l->pri = (Nodo*) malloc (sizeof(Nodo));
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
* @Nombre: LISTA_anadir
* @Def: anade un elemento a la lista
* @Arg: In: l = lista para añadir el elemento
			e = el elemento a añadir
* @Ret: 1 si de realizo correctamente, 0 si no
************************************************************************/
int LISTA_anadir (Lista * l, int e) {
	Nodo * aux = (Nodo*) malloc (sizeof(Nodo));
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
* @Nombre: LISTA_eliminar
* @Def: destruye el elemento apuntado por el PDI
* @Arg: In: l = lista en cuestión
* @Ret: -
************************************************************************/
void LISTA_eliminar (Lista * l) {
	Nodo * aux;
	if (l->ant->sig == NULL) {
		//printf("Error. El pdi esta al final de la lista.\n");
	}
	else {
		aux = l->ant->sig;
		close(aux->e);
		l->ant->sig = l->ant->sig->sig;
		//l->ant->sig = aux->sig;
		free(aux);
	}
}

/***********************************************************************
*
* @Nombre: LISTA_consultar
* @Def: retorna el elemento actual del PDI
* @Arg: In: l = lista en cuestion
* @Ret: el elemento
************************************************************************/
int LISTA_consultar (Lista l) { 
	if (l.ant->sig == NULL) {
		//printf("Error, estamos en el final de la lista.\n");
		return -1;
	}
	else {
		return l.ant->sig->e;
	}
}

/***********************************************************************
*
* @Nombre: LISTA_vacia
* @Def: indica si la lista está vacia
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está vacia, 0 si no
************************************************************************/
int  LISTA_vacia (Lista l) {
	return l.pri->sig == NULL;
}

/***********************************************************************
*
* @Nombre: LISTA_avanzar
* @Def: mueve el PDI de la lista hacia adelante
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_avanzar (Lista * l) {
	if (l->ant->sig == NULL) {
		//printf("Error al avanzar. La lista esta al final.\n");
	}
	else {
		l->ant = l->ant->sig;
	}
}

/***********************************************************************
*
* @Nombre: LISTA_final
* @Def: indica si el PDI llegó al final
* @Arg: In: l = lista en cuestion
* @Ret: 1 si está al final, 0 si no
************************************************************************/
int  LISTA_final (Lista l) {
	return l.ant->sig == NULL;
}

/***********************************************************************
*
* @Nombre: LISTA_irPrincipio
* @Def: mueve el PDI al primer elemento de la lista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_irPrincipio (Lista * l) {
	l->ant = l->pri;
}

/***********************************************************************
*
* @Nombre: LISTA_destruir
* @Def: destruye un alista
* @Arg: In: l = lista en cuestion
* @Ret: -
************************************************************************/
void LISTA_destruir (Lista * l) {
	//Vaciar la lista de elementos validos
	LISTA_irPrincipio(l);
	while (!LISTA_final(*l)) {
		LISTA_eliminar(l);
	}
	//Liberar fantasma
	free(l->pri); //free(l->ant);
	l->pri = NULL;
	l->ant = NULL;
}