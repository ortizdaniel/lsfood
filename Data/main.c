#include "util.h"
#include "networkpic.h"

int main(void) {
	print(1, "Init: %d\n", network_pic_init(8151));
	while (1) pause();
	return 0;
}