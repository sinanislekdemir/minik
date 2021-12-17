#ifndef _memory_hpp
#define _memory_hpp

#include "variable.hpp"

#if defined(BOARD_ATMEGA)
#define BITS 8
#elif defined(BOARD_ESP32)
#define BITS 32
#else
#define BITS 8
#endif

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
void free_program(unsigned int pid);
void new_variable(variable *v);
void defrag_variables();
void mem_dump();

#endif
