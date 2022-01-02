#ifndef _cmanager_math_hpp
#define _cmanager_math_hpp

#include "program.hpp"

int get_v1_v2(command *c, variable *v1, variable *v2);
int save_result(command *c, double result);

int command_add(command *c, program *_p);
int command_sub(command *c, program *_p);
int command_div(command *c, program *_p);
int command_mul(command *c, program *_p);
int command_xor(command *c, program *_p);
int command_or(command *c, program *_p);
int command_and(command *c, program *_p);
int command_pow(command *c, program *_p);
int command_trigonometry(command *c, program *_p);
#endif
