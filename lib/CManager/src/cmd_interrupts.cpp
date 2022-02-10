#include "cmd_interrupts.hpp"
#include "helpers.hpp"
#include <Arduino.h>

unsigned int _occupied_pins[64] = {0};

int command_int(command *c, program *p) {
	// INT <PIN> HIGH label
	sub *interrupt_sub = p->find_sub(c->args[2].name);
#ifndef DISABLE_EXCEPTIONS
	if (interrupt_sub == NULL) {
		c->exception = raise("Interrupt address not found", c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif

	variable *pin = get_var(c, 0);
	unsigned int a_pin = int(ctod(pin->data));

	variable *state = get_var(c, 1);

	bool found = false;
	for (unsigned int i = 0; i < 64; i++) {
		if (_occupied_pins[i] == a_pin) {
			found = true;
			break;
		}
	}

	if (!found) {
		pinMode(a_pin, INPUT);
	}

	int a_state = int(ctod(state->data));
	p->register_interrupt(a_pin, a_state, interrupt_sub);
	return 0;
}
