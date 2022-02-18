#include "cmd_interrupts.hpp"
#include "helpers.hpp"
#include <Arduino.h>

unsigned int _occupied_pins[64] = {0};

int command_int(command c, program *p) {
	// INT <PIN> HIGH label
	if (c.variable_type[2] != TYPE_LABEL)
	{
		error_msg("Sub index not found", p->pid);
		return -1;
	}

	double state = get_double(c, 1);
	unsigned int pin = (unsigned int)(get_int(c, 0));
	int interrupt_sub = c.variable_index[2];

	if (interrupt_sub == -1) {
		error_msg("Interrupt sub not found", c.pid);
		return -1;
	}

	bool found = false;
	for (unsigned int i = 0; i < 64; i++) {
		if (_occupied_pins[i] == pin) {
			found = true;
			break;
		}
	}

	if (!found) {
		pinMode(pin, INPUT);
	}

	p->register_interrupt(pin, state, interrupt_sub);
	return 0;
}
