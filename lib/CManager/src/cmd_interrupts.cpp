#include "cmd_interrupts.hpp"
#include "helpers.hpp"
#include <Arduino.h>

unsigned int _occupied_pins[64] = {0};

int command_int(command *c, program *_p) {
	// INT <PIN> HIGH label
	sub *interrupt_sub = _p->find_sub(c->args[2].name);
#ifndef DISABLE_EXCEPTIONS
	if (interrupt_sub == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Jump location [%s] not found", c->args[0].name);
		c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
		free(msg);
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
	_p->register_interrupt(a_pin, a_state, interrupt_sub);
	return 0;
}
