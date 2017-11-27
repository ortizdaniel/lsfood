/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 22/11/17
*
************************************************************************/

#include "networkpic.h"

static int sock;
static int alive = 0;
static pthread_t pid;

extern int n_users;
extern Lista l;

/***********************************************************************
*
* @Nombre: _handle_packet
* @Def: gestiona un paquete que ha enviado un Picard
* @Arg: In: pic - file descriptor del Picard en caso de tener que enviar
			algo
			p - el paquete que envio Picard
* @Ret: un numero dependiendo de la acción que se realizó
************************************************************************/
int _handle_packet(int pic, Packet p) {
	//printf("0x%.2X\n%s\n%d\n%s\n", p.type, p.header, p.length, p.data);
	if (p.type == 2) {
		if (strcmp(p.header, "[PIC_NAME]") == 0) {
			char* name = strchr(p.data, '_') + 1;
			*strchr(p.data, ']') = 0;
			send_packet(pic, 2, "[CONOK]", 0, "");
			print(1, "Desconnectant %s\n", name);
			n_users--;
			return 1;
		} else {
			send_packet(pic, 2, "[CONKO]", 0, "");
		}
	} else {
		print(1, "Recibi otro paquete...\n");
		send_packet(pic, 111, "", 0, "");
	}
	return 0;
}

/***********************************************************************
*
* @Nombre: _pic_handler
* @Def: function de thread dedicado para gestionar un Picard
* @Arg: In: arg - file descriptor del Picard
* @Ret: NULL siempre
************************************************************************/
void* _pic_handler(void* arg) {
	//NOTA: HAY QUE CAMBIAR ESTO, PARA LA FASE 3 SOLO ESTÁ HECHO QUE RESPONDE
	//A UNA SOLICITUD DE DESCONEXIÓN DE PICARD

	int pic = (intptr_t) arg;
	struct timeval timeout; //tiempo del tiemout
	timeout.tv_sec = 3; //segundos
	timeout.tv_usec = 0; //microsegundos
	setsockopt(pic, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

	//setsockopt(pic, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	//print(1, "PIC: %d\n", pic);
	while (1) {
		Packet p;
		//print(1, "Esperando paquete...\n");
		int n = read_packet(pic, &p);
		//print(1, "READ: %d %s\n", n, strerror(errno));
		if (n > 0) {
			//printf("0x%.2X\n%s\n%d\n%s\n", p.type, p.header, p.length, p.data);
			if (_handle_packet(pic, p)) {
				close(pic);
				break;
			}
			free(p.data);
		}
	}
	return NULL;
}

/***********************************************************************
*
* @Nombre: _listen_thread
* @Def: function de thread que acepta conexiones de Picards y crea más
		threads dedicados cuando se acepta una conexión
* @Arg: In: arg - no usado
* @Ret: NULL siempre
************************************************************************/
void* _listen_thread(void* arg) {
	if (arg == NULL) {};
	print(1, "Esperant clients...\n");
	while (alive) {
		struct sockaddr_in con;
		socklen_t pic_len = sizeof(con);

		int pic = accept(sock, (void *) &con, &pic_len);
		if (pic < 0) {
			if (pic != -1) print(2, "[Error de conexion (Picard)]\n");
		} else {
			Packet p;
			size_t size = read_packet(pic, &p);
			if (size > 0) {
				//se ha leido una trama correctamente
				//printf("0x%.2X\n%s\n%d\n%s\n", p.type, p.header, p.length, p.data);
				if (p.type == 0x01 && strcmp(p.header, "[PIC_INF]") == 0) {
					char* name = strchr(p.data, '_') + 1;
					*strchr(name, '&') = 0;
					print(1, "Connectat %s\n", name);
					send_connect_ok(pic);

					//print(1, "PIC: %d\n", pic);
					LISTA_anadir(&l, pic);

					//new thread
					//

					pthread_t id;
					pthread_create(&id, NULL, _pic_handler, (void *) (intptr_t) pic);
					n_users++;
				} else {
					send_connect_ko(pic);
				}
				free(p.data); //TO-DO
				//close(pic);
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
int net_pic_init(const char* ip, int port) {
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
* @Nombre: send_connect_ok
* @Def: envia una trama de ok conexión a un picard
* @Arg: In: fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ok(int fd) {
	return send_packet(fd, 1, "[CONOK]", 0, "");
}

/***********************************************************************
*
* @Nombre: send_connect_ko
* @Def: envia una trama de ko conexión a un picard
* @Arg: In: fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ko(int fd) {
	return send_packet(fd, 1, "[CONKO]", 0, "");
}