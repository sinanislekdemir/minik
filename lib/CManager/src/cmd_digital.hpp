#ifndef _cmd_digital_hpp
#define _cmd_digital_hpp

#include "program.hpp"

int command_pinmode(command *c, program *p);
int command_digitalwrite(command *c, program *p);
int command_digitalread(command *c, program *p);

#endif
