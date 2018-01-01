#include <signal.h>

#include "util.h"
#include "networkpic.h"
#include "networkent.h"

#define MAX_IP 16

typedef struct {
	char ip[MAX_IP];
	int port_pic;
	int port_ent;
} Config;

void handle_int(int n);
int read_config(const char* path, Config* out);

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		print(2, "[Número de parametros de entrada inválidos]\n");
		return 1;
	}

	Config c;
	int n;
	if ((n = read_config(argv[1], &c)) == 0) {
		if (enterprises_init() == 0) {
			exit(2);
		}

		signal(SIGINT, handle_int);
		signal(SIGTERM, handle_int);
		if (network_pic_init(c.ip, c.port_pic) != 0) {
			network_pic_end();
			print(2, "[Error inicializar socket Picards...]\n");
			exit(1);
		}
		if (network_ent_init(c.ip, c.port_ent)) {
			network_pic_end();
			network_ent_end();
			print(2, "[Error inicializar socket Enterprises...]\n");
			exit(1);
		}
		print(1, "Executant Data\nEsperant clients...\nEsperant enterprises...\n");
		while (1) pause();
	} else {
		print(2, "[Error de lectura del fichero de configuración]%d\n", n);
	}
	
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
	if (n == SIGINT || n == SIGTERM) {
		print(1, "\n[Cerrando... por favor espere...]\n");
		network_ent_end();
		network_pic_end();
		enterprises_end();
		exit(1);
	}
}

/***********************************************************************
*
* @Nombre: read_config
* @Def: lee el fichero de configuración
* @Arg: In: path = path al fichero de configuración
		Out: out = configuracion de salida
* @Ret: 0 se todo fue correcto, un número si falló
************************************************************************/
int read_config(const char* path, Config* out) {
	int fd = open(path, O_RDONLY);
	if (fd < 0) return 1;

	char* ip = read_until(fd, '\n');
	if (ip == NULL) {
		close(fd);
		return 2;
	}
	strcpy(out->ip, ip);
	free(ip);

	char* port_pic = read_until(fd, '\n');
	if (port_pic == NULL) {
		close(fd);
		return 3;
	}
	out->port_pic = is_numeric(port_pic) ? atoi(port_pic) : -1;
	free(port_pic);

	char* port_ent = read_until(fd, '\0');
	if (port_ent == NULL) {
		close(fd);
		return 4;
	}
	out->port_ent = is_numeric(port_ent) ? atoi(port_ent) : -1;
	free(port_ent);

	return 0;
}