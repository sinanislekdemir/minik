#ifndef _error_hpp
#define _error_hpp

#include "constants.hpp"

struct error {
  char *message;
  unsigned int code;
  unsigned int pid;
};

#endif
