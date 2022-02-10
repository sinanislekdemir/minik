#include "pins.hpp"
#include "helpers.hpp"

int pin_number(command c, program *_p) {
	if (c.variable_type[1] == TYPE_NUM) {
		return int(get_d(c.variable_index[0]));
	}
	if (c.variable_type[1] == TYPE_VARIABLE) {
		if (c.variable_index[1] == -1) {
			return -1;
		} else {
			return int(get_d(c.variable_index[0]));
		}
	}
	return -1;
}
int pin_value(command c, program *_p) {
	if (c.variable_type[1] == TYPE_NUM) {
		return int(get_d(c.variable_index[1]));
	}
	if (c.variable_type[0] == TYPE_VARIABLE) {
		if (c.variable_index[1] == -1) {
			return -1;
		} else {
			return int(get_d(c.variable_index[1]));
		}
	}
	return -1;
}
