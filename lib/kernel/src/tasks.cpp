#include "tasks.hpp"

task tasks[CORES];
int last_core = 0;

// Deamons run on core 0
daemon_task daemons = {NULL, NULL, 0, 0, 0};

void init_cores() {
	for (unsigned int i = 0; i < CORES; i++) {
		tasks[i] = {NULL, 0, 0, 0, 0, NULL};
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

void add_task(program *_p, unsigned int priority) {
	int core = _next_core();
	if (tasks[core].prog == NULL) {
		tasks[core].prog = _p;
		tasks[core].priority = priority;
		tasks[core].start_time = millis();
		tasks[core].end_time = 0;
		tasks[core].exit_code = RUNNING;
		tasks[core].next = NULL;
		return;
	}
	task *t = (task *)malloc(sizeof(task));
	t->prog = _p;
	t->priority = priority;
	t->start_time = millis();
	t->end_time = 0;
	t->exit_code = RUNNING;
	t->next = NULL;

	task *head = &tasks[core];
	while (true) {
		if (head->next == NULL) {
			head->next = t;
			break;
		}
		head = head->next;
	}
}

bool step_tasks(int core) {
	if (core == 0) {
		daemon_task *dhead = &daemons;
		while (dhead != NULL) {
			if (dhead->exit_code != RUNNING) {
				dhead = dhead->next;
				continue;
			}
			int status = dhead->task->process();
#ifdef BOARD_ESP32
			vTaskDelay(1);
#endif
			if (status != RUNNING) {
				dhead->exit_code = status;
				dhead->end_time = millis();
			}
			dhead = dhead->next;
		}
	}

	task *head = &tasks[core];
	while (head != NULL) {
		if (head->exit_code != RUNNING) {
			head = head->next;
			continue;
		}
		for (unsigned int i = 0; i < head->priority; i++) {
			int status = head->prog->step();
#ifdef BOARD_ESP32
			vTaskDelay(1);
#endif
			if (status != RUNNING) {
				head->exit_code = status;
				head->end_time = millis();
				head->prog->destroy();
				free(head->prog);
			}
		}
		head = head->next;
	}
	return true;
}
