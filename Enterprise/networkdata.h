/***********************************************************************
*
* @Archivo:	networkdata.h networkdata.c
* @Finalidad: 	proporcionar herramientas para comunicarse facilmente con
				Data
* @Autor: Daniel y Elena
* @Fecha: 19/11/17
*
************************************************************************/

#ifndef _NETWORK_DATA_H
#define _NETWORK_DATA_H

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
int send_connect_data(char* name, char* my_ip, int my_port, char* data_ip, int data_port);

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
int send_update_data(char* data_ip, int data_port, int my_port, int n_users);

/***********************************************************************
*
* @Nombre: send_disconnect_data
* @Def: se conecta y envia la trama de desconexión a Data
* @Arg: In: my_port = puerto del Enterprise
			data_ip = ip de Data con la cual conectarse
			data_port = puerto de Data
* @Ret: 0 si todo se ha hecho correctamente, un numero en caso de que no
************************************************************************/
int send_disconnect_data(char* data_ip, int data_port, int my_port);

#endif