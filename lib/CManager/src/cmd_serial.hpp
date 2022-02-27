#ifndef _serial_hpp
#define _serial_hpp

#include "program.hpp"

int command_serial_println(command c, program *p);
int command_serial_print(command c, program *p);
int command_getln(command c, program *p);

#endif
