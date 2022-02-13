#include "command.hpp"
#include "constants.hpp"
#include <Arduino.h>

command commands[MAX_CMDS] = {0};

double get_double(command c, int index) {
	if (c.variable_type[index] == TYPE_NUM) {
		return c.variable_constant[index];
	}
	if (c.variable_type[index] == TYPE_ADDRESS) {
		return read_area_double(c.variable_index[index]);
	}
	return 0;
}

int get_int(command c, int index) {
	if (c.variable_type[index] == TYPE_NUM) {
		return int(c.variable_constant[index]);
	}
	if (c.variable_type[index] == TYPE_ADDRESS) {
		return read_area_int(c.variable_index[index]);
	}
	return 0;
}

long get_long(command c, int index) {
	if (c.variable_type[index] == TYPE_NUM) {
		return long(c.variable_constant[index]);
	}
	if (c.variable_type[index] == TYPE_ADDRESS) {
		return read_area_long(c.variable_index[index]);
	}
	return 0;
}

void get_string(command c, int index, char *back, unsigned int size) { read_area_str(c.variable_index[index], size, back); }

char get_byte(command c, int index) {
	if (c.variable_index[index] >= 0) {
		return read_area_char(index);
	}
	return char(c.variable_constant[index]);
}