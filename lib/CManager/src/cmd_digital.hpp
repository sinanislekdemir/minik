#ifndef _cmd_digital_hpp
#define _cmd_digital_hpp

#include "program.hpp"

int command_pinmode(command *c, program *_p);
int command_digitalwrite(command *c, program *_p);
int command_digitalread(command *c, program *_p);

#endif
