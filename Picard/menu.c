/***********************************************************************
*
* @Archivo:	menu.h menu.c
* @Finalidad: interactuar con el terminal; introducción del usuario
* @Autor: Daniel y Elena
* @Fecha: 24/10/17
*
************************************************************************/

#include "menu.h"

//SON SOLO PARA EL C Y >>NO SE UTILIZAN EN OTRO SITIO<<
#define CONNECTA_M "CONNECTA"
#define MOSTRA_MENU_M "MOSTRA MENU"
#define DEMANA_M "DEMANA"
#define ELIMINA_M "ELIMINA"
#define DESCONNECTA_M "DESCONNECTA"
#define PAGAR_M "PAGAR"

/***********************************************************************
*
* @Nombre: menu
* @Def: muestra el menu con el nombre y dinero del Picard actual
* @Arg: In: nombre = string que contiene el nombre de Picard
* 			dinero = cantidad de dinero que tiene Picard
* @Ret: -
************************************************************************/
void menu(const char* nombre, int dinero) {
	print(1, "Benvingut %s\nTé %d euros disponibles\nIntrodueixi comandes...\n\n", nombre, dinero);
}

/***********************************************************************
*
* @Nombre: show_prompt
* @Def: muestra nombre_picard> para que este introduzca una opcion
* @Arg: In: usr = string que contiene el nombre de Picard
* @Ret: -
************************************************************************/
void show_prompt(const char* nombre) {
	print(1, "%s> ", nombre);
}

/***********************************************************************
*
* @Nombre: _is_numeric
* @Def: funcion privada para saber si una string contiene unicamente
		digitos
* @Arg: In: str = string en consulta
* @Ret: 1 si es numerica, 0 si no
************************************************************************/
int _is_numeric(char* str) {
	char* ptr;
	for (ptr = str; *ptr != '\0'; ptr++) {
		if (!isdigit(*ptr)) return 0;
	}
	return 1;
}

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
int read_cmd(char** cmd, char** arg1, char** arg2) {
	char *buffer = read_until(0, '\n');
	char *first = strchr(buffer, ' ');
	char *next;

	if (first != NULL) {
		next = strchr(first + 1, ' ');
		if (next != NULL) {
			//DEMANA foo bar, ELIMINA foo bar
			*first = '\0';
			*next = '\0';
			*cmd = buffer;
			*arg1 = first + 1;
			*arg2 = next + 1;
			return _is_numeric(*arg1) ? 3 : -1;
		} 
	}
	//MOSTRA MENU o "foo bar"
	//CONNECTA, DESCONNECTA, PAGAR, o una palabra
	*cmd = buffer;
	*arg1 = NULL;
	*arg2 = NULL;
	return 1;
}

/***********************************************************************
*
* @Nombre: cmd_error
* @Def: avisa por el file descriptor 2 que la comanda introducida es
		invalida
* @Arg: -
* @Ret: -
************************************************************************/
void cmd_error() {
	print(2, "Comanda no reconeguda\n");
}

/***********************************************************************
*
* @Nombre: cmd_error
* @Def: avisa por el file descriptor 2 que la comanda introducida es
		valida
* @Arg: -
* @Ret: -
************************************************************************/
void cmd_ok() {
	print(1, "[Comanda OK]\n");
}

/***********************************************************************
*
* @Nombre: show_goodbye
* @Def: le dice adios al usuario
* @Arg: -
* @Ret: -
************************************************************************/
void show_goodbye() {
	print(1, "Gràcies per fer servir LsEat. Fins la propera.\n");
}

/***********************************************************************
*
* @Nombre: check_input
* @Def: verifica la entrada por terminal del usuario
* @Arg: In: cmd = comando
		In: n_args = numero de argumentos
* @Ret: el numero de la operacion que se queria realizar segun el enum
		(mirar .h) o error en caso de ser invalido
************************************************************************/
int check_input(const char* cmd, int n_args) {
	if (n_args == 1) {
		if (strcmpign(cmd, CONNECTA_M) == 0) {
			return CONNECTA;
			/**/
		} else if (strcmpign(cmd, MOSTRA_MENU_M) == 0) {
			return MOSTRA_MENU;
		} else if (strcmpign(cmd, PAGAR_M) == 0) {
			return PAGAR;
		} else if (strcmpign(cmd, DESCONNECTA_M) == 0) {
			return DESCONNECTA;
		} else {
			return ERROR;
		}
	} else if (n_args == 3) {
		if (strcmpign(cmd, DEMANA_M) == 0) {
			return DEMANA;
		} else if (strcmpign(cmd, ELIMINA_M) == 0) {
			return ELIMINA;
		} else {
			return ERROR;
		}
	} else {
		return ERROR;
	}
}