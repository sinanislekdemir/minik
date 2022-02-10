#ifndef _tokenizer_h_
#define _tokenizer_h_

#include "command.hpp"
#include <Arduino.h>

int parse(const char *cmd, unsigned int pid, int index);
#endif
