#ifndef _memory_hpp
#define _memory_hpp

#include "constants.hpp"
#include "variable.hpp"

// jump request
struct jump_request {
  int request;
  int pid;
  int label;
};

struct variable {
  char *name;
  char *data;
  unsigned int datasize;
  unsigned int pid;
  unsigned int type;
  bool deleted;
  variable *next;
};

variable *find_variable(const char *name, unsigned int pid);
void free_variable(const char *name, unsigned int pid);
void new_variable(variable *v);
void defrag_variables();
void mem_dump();

#endif
