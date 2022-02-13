#include "cmd_navigate.hpp"
#include "helpers.hpp"

extern sub _subs[MAX_SUBS];

int command_call(command c, program *p) {
	if (c.variable_type[0] != TYPE_LABEL) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	p->cursor = c.variable_index[0];
	_subs[p->subs[p->cursor]].cursor = 0;
	return 1; // don't change cursor
}

int command_goto(command c, program *p) {
	if (c.arg_count < 1) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	double location = get_double(c, 1);

	_subs[p->subs[p->cursor]].cursor = int(location);
	return 1;
}

int command_halt(command c, program *p) { return -1; }
