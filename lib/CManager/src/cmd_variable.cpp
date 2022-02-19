#include "cmd_variable.hpp"
#include "helpers.hpp"
#include <Arduino.h>

int command_set(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif

	if (c.variable_type[1] == TYPE_ADDRESS) {
		char buffer[MAX_LINE_LENGTH] = {0};
		char type = area_type(c.variable_index[1]);
		if (type == TYPE_NUM) {
			write_area(c.variable_index[0], read_area_double(c.variable_index[1]));
			return 0;
		}
		read_area_str(c.variable_index[1], 0, buffer);
		write_area(c.variable_index[0], buffer);
		return 0;
	}

	if (c.variable_type[1] == TYPE_NUM) {
		double val = get_double(c, 1);
		write_area(c.variable_index[0], val);
		return 0;
	}

	if (c.variable_type[1] == TYPE_BYTE) {
		char val = (char)c.variable_constant[1];
		write_area(c.variable_index[0], val);
		return 0;
	}

	// There should be no TYPE_STR here anymore, STR type
	// is saved during compilation
	if (c.variable_type[1] == TYPE_STR) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	return 0;
}

int command_cpy(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 3) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS || c.variable_type[1] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	char buffer[MAX_LINE_LENGTH] = {0};
	double size = get_double(c, 2);
	read_area_str(c.variable_index[1], (unsigned int)(size), buffer);
	write_area(c.variable_index[0], buffer, size);
	return 0;
}

int command_del(command c, program *p) {
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	double size = get_double(c, 1);
	free_area(c.variable_index[0], (unsigned int)size);
	return 0;
}

int command_alloc(command c, program *p) {
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	double size = get_double(c, 1);
	free_area(c.variable_index[0], (unsigned int)size);
	return 0;
}

int command_append(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	char byte = get_byte(c, 1);
	return append_area(c.variable_index[0], byte);
}
