#ifndef _command_hpp
#define _command_hpp

#include "error.hpp"
#include "memory.hpp"

struct command {
	char *cmd;
	variable *args;
	unsigned int argc;
	unsigned int pid;
	unsigned int index;
	command *next;
	error *exception;
};

variable *get_var(command *c, int index);
int validate_command(command *c, const char *cmd, unsigned int expected_argc);
#endif
