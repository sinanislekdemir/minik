#include "cmd_time.hpp"
#include "helpers.hpp"

int command_sleep(command *c, program *_p) {
	variable *v = get_var(c, 0);
#ifndef DISABLE_EXCEPTIONS
	if (v == NULL) {
		c->exception = raise(ERR_STR_VAR_NOT_FOUND, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	_p->_sleep = true;
	_p->_sleep_duration = (unsigned long)(ctod(v->data));
	_p->_sleep_start = millis();
	return 0;
}

int command_millis(command *c, program *_p) {
	new_number(c->args[0].data, double(millis()), c->pid);
	return 0;
}
