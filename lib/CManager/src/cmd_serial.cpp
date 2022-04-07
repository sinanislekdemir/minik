#include "cmd_serial.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "globals.hpp"
#include <Arduino.h>

extern KernelGlobals KGlobals;

bool _breaker(char c) { return c == '\n'; }
bool _ignore(char c) { return (c < 32); }

int _serial_getline(char *result, unsigned int buffer_length) {
	char c = 0;
	unsigned int cursor = 0;
	while (strlen(result) < buffer_length) {
		if (!Serial.available()) {
			continue;
		}
		c = Serial.read();
		if (_breaker(c)) {
			break;
		}
		if (_ignore(c)) {
			continue;
		}
		Serial.print(c);
		result[cursor++] = c;
	}

	Serial.println("");
	return 0;
}

int command_serial_println(command c, program *p) {
	char buffer[MAX_LINE_LENGTH] = {0};
	if (c.variable_type[0] == TYPE_NUM) {
		Serial.println(c.variable_constant[0]);
		return 0;
	}
	if (c.variable_type[0] == TYPE_BYTE) {
		Serial.println(int(c.variable_constant[0]));
		return 0;
	}
	if (c.variable_type[0] == TYPE_ADDRESS) {
		char type = area_type(c.variable_index[0]);
		if (type == TYPE_NUM) {
			Serial.println(get_double(c, 0));
		} else if (type == TYPE_BYTE) {
			Serial.println(int(get_byte(c, 0)));
		} else {
			get_string(c, 0, buffer, 0);
			Serial.println(buffer);
		}
	}
	return 0;
}

int command_serial_print(command c, program *p) {
	char buffer[MAX_LINE_LENGTH] = {0};
	if (c.variable_type[0] == TYPE_NUM) {
		Serial.println(c.variable_constant[0]);
		return 0;
	}
	if (c.variable_type[0] == TYPE_BYTE) {
		Serial.println(int(c.variable_constant[0]));
		return 0;
	}
	if (c.variable_type[0] == TYPE_ADDRESS) {
		char type = area_type(c.variable_index[0]);
		if (type == TYPE_NUM) {
			Serial.print(get_double(c, 0));
		} else if (type == TYPE_BYTE) {
			Serial.print(int(get_byte(c, 0)));
		} else {
			get_string(c, 0, buffer, 0);
			Serial.print(buffer);
		}
	}
	return 0;
}

int command_getln(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	if (c.arg_count < 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
#endif

	unsigned int buf_size = (unsigned int)(get_double(c, 1));
	if (buf_size > MAX_LINE_LENGTH) {
		buf_size = MAX_LINE_LENGTH;
	}

	char buffer[MAX_LINE_LENGTH] = {0};

        KGlobals.acquire_serial_lock(p->pid);
	_serial_getline(buffer, buf_size);
        KGlobals.release_serial_lock();

	return write_area(c.variable_index[0], buffer, buf_size);
}
