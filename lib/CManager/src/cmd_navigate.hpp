#ifndef _cmd_navigate_hpp
#define _cmd_navigate_hpp

#include "program.hpp"

int command_call(command c, program *p);
int command_goto(command c, program *p);
int command_halt(command c, program *p);
#endif
