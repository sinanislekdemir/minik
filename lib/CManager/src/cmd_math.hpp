#ifndef _cmanager_math_hpp
#define _cmanager_math_hpp

#include "program.hpp"

int command_add(command c, program *p);
int command_sub(command c, program *p);
int command_div(command c, program *p);
int command_mul(command c, program *p);
int command_xor(command c, program *p);
int command_or(command c, program *p);
int command_and(command c, program *p);
int command_pow(command c, program *p);
int command_trigonometry(command c, program *p);
#endif
