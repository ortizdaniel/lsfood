/***********************************************************************
*
* @Archivo:	network.h network.c
* @Finalidad: conectividad
* @Autor: Daniel y Elena
* @Fecha: 28/10/17
*
************************************************************************/

#include "network.h"

/***********************************************************************
*
* @Nombre: read_packet
* @Def: lee un paquete del servidor con el cual está conectado
* @Arg: Out: p = puntero al paquet de salida
* @Ret: numero de bytes leidos
************************************************************************/
size_t read_packet(int sock, Packet* p) {
	ssize_t leidos = 0;
	ssize_t tmp = 0;
	if ((tmp = read(sock, &p->type, MAX_TYPE)) <= 0) {
		//print(1, "TMP: %d\n", tmp);
		return -1;
	}
	leidos += tmp;
	
	memset(p->header, 0, MAX_HEADER_FAKE);
	if ((tmp = read(sock, p->header, MAX_HEADER_REAL)) < MAX_HEADER_REAL) {
		//print(1, "TMP: %d\n", tmp);
		return -2;
	}
	//char* ptr = strchr(p->header, ' ');
	//if (ptr != NULL) *ptr = '\0';
	leidos += tmp;
	
	char low, high;
	if ((tmp = read(sock, &high, 1)) <= 0) {
		//print(1, "TMP: %d\n", tmp);
		return -3;
	}
	
	leidos += tmp;
	if ((tmp = read(sock, &low, 1)) <= 0) {
		//print(1, "TMP: %d\n", tmp);
		return -4;
	}
	
	leidos += tmp;
	p->length = (high << 8) | low;

	/*if ((tmp = read(sock, &p->length, MAX_LENGTH)) <= 0) {
		return 0;
	}
	leidos += tmp;*/

	p->data = (char *) calloc(p->length + 1, sizeof(char));
	if (p->data == NULL) {
		//print(1, "TMP: %d\n", tmp);
		return -5;
	}

	if ((tmp = read(sock, p->data, p->length)) < p->length) {
		//print(1, "TMP: %d\n", tmp);
		return -6;
	}
	leidos += tmp;
	return leidos;
}

/***********************************************************************
*
* @Nombre: send_packet
* @Def: funcion para enviar un paquete
* @Arg: In: argumentos de la trama
* @Ret: numero de bytes escritos por el socket
************************************************************************/
ssize_t send_packet(int sfd, uint8_t type, char* header, uint16_t length, char* data) {
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