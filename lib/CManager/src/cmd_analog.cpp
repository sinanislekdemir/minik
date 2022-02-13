#include "cmd_analog.hpp"
#include "helpers.hpp"
#include "pins.hpp"
#include <Arduino.h>

int channels[16] = {-1};

#ifdef BOARD_ESP32
/**
 * @brief In ESP32, we dont have analogWrite command
 *        Instead, we do have led commands to help us.
 * 
 * @param pin 
 * @return int 
 */
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

int command_analogread(command c, program *p) {
	// AREAD src pin
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 2) {
		c.exception = true;
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
#endif
	int pin = pin_number(c, p);
	int val = analogRead(pin);
	write_area(c.variable_index[0], val);

	return 0;
}

int command_analogwrite(command c, program *p) {
	int pin = pin_number(c, p);
	int val = pin_value(c, p);
	if (val > 255) {
		val = 255;
	}
	if (val < 0) {
		val = 0;
	}
#ifdef BOARD_ESP32
	int channel = _get_channel(pin);
	if (channel == -1) {
		error_msg("No empty channels", c.pid);
		c.exception = true;
		return -1;
	}

	ledcWrite(channel, val);
#endif

#ifdef BOARD_ATMEGA
	analogWrite(pin, val);
#endif
	return 0;
}
int command_analogref(command c, program *p) { return 0; }
