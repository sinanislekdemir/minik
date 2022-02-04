#include "kernel.hpp"
#include "daemon.hpp"
#include "drivers.hpp"
#include "source.hpp"
#include "tasks.hpp"

volatile int serial_lock;
volatile int kernel_wifi_lock;
volatile int kernel_bt_lock;
int kernel_next_pid;

extern daemon_task daemons;
extern StatusEngine *status_engine;

#ifdef BOARD_ESP32
TaskHandle_t task_handles[CORES];

void core_run(void *_core) {
	unsigned int core = (unsigned int)(ulong)_core;
	while (step_tasks(core)) {
		yield();
	}
}
#endif

void _add_daemon(daemon *dae) {
	if (daemons.task == NULL) {
		daemons.end_time = 0;
		daemons.exit_code = RUNNING;
		daemons.next = NULL;
		daemons.start_time = millis();
		daemons.task = dae;
		return;
	}
	daemon_task *d = (daemon_task *)malloc(sizeof(daemon_task));
	d->task = dae;
	d->next = NULL;
	d->exit_code = RUNNING;
	d->start_time = millis();
	d->end_time = 0;
	daemon_task *head = &daemons;

	while (true) {
		if (head->next == NULL) {
			head->next = d;
			break;
		}
		head = head->next;
	}
}

void register_kernel_tasks() { _add_daemon(status_engine); }

/**
   Main Kernel Entrypoint
 */
int kmain() {
	Serial.begin(9600);
	register_kernel_tasks();

	register_statements();
	init_cores();

	status_engine->change_status_pin(STATUS_PIN);
	status_engine->set_status(READY);

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
	SourceEngine *source_engine = new SourceEngine();
	_add_daemon(source_engine);
#ifdef BOARD_ESP32
	char core_id[2];
	for (long i = 0; i < CORES; i++) {
		char *name = (char *)malloc(8);
		memset(name, 0, 8);
		memset(core_id, 0, 2);
		itoa(i, core_id, 10);
		strcat(name, "core_");
		strcat(name, core_id);
		xTaskCreatePinnedToCore(core_run, name, STACK_SIZE, (void *)i, 1, &task_handles[i], i);
		free(name);
	}
#endif
#ifdef BOARD_ATMEGA
	while (step_tasks(0)) {
	}
#endif
#ifndef BOARD_ESP32
	mem_dump();
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
	status_engine->set_status(STOP);
	while (1) {
		status_engine->process();
	}
}

char *kernel_stats(program *p) {
	char *result = (char *)malloc(1024);
	return result;
}
