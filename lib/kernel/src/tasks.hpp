#ifndef _kernel_tasks_hpp
#define _kernel_tasks_hpp

#include "program.hpp"

#include <Arduino.h>

#define DAEMONS 2
#ifdef BOARD_ESP32
#define CORES 2
#define MAX_PROGRAM_COUNT 4
#define STACK_SIZE 8192
#else
#define STACK_SIZE 0
#define MAX_PROGRAM_COUNT 4
#endif

bool step_tasks(int core);
void init_cores();

#endif
