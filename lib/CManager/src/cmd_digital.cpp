#include "cmd_digital.hpp"
#include "helpers.hpp"
#include "pins.hpp"
#include <Arduino.h>

int command_pinmode(command *c, program *p) {
	int pin = pin_number(c, p);
	int val = pin_value(c, p);
	if (pin == -1 || val == -1) {
		error_msg("Invalid PIN Def", p->pid);
		return -1;
	}

	pinMode(pin, val);
	return 0;
}

int command_digitalwrite(command *c, program *p) {
	// DWRITE pin HIGH/LOW
	int pin = pin_number(c, p);
	int value = pin_value(c, p);
	if (pin == -1 || value == -1) {
		error_msg("Invalid PIN Def", p->pid);
		return -1;
	}
	digitalWrite(pin, value);
	return 0;
}

int command_digitalread(command *c, program *p) {
	// DREAD src pin
	int pin = pin_number(c, p);
	if (pin == -1) {
		return -1;
	}
	int state = digitalRead(pin);
	new_number(c->args[0].data, double(state), c->pid);
	return 0;
}
