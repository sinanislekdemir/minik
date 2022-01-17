#include "cmd_analog.hpp"
#include "helpers.hpp"
#include "pins.hpp"
#include <Arduino.h>

int channels[16] = {-1};
#ifdef BOARD_ESP32
int _get_channel(int pin) {
	for (unsigned int i = 0; i < 16; i++) {
		if (channels[i] == pin) {
			return i;
		}
	}
	for (unsigned int i = 0; i < 16; i++) {
		if (channels[i] == -1) {
			channels[i] = pin;
			ledcAttachPin(pin, i);
			ledcSetup(i, 4000, 8);
			return i;
		}
	}
	return -1;
}
#endif

int command_analogread(command *c, program *_p) {
	// AREAD src pin
#ifndef DISABLE_EXCEPTIONS
	if (c->argc != 2) {
		c->exception = raise(ERR_STR_NOT_ENOUGH_PARAMS, c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif
	variable *pin = get_var(c, 1);
	unsigned int a_pin = int(ctod(pin->data));
	int val = analogRead(a_pin);
	new_number(c->args[0].data, double(val), c->pid);
	return 0;
}

int command_analogwrite(command *c, program *_p) {
	int *pins = pins_def(c, _p);
	if (pins[1] > 255) {
		pins[1] = 255;
	}
	if (pins[1] < 0) {
		pins[1] = 0;
	}
	if (pins == NULL) {
		return -1;
	}
#ifdef BOARD_ESP32
	int channel = _get_channel(pins[0]);
	if (channel == -1) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		strcat(msg, "No empty channels");
		c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
		free(msg);
		return -1;
	}

	ledcWrite(channel, pins[1]);
#endif

#ifdef BOARD_ATMEGA
	analogWrite(pins[0], pins[1]);
#endif
	free(pins);
	return 0;
}
int command_analogref(command *c, program *_p) { return 0; }
