/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 29/10/17
*
************************************************************************/

#ifndef _NETWORK_PICARD_H
#define _NETWORK_PICARD_H

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

#include "network.h"
#include "enterpriselist.h"


/***********************************************************************
*
* @Nombre: net_pic_init
* @Def: inicializa el modulo de conectividad con Picard(s)
* @Arg: In: ip = ip en la cual espear conexiones de Picard
			port = puerto en el cual escuchar las conexiones de Picard
* @Ret: 0 si todo se ha creado correctamente, un numero en caso de que no
************************************************************************/
int network_pic_init(char* ip, int port);

/***********************************************************************
*
* @Nombre: send_connect_ok
* @Def: envia una trama de ok conexión a un picard
* @Arg: In: pic_fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ok(int pic_fd); 

/***********************************************************************
*
* @Nombre: send_connect_ko
* @Def: envia una trama de ko conexión a un picard
* @Arg: In: pic_fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
int send_connect_ko(int pic_fd);

/***********************************************************************
*
* @Nombre: net_pic_end
* @Def: finaliza el modulo, cierra todas las conexiones activas, finaliza
		los threads, liberar recursos
* @Arg: -
* @Ret: -
************************************************************************/
int network_pic_end();

#endif