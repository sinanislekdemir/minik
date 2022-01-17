#include "cmd_digital.hpp"
#include "helpers.hpp"
#include "pins.hpp"
#include <Arduino.h>

int command_pinmode(command *c, program *_p) {
	int *pins = pins_def(c, _p);
	if (pins == NULL) {
		error_msg("Invalid PIN Def", _p->pid);
		return -1;
	}
	pinMode(pins[0], pins[1]);
	free(pins);
	return 0;
}

int command_digitalwrite(command *c, program *_p) {
	// DWRITE pin HIGH/LOW
	int *pins = pins_def(c, _p);
	if (pins == NULL) {
		error_msg("Invalid PIN Def", _p->pid);
		return -1;
	}
	digitalWrite(pins[0], pins[1]);
	free(pins);
	return 0;
}
int command_digitalread(command *c, program *_p) {
	// DREAD src pin
#ifndef DISABLE_EXCEPTIONS
	if (c->argc != 2) {
		c->exception = raise(ERR_STR_NOT_ENOUGH_PARAMS, c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif
	variable *pin = get_var(c, 1);
	unsigned int a_pin = int(ctod(pin->data));
	int state = digitalRead(a_pin);
	new_number(c->args[0].data, double(state), c->pid);
	return 0;
}
