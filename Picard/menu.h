/***********************************************************************
*
* @Archivo:	menu.h menu.c
* @Finalidad: interactuar con el terminal; introducción del usuario
* @Autor: Daniel y Elena
* @Fecha: 24/10/17
*
************************************************************************/

#ifndef _MENU_H
#define _MENU_H

#include <ctype.h>
#include "util.h"

#define MAX_IN 128
#define MAX_CMD 32

enum menu_option {
	ERROR = -1,
	CONNECTA = 0,
	MOSTRA_MENU = 1,
	DEMANA = 2,
	ELIMINA = 3,
	DESCONNECTA = 4,
	PAGAR = 5
};

/***********************************************************************
*
* @Nombre: menu
* @Def: muestra el menu con el nombre y dinero del Picard actual
* @Arg: In: nombre = string que contiene el nombre de Picard
* 			dinero = cantidad de dinero que tiene Picard
* @Ret: -
************************************************************************/
void menu(const char* nombre, int dinero);

/***********************************************************************
*
* @Nombre: show_prompt
* @Def: muestra nombre_picard> para que este introduzca una opcion
* @Arg: In: usr = string que contiene el nombre de Picard
* @Ret: -
************************************************************************/
void show_prompt(const char* usr);

/***********************************************************************
*
* @Nombre: read_cmd
* @Def: lee un comando que introduce Picard por terminal y trata los
		parametros
* @Arg: Out: cmd = string que contendra el comando introducido p.ej DEMANA
* 			 arg1 = primer argumento del comando
*			 arg2 = segundo argumento del comando
* @Ret: cantidad de argumentos (cmd incluido) que se han introducido si
		es que se introdujo un comando valido.
		P.ej: si introducen DEMANA 56 BRAVAS entonces:
			cmd = DEMANA
			arg1 = 56
			arg2 = BRAVAS
			retorno = 3
		P.ej: si introducen MOSTRA MENU
			cmd = MOSTRA MENU
			retorno = 1
************************************************************************/
int read_cmd(char** cmd, char** arg1, char** arg2);

/***********************************************************************
*
* @Nombre: cmd_error
* @Def: avisa por el file descriptor 2 que la comanda introducida es
		invalida
* @Arg: -
* @Ret: -
************************************************************************/
void cmd_error();

/***********************************************************************
*
* @Nombre: cmd_error
* @Def: avisa por el file descriptor 2 que la comanda introducida es
		valida
* @Arg: -
* @Ret: -
************************************************************************/
void cmd_ok();

/***********************************************************************
*
* @Nombre: check_input
* @Def: verifica la entrada por terminal del usuario
* @Arg: In: cmd = comando
		In: n_args = numero de argumentos
* @Ret: el numero de la operacion que se queria realizar segun el enum
		(mirar .h) o error en caso de ser invalido
************************************************************************/
int check_input(const char* cmd, int n_args);

/***********************************************************************
*
* @Nombre: show_goodbye
* @Def: le dice adios al usuario
* @Arg: -
* @Ret: -
************************************************************************/
void show_goodbye();

#endif