#include "kernel.hpp"
#include "drivers.hpp"
#include "tasks.hpp"
#include "term.hpp"

volatile int serial_lock;
volatile int kernel_wifi_lock;
volatile int kernel_bt_lock;
int kernel_next_pid;

extern StatusEngine status_engine;
extern Term main_term;

#ifdef BOARD_ESP32
TaskHandle_t task_handles[CORES];

void core_run(void *_core) {
	unsigned int core = (unsigned int)(ulong)_core;
	while (step_tasks(core)) {
		yield();
	}
}
#endif

/**
   Main Kernel Entrypoint
 */
int kmain() {
	Serial.begin(9600);

	register_statements();
	init_cores();

	status_engine.change_status_pin(STATUS_PIN);
	status_engine.set_status(READY);

	serial_lock = -1;
	kernel_next_pid = 1;

	Serial.print("Platform ");
#ifdef BOARD_ESP32
	Serial.println("ESP32");
#endif
#ifdef BOARD_ATMEGA
	Serial.println("ATMEGA");
#endif
	Serial.println("Minik Kernel KMain");
	init_drivers();
	print_status();
	Serial.println("Send a few bytes to start terminal on Serial");
	main_term = Term();
	main_term.start_server();

#ifdef BOARD_ESP32
	char core_id[2] = {0};
	for (long i = 0; i < CORES; i++) {
		char name[8] = {0};
		itoa(i, core_id, 10);
		strcat(name, "core_");
		strcat(name, core_id);
		xTaskCreatePinnedToCore(core_run, name, STACK_SIZE, (void *)i, 1, &task_handles[i], i);
	}
#endif
#ifdef BOARD_ATMEGA
	while (step_tasks(0)) {
	}
#endif
#ifndef BOARD_ESP32
	stop();
#endif
	return 0;
}

void stop() {
#ifdef BOARD_ESP32
	Serial.println(".shutdown.");
#endif
#ifdef BOARD_ATMEGA
	if (Serial) {
		Serial.println(".shutdown.");
	}
#endif
	status_engine.set_status(STOP);
	while (1) {
		status_engine.process();
	}
}
