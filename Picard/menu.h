#ifndef _MENU_H
#define _MENU_H

#include <ctype.h>
#include "util.h"

#define MAX_IN 128

void menu(const char* nombre, int dinero);

void show_prompt(const char* usr);

int read_cmd(char* cmd, char* arg1, char* arg2);

void cmd_error();

void cmd_ok();

#endif