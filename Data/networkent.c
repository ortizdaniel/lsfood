/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 19/11/17
*
************************************************************************/

#include "networkent.h"
#include "util.h"

static volatile int sock = -1;
static volatile int alive = 0;
static pthread_t pid;
static volatile int ent = -1;
extern pthread_mutex_t mtx;

/***********************************************************************
*
* @Nombre: _handle_packet
* @Def: gestiona un paquete que llega desde Enterprise: conexión, update,
		o desconexión concretamente
* @Arg: In: fd = file descriptor con el cual comunicarse con Enterprise
			p = paquete que vino desde Enterprise
* @Ret: -
************************************************************************/
void _handle_packet(int fd, Packet p) {
	if (p.type == 1) {
		if (strcmp(p.header, "[ENT_INF]") == 0) {
			char* name_start = strchr(p.data, '_') + 1;
			char* name_end = strchr(name_start, '&');
			*name_end = 0;

			char* port_start = strchr(name_end + 1, '_') + 1;
			char* port_end = strchr(port_start, '&');
			*port_end = 0;

			char* ip_start = strchr(port_end + 1, '_') + 1;
			char* ip_end = strchr(ip_start, ']');
			*ip_end = 0;

			Enterprise *e = (Enterprise*) calloc(1, sizeof(Enterprise));
			asprintf(&e->nombre, "%s", name_start);
			e->port = atoi(port_start);
			strcpy(e->ip, ip_start);
			e->users_connected = 0;
			e->alive = 1;
			
			print(1, "Enterprise [%s] se ha conectado\n", name_start);

			pthread_mutex_lock(&mtx);
			enterprises_add(e);
			pthread_mutex_unlock(&mtx);

			send_packet(fd, 1, "CONOK", 0, "");
		} else {
			send_packet(fd, 1, "CONKO", 0, "");
		}
	} else if (p.type == 2) {
		if (strcmp(p.header, "[ENT_INF]") == 0) {
			char* port_start = strchr(p.data, '_') + 1;
			char* port_end = strchr(port_start, ']');
			*port_end = 0;
			int port = atoi(port_start);

			int i;

			pthread_mutex_lock(&mtx);
			for (i = 0; i < enterprises_size(); i++) {
				Enterprise *e = enterprises_get(i);
				if (port == e->port && e->alive == 1) {
					e->alive = 0;
					break;
				}
			}
			pthread_mutex_unlock(&mtx);

			print(1, "Se ha desconectado el Enterprise en el puerto [%d]\n", port);

			send_packet(fd, 2, "[CONOK]", 0, "");
		} else {
			send_packet(fd, 2, "[CONKO]", 0, "");
		}
	} else if (p.type == 7) {
		if (strcmp(p.header, "[UPDATE]") == 0) {
			char* port_start = strchr(p.data, '_') + 1;
			char* port_end = strchr(port_start, '&');
			*port_end = 0;
			int port = atoi(port_start);

			char* num_end = strchr(port_end + 1, ']');
			*num_end = 0;
			int n = atoi(port_end + 1);

			int i;
			pthread_mutex_lock(&mtx);
			for (i = 0; i < enterprises_size(); i++) {
				Enterprise* e = enterprises_get(i);
				if (port == e->port && e->alive == 1) {
					e->users_connected = n;
					break;
				}
			}
			enterprises_order();
			pthread_mutex_unlock(&mtx);
		} else {
			send_packet(fd, 7, "[UPDATEKO]", 0, "");
		}
	}
}

/***********************************************************************
*
* @Nombre: _listen_enterprise
* @Def: función de thread que escucha las conexiones con Enterprise
* @Arg: In: arg = no usado
* @Ret: NULL
************************************************************************/
void* _listen_enterprise(void* arg) {
	if (arg == NULL) {};
	while (alive) {
		struct sockaddr_in con;
		socklen_t ent_len = sizeof(con);

		ent = accept(sock, (void *) &con, &ent_len);
		if (ent < 0) {
			if (ent != -1) print(2, "[Error de conexion (Enterprise)]\n");
		} else {
			Packet p;
			size_t size = read_packet(ent, &p);
			if (size > 0) {
				//se ha leido una trama correctamente
				//print(1, "%d\n%s\n%d\n%s\n=========\n", p.type, p.header, p.length, p.data);
				_handle_packet(ent, p);

				/*int i = 0;
				for (; i < enterprises_size(); i++) {
					Enterprise* e = enterprises_get(i);
					//print(1, "IP: %s\nNOMBRE: %s\n, USERS: %d\n", e->ip, e->nombre, e->users_connected);
				}*/

				free(p.data); //TO-DO

				close(ent);
			} else {
				print(2, "[Error de lectura de paquete (Enterprise)]\n");
				
				close(ent);
			}
		}
	}
	return NULL;
}

/***********************************************************************
*
* @Nombre: network_ent_init
* @Def: inicializa el modulo de conectividad con Enterprise(s)
* @Arg: In: ip = ip en la cual espear conexiones de Enterprise
			port = puerto en el cual escuchar las conexiones de Enterprise
* @Ret: 0 si todo se ha creado correctamente, un numero en caso de que no
************************************************************************/
int network_ent_init(char* ip, int port) {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (inet_aton(ip, &addr.sin_addr) == 0) {
		return 5;
	}

	if (bind(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 2;
	}

	if (listen(sock, 5) < 0) {
		close(sock);
		return 3;
	}

	alive = 1;
	if (pthread_create(&pid, NULL, _listen_enterprise, NULL) != 0) {
		alive = 0;
		close(sock);
		return 4;
	}

	/*tenemos que poner un timout al socket de escucha o si no cuando haya un SIGINT
	no se detiene el thread, un timeout de 3 segundos lo vemos apropiado
	*/
	struct timeval timeout; //tiempo del tiemout
	timeout.tv_sec = 2; //segundos
	timeout.tv_usec = 0; //microsegundos

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

	return 0;
}

/***********************************************************************
*
* @Nombre: network_ent_end
* @Def: finaliza el modulo, cierra todas las conexiones activas, finaliza
		los threads, liberar recursos
* @Arg: -
* @Ret: -
************************************************************************/
int network_ent_end() {
	alive = 0;
	pthread_join(pid, NULL);
	close(sock);
	close(ent);
	return 0;
}