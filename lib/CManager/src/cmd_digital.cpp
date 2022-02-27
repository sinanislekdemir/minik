#include "cmd_digital.hpp"
#include "helpers.hpp"
#include <Arduino.h>

int command_pinmode(command c, program *p) {
	int pin = int(get_double(c, 0));
	int val = int(get_double(c, 1));
	if (pin == -1 || val == -1) {
		error_msg("Invalid PIN Def", p->pid);
		return -1;
	}
	pinMode(pin, val);
	return 0;
}

int command_digitalwrite(command c, program *p) {
	// DWRITE pin HIGH/LOW
	int pin = int(get_double(c, 0));
	int value = int(get_double(c, 1));

	if (pin == -1 || value == -1) {
		error_msg("Invalid PIN Def", p->pid);
		return -1;
	}
	digitalWrite(pin, value);
	return 0;
}

int command_digitalread(command c, program *p) {
	// DREAD src pin
	int pin = int(get_double(c, 1));
	if (pin == -1) {
		return -1;
	}
	int state = digitalRead(pin);
	write_area(c.variable_index[0], double(state));
	return 0;
}
