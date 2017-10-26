#include "menu.h"

void menu(const char* nombre, int dinero) {
	print(1, "Benvingut %s\nTé %d euros disponibles\nIntrodueixi comandes...\n\n", nombre, dinero);
}

void show_prompt(const char* nombre) {
	print(1, "%s> ", nombre);
}

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
	char *last = strrchr(buffer, ' ');
	if (first == NULL || first == last) {
		//CONNECTA, DESCONNECTA, PAGAR, o una palabra
		//MOSTRA MENU o "foo bar"
		strcpy(cmd, buffer);
		*arg1 = '\0';
		*arg2 = '\0';
		return 1;
	} else {
		//DEMANA foo bar, ELIMINA foo bar
		*first = '\0';
		*last = '\0';
		strcpy(cmd, buffer);
		strcpy(arg1, first + 1);
		strcpy(arg2, last + 1);
		return _is_numeric(arg1) ? 3 : -1;
	}
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