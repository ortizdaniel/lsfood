/***********************************************************************
*
* @Archivo:	enterpriselist.h enterpriselist.c
* @Finalidad: semi-implementación de un "ArrayList" para almacenar
			  la información de los Enterprises
* @Autor: Daniel y Elena
* @Fecha: 26/10/17
*
************************************************************************/

#include "enterpriselist.h"

static int size = 0;
static volatile int added = 0;
static Enterprise** enterprises = NULL;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/***********************************************************************
*
* @Nombre: enterprises_init
* @Def: inicializa la lista
* @Arg: -
* @Ret: 1 si se creó, 0 si no
************************************************************************/
int enterprises_init() {
	enterprises = (Enterprise **) calloc(INIT_SIZE, sizeof(Enterprise*));
	if (enterprises == NULL) return 0;
	size = INIT_SIZE;
	return 1;
}

/***********************************************************************
* PRE: pos >= 0 && pos < enterprises_size()
* @Nombre: enterprises_get
* @Def: retorna un elemento i de la lista
* @Arg: In: pos = posición deseada
* @Ret: puntero al Enterprise deseado
************************************************************************/
Enterprise* enterprises_get(int pos) {
	return enterprises[pos];
}

/***********************************************************************
*
* @Nombre: enterprises_size
* @Def: informa del tamaño de la lista actual
* @Arg: -
* @Ret: el tamaño de la lista
************************************************************************/
int enterprises_size() {
	return added;
}

/***********************************************************************
*
* @Nombre: enterprises_add
* @Def: añade un Enterprise* a la lista
* @Arg: In: e = puntero al Enterprise a añadir
* @Ret: 1 si se añadió correctamente, 0 si no
************************************************************************/
int enterprises_add(Enterprise* e) {
	if (added < size) {
		enterprises[added++] = e;
		enterprises_order();
		return 1;
	} else {
		Enterprise** tmp = (Enterprise**) realloc(enterprises, sizeof(Enterprise*) * size * 2);
		if (tmp == NULL) return 0;

		enterprises = tmp;
		size *= 2;
		enterprises[added++] = e;
		enterprises_order();
		return 1;
	}
}

/***********************************************************************
*
* @Nombre: enterprises_end
* @Def: finaliza la lista y libera todos los recursos
* @Arg: -
* @Ret: -
************************************************************************/
void enterprises_end() {
	int i;
	for (i = 0; i < enterprises_size(); i++) {
		free(enterprises[i]->nombre);
		free(enterprises[i]);
	}
	free(enterprises);
}


/***********************************************************************
*
* @Nombre: enterprises_order
* @Def: ordena el array de Enterprises de tal manera que los que tengan
		menos usuarios estén al principio de la lista
* @Arg: -
* @Ret: -
************************************************************************/
void enterprises_order() {
	//Ordernar de menor a mayor por numero de usuarios, de tal manera que los que tengan menos
	//usuairos estan al principio de la lista
	//insertionsort O(n^2)
	Enterprise* x;
	int i, j;
	for (i = 1; i < enterprises_size(); i++) {
		x = enterprises[i];

		j = i - 1;
		while (j >= 0 && x->users_connected < enterprises[j]->users_connected) {
			enterprises[j + 1] = enterprises[j];
			j--;
		}

		enterprises[j + 1] = x;
	}
}