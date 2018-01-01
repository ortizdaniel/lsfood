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

extern Plato** platos;
extern int n_platos;
extern int n_users;
extern Lista picards;
extern Lista_Lista reservas;
extern pthread_mutex_t mtx_platos;
extern pthread_mutex_t mtx_picards;
extern pthread_mutex_t mtx_reservas;
extern pthread_mutex_t mtx_users;

void _gestiona_demana(int pic, int* dineros, Lista_Reserva* pedidos, char* nombre, int cantidad);
void _gestiona_elimina(int pic, int* dineros, Lista_Reserva* pedidos, char* nombre, int cantidad, int send);

/***********************************************************************
*
* @Nombre: _handle_packet
* @Def: gestiona un paquete que ha enviado un Picard
* @Arg: In: pic - file descriptor del Picard en caso de tener que enviar
			algo
			p - el paquete que envio Picard
* @Ret: 1 si se desconectó, 0 en el resto de casos
************************************************************************/
int _handle_packet(int pic, Packet p) {
	//printf("0x%.2X\n%s\n%d\n%s\n", p.type, p.header, p.length, p.data);
	static __thread int dineros = 0; //dinero del picard
	static __thread Lista_Reserva pedidos; //lista para guardar los pedidos que realiza
	if (p.type == DESCONEXION) { //Desconexion
		if (strcmp(p.header, "[PIC_NAME]") == 0) {
			char* name = strchr(p.data, '_') + 1;
			*strchr(p.data, ']') = 0;
			send_packet(pic, DESCONEXION, "[CONOK]", 0, "");
			print(1, "Desconnectant %s\n", name);
			pthread_mutex_lock(&mtx_users);
			n_users--;
			pthread_mutex_unlock(&mtx_users);

			if (!L_RESERVA_vacia(pedidos)) {
				L_RESERVA_irPrincipio(&pedidos);
				while (!L_RESERVA_final(pedidos)) {
					char* nombre;
					int cant;
					L_RESERVA_consultar(pedidos, &nombre, &cant, NULL);
					_gestiona_elimina(pic, &dineros, &pedidos, nombre, cant, 0);
				}
			}
			return 1;
		} else {
			send_packet(pic, DESCONEXION, "[CONKO]", 0, "");
		}
	} else if (p.type == CONEXION) { //Conexion
		if (strcmp(p.header, "[PIC_INF]") == 0) {
			char* name = strchr(p.data, '_') + 1;
			char* end_name = strchr(name, '&');
			*end_name = 0;
			char* money = strchr(end_name + 1, '_') + 1;
			*strchr(money, ']') = 0;
			dineros = atoi(money);
			print(1, "Connectat %s - Dinero: %d\n", name, dineros);
			send_connect_ok(pic);
					//print(1, "PIC: %d\n", pic);
			pthread_mutex_lock(&mtx_picards);
			LISTA_anadir(&picards, pic);
			pthread_mutex_unlock(&mtx_picards);
					//new thread
			pthread_mutex_lock(&mtx_users);
			n_users++;
			pthread_mutex_unlock(&mtx_users);

			L_RESERVA_crea(&pedidos);

			pthread_mutex_lock(&mtx_reservas);
			L_LISTA_anadir(&reservas, &pedidos);
			pthread_mutex_unlock(&mtx_reservas);
		} else {
			send_connect_ko(pic);
		}
	} else if (p.type == SHOW_MENU) {
		if (strcmp(p.header, "[SHW_MENU]") == 0) {
			int i;
			print(1, "Enviando menu...\n");
			pthread_mutex_lock(&mtx_platos);
			for (i = 0; i < n_platos; i++) {
				Plato* plato = platos[i];
				char *data;
				asprintf(&data, "[%s&%d&%d]", plato->nombre, plato->precio, plato->cantidad);
				send_packet(pic, SHOW_MENU, "[DISH]", strlen(data), data);
			}
			pthread_mutex_unlock(&mtx_platos);
			send_packet(pic, SHOW_MENU, "[END_MENU]", 0, "");
			print(1, "Menu enviado.\n");
		}
	} else if (p.type == DEMANA || p.type == ELIMINA) {
		if ((strcmp(p.header, "[NEW_ORD]") == 0 && p.type == DEMANA) || (strcmp(p.header, "[DEL_ORD]") == 0 && p.type == ELIMINA)) {
			char* units = strchr(p.data, '&');
			*units++ = 0;
			*strchr(units, ']') = 0;
			if (p.type == DEMANA) {
				_gestiona_demana(pic, &dineros, &pedidos, p.data + 1, atoi(units));
			} else if (p.type == ELIMINA) {
				_gestiona_elimina(pic, &dineros, &pedidos, p.data + 1, atoi(units), 1);
			}
		} else {
			send_packet(pic, p.type == DEMANA ? DEMANA : ELIMINA, "[ORDKO]", strlen(ERROR_TRAMA), ERROR_TRAMA);
		}
	} else if (p.type == PAGAR) {
		if (strcmp(p.header, "[PAY]") == 0) {
			int total = 0;
			int subtotal;
			L_RESERVA_irPrincipio(&pedidos);	
			while (!L_RESERVA_final(pedidos)) {
				L_RESERVA_consultar(pedidos, NULL, NULL, &subtotal);
				total += subtotal;
				L_RESERVA_eliminar(&pedidos);
				//L_RESERVA_avanzar(pedidos);
			}
			char* data;
			asprintf(&data, "[%d]", total);
			send_packet(pic, PAGAR, "[PAYOK]", strlen(data), data);
			print(1, "Enviando factura: %d euros.\n", total);
		} else {
			send_packet(pic, PAGAR, "[PAYKO]", 0, "");
		}
		
	} else {
		print(1, "Recibi otro paquete...\n");
		send_packet(pic, 111, "", 0, "");
	}
	return 0;
}

