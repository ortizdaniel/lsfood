/***********************************************************************
*
* @Archivo:	networkdata.h networkdata.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Data
* @Autor: Daniel y Elena
* @Fecha: 19/11/17
*
************************************************************************/

#include "networkdata.h"

/***********************************************************************
*
* @Nombre: send_connect_data
* @Def: se conecta y envia la trama de conexión a Data
* @Arg: In: name = nombre del Enterprise
			my_ip = ip del Enterprise
			my_port = puerto del Enterprise
			data_ip = ip de Data con la cual conectarse
			data_port = puerto de Data
* @Ret: 0 si todo se ha hecho correctamente, un numero en caso de que no
************************************************************************/
int send_connect_data(char* name, char* my_ip, int my_port, char* data_ip, int data_port) {

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(data_port);
	if (inet_aton(data_ip, &addr.sin_addr) == 0) {
		close(sock);
		return 2;
	}

	if (connect(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 3;
	}

	char* data;
	int bytes = asprintf(&data, "[ENTERPRISE_%s&ENTERPRISE_%d&ENTERPRISE_%s]", name, my_port, my_ip);
	send_packet(sock, CONEXION, "[ENT_INF]", bytes, data);
	free(data);

	Packet p;
	if (read_packet(sock, &p) > 0 && p.type == CONEXION && strcmp(p.header, "CONOK") == 0) {
		free(p.data);
		close(sock);
		return 0;
	}
	close(sock);
	return -1;
}

/***********************************************************************
*
* @Nombre: send_update_data
* @Def: se conecta y envia la trama de update a Data
* @Arg: In: n_users = numero de usuarios actual que tiene Enterprise
			my_port = puerto del Enterprise
			data_ip = ip de Data con la cual conectarse
			data_port = puerto de Data
* @Ret: 0 si todo se ha hecho correctamente, un numero en caso de que no
************************************************************************/
int send_update_data(char* data_ip, int data_port, int my_port, int n_users) {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(data_port);
	if (inet_aton(data_ip, &addr.sin_addr) == 0) {
		close(sock);
		return 2;
	}

	if (connect(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 3;
	}

	char* data;
	int bytes = asprintf(&data, "[ENTERPRISE_%d&%d]", my_port, n_users);
	send_packet(sock, UPDATE, "[UPDATE]", bytes, data);
	free(data);

	Packet p;
	if (read_packet(sock, &p) > 0 && p.type == UPDATE && strcmp(p.header, "[UPDATEOK]") == 0) {
		free(p.data);
		close(sock);
		return 0;
	}
	close(sock);
	return -1;
}

/***********************************************************************
*
* @Nombre: send_disconnect_data
* @Def: se conecta y envia la trama de desconexión a Data
* @Arg: In: my_port = puerto del Enterprise
			data_ip = ip de Data con la cual conectarse
			data_port = puerto de Data
* @Ret: 0 si todo se ha hecho correctamente, un numero en caso de que no
************************************************************************/
int send_disconnect_data(char* data_ip, int data_port, int my_port) {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(data_port);
	if (inet_aton(data_ip, &addr.sin_addr) == 0) {
		close(sock);
		return 2;
	}

	if (connect(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 3;
	}

	char* data;
	int bytes = asprintf(&data, "[ENTERPRISE_%d]", my_port);
	send_packet(sock, DESCONEXION, "[ENT_INF]", bytes, data);
	free(data);

	Packet p;
	if (read_packet(sock, &p) > 0 && p.type == DESCONEXION && strcmp(p.header, "[CONOK]") == 0) {
		free(p.data);
		close(sock);
		return 0;
	}
	close(sock);
	return -1;
}