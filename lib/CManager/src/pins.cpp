#include "pins.hpp"
#include "helpers.hpp"

int pin_number(command c, program *p) {
	if (c.variable_type[1] == TYPE_NUM) {
		return int(c.variable_constant[1]);
	}
	if (c.variable_type[1] == TYPE_ADDRESS) {
		return read_area_int(c.variable_index[1]);
	}
	return -1;
}

int pin_value(command c, program *p) {
	if (c.variable_type[2] == TYPE_NUM) {
		return int(c.variable_constant[2]);
	}
	if (c.variable_type[2] == TYPE_ADDRESS) {
		return read_area_int(c.variable_index[2]);
	}
	return -1;
}
