#ifndef _cmd_system_hpp
#define _cmd_system_hpp

#include "program.hpp"
#include <Arduino.h>

typedef int (*syscall_function)(program *_p);

struct sys_call {
	int (*fnc)(program *_p);
	int driver_address;
	sys_call *next;
};

int command_core(command *c, program *_p);
int command_sys(command *c, program *_p);

#endif
