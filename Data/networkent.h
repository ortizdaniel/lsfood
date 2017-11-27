/***********************************************************************
*
* @Archivo:	networkpic.h networkpic.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Picards
* @Autor: Daniel y Elena
* @Fecha: 19/11/17
*
************************************************************************/

#ifndef _NETWORK_ENT_H
#define _NETWORK_ENT_H

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
* @Nombre: network_ent_init
* @Def: inicializa el modulo de conectividad con Enterprise(s)
* @Arg: In: ip = ip en la cual espear conexiones de Enterprise
			port = puerto en el cual escuchar las conexiones de Enterprise
* @Ret: 0 si todo se ha creado correctamente, un numero en caso de que no
************************************************************************/
int network_ent_init(char* ip, int port);

/***********************************************************************
*
* @Nombre: network_ent_end
* @Def: finaliza el modulo, cierra todas las conexiones activas, finaliza
		los threads, liberar recursos
* @Arg: -
* @Ret: -
************************************************************************/
int network_ent_end();

#endif