/***********************************************************************
*
* @Archivo:	network.h network.c
* @Finalidad: conectividad
* @Autor: Daniel y Elena
* @Fecha: 28/10/17
*
************************************************************************/

#include "network.h"

int connected = 0;
int sock = -1;

extern Config c;

ssize_t _send_packet(int sfd, uint8_t type, char* header, uint16_t length, char* data);

/***********************************************************************
*
* @Nombre: net_connect
* @Def: se intenta conectar contra un servidor en una IP y puerto remoto
* @Arg: In: ip = direccion ip
* 			port = puerto
* @Ret: 0 si se conecto correctamente, un numero si no
************************************************************************/
int net_connect(char* ip, int port) {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (inet_aton(ip, &addr.sin_addr) == 0) {
		close(sock);
		return 2;
	}

	if (connect(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 3;
	}

	connected = 1;
	return 0;
}

/***********************************************************************
*
* @Nombre: is_connected
* @Def: dice si se esta conectado contra algun servidor
* @Arg: -
* @Ret: 1 si esta conectado, 0 si no
************************************************************************/
int is_connected() {
	return connected;
}

/***********************************************************************
*
* @Nombre: net_disconnect
* @Def: cierra la conexion (socket)
* @Arg: -
* @Ret: -
************************************************************************/
void net_disconnect() {
	//print(1, "Desconectando...\n");
	connected = 0;
	close(sock);
}

/***********************************************************************
*
* @Nombre: _send_packet
* @Def: funcion privada para enviar un paquete
* @Arg: In: argumentos de la trama
* @Ret: numero de bytes escritos por el socket
************************************************************************/
ssize_t _send_packet(int sfd, uint8_t type, char* header, uint16_t length, char* data) {
	char* buffer = (char *) malloc (SIZE_NO_DATA + strlen(data));
	if (buffer == NULL) {
		return 0;
	}
	int size = sprintf(buffer, "%c%-10s%c%c%s", type, header, (length >> 8) & 0xFF, length & 0xFF, data);
	unsigned int i; //guarradas
	for (i = 1; i < SIZE_NO_DATA; i++) {
		if (buffer[i] == ' ') buffer[i] = '\0';
	}
	int bytes = write(sfd, buffer, size);
	free(buffer);
	return bytes;
}

/***********************************************************************
*
* @Nombre: net_handle
* @Def: gestiona la RESPUESTA de un paquete de CONEXION, no de otra cosa
* @Arg: In: p = paquete
* @Ret: 0, muestra por terminal los errores o demás
************************************************************************/
int net_handle(Packet p) {
	char* port_start = strchr(p.data, '&');
	port_start = strchr(port_start, '_') + 1;
	char* port_end = strchr(port_start, '&');
	*port_end = 0;

	char* ip_start = strchr(port_end + 1, '_') + 1;
	char* ip_end = strchr(ip_start, ']');
	*ip_end = 0;
	if (net_connect(ip_start, atoi(port_start)) == 0) {
		char* data;
		asprintf(&data, "[PICARD_%s&PICARD_%d]", c.nombre, c.dinero);
		_send_packet(sock, TYPE_CONEXION, "[PIC_INF]", strlen(data), data);
		free(data);

		Packet p;
		if (read_packet(&p) > 0) {
			if (p.type == TYPE_CONEXION && strcmp(p.header, "[CONOK]") == 0) {
				print(1, "[Connexió amb Enterprise OK]\n");
			}  else {
				print(1, "[Connexió amb Enterprise KO]\n");
				net_disconnect();
			}
		} else {
			print(1, "[Connexió amb Enterprise KO]\n");
			net_disconnect();
		}
		free(p.data);
	} else {
		print(1, "[Connexió amb Enterprise KO]\n");
		net_disconnect();
	}
	return 0;
}

/***********************************************************************
*
* @Nombre: send_connect
* @Def: envia la trama de conexion
* @Arg: In: name = nombre del Picard
* @Ret: 0 si hubo error, si no, numero de bytes totales enviados
************************************************************************/
int send_connect(char* name) {
	char* data = (char*) malloc(9 + strlen(name));
	if (data == NULL) return 0;
	return _send_packet(sock, TYPE_CONEXION, HEADER_PIC_NAME, sprintf(data, "[PICARD_%s]", name), data);
}

/***********************************************************************
*
* @Nombre: read_packet
* @Def: lee un paquete del servidor con el cual está conectado
* @Arg: Out: p = puntero al paquet de salida
* @Ret: numero de bytes leidos
************************************************************************/
size_t read_packet(Packet* p) {
	ssize_t leidos = 0;
	ssize_t tmp = 0;
	if ((tmp = read(sock, &p->type, MAX_TYPE)) <= 0) {
		return 0;
	}
	leidos += tmp;

	memset(p->header, 0, MAX_HEADER_FAKE);
	if ((tmp = read(sock, p->header, MAX_HEADER_REAL)) < MAX_HEADER_REAL) {
		return 0;
	}

	leidos += tmp;

	char low, high;
	if ((tmp = read(sock, &high, 1)) <= 0) {
		return 0;
	}
	leidos += tmp;
	if ((tmp = read(sock, &low, 1)) <= 0) {
		return 0;
	}
	leidos += tmp;
	p->length = (high << 8) | low;

	p->data = (char *) calloc(p->length + 1, sizeof(char));
	if (p->data == NULL) {
		return 0;
	}

	if ((tmp = read(sock, p->data, p->length)) < p->length) {
		return 0;
	}
	leidos += tmp;
	return leidos;
}

/***********************************************************************
*
* @Nombre: net_end
* @Def: envia la trama de desconexion y cierra la
		conexion
* @Arg: -
* @Ret: -
************************************************************************/
int net_end() {
	char* data;
	asprintf(&data, "[PICARD_%s]", c.nombre);
	int bytes = _send_packet(sock, TYPE_DESCONEXION, "[PIC_NAME]", strlen(data), data);
	print(1, "[Desconnecta Enterprise OK]\n");
	free(data);
	return bytes;
}

/***********************************************************************
*
* @Nombre: net_verify_alive
* @Def: verifica si el Enterprise al que se está conectado ha enviado
		una trama de desconexion avisando que se ha caido
* @Arg: -
* @Ret: 0 si se cayó Enterprise, 1 si no
************************************************************************/
int net_verify_alive() {
	int bytes;
	ioctl(sock, FIONREAD, &bytes);
	if (bytes > 0) {
		Packet p;
		read_packet(&p);
		if (p.type == TYPE_DESCONEXION && strcmp(p.header, "[CONKO]") == 0) {
			print(1, "[Servidor Enterprise se ha caido! Deteniendo ejecución...]\n");
			return 0;
		}
		free(p.data);
	}
	return 1;
}

/***********************************************************************
*
* @Nombre: net_ask_menu
* @Def: le pide al Enterprise que envie el menu, lo recibe y muestra por
		pantalla
* @Arg: -
* @Ret: -
************************************************************************/
void net_ask_menu() {
	_send_packet(sock, TYPE_SHOW_MENU, "[SHW_MENU]", 0, "");
	Packet p;
	print(1, "MENU DISPONIBLE\n");
	int flag = 0;
	while (1) {
		if (read_packet(&p) > 0 && p.type == TYPE_SHOW_MENU) {
			if (strcmp(p.header, "[DISH]") == 0) {
				char* precio = strchr(p.data, '&');
				*precio++ = 0;
				//p.data + 1 = nombre
				char* stock = strchr(precio + 1, '&');
				*stock++ = 0;
				*strchr(stock, ']') = 0;
				if (atoi(stock) != 0) {
					print(1, "%s (%s euros)\n", p.data + 1, precio);
					flag = 1;
				}
			} else if (strcmp(p.header, "[END_MENU]") == 0) {
				break;
			}
		}
		free(p.data);
	}
	if (flag == 0) {
		print(1, "No hay ningun plato disponible...\n");
	}
}

/***********************************************************************
*
* @Nombre: net_demana
* @Def: intenta realizar una reserva al restaurante Enterprise
* @Arg: In: nombre de plato, cantidad deseada
* @Ret: -
************************************************************************/
void net_demana(char* nombre, int cantidad) {
	char* data;
	asprintf(&data, "[%s&%d]", nombre, cantidad);
	_send_packet(sock, TYPE_DEMANA, "[NEW_ORD]", strlen(data), data);
	Packet p;
	if (read_packet(&p) > 0) {
		print(1, "%s\n", p.data);
		free(p.data);
	}
}

/***********************************************************************
*
* @Nombre: net_elimina
* @Def: intenta eliminar una reserva o parte de una al restaurante Enterprise
* @Arg: In: nombre de plato, cantidad deseada
* @Ret: -
************************************************************************/
void net_elimina(char* nombre, int cantidad) {
	char* data;
	asprintf(&data, "[%s&%d]", nombre, cantidad);
	_send_packet(sock, TYPE_ELIMINA, "[DEL_ORD]", strlen(data), data);
	Packet p;
	if (read_packet(&p) > 0) {
		print(1, "%s\n", p.data);
		free(p.data);
	}
}

/***********************************************************************
*
* @Nombre: net_pagar
* @Def: avisa a Enterprise de que se quiere pagar
* @Arg: -
* @Ret: -
************************************************************************/
void net_pagar() {
	_send_packet(sock, TYPE_PAGAR, "[PAY]", 0, "");
	Packet p;
	if (read_packet(&p) > 0) {
		*strchr(p.data, ']') = 0;
		print(1, "Son %s euros. Carregat en el seu compte.\n", p.data + 1);
		free(p.data);
	}
}