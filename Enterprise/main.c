#include <signal.h>

#include "util.h"
#include "networkdata.h"
#include "networkpic.h"
#include "lista.h"
#include "lista_lista.h"

Plato** platos;
int n_platos;
Config c;
int n_users = 0;
Lista picards; //lista de file descriptors de picards
Lista_Lista reservas; //lista de lista de los pedidos de cada Picard
pthread_mutex_t mtx_users = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_platos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_picards = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_reservas = PTHREAD_MUTEX_INITIALIZER;

//Cabeceras
void destroy_config(Config* c);
int read_config(const char* path, Config* c);
void handle_int(int n);
Plato** read_food(const char* path, int* n_platos);
void destroy_menu();

int main(int argc, char const *argv[]) {
	if (argc < 3) {
		print(2, "[Número de parametros de entrada inválidos]\n");
		return 1;
	}

	util_init();

	if (read_config(argv[1], &c) == 0) {
		if ((platos = read_food(argv[2], &n_platos)) == NULL) {
			print(2, "[Error al leer fichero de menú]\n");
			destroy_config(&c);
			exit(1);
		}

		print(1, "Carregat menú!\n");
		if (LISTA_crea(&picards) == 0) {
			destroy_config(&c);
			destroy_menu();
			exit(2);
		}

		L_LISTA_crea(&reservas);

		//init cosas
		signal(SIGALRM, handle_int);
		signal(SIGINT, handle_int);
		signal(SIGTERM, handle_int);

		if (send_connect_data(c.nombre, c.ip_pic, c.port_pic, c.ip_data, c.port_data) != 0) {
			print(2, "No se pudo conectar a Data.\n");
			handle_int(SIGINT);
		}

		if (net_pic_init(c.ip_pic, c.port_pic) != 0) {
			print(2, "No se pudo inicializar el socket para clientes.\n");
			handle_int(SIGINT);
		}

		print(1, "Benvingut al restaurant %s\n", c.nombre);
		alarm(c.t_act);
		while (1) pause();
	} else {
		print(2, "[Error de lectura del fichero de configuración]\n");
	}
	return 0;
}

/***********************************************************************
*
* @Nombre: handle_int
* @Def: gestiona las interrupciones
* @Arg: In: n - el identificador de la interrupcion
* @Ret: -
************************************************************************/
void handle_int(int n) {
	if (n == SIGALRM) {
		send_update_data(c.ip_data, c.port_data, c.port_pic, n_users);
		alarm(c.t_act);
		int i;
	} else if (n == SIGINT || n == SIGTERM) {
		avisar_caida();
		destroy_config(&c);
		destroy_menu();
		LISTA_destruir(&picards);
		send_disconnect_data(c.ip_data, c.port_data, c.port_pic);
		L_LISTA_irPrincipio(&reservas);
		while (!L_LISTA_final(reservas)) {
			Lista_Reserva *l = L_LISTA_consultar(reservas);
			L_RESERVA_destruir(l);
			L_LISTA_avanzar(&reservas);
		}
		L_LISTA_destruir(&reservas);
		util_end();
		net_pic_end();
		exit(1);
	}
}

/***********************************************************************
*
* @Nombre: destroy_menu
* @Def: destruye el menú de platos
* @Arg: -
* @Ret: -
************************************************************************/
void destroy_menu() {
	int i;
	for (i = 0; i < n_platos; i++) {
		free(platos[i]->nombre);
		free(platos[i]);
	}
	free(platos);
}

/***********************************************************************
*
* @Nombre: destroy_config
* @Def: destruye la configuración
* @Arg: In: c = configuracion
* @Ret: -
************************************************************************/
void destroy_config(Config* c) {
	free(c->nombre);
}

/***********************************************************************
*
* @Nombre: read_config
* @Def: lee el fichero de configuración
* @Arg: In: path = path al fichero de configuración
		Out: c = configuracion de salida
* @Ret: 0 se todo fue correcto, 1 si falló
************************************************************************/
int read_config(const char* path, Config* c) {
	int fd = open(path, O_RDONLY);
	if (fd < 0) return 1;

	c->nombre = read_until(fd, '\n');
	char* ptr = read_until(fd, '\n');
	c->t_act = atoi(ptr);
	free(ptr);
	ptr = read_until(fd, '\n');
	strcpy(c->ip_data, ptr);
	free(ptr);
	ptr = read_until(fd, '\n');
	c->port_data = atoi(ptr);
	free(ptr);
	ptr = read_until(fd, '\n');
	strcpy(c->ip_pic, ptr);
	free(ptr);
	ptr = read_until(fd, '\n');
	c->port_pic = atoi(ptr);
	free(ptr);
	return 0;
}

/***********************************************************************
*
* @Nombre: read_food
* @Def: lee el fichero de menú
* @Arg: In: path = path al fichero de menú
		Out: n_platos = numero de platos leidos
* @Ret: un puntero a un array de Plato, NULL si falló
************************************************************************/
Plato** read_food(const char* path, int* n_platos) {
	int fd = open(path, O_RDONLY);
	if (fd < 0) return NULL;
	Plato** platos = (Plato**) malloc(sizeof(Plato*));
	*n_platos = 0;
	if (platos == NULL) {
		close(fd);
		return NULL;
	}

	Plato* plato = (Plato*) malloc(sizeof(Plato));
	char* buffer;
	char basura;
	while (read(fd, &basura, sizeof(char)) > 0) {
		lseek(fd, -1, SEEK_CUR);
		plato->nombre = read_until(fd, '\n');

		buffer = read_until(fd, '\n');
		plato->cantidad = atoi(buffer);
		free(buffer);

		buffer = read_until(fd, '\n');
		plato->precio = atoi(buffer);
		free(buffer);

		platos[*n_platos] = plato;
		*n_platos = *n_platos + 1;

		platos = realloc(platos, sizeof(Plato*) * (*n_platos * 2));
		plato = (Plato*) malloc(sizeof(Plato));
	}

	close(fd);
	return platos;
}