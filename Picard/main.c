#include <signal.h>
#include "menu.h"
#include "network.h"

#define MAX_CMD 32
#define CONNECTA "CONNECTA"
#define MOSTRA_MENU "MOSTRA MENU"
#define DEMANA "DEMANA"
#define ELIMINA "ELIMINA"
#define DESCONNECTA "DESCONNECTA"
#define PAGAR "PAGAR"

Config c;

void handle_int(int n);

int main(int argc, const char *argv[]) {
	if (argc < 2) {
		print(2, "Error: numero de argumentos de entrada no valido.\n");
		return 1;
	}

	int ret = read_config(argv[1], &c);
	if (ret != 0) {
		if (ret > 5) {
			destroy_config(c);
		} else {
			free(c.nombre);
		}
		print(2, "Error: Fichero de configuracion invalido.\n");
		return 2;
	}
	//Hasta aqui no hay errores de config
	signal(SIGINT, handle_int);

	menu(c.nombre, c.dinero);
	char cmd[MAX_CMD], arg1[MAX_CMD], arg2[MAX_CMD];
	int n_args;
	int exit = 0;
	do {
		show_prompt(c.nombre);
		n_args = read_cmd(cmd, arg1, arg2);

		if (n_args == 1) {
			if (strcmpign(cmd, CONNECTA) == 0) {
				print(1, "Connectant amb LsEat...\n");
				if (is_connected()) {
					print(1, "Ya estas conectado! No puedes conectarte otra vez.\n");
				} else {
					if (net_connect(c.ip, c.port) == 0) {
						print(1, "[Connexió amb Data OK]\n");
						//TODO desconectar de Data, conectar con Enterprise
						
					} else {
						print(1, "[Error de connexió]\n");
					}
				}
			} else if (strcmpign(cmd, MOSTRA_MENU) == 0) {
				cmd_ok();
			} else if (strcmpign(cmd, PAGAR) == 0) {
				cmd_ok();
			} else if (strcmpign(cmd, DESCONNECTA) == 0) {
				show_goodbye();
				exit = 1;
			} else {
				cmd_error();
			}
		} else if (n_args == 3) {
			if (strcmpign(cmd, DEMANA) == 0) {
				cmd_ok();
			} else if (strcmpign(cmd, ELIMINA) == 0) {
				cmd_ok();
			} else {
				cmd_error();
			}
		} else {
			cmd_error();
		}
	} while (!exit);

	destroy_config(c);
	return 0;
}

void handle_int(int n) {
	if (n == SIGINT) {
		print(1, "\n[Tancat inesperadament]\n[Alliberant recursos...]\n");
		destroy_config(c);
		print(1, "[Recursos alliberats]\n");
		exit(1);
	}
}