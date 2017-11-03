#include "network.h"

int connected = 0;
int sock = -1;

int net_connect(char* ip, int port) {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (inet_aton(ip, &addr.sin_addr) == 0) {
		close(sock);
		return 2;
	}

	if (connect(sock, (void *) &addr, sizeof(addr)) < 0) {
		close(sock);
		return 3;
	}

	connected = 1;
	return 0;
}

int is_connected() {
	return connected;
}

void net_disconnect() {
	connected = 0;
	close(sock);
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

int send_aux() {
	return _send_packet(sock, 0x01, "[DANI]", strlen("[ELENA]"), "[ELENA]");
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

	p->data = (char *) calloc(p->length + 1, sizeof(char));
	if (p->data == NULL) {
		return 0;
	}

	if ((tmp = read(sfd, p->data, p->length)) < p->length) {
		return 0;
	}
	leidos += tmp;
	return leidos;
}