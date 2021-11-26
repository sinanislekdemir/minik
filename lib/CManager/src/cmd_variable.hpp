#ifndef _cmd_variable_hpp
#define _cmd_variable_hpp

#include "program.hpp"

// Create variable SET <var> <val>
int command_set(command *c, program *_p);
// Copy variable CPY <dest> <from> <size> <src>
int command_cpy(command *c, program *_p);
// Delete variable DEL <var>
int command_del(command *c, program *_p);
// Convert variable (number) into string STR <dest> <src>
int command_str(command *c, program *_p);
// Convert varible (string) into number NUM <dest> <src>
int command_num(command *c, program *_p);
#endif
