#ifndef _tokenizer_h_
#define _tokenizer_h_

#include "command.hpp"
#include <Arduino.h>

command *parse(const char *cmd, unsigned int pid);
int validate_command(command *c, const char *cmd, int expected_argc);

#endif
