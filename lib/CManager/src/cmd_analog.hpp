#ifndef _cmd_analog_hpp
#define _cmd_analog_hpp

#include "program.hpp"

int command_analogread(command *c, program *_p);
int command_analogwrite(command *c, program *_p);
int command_analogref(command *c, program *_p);

#endif
