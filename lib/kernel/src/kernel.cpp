#include "kernel.hpp"
#include "daemon.hpp"
#include "source.hpp"
#include "tasks.hpp"

volatile int serial_lock;
int kernel_next_pid;

extern daemon_task daemons;
extern StatusEngine *status_engine;
extern SourceEngine *source_engine;

/**
   Main Kernel Entrypoint
 */
int kmain() {
	Serial.begin(9600);
	register_kernel_tasks();
	register_statements();

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
	Serial.println("Ready to receive source code");
	Serial.flush();
	Serial.print("Free ram: ");
	Serial.println(free_ram());
	Serial.println("ready to receive");
	while (step_tasks()) {
	}
	mem_dump();
	stop();
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

void register_kernel_tasks() {
	_add_daemon(status_engine);
	_add_daemon(source_engine);
}