/***********************************************************************
*
* @Nombre: _gestiona_demana
* @Def: gestiona una reserva que intenta hacer un Picard
* @Arg: In: pic - file descriptor del Picard para comunicarse
			nombre - nombre del plato
			cantidad - cantidad deseada
		Out: dineros - puntero al dinero del Picard
			pedidos - punto a la lista de reservas de Picard
* @Ret: -
************************************************************************/
void _gestiona_demana(int pic, int* dineros, Lista_Reserva* pedidos, char* nombre, int cantidad) {
	int i, flag = 0;
	pthread_mutex_lock(&mtx_platos);
	for (i = 0; i < n_platos; i++) {
		Plato* plato = platos[i];
		if (strcmpign(plato->nombre, nombre) == 0) {
			flag = 1;
			if (cantidad <= plato->cantidad) {
				int subtotal = cantidad * plato->precio;
				if (subtotal <= *dineros) {
					//hacer reserva, restar cantidad de comida, restar dinero
					int flag2 = 0;
					char* nombre_consulta;
					int cantidad_consulta, subtotal_consulta;
					L_RESERVA_irPrincipio(pedidos);
					while (!L_RESERVA_final(*pedidos)) {
						L_RESERVA_consultar(*pedidos, &nombre_consulta, &cantidad_consulta, &subtotal_consulta);
						if (strcmpign(nombre_consulta, nombre) == 0) {
							flag2 = 1;
							break;
						}
						L_RESERVA_avanzar(pedidos);
					}
					if (flag2 == 0) { //no se ha pedido anteriormente
						L_RESERVA_anadir(pedidos, nombre, cantidad, subtotal);
						*dineros -= subtotal;
					} else if (flag2 == 1) { //se ha pedido antes
						int nueva_cantidad = cantidad_consulta + cantidad;
						int nuevo_subtotal = subtotal_consulta + cantidad * plato->precio;
						*dineros -= cantidad * plato->precio;
						L_RESERVA_eliminar(pedidos);
						L_RESERVA_anadir(pedidos, nombre, nueva_cantidad, nuevo_subtotal);
					}
					plato->cantidad -= cantidad;
					send_packet(pic, DEMANA, "[ORDOK]", strlen(RESERVA_CORRECTA), RESERVA_CORRECTA);
					print(1, "Reserva correcta %d %s\n", cantidad, nombre);
				} else {
					print(1, "No te alcanza el dinero\n");
					send_packet(pic, DEMANA, "[ORDKO]", strlen(SIN_DINERO), SIN_DINERO);
				}
			} else {
				print(1, "No hay stock\n");
				send_packet(pic, DEMANA, "[ORDKO]", strlen(SIN_STOCK), SIN_STOCK);
			}
		}
	}
	pthread_mutex_unlock(&mtx_platos);
	if (flag == 0) {
		send_packet(pic, DEMANA, "[ORDKO]", strlen(PLATO_INVALIDO), PLATO_INVALIDO);
	}
}

