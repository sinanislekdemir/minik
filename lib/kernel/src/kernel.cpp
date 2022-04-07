#include "kernel.hpp"
#include "boot.hpp"
#include "drivers.hpp"
#include "globals.hpp"
#include "tasks.hpp"

KernelGlobals KGlobals;

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
        Serial.println("Register components");
	register_statements();
        Serial.println("Init cores");
	init_cores();
        Serial.println("Init kernel globals");
	KGlobals = KernelGlobals();
        Serial.println("Set status pin");
	KGlobals.status_engine.change_status_pin(STATUS_PIN);
	KGlobals.status_engine.set_status(READY);
        Serial.println("Acquire kernel serial lock");
	KGlobals.acquire_serial_lock(0);

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
	minik_boot();
	Serial.println("Send a few bytes to start terminal on Serial");
	KGlobals.main_terminal.start_server();

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
	KGlobals.status_engine.set_status(STOP);
	while (1) {
		KGlobals.status_engine.process();
	}
}
