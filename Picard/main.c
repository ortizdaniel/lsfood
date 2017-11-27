#include <signal.h>
#include "menu.h"
#include "network.h"

Config c;

void handle_int(int n);
int connecta();

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
		print(2, "Error: Fichero de configuracion invalid2o.\n");
		return 2;
	}
	//Hasta aqui no hay errores de config
	signal(SIGINT, handle_int);

	menu(c.nombre, c.dinero);
	char *cmd, *arg1, *arg2;
	int n_args;
	int exit = 0;
	do {
		show_prompt(c.nombre);
		n_args = read_cmd(&cmd, &arg1, &arg2);
		int check = check_input(cmd, n_args);
		switch (check) {
			case CONNECTA:
				connecta();
				break;
			case MOSTRA_MENU:
				cmd_ok();
				break;
			case DEMANA:
				cmd_ok();
				break;
			case ELIMINA:
				cmd_ok();
				break;
			case DESCONNECTA:
				net_end();
				net_disconnect();
				show_goodbye();
				exit = 1;
				break;
			case PAGAR:
				cmd_ok();
				break;
			case ERROR:
			default:
				cmd_error();
				break;
		}

		free(cmd);
	} while (!exit);

	destroy_config(c);
	return 0;
}

/***********************************************************************
*
* @Nombre: handle_int
* @Def: gestiona las interrupciones
* @Arg: In: n - el identificador de la interrupcion
* @Ret: -
************************************************************************/
void handle_int(int n) {
	if (n == SIGINT) {
		print(1, "\n[Tancat inesperadament]\n[Alliberant recursos...]\n");
		destroy_config(c);
		print(1, "[Recursos alliberats]\n");
		exit(1);
	}
}

/***********************************************************************
*
* @Nombre: connecta
* @Def: se intenta conectar contra Data, leer una respuesta, y conectarse
		con Enterprise
* @Arg: -
* @Ret: 0 si es todo correcto, numeros distintos de 0 en caso de que no
************************************************************************/
int connecta() {
	print(1, "Connectant amb LsEat...\n");
	if (is_connected()) {
		print(1, "Ya estas conectado! No puedes conectarte otra vez.\n");
		return 5;
	} else {
		if (net_connect(c.ip, c.port) == 0) {
			print(1, "[Connexió amb Data OK]\n");
			send_connect(c.nombre);
			//TODO desconectar de Data, conectar con Enterprise
			Packet recibido;
			if (read_packet(&recibido) > 0) {
				if (recibido.type == 0x01) {
					if (strcmp(recibido.header, "[ENT_INF]") == 0) {
						net_disconnect();
						net_handle(recibido);
					} else if (strcmp(recibido.header, "[CONKO]") == 0) {
						print(1, "No hay restaurantes disponibles.\n");
						net_disconnect();
						free(recibido.data);
						return 1;
					} else {
						print(1, "[Paquete con header incorrecto recibido. Cerrando conexión.]\n");
						net_disconnect();
						free(recibido.data);
						return 2;
					}
				} else {
					print(1, "[Paquete con tipo incorrecto recibido. Cerrando conexión.]\n");
					net_disconnect();
					free(recibido.data);
					return 3;
				}
				free(recibido.data); //siempre por lo menos hay memoria para un caracter
			}
		} else {
			print(1, "[Error de connexió]\n");
			return 4;
		}
	}
	return 0;
}