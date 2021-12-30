#ifndef _kernel_tasks_hpp
#define _kernel_tasks_hpp

#include "daemon.hpp"
#include "program.hpp"

#include <Arduino.h>

/**
   Priority of a task:
   loop:
     for i = 0; i < priority; i++
       step
 */

struct daemon_task {
	daemon *task;
	daemon_task *next;
	int exit_code;
	unsigned long start_time;
	unsigned long end_time;
};

struct task {
	program *prog;
	unsigned int priority;
	int exit_code;
	unsigned long start_time;
	unsigned long end_time;
	task *next;
};

void add_task(program *_p, unsigned int priority);
bool step_tasks();

#endif