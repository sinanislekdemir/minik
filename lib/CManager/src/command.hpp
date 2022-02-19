#ifndef _command_hpp
#define _command_hpp

#define VAR_TYPE_NONE 0
#define VAR_TYPE_NUM 1
#define VAR_TYPE_STR 2

#ifdef BOARD_ESP32
#define MAX_CMDS 1000
#else
#define MAX_CMDS 100
#endif

#include "memory.hpp"

struct command {
	char statement;	       // no need to copy the cmd
	short variable_index[3];
	char variable_type[3];
	double variable_constant[3];
	short arg_count;
	char pid;
	bool exception;
};

double get_double(command c, int index);
int get_int(command c, int index);
long get_long(command c, int index);
void get_string(command c, int index, char *back, unsigned int size);
char get_byte(command c, int index);

#endif
