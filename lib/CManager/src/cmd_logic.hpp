#ifndef _cmd_logic_hpp
#define _cmd_logic_hpp

#include "program.hpp"

int command_je(command c, program *p);	 // Jump if a = b
int command_jne(command c, program *p); // Jump if a ! b
int command_jg(command c, program *p);	 // Jump if a > b
int command_jge(command c, program *p); // Jump if a >= b
int command_jl(command c, program *p);	 // Jump if a < b
int command_jle(command c, program *p); // Jump if a <= b

int command_cmp(command c, program *p);

int command_lrotate(command c, program *p);
int command_rrotate(command c, program *p);
int command_lshift(command c, program *p);
int command_rshift(command c, program *p);
#endif
