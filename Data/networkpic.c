/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 29/10/17
*
************************************************************************/

#include "networkpic.h"
#include "util.h"

static int sock = -1;
static int pic = -1;
static int alive = 0;
static pthread_t pid;
extern pthread_mutex_t mtx;

/***********************************************************************
*
* @Nombre: _listen_thread
* @Def: función de thread que escucha las conexiones con Picards, los trata
			y les responde diciendo a cual Enterprise pueden (o no)
			conectarse
* @Arg: In: arg = no usado
* @Ret: NULL
************************************************************************/
void* _listen_thread() {
	while (alive) {
		struct sockaddr_in con;
		socklen_t pic_len = sizeof(con);

		pic = accept(sock, (void *) &con, &pic_len);
		if (pic < 0) {
			if (pic != -1) print(2, "[Error de conexion (Picard)]\n");
		} else {
			Packet p;
			size_t size = read_packet(pic, &p);
			if (size > 0) {
				//se ha leido una trama correctamente
				//printf("0x%.2X\n%s\n%d\n%s\n", p.type, p.header, p.length, p.data);
				if (p.type == CONEXION && strcmp(p.header, "[PIC_NAME]") == 0) {
					char* name = strchr(p.data, '_') + 1;
					*strchr(name, ']') = 0;
					print(1, "Connectant %s\n", name);
					send_connect_ok(pic);
					print(1, "Desconnectant %s\n", name);
				} else {
					send_connect_ko(pic);
				}
				free(p.data); //TO-DO
				close(pic);
			} else {
				print(2, "[Error de lectura de paquete (Picard)]\n");
				send_connect_ko(pic);
				close(pic);
			}
		}
	}
	return NULL;
}

/***********************************************************************
*
* @Nombre: net_pic_init
* @Def: inicializa el modulo de conectividad con Picard(s)
* @Arg: In: ip = ip en la cual espear conexiones de Picard
			port = puerto en el cual escuchar las conexiones de Picard
* @Ret: 0 si todo se ha creado correctamente, un numero en caso de que no
************************************************************************/
int network_pic_init(char* ip, int port) {
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
	if (pthread_create(&pid, NULL, _listen_thread, NULL) != 0) {
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
* @Nombre: net_pic_end
* @Def: finaliza el modulo, cierra todas las conexiones activas, finaliza
		los threads, liberar recursos
* @Arg: -
* @Ret: -
************************************************************************/
int network_pic_end() {
	alive = 0;
	pthread_join(pid, NULL);
	close(sock);
	close(pic);
	return 0;
}

/***********************************************************************
*
* @Nombre: send_connect_ok
* @Def: envia una trama de ok conexión a un picard
* @Arg: In: pic_fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ok(int pic_fd) {
	Enterprise* dest = NULL;
	int i;
	pthread_mutex_lock(&mtx);
	for (i = 0; i < enterprises_size(); i++) {
		Enterprise* aux = enterprises_get(i);
		if (aux->alive == 1) {
			dest = aux;
			aux->users_connected++;
			enterprises_order();
			break;
		}
	}
	pthread_mutex_unlock(&mtx);

	if (dest == NULL) {
		return send_connect_ko(pic_fd);
	} else {
		char* data;
		int bytes = asprintf(&data, "[ENTERPRISE_%s&ENTERPRISE_%d&ENTERPRISE_%s]", \
			dest->nombre, dest->port, dest->ip);
		//print(1, "%s\n", data);
		return send_packet(pic_fd, CONEXION, "[ENT_INF]", bytes, data);
	}
}

/***********************************************************************
*
* @Nombre: send_connect_ko
* @Def: envia una trama de ko conexión a un picard
* @Arg: In: pic_fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ko(int pic_fd) {
	return send_packet(pic_fd, CONEXION, "[CONKO]", 0, "");
}