#include "menu.h"

void menu(const char* nombre, int dinero) {
	print(1, "Benvingut %s\nTé %d euros disponibles\nIntrodueixi comandes...\n\n", nombre, dinero);
}

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

int read_cmd(char* cmd, char* arg1, char* arg2) {
	char buffer[MAX_IN];
	read_clean(0, buffer, MAX_IN);
	char *first = strchr(buffer, ' ');
	char *next;

	if (first != NULL) {
		*first = '\0';
		next = strchr(first + 1, ' ');
		if (next != NULL) {
			//DEMANA foo bar, ELIMINA foo bar
			*next = '\0';
			strcpy(cmd, buffer);
			strcpy(arg1, first + 1);
			strcpy(arg2, next + 1);
			return _is_numeric(arg1) ? 3 : -1;
		} 
	}
	//MOSTRA MENU o "foo bar"
	//CONNECTA, DESCONNECTA, PAGAR, o una palabra
	strcpy(cmd, buffer);
	*arg1 = '\0';
	*arg2 = '\0';
	return 1;
}

void cmd_error() {
	print(2, "Comanda no reconeguda\n");
}

void cmd_ok() {
	print(1, "[Comanda OK]\n");
}

void show_goodbye() {
	print(1, "Gràcies per fer servir LsEat. Fins la propera.\n");
}