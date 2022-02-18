#include "tasks.hpp"
#include "status.hpp"
#include "term.hpp"

int last_core = 0;

// Deamons run on core 0
program programs[MAX_PROGRAM_COUNT];
extern sub _subs[MAX_SUBS];

extern Term main_term;
extern StatusEngine status_engine;

void init_cores() {
	main_term = Term();
	status_engine = StatusEngine();
	for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
		programs[i] = program();
	}
	for (unsigned int i = 0; i < MAX_SUBS; i++) {
		_subs[i].command_count = -1;
		_subs[i].cursor = 0;
		_subs[i].exit = 0;
		_subs[i].pid = 0;
		for (unsigned k = 0; k < 16; k++) {
			_subs[i].back_history[k] = -1;
		}
		for (unsigned k = 0; k < MAX_SUB_COMMANDS; k++) {
			_subs[i].commands[k] = -1;
		}
		memset(_subs[i].name, 0, 24);
	}
}

int _next_core() {
#ifdef BOARD_ESP32
	if (last_core == 0) {
		last_core = 1;
	} else {
		last_core = 0;
	}
	return last_core;
#endif
	return 0;
}

bool step_tasks(int core) {
	if (core == 0) {
		main_term.process();
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
		status_engine.process();
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
	}

	for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
		if (programs[i].pid == 0 || programs[i].exit_code == 0 || programs[i].compiling) {
			continue;
		}
		if (programs[i].start_time == 0) {
			programs[i].start_time = millis();
		}
		int result = programs[i].step();
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
		if (result != RUNNING) {
			programs[i].exit_code = 0;
			programs[i].end_time = millis();
			programs[i].destroy();
			programs[i].pid = 0;
		}
	}

	return true;
}
