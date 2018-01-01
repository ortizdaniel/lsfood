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

#define _GNU_SOURCE

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
#include <sys/ioctl.h>

#include "util.h"

//tipos deprecados, no se usan
#define MAX_TYPE sizeof(uint8_t)
#define MAX_HEADER_REAL 10
#define MAX_HEADER_FAKE 11
#define MAX_LENGTH sizeof(uint16_t)
#define SIZE_NO_DATA MAX_TYPE + MAX_HEADER_REAL + MAX_LENGTH

#define TYPE_CONEXION 1
#define TYPE_DESCONEXION 2
#define TYPE_SHOW_MENU 3
#define TYPE_DEMANA 4
#define TYPE_ELIMINA 5
#define TYPE_PAGAR 6
#define HEADER_PIC_NAME "[PIC_NAME]"

typedef struct {
	uint8_t type;
	char header[MAX_HEADER_FAKE];
	uint16_t length;
	char* data;
} Packet;

/***********************************************************************
*
* @Nombre: net_connect
* @Def: se intenta conectar contra un servidor en una IP y puerto remoto
* @Arg: In: ip = direccion ip
* 			port = puerto
* @Ret: 0 si se conecto correctamente, un numero si no
************************************************************************/
int net_connect(char* ip, int port);

/***********************************************************************
*
* @Nombre: send_connect
* @Def: envia la trama de conexion
* @Arg: In: name = nombre del Picard
* @Ret: 0 si hubo error, si no, numero de bytes totales enviados
************************************************************************/
int send_connect(char* name);

/***********************************************************************
*
* @Nombre: net_handle
* @Def: gestiona la RESPUESTA de un paquete de CONEXION, no de otra cosa
* @Arg: In: p = paquete
* @Ret: 0, muestra por terminal los errores o demás
************************************************************************/
int net_handle(Packet p);

/***********************************************************************
*
* @Nombre: is_connected
* @Def: dice si se esta conectado contra algun servidor
* @Arg: -
* @Ret: 1 si esta conectado, 0 si no
************************************************************************/
int is_connected();

/***********************************************************************
*
* @Nombre: net_disconnect
* @Def: cierra la conexion (socket)
* @Arg: -
* @Ret: -
************************************************************************/
void net_disconnect();

/***********************************************************************
*
* @Nombre: read_packet
* @Def: lee un paquete del servidor con el cual está conectado
* @Arg: Out: p = puntero al paquet de salida
* @Ret: numero de bytes leidos
************************************************************************/
size_t read_packet(Packet* p);

/***********************************************************************
*
* @Nombre: net_end
* @Def: envia la trama de desconexion y cierra la
		conexion
* @Arg: -
* @Ret: -
************************************************************************/
int net_end();

/***********************************************************************
*
* @Nombre: net_verify_alive
* @Def: verifica si el Enterprise al que se está conectado ha enviado
		una trama de desconexion avisando que se ha caido
* @Arg: -
* @Ret: 0 si se cayó Enterprise, 1 si no
************************************************************************/
int net_verify_alive();

/***********************************************************************
*
* @Nombre: net_ask_menu
* @Def: le pide al Enterprise que envie el menu, lo recibe y muestra por
		pantalla
* @Arg: -
* @Ret: -
************************************************************************/
void net_ask_menu();

/***********************************************************************
*
* @Nombre: net_demana
* @Def: intenta realizar una reserva al restaurante Enterprise
* @Arg: In: nombre de plato, cantidad deseada
* @Ret: -
************************************************************************/
void net_demana(char* nombre, int cantidad);

/***********************************************************************
*
* @Nombre: net_elimina
* @Def: intenta eliminar una reserva o parte de una al restaurante Enterprise
* @Arg: In: nombre de plato, cantidad deseada
* @Ret: -
************************************************************************/
void net_elimina(char* nombre, int cantidad);

/***********************************************************************
*
* @Nombre: net_pagar
* @Def: avisa a Enterprise de que se quiere pagar
* @Arg: -
* @Ret: -
************************************************************************/
void net_pagar();

#endif