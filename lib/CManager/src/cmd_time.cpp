#include "cmd_time.hpp"
#include "helpers.hpp"

int command_sleep(command c, program *_p) {
	if (c.arg_count < 1) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	double duration = get_double(c, 0);
	_p->_sleep = true;
	_p->_sleep_duration = (unsigned long)(duration);
	_p->_sleep_start = millis();
	return 0;
}

int command_millis(command c, program *_p) {
	write_area(c.variable_index[0], double(millis()));
	return 0;
}
