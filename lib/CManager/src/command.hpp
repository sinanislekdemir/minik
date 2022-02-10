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
	char variable_type[3]; // 0 - none | 1 - number | 2 - string
	short variable_index[3];
	char arg_count;
	char pid;
	bool exception;
};

svariable get_var(command *c, int index);
#endif
