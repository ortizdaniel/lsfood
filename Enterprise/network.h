/***********************************************************************
*
* @Archivo:	network.h network.c
* @Finalidad: conectividad
* @Autor: Daniel y Elena
* @Fecha: 28/10/17
*
************************************************************************/

#ifndef _NETWORK_H
#define _NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//tipos deprecados, no se usan
#define MAX_TYPE sizeof(uint8_t)
#define MAX_HEADER_REAL 10
#define MAX_HEADER_FAKE 11
#define MAX_LENGTH sizeof(uint16_t)
#define SIZE_NO_DATA MAX_TYPE + MAX_HEADER_REAL + MAX_LENGTH

#define CONEXION 1
#define DESCONEXION 2
#define SHOW_MENU 3
#define DEMANA 4
#define ELIMINA 5
#define PAGAR 6
#define UPDATE 7

#define ERROR_TRAMA "Error en la trama."
#define RESERVA_CORRECTA "Reserva hecha correctamente."
#define SIN_DINERO "No hay dinero suficiente."
#define SIN_STOCK "No hay stock suficiente."
#define PLATO_INVALIDO "No existe el plato."

typedef struct {
	uint8_t type;
	char header[MAX_HEADER_FAKE];
	uint16_t length;
	char* data;
} Packet;

/***********************************************************************
*
* @Nombre: send_packet
* @Def: funcion para enviar un paquete
* @Arg: In: argumentos de la trama
* @Ret: numero de bytes escritos por el socket
************************************************************************/
ssize_t send_packet(int sfd, uint8_t type, char* header, uint16_t length, char* data);

/***********************************************************************
*
* @Nombre: read_packet
* @Def: lee un paquete del servidor con el cual está conectado
* @Arg: Out: p = puntero al paquet de salida
* @Ret: numero de bytes leidos
************************************************************************/
size_t read_packet(int sock, Packet* p);

#endif