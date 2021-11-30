#ifndef _command_hpp
#define _command_hpp

#include "memory.hpp"

struct command {
  char *cmd;
  variable *args;
  unsigned int argc;
  unsigned int pid;
  unsigned int index;
  command *next;
};

variable *get_var(command *c, int index);
#endif
