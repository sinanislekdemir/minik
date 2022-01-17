#include "command.hpp"
#include "constants.hpp"
#include <Arduino.h>

variable *get_var(command *c, int index) {
	if (c->args[index].type == TYPE_NUM || c->args[index].type == TYPE_STR || c->args[index].type == TYPE_BYTE) {
		return &c->args[index];
	}

	if (c->args[index].type == TYPE_VARIABLE) {
		return find_variable(c->args[index].data, c->pid);
	}
	return NULL;
}

int validate_command(command *c, unsigned int expected_argc) {
	if (c->argc < expected_argc) {
		c->exception = raise(ERR_STR_NOT_ENOUGH_PARAMS, c->pid, ERR_NOT_ENOUGH_ARGUMENTS);
		return -1;
	}

	return 0;
}
