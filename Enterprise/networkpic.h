/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 22/11/17
*
************************************************************************/

#ifndef _NETWORK_PIC_H
#define _NETWORK_PIC_H

#define _GNU_SOURCE //necesario para la función asprintf()

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
#include <errno.h>

#include "network.h"
#include "util.h"
#include "lista.h"
#include "lista_lista.h"


/***********************************************************************
*
* @Nombre: net_pic_init
* @Def: inicializa el modulo de conectividad con Picard(s)
* @Arg: In: ip = ip en la cual espear conexiones de Picard
			port = puerto en el cual escuchar las conexiones de Picard
* @Ret: 0 si todo se ha creado correctamente, un numero en caso de que no
************************************************************************/
int net_pic_init(const char* ip, int port);


/***********************************************************************
*
* @Nombre: send_connect_ok
* @Def: envia una trama de ok conexión a un picard
* @Arg: In: fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
inline int send_connect_ok(int fd);

/***********************************************************************
*
* @Nombre: send_connect_ko
* @Def: envia una trama de ko conexión a un picard
* @Arg: In: fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
inline int send_connect_ko(int fd);

/***********************************************************************
*
* @Nombre: net_pic_end
* @Def: cierra el socket para aceptar conexiones de Picard y detiene el
		thread
* @Arg: -
* @Ret: -
************************************************************************/
void net_pic_end();

/***********************************************************************
*
* @Nombre: avisar_caida
* @Def: avisa a todos los Picards conectados que el servidor Enterprise
		está deteniendo su ejecución
* @Arg: -
* @Ret: -
************************************************************************/
void avisar_caida();

#endif