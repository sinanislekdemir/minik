#ifndef _logic_hpp
#define _logic_hpp

#include "program.hpp"
#include "constants.hpp"

int command_je(command *c, program *_p); // Jump if a = b
int command_jne(command *c, program *_p); // Jump if a ! b
int command_jg(command *c, program *_p);  // Jump if a > b
int command_jge(command *c, program *_p); // Jump if a >= b
int command_jl(command *c, program *_p);  // Jump if a < b
int command_jle(command *c, program *_p); // Jump if a <= b

int command_cmp(command *c, program *_p);

#endif
