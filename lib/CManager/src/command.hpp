#ifndef _command_hpp
#define _command_hpp

#include "memory.hpp"

struct command {
  char *command;
  variable *args;
  unsigned int argc;
  unsigned int pid;
};

variable *get_var(command *c, int index);
#endif