/***********************************************************************
*
* @Nombre: _gestiona_elimina
* @Def: gestiona una reserva que intenta eliminar un Picard
* @Arg: In: pic - file descriptor del Picard para comunicarse
			nombre - nombre del plato
			cantidad - cantidad deseada
			send - indica si se ha de enviar una respuesta o no al Picard
		Out: dineros - puntero al dinero del Picard
			pedidos - punto a la lista de reservas de Picard
* @Ret: -
************************************************************************/
void _gestiona_elimina(int pic, int* dineros, Lista_Reserva* pedidos, char* nombre, int cantidad, int send) {
	int i, flag = 0;
	char* mensaje;
	L_RESERVA_irPrincipio(pedidos);
	while (!L_RESERVA_final(*pedidos)) {
		char* nombre_consulta;
		int cantidad_consulta;
		L_RESERVA_consultar(*pedidos, &nombre_consulta, &cantidad_consulta, NULL);
		if (strcmpign(nombre_consulta, nombre) == 0) {
			if (cantidad <= cantidad_consulta) {
				flag = 1;
				pthread_mutex_lock(&mtx_platos);
				for (i = 0; i < n_platos; i++) {
					Plato* plato = platos[i];
					if (strcmpign(plato->nombre, nombre_consulta) == 0) {
						plato->cantidad += cantidad;
						int nueva_cantidad = cantidad_consulta - cantidad;
						int nuevo_subtotal = nueva_cantidad * plato->precio;
						*dineros += cantidad * plato->precio;
						L_RESERVA_eliminar(pedidos);
						if (nueva_cantidad > 0) {
							L_RESERVA_anadir(pedidos, nombre, nueva_cantidad, nuevo_subtotal);
						}
						mensaje = "Cantidad eliminada correctamente.";
						print(1, "Eliminado correctamente %d %s\n", cantidad, nombre);
						break;
					} 
				}
				pthread_mutex_unlock(&mtx_platos);
			} else {
				flag = 2;
				mensaje = "No has pedido tanta cantidad.";
			}
			break;
		}
		L_RESERVA_avanzar(pedidos);
	}
	if (flag == 0) {
		mensaje = "El plato indicado no lo has pedido o no existe.";
	}
	if (send) send_packet(pic, ELIMINA, flag == 1 ? "[ORDOK]" : "[ORDKO]", strlen(mensaje), mensaje);
}

/***********************************************************************
*
* @Nombre: _pic_handler
* @Def: function de thread dedicado para gestionar un Picard
* @Arg: In: arg - file descriptor del Picard
* @Ret: NULL siempre
************************************************************************/
void* _pic_handler(void* arg) {
	int pic = (intptr_t) arg;
	while (1) {
		Packet p;
		int n = read_packet(pic, &p);
		if (n > 0) {
			if (_handle_packet(pic, p) == 1) { //si se desconectó
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
void* _listen_thread() {
	print(1, "Esperant clients...\n");
	while (alive) {
		struct sockaddr_in con;
		socklen_t pic_len = sizeof(con);

		int pic = accept(sock, (void *) &con, &pic_len);
		if (pic < 0) {
			if (pic != -1) print(2, "[Error de conexion (Picard)]\n");
		} else {
			pthread_t id;
			pthread_create(&id, NULL, _pic_handler, (void *) (intptr_t) pic);
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
inline int send_connect_ok(int fd) {
	return send_packet(fd, CONEXION, "[CONOK]", 0, "");
}

/***********************************************************************
*
* @Nombre: send_connect_ko
* @Def: envia una trama de ko conexión a un picard
* @Arg: In: fd = file descriptor del picard
* @Ret: numero de bytes totales escritos
************************************************************************/
inline int send_connect_ko(int fd) {
	return send_packet(fd, CONEXION, "[CONKO]", 0, "");
}

/***********************************************************************
*
* @Nombre: net_pic_end
* @Def: cierra el socket para aceptar conexiones de Picard y detiene el
		thread
* @Arg: -
* @Ret: -
************************************************************************/
void net_pic_end() {
	alive = 0;
	close(sock);
}

/***********************************************************************
*
* @Nombre: avisar_caida
* @Def: avisa a todos los Picards conectados que el servidor Enterprise
		está deteniendo su ejecución
* @Arg: -
* @Ret: -
************************************************************************/
void avisar_caida() {
	LISTA_irPrincipio(&picards);
	while (!LISTA_final(picards)) {
		send_packet(LISTA_consultar(picards), DESCONEXION, "[CONKO]", 0, "");
		LISTA_avanzar(&picards);
	}
}