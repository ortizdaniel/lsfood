#include "enterpriselist.h"

static int size = 0;
static int added = 0;
static Enterprise* enterprises = NULL;

int enterprises_init() {
	enterprises = (Enterprise *) calloc(INIT_SIZE, sizeof(Enterprise));
	if (enterprises == NULL) return 0;
	size = INIT_SIZE;
	return 1;
}

Enterprise* enterprises_get() {
	return enterprises;
}

int enterprises_size() {
	return added;
}

int enterprises_add(Enterprise e) {
	if (added < size) {
		enterprises[added++] = e;
		return 1;
	} else {
		Enterprise* tmp = (Enterprise*) realloc(enterprises, size * 2);
		if (tmp == NULL) return 0;

		enterprises = tmp;
		size *= 2;
		enterprises[added++] = e;
		return 1;
	}
}

void enterprises_end() {
	free(enterprises);
}