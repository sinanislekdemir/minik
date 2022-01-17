#include "cmd_serial.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <Arduino.h>

extern error err;
extern int serial_lock;

bool _breaker(char c) { return c == '\n'; }
bool _ignore(char c) { return (c < 32); }

char *_serial_getline(unsigned int buffer_size) {
	char *result = (char *)malloc(buffer_size);
	memset(result, 0, buffer_size);

	char c = 0;
	unsigned int cursor = 0;
	while (strlen(result) < buffer_size) {
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
	return result;
}

int command_serial_println(command *c, program *_p) {
	if (c->args[0].type == TYPE_STR) {
		Serial.println(c->args[0].data);
		return 0;
	}

	if (c->args[0].type == TYPE_NUM) {
		Serial.println(ctod(c->args[0].data));
		return 0;
	}
	if (c->args[0].type == TYPE_BYTE) {
		Serial.println(c->args[0].data[0]);
		return 0;
	}

	if (c->args[0].type == TYPE_VARIABLE) {
		variable *v = find_variable(c->args[0].data, c->pid);
#ifndef DISABLE_EXCEPTIONS
		if (v == NULL) {
			c->exception = raise(ERR_STR_VAR_NOT_FOUND, c->pid, ERR_VARIABLE_NOT_FOUND);
			return -1;
		}
#endif

		if (v->type == TYPE_NUM) {
			Serial.println(ctod(v->data));
			return 0;
		}

		if (v->type == TYPE_STR) {
			Serial.println(v->data);
			return 0;
		}
		if (v->type == TYPE_BYTE) {
			Serial.println(v->data[0]);
			return 0;
		}
	}
	return 0;
}

int command_serial_print(command *c, program *_p) {
	if (c->args[0].type == TYPE_STR) {
		Serial.print(c->args[0].data);
		return 0;
	}
	if (c->args[0].type == TYPE_NUM) {
		Serial.print(ctod(c->args[0].data));
		return 0;
	}
	if (c->args[0].type == TYPE_BYTE) {
		Serial.print(c->args[0].data[0]);
		return 0;
	}

	if (c->args[0].type == TYPE_VARIABLE) {
		variable *v = find_variable(c->args[0].data, c->pid);
#ifndef DISABLE_EXCEPTIONS
		if (v == NULL) {
			c->exception = raise(ERR_STR_VAR_NOT_FOUND, c->pid, ERR_VARIABLE_NOT_FOUND);
			return -1;
		}
#endif

		if (v->type == TYPE_NUM) {
			Serial.print(ctod(v->data));
			return 0;
		}

		if (v->type == TYPE_STR) {
			Serial.print(v->data);
			return 0;
		}
		if (v->type == TYPE_BYTE) {
			Serial.print(v->data[0]);
			return 0;
		}
	}
	return 0;
}

int command_getln(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->args[0].type != TYPE_VARIABLE) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_INVALID_COMMAND);
		return -1;
	}
	if (c->argc < 2) {
		c->exception = raise(ERR_STR_NOT_ENOUGH_PARAMS, c->pid, ERR_NOT_ENOUGH_ARGUMENTS);
		return -1;
	}
#endif

	variable *buflen = get_var(c, 1);

#ifndef DISABLE_EXCEPTIONS
	if (buflen->type != TYPE_NUM) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_INVALID_PARAMETER_TYPE);
		return -1;
	}
#endif

	while (serial_lock != -1) {
		delay(100); // wait for the lock
	}

	serial_lock = _p->pid;
	char *data = _serial_getline((unsigned int)(ctod(buflen->data)));
	serial_lock = -1;
	variable *v = find_variable(c->args[0].data, c->pid);

	if (v == NULL) {
		v = (variable *)malloc(sizeof(variable));
		v->next = NULL;

		uint8_t dsize = strlen(data) + 1;
		v->data = (char *)malloc(dsize);
		memset(v->data, 0, dsize);
		strcpy(v->data, data);
		free(data);
		v->datasize = dsize;

		v->type = TYPE_STR;
		v->pid = c->pid;
		dsize = strlen(c->args[0].data) + 1;
		v->name = (char *)malloc(dsize);
		memset(v->name, 0, dsize);
		strcpy(v->name, c->args[0].data);
		new_variable(v);
		return 0;
	}

	if (v->data != NULL) {
		free(v->data);
	}
	if (v->type != TYPE_STR) {
		v->type = TYPE_STR;
	}

	uint8_t dsize = strlen(data) + 1;
	v->data = (char *)malloc(dsize);
	memset(v->data, 0, dsize);
	strcpy(v->data, data);
	free(data);
	v->datasize = dsize;

	return 0;
}
