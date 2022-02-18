#include "cmd_navigate.hpp"
#include "helpers.hpp"

extern sub _subs[MAX_SUBS];

int _free_ram() {
#ifdef BOARD_ATMEGA
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#endif

#ifdef BOARD_ESP32
	return ESP.getFreeHeap();
#endif
	return 0;
}

int command_call(command c, program *p) {
	if (c.variable_type[0] != TYPE_LABEL) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	p->cursor = c.variable_index[0];
	int ind = p->subs[p->cursor];
	Serial.println(ind);
	_subs[ind].cursor = 0;
	return 1; // don't change cursor
}

int command_goto(command c, program *p) {
	if (c.arg_count < 1) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, c.pid);
		return -1;
	}
	double location = get_double(c, 0);

	_subs[p->subs[p->cursor]].cursor = int(location);
	return 1;
}

int command_halt(command c, program *p) { return -2; }
