#ifndef _statement_hpp
#define _statement_hpp

#include "program.hpp"

typedef int (*statement_function)(command *c, program *_p);

struct statement {
  int (*f)(command *c, program *_p);
  char *command;
};

void register_statements();

#endif
