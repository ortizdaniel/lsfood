#include "networkpic.h"
#include "util.h"

static int sock = -1;
static int alive = 0;
static pthread_t pid;

void* _listen_thread(void* arg) {
	if (arg == NULL) {};
	while (alive) {
		struct sockaddr_in con;
		socklen_t pic_len = sizeof(con);

		int pic = accept(sock, (void *) &con, &pic_len);
		if (pic < 0) {
			print(2, "[Error de conexion (Picard)]\n");
		} else {
			Packet p;
			size_t size = read_packet(pic, &p);
			if (size > 0) {
				//se ha leido una trama correctamente
				
				free(p.data); //TO-DO
				close(pic);
			} else {
				print(2, "[Error de lectura de paquete (Picard)]\n");
				close(pic);
			}
		}
	}
	return NULL;
}

int network_pic_init(int port) {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

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

	return 0;
}

int network_pic_end() {
	alive = 0;
	close(sock);
	pthread_join(pid, NULL);
	return 0;
}

ssize_t _send_packet(int sfd, uint8_t type, char* header, uint16_t length, char* data) {
	ssize_t sent = 0;
	ssize_t tmp = 0;

	if ((tmp = write(sfd, &type, MAX_TYPE)) <= 0) {
		return 0;
	}

	sent += tmp;
	if ((tmp = write(sfd, header, MAX_HEADER_REAL)) < MAX_HEADER_REAL) {
		return 0;
	}
	sent += tmp;

	if ((tmp = write(sfd, &length, MAX_LENGTH)) <= 0) {
		return 0;
	}
	sent += tmp;

	if ((tmp = write(sfd, data, length)) < length) {
		return 0;
	}

	sent += tmp;
	return sent;
}

size_t send_packet(int sfd, Packet p) {
	return _send_packet(sfd, p.type, p.header, p.length, p.data);
}

int create_packet(Packet* p, int data_length) {
	memset(p, 0, sizeof(Packet));
	p->data = (char *) calloc(data_length, sizeof(char));
	if (p->data == NULL) return 0;
	return 1;
}

size_t read_packet(int sfd, Packet* p) {
	ssize_t leidos = 0;
	ssize_t tmp = 0;
	if ((tmp = read(sfd, &p->type, MAX_TYPE)) <= 0) {
		return 0;
	}
	leidos += tmp;

	memset(p->header, 0, MAX_HEADER_FAKE);
	if ((tmp = read(sfd, p->header, MAX_HEADER_REAL)) < MAX_HEADER_REAL) {
		return 0;
	}
	leidos += tmp;

	if ((tmp = read(sfd, &p->length, MAX_LENGTH)) <= 0) {
		return 0;
	}
	leidos += tmp;

	p->data = (char *) calloc(p->length + 1, sizeof(char)); //un espacio mas para \0
	if (p->data == NULL) {
		return 0;
	}

	if ((tmp = read(sfd, p->data, p->length)) < p->length) {
		return 0;
	}

	leidos += tmp;

	int bytes = 0;
	return leidos;
}